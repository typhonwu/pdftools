noinst_LIBRARIES = libsemantic.a
libsemantic_a_SOURCES = \
	document.cpp \
	page.cpp \
	pagelabel.cpp \
	outline.cpp \
	glyph.cpp \
	paragraph.cpp \
	text.cpp \
	end.cpp

noinst_HEADERS = \
	document.h \
	page.h \
	pagelabel.h \
	outline.h \
	glyph.h \
	paragraph.h \
	text.h \
	end.h

libsemantic_a_CXXFLAGS = -I.. $(OPENMP_CXXFLAGS) $(XML_CPPFLAGS)
AM_CXXFLAGS = $(OPENMP_CXXFLAGS)
