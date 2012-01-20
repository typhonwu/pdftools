#ifndef PAGE_H
#define PAGE_H

class Page {
private:
    int *m_media_box;
    int *m_crop_box;
    
public:
    Page();
    ~Page();
    
    void set_media_box(int a, int b, int c, int d);
    void set_crop_box(int a, int b, int c, int d);
};


#endif
