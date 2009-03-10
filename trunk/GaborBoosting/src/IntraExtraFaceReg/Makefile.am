INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/Gender \
	-I$(top_srcdir)/src/MultiClassification -I$(top_srcdir)/src/PCA -I$(top_srcdir)/src/Segmentation \
	-I$(top_srcdir)/src/IntraExtraFaceReg $(all_includes) -I/usr/local/include/opencv
METASOURCES = AUTO
bin_PROGRAMS = Selection Testing Training
Training_SOURCES = FaceTraining.cpp
Training_LDADD = $(top_builddir)/src/libAdaGabor.a -lcv -lcxcore -lhighgui -lml
Selection_SOURCES = FaceFeatureSelection.cpp
Selection_LDADD = $(top_builddir)/src/libAdaGabor.a -lml -lhighgui -lcxcore \
	-lcv
