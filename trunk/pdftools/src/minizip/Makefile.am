noinst_LIBRARIES = libminizip.a
libminizip_a_SOURCES = \
	zip.c 

noinst_HEADERS = \
	zip.h \
	ioapi.h \
	iowin32.h

libminizip_a_CFLAGS = -I.. $(OPENMP_CXXFLAGS) -DNOCRYPT

EXTRA_libminizip_a_SOURCES = iowin32.c ioapi.c

EXTRA_DIST = MiniZip64_info.txt

libminizip_a_LIBADD = $(MINIZIP_DEPENDENCIES)
libminizip_a_DEPENDENCIES = $(MINIZIP_DEPENDENCIES)