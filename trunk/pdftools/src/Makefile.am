SUBDIRS = nodes glyphs semantic zip xml html epub graphics

bin_PROGRAMS = pdftools
pdftools_SOURCES = main.cpp \
	parser.cpp \
	scanner.cpp \
	token.cpp \
	utils.cpp \
	analyze.cpp \
	converter.cpp \
	generator.cpp \
	pageparser.cpp \
	pageanalyze.cpp \
	cmapparser.cpp \
	genericparser.cpp

dist_man_MANS = pdftools.1

noinst_HEADERS = \
	parser.h \
	scanner.h \
	token.h \
	utils.h \
	analyze.h \
	converter.h \
	generator.h \
	pageparser.h \
	pageanalyze.h \
	cmapparser.h \
	genericparser.h

pdftools_CXXFLAGS = 
pdftools_LDADD = \
	semantic/libsemantic.a \
	epub/libepub.a \
	zip/libzip.a \
	html/libhtml.a \
	xml/libxml.a \
	glyphs/libglyphs.a \
	graphics/libgraphics.a \
	nodes/libnodes.a \
	$(XML_LIBS)

if HELP2MAN
pdftools.1: pdftools$(EXEEXT) pdftools.h2m
	@echo generating help file $@...
	@$(HELP2MAN) --output=$@ -N -i pdftools.h2m ./pdftools$(EXEEXT)
endif

