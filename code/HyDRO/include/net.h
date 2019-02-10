//
//  net.h
//  HyDRO
//
//  Created by 王 on 2018/12/1.
//  Copyright © 2018 王. All rights reserved.
//

#ifndef net_h
#define net_h

#include <cstring>
#include "node.h"
#include "parameters.h"
#include <iostream>

typedef enum{DATA, ACK, RELAXATION, ACTIVATION} Notify_t;

struct Packet {
    //Header
    int sourceID, destID, forwardID;
    
    //Packet size
    int header_size;
    int payload_size;
    
    int idx;
    int belong_node;
    int is_drop=0;
    
    time_t born_time;
    
    Notify_t type;
    
    ~Packet() {}
    
    Packet(){
        sourceID = 0;
        destID = 0;
        header_size = HEADER_SIZE;
        payload_size = PAYLOAD_SIZE;
        type = ACTIVATION;
    }
    
    Packet(int s, int d, Notify_t t=DATA, int h = HEADER_SIZE, int p=PAYLOAD_SIZE){
        sourceID = s;
        destID = d;
        type = t;
        header_size = h;
        payload_size = p;
    }
};


#endif /* net_h */
