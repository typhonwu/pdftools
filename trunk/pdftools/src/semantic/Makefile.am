noinst_LIBRARIES = libsemantic.a
libsemantic_a_SOURCES = \
	document.cpp \
	page.cpp \
	pagelabel.cpp \
	outline.cpp \
	glyph.cpp \
	paragraph.cpp \
	text.cpp \
	break.cpp \
	context.cpp \
	font.cpp

noinst_HEADERS = \
	document.h \
	page.h \
	pagelabel.h \
	outline.h \
	glyph.h \
	paragraph.h \
	text.h \
	break.h \
	context.h \
	font.h

libsemantic_a_CXXFLAGS = -I.. $(OPENMP_CXXFLAGS) $(XML_CPPFLAGS)
AM_CXXFLAGS = $(OPENMP_CXXFLAGS)
EXTRA_DIST = glyphs.h