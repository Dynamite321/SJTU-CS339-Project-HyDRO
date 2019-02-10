//
//  main.cpp
//  HyDRO
//
//  Created by 王 on 2018/12/1.
//  Copyright © 2018 王. All rights reserved.
//
#include "simulator.h"

int main(int argc, const char * argv[])
{
    (new Simulator);
    Simulator &sim = Simulator::instance();
    sim.init();
    sim.run();
    
    if (sim.totalPacketNum > 0)
    {
        cout << "Packet delivery ratio: " << sim.acceptPacketNum / double(sim.totalPacketNum) << endl;
        //cout<< "Energy Cost: "<<sim.consume_energy<<endl;
    }
    else
    {
        cout << "Total packet number is 0!!!" << endl;
    }
    
    for (int i = 0; i < NODE_NUMBER-1; ++i)
    {
        cout << "Rest energy of node " << i << ": " << sim.nodes_[i]->energy() << endl;
    }
    
    return 0;
}
