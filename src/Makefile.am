include $(top_srcdir)/flymakable.am

AM_CPPFLAGS=\
-I $(top_builddir)

bin_PROGRAMS=\
ccscope

noinst_HEADERS=\
Inotify.hpp

ccscope_SOURCES=\
Inotify.cpp \
ccscope.cpp

ccscope_LDFLAGS=\
-lpficommon
