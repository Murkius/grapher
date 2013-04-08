#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Main.h"


 
class Test {
    public:
        void run_tests();
    private:
        void graphReadingFromFile();
        void graphReadWriteConsistency();
        
        void insertionsReadWriteConsistency();
        
        void dequeOfEdgesToString();

        void bfs_1();
        void bfs_2();

        void franciosaRemoveEdgesPointingTo_1();
        void franciosaRemoveEdgesPointingTo_2();
        void franciosaEdgeWithMinimumDistance();
        void franciosaEdgeInsert_1();
        void franciosaEdgeInsert_2();
        void franciosaEdgeInsert_3();
};
 
#endif
