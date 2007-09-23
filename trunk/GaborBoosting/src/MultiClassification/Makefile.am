INCLUDES = -I$(top_srcdir)/src -I/usr/local/include/opencv $(all_includes)
METASOURCES = AUTO
bin_PROGRAMS = allfeaturetraining
allfeaturetraining_SOURCES = allfeaturetraining.cpp
allfeaturetraining_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib \
	-lml -lcxcore -lcv
allfeaturetraining_LDFLAGS = -all-static
