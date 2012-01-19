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
            MapNode *trailer = dynamic_cast<MapNode *>(ref->trailer());
            map<string, TreeNode *> values = trailer->values();
            map<string, TreeNode *>::iterator m = values.begin();
            
            while(m != values.end()) {
                if ((*m).first == "/Root") {
                    RefNode *ref = dynamic_cast<RefNode *>((*m).second);
                    cout << "Root " << ref->id() << ":" << ref->generation() << endl;
                } else if  ((*m).first == "/Info") {
                    RefNode *ref = dynamic_cast<RefNode *>((*m).second);
                    cout << "Info " << ref->id() << ":" << ref->generation() << endl;
                }
                m++;
            }
            cout << endl;
        }
    }
    return NULL;
}
