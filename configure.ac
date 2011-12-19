#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(ccscope, 0.1, n.ohkawa@gmail.com)
AM_INIT_AUTOMAKE([foreign -W -Wall -Wno-portability])
AC_CONFIG_SRCDIR([src/ccscope.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AX_PTHREAD

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([memmove select])

# Treat warnings as errors
CFLAGS="${CFLAGS} -Wall -Wextra -Werror -ansi -pedantic"
CXXFLAGS="${CXXFLAGS} -Wall -Wextra -Werror -std=c++0x -pedantic"

# Check for -Wno-error=unused-parameter
AC_CACHE_CHECK([for gcc -Wno-error=unused-parameter support],
               ac_cv_c_gcc_no_error_unused_parameter,
               ac_cv_c_gcc_no_error_unused_parameter="undefined")
if test "x$ac_cv_c_gcc_no_error_unused_parameter" = "xundefined"
then
  AX_GCC_OPTION([-Wno-error=unused-parameter],[],[],
                ac_cv_c_gcc_no_error_unused_parameter="yes",
                ac_cv_c_gcc_no_error_unused_parameter="no")
fi
if test "x$ac_cv_c_gcc_no_error_unused_parameter" = "xyes"
then
  CFLAGS="${CFLAGS} -Wno-error=unused-parameter"
  CXXFLAGS="${CXXFLAGS} -Wno-error=unused-parameter"
else
  CFLAGS="${CFLAGS} -Wno-unused-parameter"
  CXXFLAGS="${CXXFLAGS} -Wno-unused-parameter"
fi


AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
