INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/Gender \
	-I$(top_srcdir)/src/MultiClassification -I$(top_srcdir)/src/PCA -I$(top_srcdir)/src/Segmentation \
	-I$(top_srcdir)/src/IntraExtraFaceReg $(all_includes) -I/usr/local/include/opencv
METASOURCES = AUTO
bin_PROGRAMS = Training
Training_SOURCES = FaceTraining.cpp
Training_LDADD = $(top_builddir)/src/libAdaGabor.a -lml -lhighgui -lcxcore -lcv \
	-L/usr/local/lib
