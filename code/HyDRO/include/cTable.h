//
//  cTable.h
//  HyDRO
//
//  Created by 王 on 2018/12/2.
//  Copyright © 2018 王. All rights reserved.
//

#ifndef cTable_h
#define cTable_h


// Data types for a Cluster Neighbour Table
typedef enum{RELAX, ACTIVE, DEAD} State_t;
struct CTableEntry {
    Node *node_;    // Address of node
    double distance_to_me_;
    double distance_to_BS_;
    
    State_t currentState_;
    Node *assistantNode_;

    time_t lastTime ; // when lastTime==900, we will set state to RELAX
    
    CTableEntry();
};

struct CTable {
    CTable() { entryCounter_ = maxEntries_ = 0; }
    CTable(int entries);
    
    int entryCounter_, maxEntries_;
    CTableEntry **entry_;
    void addEntry(CTableEntry*);
    void sortByBSDistance();
    CTableEntry* maxEnergyEntry();
};


#endif /* cTable_h */
