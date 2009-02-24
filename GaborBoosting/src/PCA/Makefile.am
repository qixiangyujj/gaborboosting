INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/Gender \
	-I$(top_srcdir)/src/MultiClassification -I$(top_srcdir)/src/PCA -I$(top_srcdir)/src/Segmentation \
	-I/usr/local/include/opencv $(all_includes)
METASOURCES = AUTO
bin_PROGRAMS = PCAtesting PCAtraining


PCAtraining_SOURCES = pcatraining.cpp
PCAtraining_LDADD = $(top_builddir)/src/libAdaGabor.a -lcvaux -L/usr/local/lib \
	-lml -lhighgui -lcxcore -lcv
