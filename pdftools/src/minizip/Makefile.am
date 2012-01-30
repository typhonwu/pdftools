noinst_LIBRARIES = libminizip.a
libminizip_a_SOURCES = \
	zip.c \
	ioapi.c

noinst_HEADERS = \
	zip.h \
	ioapi.h

libminizip_a_CFLAGS = -I.. $(OPENMP_CXXFLAGS) -DNOCRYPT

EXTRA_DIST = MiniZip64_info.txt
