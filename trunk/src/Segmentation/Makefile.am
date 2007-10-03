INCLUDES = -I/usr/local/include/opencv $(all_includes)
METASOURCES = AUTO
bin_PROGRAMS = FaceSegment
FaceSegment_SOURCES = cvfacesegment.cpp FaceSeg.cpp
FaceSegment_LDADD = -L/usr/local/lib -lhighgui -lcv -lcxcore
noinst_HEADERS = cvfacesegment.h
