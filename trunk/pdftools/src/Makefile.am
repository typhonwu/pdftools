SUBDIRS = nodes semantic

bin_PROGRAMS = pdftools
pdftools_SOURCES = main.cpp \
	parser.cpp \
	scanner.cpp \
	token.cpp \
	utils.cpp \
	analyze.cpp \
	converter.cpp

dist_man_MANS = pdftools.1

noinst_HEADERS = \
	parser.h \
	scanner.h \
	token.h \
	utils.h \
	analyze.h \
	converter.h

pdftools_CXXFLAGS = -Inodes -Isemantic
pdftools_LDADD = -Lnodes -lnodes -Lsemantic -lsemantic

if HELP2MAN
pdftools.1: pdftools$(EXEEXT) pdftools.h2m
	@echo generating help file $@...
	@$(HELP2MAN) --output=$@ -N -i pdftools.h2m ./pdftools$(EXEEXT)
endif

