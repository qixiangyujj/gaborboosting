INCLUDES = -I$(top_srcdir)/src -I/usr/local/include/opencv $(all_includes)
METASOURCES = AUTO
bin_PROGRAMS = allfeaturetraining update
allfeaturetraining_SOURCES = allfeaturetraining.cpp
allfeaturetraining_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib \
	-lml -lcxcore -lcv

update_SOURCES = minandupdate.cpp
update_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib -lml -lcxcore \
	-lcv

allfeaturetraining_LDFLAGS = -all-static
update_LDFLAGS = -all-static
