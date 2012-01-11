bin_PROGRAMS = pdftools
pdftools_SOURCES = main.cpp \
	parser.cpp \
	scanner.cpp \
	token.cpp \
	utils.cpp \
	treenode.cpp
	
dist_man_MANS = pdftools.1

noinst_HEADERS = \
	parser.h \
	scanner.h \
	token.h \
	utils.h \
	treenode.h

if HELP2MAN
pdftools.1: pdftools
	-$(HELP2MAN) --output=$@ -N -n "is a high level tools to convert PDF files to AZW or ePUB formats." ./pdftools
endif