#ifndef TEST_H
#define TEST_H

#include <assert.h>
 
class Test {
    public:
        void run_tests();
    private:
        void graphReadingFromFile();
        void dequeOfEdgesToString();
        void franciosaRemoveEdgesPointingTo();
        void franciosaRemoveEdgesPointingTo2();
        void franciosaEdgeWithMinimumDistance();
};
 
#endif
