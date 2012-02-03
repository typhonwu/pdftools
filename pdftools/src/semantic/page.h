#ifndef PAGE_H
#define PAGE_H

#include <string>

using namespace std;

class Page {
private:
    int *m_media_box;
    int *m_crop_box;
    int m_id;
    int m_generation;
    string m_link;
    
public:
    Page();
    ~Page();
    
    void set_media_box(int a, int b, int c, int d);
    void set_crop_box(int a, int b, int c, int d);
    void set_destination(int id, int generation);
    int id();
    int generation();
    void set_link(char *link);
    const char *link();
};


#endif
