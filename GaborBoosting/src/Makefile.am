bin_PROGRAMS = adaboostgabortraining MutInf GaborProcess adaboostgabortesting \
							prepare sign2weaks PairPoolPartition adaboostgabortraining.M1 GaborTesting \
		FindMutInf trainFERET testFERET
adaboostgabortraining_SOURCES = adaboostgabortraining.cpp

# set the include path found by configure
INCLUDES = $(all_includes) -I/usr/local/include/opencv

# the library search path.
adaboostgabortraining_LDFLAGS = $(all_libraries) 
noinst_HEADERS = CommonCVFunction.h GaborBoosting.h MatUsage.h PrepareData.h \
	cv2bins.h cvadaboost.h cvadaboostclientfeatureselection.h \
	cvadaboostdifffeatureselection.h cvadaboostfeatureselection.h cvbin.h cvbinarytrainingdata.h \
	cvbindiffgabadafeatureselect.h cvbingabadafeatureselect.h cventropy.h cvfaceclassifier.h cvfacedb.h \
	cvfacesvmclassifier.h cvfacesvmclientclassifier.h cvfacesvmdiffclassifier.h cvferet.h \
	cvgabadafeatureselect.h cvgabor.h cvgaborclientdatamaker.h cvgabordatamaker.h \
	cvgabordifferencedatamaker.h cvgaborfeature.h cvgaborfeaturepair.h cvgaborfeaturepairpool.h \
	cvgaborfeaturepairselect.h cvgaborfeaturepool.h cvirisdata.h cvknnweaklearner.h cvlookuptable.h \
	cvmultigabadafeatureselect.h cvmultigabadafsm1.h cvmultitrainingdata.h cvmutinf.h cvmweaklearner.h \
	cvpcafacereg.h cvpoolparams.h cvsubject.h cvtestingdata.h cvtrainingdata.h \
	cvweaklearner.h cvxm2vts.h makingfeaturedata.h
adaboostgabortraining_LDADD = $(top_builddir)/src/libAdaGabor.a -lcvaux -lcv \
	-lcxcore -lhighgui -L/usr/local/lib -lml



lib_LIBRARIES = libAdaGabor.a
libAdaGabor_a_SOURCES = MatUsage.cpp cv2bins.cpp cvadaboost.cpp \
	cvadaboostclientfeatureselection.cpp cvadaboostdifffeatureselection.cpp cvadaboostfeatureselection.cpp \
	cvbin.cpp	cventropy.cpp cvbinarytrainingdata.cpp cvbindiffgabadafeatureselect.cpp \
	cvbingabadafeatureselect.cpp cvfaceclassifier.cpp cvfacedb.cpp cvfacesvmclassifier.cpp \
	cvfacesvmclientclassifier.cpp cvfacesvmdiffclassifier.cpp cvferet.cpp cvgabadafeatureselect.cpp \
	cvgabor.cpp cvgaborclientdatamaker.cpp cvgabordatamaker.cpp \
	cvgabordifferencedatamaker.cpp cvgaborfeature.cpp	cvgaborfeaturepair.cpp cvgaborfeaturepairpool.cpp \
	cvgaborfeaturepairselect.cpp cvgaborfeaturepool.cpp	cvlookuptable.cpp cvgaborresponsedata.cpp \
	cvirisdata.cpp cvknnweaklearner.cpp cvmultigabadafeatureselect.cpp \
	cvmultigabadafsm1.cpp cvmultitrainingdata.cpp cvmutinf.cpp cvmweaklearner.cpp \
	cvpcafacereg.cpp cvpoolparams.cpp cvsubject.cpp cvtestingdata.cpp	cvtrainingdata.cpp \
	cvweaklearner.cpp cvxm2vts.cpp makingfeaturedata.cpp
MutInf_SOURCES = MutInformation.cpp
MutInf_LDADD = $(top_builddir)/src/libAdaGabor.a -lhighgui -lml \
	-L/usr/local/lib -lcxcore -lcv
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
adaboostgabortraining_M1_LDADD = $(top_builddir)/src/libAdaGabor.a -lhighgui \
	-L/usr/local/lib -lcv -lml -lcxcore
SUBDIRS = AdaBoost ClientBasedFaceReg Gender IntraExtraFaceReg \
	MultiClassification PCA Segmentation
PairPoolPartition_LDADD = $(top_builddir)/src/libAdaGabor.a -L/usr/local/lib \
	-lhighgui -lcxcore -lcv
GaborTesting_SOURCES = GaborTest.cpp
GaborTesting_LDADD = $(top_builddir)/src/libAdaGabor.a -lml -L/usr/local/lib \
	-lcv -lhighgui -lcxcore
FindMutInf_SOURCES = findMutInf.cpp
FindMutInf_LDADD = $(top_builddir)/src/libAdaGabor.a -lhighgui -L/usr/local/lib \
	-lcxcore -lcv
trainFERET_SOURCES = ferettraining.cpp
trainFERET_LDADD = $(top_builddir)/src/libAdaGabor.a -lhighgui -lml \
	-L/usr/local/lib -lcxcore -lcv
testFERET_SOURCES = ferettesting.cpp
testFERET_LDADD = $(top_builddir)/src/libAdaGabor.a -lhighgui -L/usr/local/lib \
	-lml -lcxcore -lcv
