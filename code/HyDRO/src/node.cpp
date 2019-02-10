//
//  node.cpp
//  HyDRO
//
//  Created by 王 on 2018/12/1.
//  Copyright © 2018 王. All rights reserved.
//


#include "node.h"
#include "simulator.h"
#include "energy.h"
#include <math.h>

int Node::IDs_ = 0;

void Node::init(int nneighbor){
    nneighbors_ =  nneighbor;
    neighbours_ = new Node*[nneighbor];
    for(int i=0;i<nneighbor;i++)
        addNeighbour(i);
    chTable_ = new CTable(nneighbors_);
    achTable_ = new CTable(nneighbors_);
    generateCLTable();
    updateActiveList();
}

// Have some questions
int Node::addNeighbour(int id){
    Simulator& simulator = Simulator::instance();
    Node *n = simulator.node(id);
    if(!n){
        std::cout<<"No such node!"<<std::endl;
        return -1;
    }
    
    // Check distance to the other side
    if(distance(this, n) > this->transmissionRange_)
    {
        neighbours_[id] = NULL;
        return -1;
    }
    
    neighbours_[id] = n;
    
//    if(distance(this, n) > n->transmissionRange_)
//    {
//        return -1;
//    }
//
//    n->neighbours_[this->id_] = this;
    return 0;
}

// Distance between two nodes
double Node :: distance(Node *n1, Node *n2) {
    double x1 = (double)(n1->location_.x);
    double y1 = (double)(n1->location_.y);
    double z1 = (double)(n1->location_.z);
    double x2 = (double)(n2->location_.x);
    double y2 = (double)(n2->location_.y);
    double z2 = (double)(n2->location_.z);
    return (sqrt( pow((x2-x1), 2) + pow((y2-y1), 2) +pow((z2-z1), 2)));
}

int Node::reachedThreshold(){
    if(energy_ < energy_threshold_1_)
        return -1;
    else if(energy_ > energy_threshold_2_)
        return 1;
    else
        return 0;
}

int Node::enqueuePkt(Packet *p){
    if(pktQueue_.size() == queueLimit_){
        //std::cout<<"[ERROR]: Packet que is full"<<std::endl;
        return -1;
    }
    pktQueue_.push_back(*p);
    return 0;
}

int Node::dequeuePkt(){
    if(pktQueue_.size() == 0){
        //std::cout<<"[ERROR]: Packet que is empty"<<std::endl;
        return -1;
    }
    pktQueue_.erase(pktQueue_.begin());
    return 0;
}

// Send the packet p to the node n
int Node :: send(Node *n, Packet *p) {
    //cout<<p->type<<' '<<p->sourceID<<' '<<p->destID<<endl;
    Simulator &sim = Simulator::instance();
    
    pthread_mutex_lock(&sim.mutex_send_num);
    sim.send_num[id_] += 1;
    pthread_mutex_unlock(&sim.mutex_send_num);
    sim.P_ij[id_][n->id_] = (1.0 * sim.acc_num[id_][n->id_]) / sim.send_num[id_];
    
    p->forwardID = id_;
    double trans_time = (p->header_size+p->payload_size)/BIT_RATE;
    
    Energy::spend(this, TX, trans_time); //energy consumption by transmitter
    Energy::spend(n, RX, trans_time);
    n->recv(p);
    
    return 0;
}

// Broadcast packet p
int Node :: broadcast(Packet *p) {
    for(int i=0; i<nneighbors_; i++)
        if(neighbours_[i] && (neighbours_[i] != this))
            send(neighbours_[i], p);
    delete p;
    return 0;
}

