#ifndef TEST_H
#define TEST_H

#include <assert.h>
 
class Test {
    public:
        void run_tests();
    private:
        void graphReadingFromFile();
        void dequeOfEdgesToString();

        void bfs_1();
        void bfs_2();

        void franciosaRemoveEdgesPointingTo_1();
        void franciosaRemoveEdgesPointingTo_2();
        void franciosaEdgeWithMinimumDistance();
        void franciosaEdgeInsert_1();
};
 
#endif
