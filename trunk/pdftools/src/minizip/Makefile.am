noinst_LIBRARIES = libminizip.a
libminizip_a_SOURCES = \
	zip.cpp \
	ioapi.cpp

noinst_HEADERS = \
	zip.h \
	ioapi.h \
	iowin32.h

libminizip_a_CXXFLAGS = -I.. $(OPENMP_CXXFLAGS) -DNOCRYPT -DUSE_FILE32API

EXTRA_libminizip_a_SOURCES = iowin32.cpp

EXTRA_DIST = MiniZip64_info.txt

libminizip_a_LIBADD = $(MINIZIP_DEPENDENCIES)
libminizip_a_DEPENDENCIES = $(MINIZIP_DEPENDENCIES)