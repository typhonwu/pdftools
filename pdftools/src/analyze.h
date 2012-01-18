#ifndef ANALYSE_H
#define ANALYSE_H

#include "document.h"
#include "rootnode.h"

class Analyze {
private:
    
public:
    Analyze();
    ~Analyze();
    
    Document *analyseTree(RootNode *tree);
};

#endif
