SUBDIRS = nodes

bin_PROGRAMS = pdftools
pdftools_SOURCES = main.cpp \
	parser.cpp \
	scanner.cpp \
	token.cpp \
	utils.cpp

dist_man_MANS = pdftools.1

noinst_HEADERS = \
	parser.h \
	scanner.h \
	token.h \
	utils.h

pdftools_CXXFLAGS = -Inodes
pdftools_LDADD = -Lnodes -lnodes

if HELP2MAN
pdftools.1: pdftools$(EXEEXT) pdftools.h2m
	@echo generating help file $@...
	@$(HELP2MAN) --output=$@ -N -i pdftools.h2m ./pdftools$(EXEEXT)
endif

EXTRA_DIST = pdftools.h2m
