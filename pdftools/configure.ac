m4_include([stdcxx11.m4])
AC_PREREQ(2.62)
AC_INIT([pdftools],[0.4.0],[leonhad@gmail.com],[pdftools],[http://code.google.com/p/pdftools/])
AM_INIT_AUTOMAKE([-Wall tar-ustar])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

DEBUG_CFLAGS="-O3 -DNDEBUG -pedantic -Wall -funroll-loops"
dnl This mechanism allows one to enable debug compilations...
AC_ARG_ENABLE(debug,
[  --enable-debug     enable debugging and disable optimization],
[  if test "$enable_debug" = "yes"; then
      DEBUG_CFLAGS="-g3 -DDEBUG -pedantic -Wall -fno-inline"
   fi
])
AM_CONDITIONAL(ENABLE_DEBUG, test "$enable_debug" = "yes")
CXXFLAGS="$DEBUG_CFLAGS $CXXFLAGS"

# Checks for programs.
AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_AR
AC_PROG_RANLIB
AC_CHECK_TOOL(HELP2MAN, help2man, :)
AM_CONDITIONAL(HELP2MAN, [test "$HELP2MAN" != :])
AM_PROG_INSTALL_STRIP

# Checks for libraries.
AC_CHECK_LIB([z], [gzread], , [AC_MSG_ERROR([zlib not found, see http://www.zlib.net])])
AC_CHECK_LIB([iconv], [iconv_open])

# Checks for header files.
# AC_HEADER_STDC
AC_CHECK_HEADERS([iconv.h],,AC_MSG_ERROR(libiconv is missing))

# Checks for typedefs, structures, and compiler characteristics.
# AC_HEADER_STDBOOL
# AC_C_CONST

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/pdftools.h2m
                 src/nodes/Makefile
                 src/glyphs/Makefile
                 src/semantic/Makefile
                 src/xml/Makefile
                 src/zip/Makefile
                 src/html/Makefile
                 src/epub/Makefile
                 src/graphics/Makefile
])
AC_OUTPUT
