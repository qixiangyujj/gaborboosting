bin_PROGRAMS = adaboostgabortraining MutInf GaborProcess adaboostgabortesting \
					prepare sign2weaks PairPoolPartition adaboostgabortraining.M1 GaborTesting
adaboostgabortraining_SOURCES = adaboostgabortraining.cpp

# set the include path found by configure
INCLUDES = -I/usr/local/include/opencv $(all_includes)

# the library search path.
adaboostgabortraining_LDFLAGS = $(all_libraries) 
noinst_HEADERS = cvadaboost.h cvbinarytrainingdata.h cvgabadafeatureselect.h \
						cvgaborfeature.h cvgaborfeaturepool.h cvgabor.h cvmultitrainingdata.h cvtestingdata.h \
						cvtrainingdata.h cvweaklearner.h cvgaborfeaturepair.h cvgaborfeaturepairpool.h \
						cvmutinf.h cventropy.h cvlookuptable.h cvbin.h cv2bins.h \
						cvgaborfeaturepairselect.h cvfacedb.h cvxm2vts.h cvpoolparams.h PrepareData.h makingfeaturedata.h \
				cvmultigabadafeatureselect.h cvmultigabadafsm1.h cvmweaklearner.h MatUsage.h cvferet.h cvsubject.h
adaboostgabortraining_LDADD = $(top_builddir)/src/libAdaGabor.a -lcv -lcxcore \
	-lhighgui -L/usr/local/lib -lml



lib_LIBRARIES = libAdaGabor.a
libAdaGabor_a_SOURCES = cvadaboost.cpp cvbinarytrainingdata.cpp \
						cvbin.cpp	cventropy.cpp cvgabadafeatureselect.cpp cvgabor.cpp \
						cvgaborfeature.cpp	cvgaborfeaturepair.cpp cvgaborfeaturepairpool.cpp cvgaborfeaturepool.cpp	cvlookuptable.cpp \
						cvmultitrainingdata.cpp cvmutinf.cpp cvtestingdata.cpp	cvtrainingdata.cpp cvweaklearner.cpp \
						cv2bins.cpp cvgaborfeaturepairselect.cpp cvfacedb.cpp cvxm2vts.cpp cvpoolparams.cpp \
					makingfeaturedata.cpp cvmultigabadafeatureselect.cpp cvmultigabadafsm1.cpp cvmweaklearner.cpp \
		MatUsage.cpp cvferet.cpp cvsubject.cpp
MutInf_SOURCES = MutInformation.cpp
MutInf_LDADD = $(top_builddir)/src/libAdaGabor.a -lml -L/usr/local/lib -lcxcore \
	-lcv
GaborProcess_SOURCES = DataGen.cpp
GaborProcess_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib -lml \
	-lhighgui -lcxcore -lcv
adaboostgabortesting_SOURCES = adaboostgabortesting.cpp
adaboostgabortesting_LDADD = $(top_builddir)/src/libAdaGabor.a -lml \
	-L/usr/local/lib -lhighgui -lcxcore -lcv
prepare_SOURCES = prepare.cpp
prepare_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib -lml \
	-lhighgui -lcv -lcxcore
sign2weaks_SOURCES = sign2weaks.cpp
sign2weaks_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib -lml\
	-lhighgui -lcxcore -lcv



PairPoolPartition_SOURCES = PoolPartition.cpp
adaboostgabortraining_M1_SOURCES = adaboostgabor.M1.cpp
adaboostgabortraining_M1_LDADD = $(top_builddir)/src/libAdaGabor.a \
	-L/usr/local/lib -lcv -lml -lcxcore
SUBDIRS = MultiClassification Segmentation
PairPoolPartition_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib \
	-lhighgui -lcxcore -lcv
GaborTesting_SOURCES = GaborTest.cpp
GaborTesting_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib -lcv \
	-lhighgui -lcxcore
