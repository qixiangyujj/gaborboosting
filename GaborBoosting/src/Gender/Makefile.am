INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/Gender \
	-I/usr/local/include/opencv $(all_includes)
METASOURCES = AUTO
bin_PROGRAMS = genderdetect Gendertesting
genderdetect_SOURCES = genderdetecting.cpp
genderdetect_LDADD = $(top_builddir)/src/libAdaGabor.a -lml -L/usr/local/lib \
	-lhighgui -lcxcore -lcv
Gendertesting_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib -lml\
	-lcv -lcxcore -lhighgui
Gendertesting_SOURCES = Gendertesting.cpp

