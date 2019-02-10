#include "simulator.h"
#define min(a,b) (a>b)?b:a

Simulator* Simulator :: instance_;

Simulator::Simulator()
{
    nnode_ = NODE_NUMBER;
    totalPacketNum = 0;
    acceptPacketNum = 0;
    dropPacketNum = 0;
    instance_ = this;
    nodes_ = new Node*[nnode_];
    is_alive = new bool[nnode_];
    
    memset(Accept, 0, sizeof(Accept));
    memset(acc_num, 0, sizeof(acc_num));
    memset(send_num, 0, sizeof(send_num));
    memset(P_ij, 0, sizeof(P_ij));
    memset(V, 0, sizeof(V));
    
    pthread_mutex_init(&mutex_Accept, NULL);
    pthread_mutex_init(&mutex_acc_num, NULL);
    pthread_mutex_init(&mutex_send_num, NULL);
    pthread_mutex_init(&mutex_write, NULL);
    pthread_mutex_init(&mutex_accPacket, NULL);
    pthread_mutex_init(&mutex_solar, NULL);
    pthread_mutex_init(&mutex_vibration, NULL);
    
    fout.open("out.txt");
}

Simulator::~Simulator()
{
    for (int i = 0; i < nnode_; ++i)
    {
        delete nodes_[i];
        nodes_[i] = NULL;
    }
    delete []nodes_;
    delete []is_alive;
}

void Simulator :: init()
{
    int x, y, z;
    for (int i = 0; i < nnode_; ++i)
    {
        nodes_[i] = new Node;
        is_alive[i] = 1;
        if (i < nnode_-1)
        {
            nodes_[i]->nodeType(CH);
            x = rand() % (MAX_X+1);
            y = rand() % (MAX_Y+1);
            z = rand() % (MAX_Z-MIN_Z+1)+MIN_Z;
            nodes_[i]->location(x,y,z);
            if (z <= 50)
            {
                nodes_[i]->harvest(SOLAR);
                fout << "Node " << i << ": Solar" << endl;
            }
            else
            {
                nodes_[i]->harvest(VIBRATION);
                fout << "Node " << i << ": Vibration" << endl;
            }
        }
        else
        {
            nodes_[i]->nodeType(BS);
            nodes_[i]->location(MAX_X, MAX_Y, MIN_Z);
            bs_ = nodes_[i];
        }
    }
    for (int i = 0; i < nnode_-1; ++i)
    {
        nodes_[i]->init(nnode_);
    }
    for (int i=0; i< nnode_-1; ++i)
    {
        fout<<"Node "<<i<<" connect: ";
        CTableEntry* entry;
        for(int j=0;j<nodes_[i]->chTable_->entryCounter_;j++)
        {
            entry = nodes_[i]->chTable_->entry_[j];
            fout<<entry->node_->id()<<' ';
        }
        fout<<endl;
    }
    
}

void Simulator :: producePacket()
{
    time_t cur_time;
    int interval = 0;
    while (true)
    {
        cur_time = time(NULL);
        if (cur_time - start_time > SIMULATE_TIME * 60)
        {
            return;
        }
        int source_id = rand()%(nnode_-1);
        while(is_alive[source_id]==0)
            source_id = rand()%(nnode_-1);
        Packet *p;
        p = new Packet(source_id, nnode_-1);
        p->belong_node = source_id;
        p->born_time = cur_time;
        p->idx = totalPacketNum;
        nodes_[source_id]->enqueuePkt(p);
        totalPacketNum++;
        pthread_mutex_lock(&mutex_write);
        fout << totalPacketNum <<" produced! It's source ID :"<<source_id<<endl;
        pthread_mutex_unlock(&mutex_write);
        interval = int(randExp(1.0/LAMBDA));
        while(interval == 0)
            interval = int(randExp(1.0/LAMBDA));
        while (time(NULL)-cur_time < interval && cur_time - start_time <= SIMULATE_TIME * 60);
    }
}

void Simulator :: runCHNode(int i)
{
    time_t cur_time;
    while (true)
    {
        cur_time = time(NULL);
        if (cur_time - start_time > SIMULATE_TIME * 60)
        {
            cout << "Simulate end!!!" << endl;
            return;
        }
        if (is_alive[i])
        {
            nodes_[i]->forwardData();
        }
        else
        {
            if(nodes_[i]->reachedThreshold()==1)
                is_alive[i] = true;
        }
    }
}

void Simulator :: addEnergy()
{
    time_t cur_time;
    time_t old_time;
    old_time = start_time;
    int old_flag = -1;
    while (true)
    {
        cur_time = time(NULL);
        if (cur_time - start_time > SIMULATE_TIME * 60)
            return;
        if (cur_time - old_time >= 1)
        {
            for (int i = 0; i < nnode_-1; ++i)
            {
                if (cur_time-start_time > SIMULATE_TIME*30 && nodes_[i]->harvest() == SOLAR)
                continue;
                nodes_[i]->energy(min(INIT_ENERGY,nodes_[i]->energy()+nodes_[i]->harvest()));
            }
        }
        
        if ((cur_time - start_time) % 30 == 0 && int((cur_time - start_time) / 30) != old_flag)
        {
            old_flag = int((cur_time - start_time) / 30);
            pthread_mutex_lock(&mutex_write);
            fout << "Time: " << (cur_time-start_time)/60.0 << "min " << "<50m Use: " << solar_energy << endl;
            fout << "Time: " << (cur_time-start_time)/60.0 << "min " << ">50m Use: " << vibration_energy << endl;
            pthread_mutex_unlock(&mutex_write);
        }
        old_time = cur_time;
    }
}

void Simulator :: run()
{
    start_time = time(NULL);
    cout << "Start time: " << start_time << endl;
    for (int i = 0; i < nnode_; ++i)
    {
        if (i < nnode_ - 1)
        {
            thread_pool[i] = thread(bind(&Simulator::runCHNode,this, i));
        }
    }
    thread thread_energy(bind(&Simulator::addEnergy,this));
    thread thread_packet(bind(&Simulator::producePacket,this));
    for (int i = 0; i < nnode_-1; ++i)
    {
        thread_pool[i].join();
    }
    thread_energy.join();
    thread_packet.join();
}

void Simulator :: killNode(Node* n)
{
    is_alive[n->id()] = 0;
    n->pktQueue_.clear();
}

