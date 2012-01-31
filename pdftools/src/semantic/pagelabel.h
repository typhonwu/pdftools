#ifndef PAGELABEL_H
#define PAGELABEL_H

#include <string>

using namespace std;

enum page_type {
    ARABIC,
    UPCASE_ROMAN,
    LOWCASE_ROMAN,
    UPCASE_LETTERS,
    LOWCASE_LETTERS
};

class PageLabel {
private:
    int m_start_number;
    int m_range;
    page_type m_type;
    string m_name;
    
public:
    PageLabel(int start, int range, page_type type, string &name);
    
    int start();
    int range();
    page_type type();
    string &name();
};

#endif
