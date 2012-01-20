#include "page.h"
#include <cstdlib>
#include <stdint.h>

Page::Page()
{
    m_media_box = NULL;
    m_crop_box = NULL;
}

Page::~Page()
{
    if (m_media_box) delete [] m_media_box;
    if (m_crop_box) delete [] m_crop_box;
}

void Page::set_media_box(int a, int b, int c, int d)
{
    m_media_box = new int[4];
    m_media_box[0] = a;
    m_media_box[1] = b;
    m_media_box[2] = c;
    m_media_box[3] = d;
}

void Page::set_crop_box(int a, int b, int c, int d)
{
    m_crop_box = new int[4];
    m_crop_box[0] = a;
    m_crop_box[1] = b;
    m_crop_box[2] = c;
    m_crop_box[3] = d;
    
}
