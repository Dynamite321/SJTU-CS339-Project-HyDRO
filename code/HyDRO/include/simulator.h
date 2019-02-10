//
//  simulator.h
//  HyDRO
//
//  Created by 王 on 2018/12/2.
//  Copyright © 2018 王. All rights reserved.
//

#ifndef simulator_h
#define simulator_h

#include <iostream>
#include <cstdlib>
#include <thread>
#include <ctime>
#include <cmath>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include "node.h"
#include "parameters.h"

using namespace std;

class Node;

//std::thread thread_pool[NODE_NUMBER];

class Simulator
{
private:
    int nnode_;
    int nclusters_;
    int dropPacketNum;
    
    double randExp(double lambda)
    {
        double pV = 0.0;
        while(true)
        {
            pV = (double)rand()/(double)RAND_MAX;
            if (pV != 1)
            {
                break;
            }
        }
        pV = (-1.0/lambda)*log(1-pV);
        return pV;
    }
    
public:
    thread thread_pool[NODE_NUMBER];
    static Simulator* instance_;
    Node** nodes_;
    Node* bs_;
    bool Accept[NODE_NUMBER][NODE_NUMBER];
    int acc_num[NODE_NUMBER][NODE_NUMBER];
    int send_num[NODE_NUMBER];
    double P_ij[NODE_NUMBER][NODE_NUMBER];
    double V[NODE_NUMBER];
    int totalPacketNum;
    int acceptPacketNum;
    bool* is_alive;
    double solar_energy = 0;
    double vibration_energy = 0;
    pthread_mutex_t mutex_Accept;
    pthread_mutex_t mutex_acc_num;
    pthread_mutex_t mutex_send_num;
    pthread_mutex_t mutex_write;
    pthread_mutex_t mutex_accPacket;
    pthread_mutex_t mutex_solar;
    pthread_mutex_t mutex_vibration;
    ofstream fout;
    time_t start_time;
    
    
    Simulator();
    ~Simulator();
    
    void init();
    static Simulator& instance()
    {
        return (*instance_);        // general access to scheduler
    }
    Node* node(int i)
    {
        if(i < nnode_)
            return (nodes_[i]);
        else
            return NULL;
        
    }
    Node** nodePtr(int i)
    {
        if(i < nnode_)
            return (nodes_+i);
        else return NULL;
        
    }
    Node* baseStation()
    {
        return bs_;
    }
    int nClusters()
    {
        return nclusters_;
    }
    void producePacket();
    void killNode(Node*);
    void runCHNode(int i);
    void addEnergy();
    void run();
};


#endif /* simulator_h */
