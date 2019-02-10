//
//  node.h
//  HyDRO
//
//  Created by 王 on 2018/12/1.
//  Copyright © 2018 王. All rights reserved.
//

#ifndef node_h
#define node_h

#include <vector>
#include <cstring>
#include "net.h"
#include "mac.h"


// Node type
typedef enum{CH, ACH, NCH, BS} NodeType_t;

struct CTable; //Forward declaration of 'CTable' from 'clTable.h' include below
struct CTableEntry;

// Represents a wireless sensor node
class Node{
public:
    std::vector<Packet> pktQueue_;
    CTable *chTable_, *achTable_; //The tabl {M} as represented in the protocol
    
    Node(){
        id_ = IDs_++;
        nodeType_ = NCH;
        maxEnergy_ = INIT_ENERGY;
        energy_ = INIT_ENERGY;
        location_.x = location_.y = location_.z = 0;
        nextHop_ = NULL;
        chTable_ = NULL;
        queueLimit_ = DEFAULT_QUEUE_LIMIT;
        memset(Vfuction, 0, sizeof(Vfuction));
        transmissionRange_ = MAX_RANGE;
        pktQueue_.clear();
        activeEntry.clear();
    }
    void init(int nneighbor);
    
    // Accessors
    int id() { return id_;}
    double maxEnergy() { return maxEnergy_;}
    double energy() { return energy_;}
    void energy(double val) {energy_ = val;}
    double harvest() {return harvest_;}
    void harvest(double val) {harvest_ = val;}
    double transmissionRange() { return transmissionRange_; }
    void transmissionRange(double val) { transmissionRange_ = val; }
    Cordinate location() { return location_; }
    void location(int x, int y, int z) { location_.x = x; location_.y = y; location_.z = z; }
    Node* nextHop() { return nextHop_; }
    void nodeType(NodeType_t t) { nodeType_ = t; }
    NodeType_t nodeType() { return nodeType_; }
    int cluster() { return clusterId_; }
    void cluster(int cl) { clusterId_ = cl; }
    
    //Functions
    void nextHop(Node *n) {nextHop_ = n;}
    void eventData(const char *data);
    int addNeighbour(int id);    // Add a neighbour to the neighbour list
    double distance(Node*, Node*); // Distance between two nodes
    Node* clusterHead() { return neighbours_[clusterId_]; }
    void generateCLTable();
    void generateACLTable();
    Node *findAssistantCH(); //Store and return ACH node
    //Just return the associated ACH node
    Node *assistantCH() {return assistantCH_; }
    
    int reachedThreshold(); // Check if energy level has reached a threshold
    
    // Perform routing logic
    void routeLogic();
    void updateActiveList();
    void selectNextHop(int k);
    void forwardData(); // This calls selectNextHop() and forwards packet to it.
    
    int enqueuePkt(Packet*);
    int dequeuePkt();
    int send(Node*, Packet*);    // Send the packet p to the node n(for special purposes)
    int recv(Packet*);    // Recieve and enqueu packet p
    int broadcast(Packet*);    // Broadcast packet p
    int notifyRelax();    // Broadcast a relaxation packet
    
private:
    static int IDs_;  // ID generated and ID count
    double maxEnergy_;
    int sendPacket; // Total packet send from i
    double Vfuction[K+2]; // Q-learning
    
    //MAC layer properties
    Cordinate location_;
    double energy_;
    double harvest_;
    double energy_threshold_1_ = ENERGY_THRESHOLD_1;
    double energy_threshold_2_ = ENERGY_THRESHOLD_2;
    double transmissionRange_;
    unsigned int queueLimit_; //Max pktqueue size after which the packets will be dropped.
    
    //Net layer properties
    int id_; //ID or address
    NodeType_t nodeType_; //CH,ACH,NCH,BS
    int clusterId_; // Cluster number (NOTE: ClusterID == ID of CH node)
    Node *nextHop_;    //Next hop node selected by routing protocol TODO
    Node **neighbours_; //List of neighbour nodes
    int nneighbors_;    // Num of neighbours
    Node *assistantCH_;    // ACH node of a CH node. This remains NULL for NCH and ACH type nodes
    std::vector<CTableEntry*> activeEntry;
    
};

#include "cTable.h"

#endif /* node_h */
