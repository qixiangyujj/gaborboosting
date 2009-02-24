INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/ClientBasedFaceReg \
	$(all_includes) -I/usr/local/include/opencv
METASOURCES = AUTO
bin_PROGRAMS = ClientFeatureSelec
ClientFeatureSelec_SOURCES = FeatureSelection.cpp
ClientFeatureSelec_LDADD = $(top_builddir)/src/libAdaGabor.a -lml -lhighgui \
	-lcxcore -lcv -L/usr/local/lib
