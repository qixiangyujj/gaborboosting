INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/ClientBasedFaceReg \
	$(all_includes) -I/usr/local/include/opencv
METASOURCES = AUTO
bin_PROGRAMS = ClientFeatureSelec ClientTraining
ClientFeatureSelec_SOURCES = FeatureSelection.cpp
ClientFeatureSelec_LDADD = $(top_builddir)/src/libAdaGabor.a -lml -lhighgui \
	-lcxcore -lcv -L/usr/local/lib
ClientTraining_SOURCES = Training.cpp
ClientTraining_LDADD = $(top_builddir)/src/libAdaGabor.a -lhighgui -lml \
	-lcxcore -lcv
