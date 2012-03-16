noinst_LIBRARIES = libglyphs.a
libglyphs_a_SOURCES = \
	glyph.cpp \
	paragraphglyph.cpp \
	textglyph.cpp \
	breakglyph.cpp \
	fontglyph.cpp \
	textmatrixglyph.cpp

noinst_HEADERS = \
	glyphs.h \
	glyph.h \
	paragraphglyph.h \
	textglyph.h \
	breakglyph.h \
	fontglyph.h \
	textmatrixglyph.h

libglyphs_a_CXXFLAGS = -I.. $(OPENMP_CXXFLAGS) $(XML_CPPFLAGS)
AM_CXXFLAGS = $(OPENMP_CXXFLAGS)
