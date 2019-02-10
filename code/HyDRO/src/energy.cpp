//
//  energy.cpp
//  HyDRO
//
//  Created by 王 on 2018/12/3.
//  Copyright © 2018 王. All rights reserved.
//


#include "energy.h"
#include "simulator.h"


const double Energy :: energyPerBitTx = ENERGY_PERBITTX;
const double Energy :: energyPerBitRx = ENERGY_PERBITRX;

// Reduce energy level according operation type. Then check whether it is required to broadcast a relaxation packet.
void Energy :: spend(Node *n, EnergyConsumption_t type, double time) {
    Simulator &sim = Simulator::instance();
    if(n == sim.bs_)
        return;
    double energy = n->energy();
    double consume = 0;
    
    if(type == TX)
        consume =  energyPerBitTx * time;
    else if(type == RX)
        consume =  energyPerBitRx * time;
    
    if(n->harvest() == SOLAR)
    {
        pthread_mutex_lock(&sim.mutex_solar);
        sim.solar_energy += consume;
        pthread_mutex_unlock(&sim.mutex_solar);
    }
    else
    {
        pthread_mutex_lock(&sim.mutex_vibration);
        sim.vibration_energy += consume;
        pthread_mutex_unlock(&sim.mutex_vibration);
    }
    n->energy(energy - consume);
    // Check for threshold
    if(n->reachedThreshold() == -1 && sim.is_alive[n->id()]==true)
    {
        sim.killNode(n);
        n->notifyRelax();
    }

}