// Recieve and enqueue packet
int Node :: recv(Packet *p) {
    Simulator &sim = Simulator::instance();
    int overhear = p->forwardID;

    if(this==sim.instance().bs_)
    {
        if(p->type==RELAXATION)
            return 0;
        Packet *p1 = new Packet;
        p1->sourceID = id_;
        p1->destID = overhear;
        p1->type = ACK;
        send(sim.nodes_[overhear], p1);
        if(p->type==DATA)
        {
            pthread_mutex_lock(&sim.mutex_accPacket);
            sim.acceptPacketNum += 1;
            pthread_mutex_unlock(&sim.mutex_accPacket);
        }
        return 0;
    }
    
    Notify_t type = p->type;
    CTableEntry* entry;
    bool changed = false, found = false;
    if(chTable_!=NULL){
    for(int i=0;i<chTable_->entryCounter_;i++)
    {
        entry = chTable_->entry_[i];
        if(entry->node_->id() == overhear)
        {
            found = true;
            if(entry->currentState_==RELAX && type!=RELAXATION)
            {
                entry->currentState_ = ACTIVE;
                changed = true;
            }
            else if(entry->currentState_==ACTIVE && type==RELAXATION)
            {
                entry->currentState_ = RELAX;
                changed = true;
            }
            entry->lastTime = time(NULL);
        }
    }
    }
    if(changed)
        updateActiveList();
    if(!found)
    {
        double dist_to_bs = distance(neighbours_[overhear], sim.baseStation());
        double dist_to_me = distance(neighbours_[overhear], this);
        double me_to_bs = distance(this, sim.baseStation());
        if(dist_to_bs < me_to_bs)
        {
            entry = new CTableEntry;
            entry->node_ = neighbours_[overhear];
            entry->distance_to_me_ = dist_to_me;
            entry->distance_to_BS_ = dist_to_bs;
            entry->currentState_ = RELAX;    // XXX... May depend.
            entry->lastTime = time(NULL);
            //Entry for Assistant CH node
            if(neighbours_[overhear]->nodeType_ == CH) {
                // Find its ACH and associate it
                entry->assistantNode_ = neighbours_[overhear]->findAssistantCH();
            }
            chTable_->addEntry(entry);
            delete entry;
        }
    }
    
    pthread_mutex_lock(&sim.mutex_acc_num);
    sim.acc_num[overhear][id_] += 1;
    pthread_mutex_unlock(&sim.mutex_acc_num);
    //sim.P_ij[overhear][id_] = (1.0 * sim.acc_num[overhear][id_]) / sim.send_num[overhear];
    
    if(p->type==ACK)
    {
        pthread_mutex_lock(&sim.mutex_Accept);
        sim.Accept[id_][overhear] = true;
        pthread_mutex_unlock(&sim.mutex_Accept);
        return 0;
    }
    
    if(p->destID==BROADCAST_ADDRESS)
    {
        if(p->type==ACTIVATION)
        {
            Packet *p1 = new Packet;
            p1->sourceID = id_;
            p1->destID = overhear;
            p1->type = ACK;
            send(sim.nodes_[overhear], p1);
        }
        return 0;
    }
    
    if(enqueuePkt(p) == -1) {
        // Drop packet
        return 0;
    }
    
    return 1;
}

// Maintian the CLTable to choose the next hop
void Node :: generateCLTable() {
    if(this->nodeType_ == NCH || this->nodeType_==BS) {
        return;
    }
    Simulator& sim = Simulator::instance();
    CTableEntry *entry = NULL;
    int i;
    double me_to_bs = distance(this, sim.baseStation());
    double dist_to_bs;
    double dist_to_me;
    for(i=0; i<nneighbors_; i++) {
        if(!neighbours_[i] || ((neighbours_[i]->nodeType() != CH) && (neighbours_[i]->nodeType() != BS)) || (this == neighbours_[i]))
            continue;
        dist_to_bs = distance(neighbours_[i], sim.baseStation());
        dist_to_me = distance(neighbours_[i], this);
        if(dist_to_bs < me_to_bs) {
            entry = new CTableEntry;
            entry->node_ = neighbours_[i];
            entry->distance_to_me_ = dist_to_me;
            entry->distance_to_BS_ = dist_to_bs;
            entry->currentState_ = RELAX;    // XXX... May depend.
            entry->lastTime = time(NULL);
            //Entry for Assistant CH node
            if(neighbours_[i]->nodeType_ == CH) {
                // Find its ACH and associate it
                //entry->assistantNode_ = neighbours_[i]->findAssistantCH();
            }
            chTable_->addEntry(entry);
            delete entry;
        }
    }
}

// Generate achTable_ from the neighbour_ table for assistant CH nodes. (Excludes self's ACH)
void Node :: generateACLTable() {
    if(this->nodeType_ == NCH)
        return;
    
    Simulator& sim = Simulator::instance();
    CTableEntry *entry = NULL;
    int i;
    double me_to_bs = distance(this, sim.baseStation());
    double dist_to_bs;
    double dist_to_me;
    for(i=0; i<nneighbors_; i++) {
        if(!neighbours_[i] || (neighbours_[i]->nodeType() != ACH) ||    \
           (neighbours_[i]->cluster() == this->cluster()) ||(this == neighbours_[i]))
            continue;
        dist_to_bs = distance(neighbours_[i], sim.baseStation());
        dist_to_me = distance(neighbours_[i], this);
        if(dist_to_bs < me_to_bs) {
            entry = new CTableEntry;
            entry->node_ = neighbours_[i];
            entry->distance_to_me_ = dist_to_me;
            entry->distance_to_BS_ = dist_to_bs;
            entry->currentState_ = RELAX;    // XXX... May depend.
            //Entry for Assistant CH node = NULL, since it is an ACH itself.
            entry->assistantNode_ = NULL;
            achTable_->addEntry(entry);
            delete entry;
        }
    }
    // Sort the table contents in increasing order of 'distance_to_BS_' parameter.
    achTable_->sortByBSDistance();
}

//Compute and return the ACH of the CH node
Node* Node :: findAssistantCH() {
    if(assistantCH_) {
        return assistantCH_;
    }
    Simulator& sim = Simulator::instance();
    // For all ACH type nodes...
    Node **ptr = sim.nodePtr(sim.nClusters()) + 1; // +1 to go one step of BS node
    for(int i=0; (i < sim.nClusters()) && (ptr[i]->nodeType() == ACH); i++) {
        if(ptr[i]->cluster() == this->cluster()) {
            this->assistantCH_ = ptr[i];
            break;
        }
    }
    return assistantCH_;
}

