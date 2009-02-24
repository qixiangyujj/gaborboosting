INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src -I$(top_srcdir)/src/Gender \
	-I$(top_srcdir)/src/IntraExtraFaceReg -I$(top_srcdir)/src/MultiClassification -I$(top_srcdir)/src/PCA \
	-I$(top_srcdir)/src/Segmentation -I$(top_srcdir)/src/BinFaceReg $(all_includes) \
	-I/usr/local/include/opencv
METASOURCES = AUTO
genderdetect_LDADD = $(top_builddir)/src/libAdaGabor.a -lml -L/usr/local/lib \
	-lhighgui -lcxcore -lcv

bin_PROGRAMS = facetraining
facetraining_SOURCES = facetraining.cpp
facetraining_LDADD = $(top_builddir)/src/libAdaGabor.a
