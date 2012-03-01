noinst_LIBRARIES = libglyphs.a
libglyphs_a_SOURCES = \
	glyph.cpp \
	paragraph.cpp \
	text.cpp \
	break.cpp

noinst_HEADERS = \
	glyphs.h \
	glyph.h \
	paragraph.h \
	text.h \
	break.h

libglyphs_a_CXXFLAGS = -I.. $(OPENMP_CXXFLAGS) $(XML_CPPFLAGS)
AM_CXXFLAGS = $(OPENMP_CXXFLAGS)