void Node::updateActiveList() {
    if(this->nodeType_ == NCH)
        return;
    
    activeEntry.clear();
    CTableEntry* entry;
    for(int i=0;i<chTable_->entryCounter_;i++)
    {
        entry = chTable_->entry_[i];
        if(entry->currentState_ == ACTIVE)
            activeEntry.push_back(entry);
    }
}

// Select a next-hop node from chTable_ or achTable_
void Node :: selectNextHop(int k) {
    Simulator& sim = Simulator::instance();
    
    int action_size = activeEntry.size();
    double **R_matrix = new double*[K];
    for(int i=0;i<K;i++)
    {
        R_matrix[i] = new double[action_size];
        double e;
        if(i==0)
            e = energy_ + harvest_ - TX;
        else
            e = -TX;
        for(int j=0;j<action_size;j++)
        {
            int to_node = activeEntry[j]->node_->id();
            if(i==K-1)
            {
                R_matrix[i][j] = e + sim.V[to_node] * sim.P_ij[id_][to_node] - L * (1 - sim.P_ij[id_][to_node]);
            }
            else
            {
                R_matrix[i][j] = e + sim.V[to_node] * sim.P_ij[id_][to_node];
            }
        }
    }
    
    double **Q_matrix = new double*[action_size];
    for(int i=0;i<K;i++)
    {
        Q_matrix[i] = new double[action_size];
        for(int j=0;j<action_size;j++)
        {
            int to_node = activeEntry[j]->node_->id();
            double P_rcv;
            if(i==K-1)
            {
                P_rcv = sim.P_ij[id_][to_node];
                Q_matrix[i][j] = R_matrix[i][j] + DISCOUNT_FACTOR * ((1-P_rcv) * Vfuction[K+1] + P_rcv * Vfuction[K]);
            }
            else
            {
                P_rcv = sim.P_ij[id_][to_node] * sim.P_ij[to_node][id_];
                Q_matrix[i][j] = R_matrix[i][j] + DISCOUNT_FACTOR * ((1-P_rcv) * Vfuction[i+1] + P_rcv * Vfuction[K]);
            }
            Vfuction[i] = std::max(Vfuction[i], Q_matrix[i][j]);
        }
    }
    double Max = -10000;
    for(int j=0;j<action_size;j++)
        if(Q_matrix[k][j]>Max)
        {
            Max = Q_matrix[k][j];
            nextHop_ = activeEntry[j]->node_;
        }
    sim.V[id_] = Max;
}

void Node :: forwardData() {
    CTableEntry* entry;
    bool changed = false;
    for(int i=0;i<chTable_->entryCounter_;i++)
    {
        entry = chTable_->entry_[i];
        if(entry->currentState_==ACTIVE)
        {
            if(time(NULL) - entry->lastTime >= TIME_THRESHOLD)
            {
                entry->currentState_ = RELAX;
                changed = true;
            }
        }
    }
    if(changed)
        updateActiveList();
    
    if(pktQueue_.size()==0)
        return;
    
    if(activeEntry.size() >0){
        Packet *p = new Packet(pktQueue_.front());
        if(p->belong_node != id_ || p->is_drop)
        {
            dequeuePkt();
            return;
        }
        Simulator& sim= Simulator::instance();
        if(p->sourceID != id_)
        {
            Packet *p1 = new Packet;
            p1->sourceID = id_;
            p1->destID = p->forwardID;
            p1->type = ACK;
            send(sim.nodes_[p->forwardID], p1);
        }
        
        int k=0;
        while(k<K){
            selectNextHop(k);
            pthread_mutex_lock(&sim.mutex_write);
            sim.fout <<"Time: "<<time(NULL)-sim.start_time<<", Packet ID: "<< p->idx
            << ", Forward Packet from:"<<id_<<" to "<<nextHop_->id_<<endl;
            pthread_mutex_unlock(&sim.mutex_write);
            p->belong_node = nextHop_->id_;
            send(nextHop_, p);
            time_t t1 = time(NULL);
            bool flag = 0;
            while(time(NULL)-t1 < GAP)
            {
                if(sim.Accept[id_][nextHop_->id_])
                {
                    flag = 1;
                    break;
                }
            }
            if (flag)
            {
                pthread_mutex_lock(&sim.mutex_Accept);
                sim.Accept[id_][nextHop_->id_] = 0;
                pthread_mutex_unlock(&sim.mutex_Accept);
                break;
            }
            k++;
        }
        dequeuePkt();
        p->is_drop = 1;
    }
    else{
        Packet *p = new Packet;
        p->sourceID = id_;
        p->destID = BROADCAST_ADDRESS;
        p->type = ACTIVATION;
        //cout<<"1 "<<id_<<endl;
        broadcast(p);
    }
}

// Broadcast a relaxation packet. This packet is transmitted without being queued.
int Node :: notifyRelax() {
    Packet *p = new Packet;
    p->sourceID = id_;
    p->destID = BROADCAST_ADDRESS;
    p->type = RELAXATION;
    
    return broadcast(p);
}

