//
//  parameters.h
//  HyDRO
//
//  Created by 王 on 2018/12/1.
//  Copyright © 2018 王. All rights reserved.
//

#ifndef parameters_h
#define parameters_h

#define K 4
#define L 1000000
#define LAMBDA 5
#define SOLAR 4
#define VIBRATION 1

#define NODE_NUMBER 20

#define SIMULATE_TIME 20 //min

#define MAX_X 2000
#define MAX_Y 1000
#define MIN_Z 10
#define MAX_Z 240
#define MAX_RANGE 800

#define HEADER_SIZE 15
#define PAYLOAD_SIZE 1000
#define INIT_ENERGY 5100

#define ENERGY_THRESHOLD_1 5000
#define ENERGY_THRESHOLD_2 5100

#define DEFAULT_QUEUE_LIMIT 3 // modified
#define BIT_RATE 4000.0

#define BROADCAST_ADDRESS -2
#define UNKNOWN_ADDRESS -1

#define ENERGY_PERBITTX 8.5
#define ENERGY_PERBITRX 0.5

#define DISCOUNT_FACTOR 0.95

#define GAP 1
#define TIME_THRESHOLD 900

#endif /* parameters_h */
