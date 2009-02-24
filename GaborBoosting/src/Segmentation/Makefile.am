INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/Segmentation $(all_includes) \
	-I/usr/local/include/opencv
METASOURCES = AUTO
bin_PROGRAMS = FaceSegment resizeferet
FaceSegment_SOURCES = cvfacesegment.cpp FaceSeg.cpp
FaceSegment_LDADD = -L/usr/local/lib -lhighgui -lcv -lcxcore
noinst_HEADERS = cvfacesegment.h
resizeferet_SOURCES = resize.cpp
resizeferet_LDADD = $(top_builddir)/src/libAdaGabor.a -lhighgui \
	-L/usr/local/lib -lcxcore -lcv
