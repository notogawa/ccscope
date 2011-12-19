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

ccscope_CXXFLAGS=\
$(AM_CXXFLAGS) \
@PTHREAD_CFLAGS@

ccscope_LDFLAGS=\
-lpficommon
