//
//  energy.h
//  HyDRO
//
//  Created by 王 on 2018/12/2.
//  Copyright © 2018 王. All rights reserved.
//

#ifndef energy_h
#define energy_h

#include "node.h"

struct Node;

typedef enum{TX, RX} EnergyConsumption_t;
class Energy {
public:
    static void spend(Node*, EnergyConsumption_t, double);
private:
    static const double energyPerBitTx;
    static const double energyPerBitRx;
};


#endif /* energy_h */
