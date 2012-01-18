#include "analyze.h"
#include "nodes.h"
#include <iostream>

using namespace std;

Analyze::Analyze()
{
}

Analyze::~Analyze()
{
}

Document *Analyze::analyseTree(RootNode* tree)
{
    if (!tree) {
        // Invalid tree
        return NULL;
    }
    vector<TreeNode *> root = tree->child();
    vector<TreeNode *>::iterator i;
    
    for (i = root.begin(); i < root.end(); i++) {
        XREFNode *ref = dynamic_cast<XREFNode *>(*i);
        if (ref) {
            cout << "Start address: " << ref->start_address() << endl;
        }
    }
    return NULL;
}
