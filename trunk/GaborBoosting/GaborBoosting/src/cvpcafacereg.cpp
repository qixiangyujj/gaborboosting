/***************************************************************************
 *   Copyright (C) 2007 by Mian Zhou   *
 *   M.Zhou@reading.ac.uk   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "cvpcafacereg.h"

CvPCAFaceReg::CvPCAFaceReg()
{
  isImgLoaded = false;
  svm = new CvSVM;
}


CvPCAFaceReg::~CvPCAFaceReg()
{
  clear();
}




/*!
    \fn CvPCAFaceReg::setDB(CvFaceDB* db)
 */
void CvPCAFaceReg::setDB(CvFaceDB* db)
{
  database = db->clone();
  char * dbname = database->getName();
  if( !strcmp(dbname, "XM2VTS")) this->db_type = XM2VTS;
  else if ( !strcmp(dbname, "FERET")) this->db_type = FERET;
  printf("...............a %s database copied!\n\n", dbname);
  
  if(db_type == XM2VTS)
  {
    int nClass = ((CvXm2vts*)database)->get_num_sub();
    int nperClass = ((CvXm2vts*)database)->get_num_pic();

    nTrainImages = nClass*nperClass;
//     classIdx = cvCreateMat(nsamples, 1, CV_32SC1);
//     int n = 0;
//     for(int i = 0; i < nClass; i++)
//     {
//       for(int j = 0; j < nperClass; j++)
//       {
//         cvSetReal1D(classIdx, n, i+1);
//         n++;
//       }
//     }
  }
  else if(db_type == FERET)
  {
//     nClass = ((CvFeret*)database)->getSub();
//     nsamples = ((CvFeret*)database)->getNum();
//     classIdx = cvCreateMat(nsamples, 1, CV_32SC1);
//     int n = 0;
//     for(int i = 0; i < nClass; i++)
//     {
//       CvSubject subject = ((CvFeret*)database)->getSubject(i);
//       int num = subject.getnum();
//       int id = subject.getId();
//       for(int j = 0; j < num; j++)
//       {
//         cvSetReal1D(classIdx, n, id);
//         n++;
//       }
//     }
  }
}


/*!
    \fn CvPCAFaceReg::pca()
 */
void CvPCAFaceReg::pca()
{

  loadFaceImgArray( database );
  
  
  
  if( nTrainImages < 2)
  {
    fprintf(stderr, "Need 2 or more training images\n"
	  "Input file contains only %d\n", nTrainImages);
    return;
  }
  
  doPCA();
  
  projectedTrainFaceMat = cvCreateMat(nTrainImages, nEigens, CV_32FC1);
  for (int i = 0; i < nTrainImages; i++)
  {
    cvEigenDecomposite(
	  faceImgArr[i],
	  nEigens,
	  eigenVectArr,
	  0, 0,
	  pAvgTrainImg,
	  projectedTrainFaceMat->data.fl + i*nEigens);
  }

}


/*!
    \fn CvPCAFaceReg::recognize()
 */
void CvPCAFaceReg::recognize()
{
    /// @todo implement me
}


/*!
    \fn CvPCAFaceReg::doPCA()
 */
void CvPCAFaceReg::doPCA()
{
  
  CvTermCriteria calcLimit;
  CvSize faceImgSize;
  
  nEigens = nTrainImages - 1;
  //nEigens = 10;
  
  
  faceImgSize.width = faceImgArr[0]->width;
  faceImgSize.height = faceImgArr[0]->height;
  eigenVectArr = (IplImage**)cvAlloc(sizeof(IplImage *)*nEigens);

 
  for(int i = 0; i < nEigens; i++)
  {
    eigenVectArr[i] = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);
  }

  eigenValMat = cvCreateMat( 1, nEigens, CV_32FC1 );

  pAvgTrainImg = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);
  
  calcLimit = cvTermCriteria( CV_TERMCRIT_ITER, nEigens, 1 );
  
  printf("\nCalculate PCA ............\n");
  cvCalcEigenObjects(
       nTrainImages,
       (void*)faceImgArr,
       (void*)eigenVectArr,
       CV_EIGOBJ_NO_CALLBACK,
       0,
       0,
       &calcLimit,
       pAvgTrainImg,
       eigenValMat->data.fl);
  printf("   done!\n");
  
  
  printf("%d Eigen Vectors are generated.\n", nEigens);
  
  
}


/*!
    \fn CvPCAFaceReg::storeTrainingData( const char * filename )
 */
void CvPCAFaceReg::storeTrainingData( const char * filename )
{
  CvFileStorage * fileStorage;
  fileStorage = cvOpenFileStorage( filename, 0, CV_STORAGE_WRITE );
  
  cvWriteInt( fileStorage, "nEigens", nEigens);
  cvWriteInt( fileStorage, "nTrainImages", nTrainImages );
  cvWrite( fileStorage, "eigenValMat", eigenValMat, cvAttrList(0,0));
  cvWrite( fileStorage, "projectedTrainFaceMat", projectedTrainFaceMat, cvAttrList(0,0));
  cvWrite( fileStorage, "avgTrainImg", pAvgTrainImg,  cvAttrList(0,0));
  for (int i = 0; i < nEigens; i++)
  {
     char varname[200];
     sprintf( varname, "eigenVect_%d", i );
     cvWrite( fileStorage, varname, eigenVectArr[i], cvAttrList(0,0) );
  }
  cvReleaseFileStorage( &fileStorage );
}


/*!
    \fn CvPCAFaceReg::loadFaceImgArray(CvFaceDB *db)
 */
int CvPCAFaceReg::loadFaceImgArray(CvFaceDB *db)
{
  printf("Loading training images ............ \n");
  int nFace;
  if(db_type == XM2VTS)
  {
    char *filename = new char[50];
    int nClass = ((CvXm2vts*)database)->get_num_sub();
    int nperClass = ((CvXm2vts*)database)->get_num_pic();

    faceImgArr = (IplImage **)cvAlloc(nTrainImages*sizeof(IplImage *));
    
    
    
    nFace = 0;
    for (int i = 1; i <= nClass; i++)
    {
      for(int j = 1; j <= nperClass; j++)
      {
        ((CvXm2vts*)database)->getfilename( i, j, filename );
        faceImgArr[nFace] = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
        nFace++;

      }
    }
    delete filename;
  }
  else if(db_type == FERET)
  {
  }
  printf(" done!\n");
  isImgLoaded = true;
  return nFace;
}




/*!
    \fn CvPCAFaceReg::showAvgImage()
 */
void CvPCAFaceReg::showAvgImage()
{
  IplImage *newimage = cvCreateImage( cvGetSize(pAvgTrainImg), IPL_DEPTH_8U, 1 );
  cvConvertScaleAbs( (IplImage*)pAvgTrainImg, (IplImage*)newimage, 1, 0 );
  cvNamedWindow("Average Image",1);
  cvShowImage("Average Image",newimage);
  cvWaitKey(0);
  cvDestroyWindow("Average Image");
  cvReleaseImage(&newimage);
}


/*!
    \fn CvPCAFaceReg::saveAvgImage(const char* filename)
 */
void CvPCAFaceReg::saveAvgImage(const char* filename)
{
  IplImage *newimage = cvCreateImage( cvGetSize(pAvgTrainImg), IPL_DEPTH_8U, 1 );
  cvConvertScaleAbs( (IplImage*)pAvgTrainImg, (IplImage*)newimage, 1, 0 );
  cvSaveImage( filename, (IplImage*)newimage );
  cvReleaseImage(&newimage);
}


/*!
    \fn CvPCAFaceReg::showEigenImage(int n)
 */
void CvPCAFaceReg::showEigenImage(int n)
{
  if (( n < 0 )&&( n >= nEigens ))
    perror("Eigen index is out of range.\n");
  else
  {
    IplImage *newimage = cvCreateImage( cvGetSize(eigenVectArr[n]), IPL_DEPTH_8U, 1 );
    IplImage *fimage = cvCreateImage( cvGetSize(eigenVectArr[n]), IPL_DEPTH_32F, 1 );
    cvNormalize((IplImage*)eigenVectArr[n], (IplImage*)fimage, 0, 255, CV_MINMAX, NULL );
    cvConvertScaleAbs( (IplImage*)fimage, (IplImage*)newimage, 1, 0 );
    char *title = new char[10];
    sprintf(title, "%d", n);
    cvNamedWindow(title,1);
    cvShowImage(title,newimage);
    cvWaitKey(0);
    cvDestroyWindow(title);
    cvReleaseImage(&newimage);
    cvReleaseImage(&fimage);
  }
}


/*!
    \fn CvPCAFaceReg::loadTrainingData(const char * filename)
 */
void CvPCAFaceReg::loadTrainingData(const char * filename)
{
  CvFileStorage *fileStorage;
  
  fileStorage = cvOpenFileStorage( filename, 0, CV_STORAGE_READ );
  if( !fileStorage )
  {
    fprintf( stderr, "Can not open %s\n", filename );
  }
  
  nEigens = cvReadIntByName( fileStorage, 0, "nEigens", 0);
  nTrainImages = cvReadIntByName( fileStorage, 0, "nTrainImages", 0 );
  eigenValMat = (CvMat *)cvReadByName( fileStorage, 0, "eigenValMat", 0 );
  projectedTrainFaceMat = (CvMat *)cvReadByName( fileStorage, 0, "projectedTrainFaceMat", 0 );
  pAvgTrainImg = (IplImage *)cvReadByName( fileStorage, 0, "avgTrainImg", 0 );
  eigenVectArr = (IplImage **)cvAlloc( nTrainImages * sizeof(IplImage *) );
  for( int i = 0; i < nEigens; i++ )
  {
    char varname[200];
    sprintf( varname, "eigenVect_%d", i );
    eigenVectArr[i] = (IplImage *)cvReadByName( fileStorage, 0, varname, 0 );
  }
  
  cvReleaseFileStorage(&fileStorage);
}


/*!
    \fn CvPCAFaceReg::clear()
 */
void CvPCAFaceReg::clear()
{
  delete database;
  cvReleaseMat( &eigenValMat );
  cvReleaseMat( &projectedTrainFaceMat );
  cvReleaseImage( &pAvgTrainImg );
  delete svm;
  if(isImgLoaded)
  {
    for (int i = 0; i < nTrainImages; i++)
    {
      IplImage *img = (IplImage *)faceImgArr[i];
      cvReleaseImage( &img );
    }
  }
  
  for (int i = 0; i < nEigens ; i++)
  {
    IplImage *eigenVect = (IplImage *)eigenVectArr[i];
    cvReleaseImage( &eigenVect );
  }
}


/*!
    \fn CvPCAFaceReg::svmlearning(int client_no, int nfeatures)
 */
void CvPCAFaceReg::svmlearning(int client_no, int nfeatures)
{
  CvMat* trainData = cvCreateMat(nTrainImages, nfeatures, CV_32FC1);
  CvMat* response = cvCreateMat(nTrainImages, 1, CV_32FC1);
  
  
  for(int i = 0; i < nTrainImages; i++)
  {
    if((i>=(client_no-1)*4) && (i<client_no*4))
      cvSetReal1D( response, i, 1.0 );
    else
      cvSetReal1D( response, i, 2.0 );
    for(int j = 0; j < nfeatures; j++)
    {
      float val = cvGetReal2D( projectedTrainFaceMat, i, j );
      cvSetReal2D( trainData, i, j, val);
    }
  }
  
  
 
  CvTermCriteria term_crit = cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 200, 1.0);
  /*Type of SVM, one of the following types:
    CvSVM::C_SVC - n-class classification (n>=2), allows imperfect separation of classes with penalty multiplier C for outliers.
    CvSVM::NU_SVC - n-class classification with possible imperfect separation. Parameter nu (in the range 0..1, the larger the value, the smoother the decision boundary) is used instead of C.
    CvSVM::ONE_CLASS - one-class SVM. All the training data are from the same class, SVM builds a boundary that separates the class from the rest of the feature space.
    CvSVM::EPS_SVR - regression. The distance between feature vectors from the training set and the fitting hyperplane must be less than p. For outliers the penalty multiplier C is used.
    CvSVM::NU_SVR - regression; nu is used instead of p. */
  int _svm_type = CvSVM::NU_SVC;
  /*The kernel type, one of the following types:
    CvSVM::LINEAR - no mapping is done, linear discrimination (or regression) is done in the original feature space. It is the fastest option. d(x,y) = x•y == (x,y)
    CvSVM::POLY - polynomial kernel: d(x,y) = (gamma*(x•y)+coef0)degree
    CvSVM::RBF - radial-basis-function kernel; a good choice in most cases: d(x,y) = exp(-gamma*|x-y|2)
    CvSVM::SIGMOID - sigmoid function is used as a kernel: d(x,y) = tanh(gamma*(x•y)+coef0) */
    
  int _kernel_type = CvSVM::POLY;
    
  double _degree = 3.0;
  double _gamma = 1.0;
  double _coef0 = 0.0;
  double _C = 1.0;
  double _nu = 1.0;
  double _p = 1.0;
    
  CvSVMParams  params( CvSVM::C_SVC, CvSVM::POLY, _degree, _gamma, _coef0, _C, _nu, _p, 0, term_crit );
    
  svm->train( trainData, response, 0, 0, params );
    
  svm->save( "svm.xml", 0 );
  
  
  cvReleaseMat( &trainData );
  cvReleaseMat( &response );
}


/*!
    \fn CvPCAFaceReg::svmtesting( const char* filelist, int nfeatures )
 */
void CvPCAFaceReg::svmtesting( const char* filelist, int nfeatures )
{
  char *imgname = new char[50];
  FILE *fs = fopen(filelist, "r");
  assert(fs);
  FILE *file = fopen("output.txt","w");
  assert(file);
  double l;
  while(feof(fs) == 0)
  {
    if (fscanf(fs, "%s", imgname) == EOF) break;
    l = svmpredict( imgname, nfeatures );
    fprintf( file, "%s   %f\n", imgname, l );
    printf( "%s is Class %d\n", imgname, (int)l );
  }
  delete [] imgname;
  fclose(file);
  fclose(fs);

}


/*!
    \fn CvPCAFaceReg::loadTestImgArray(CvFaceDB *db)
 */
int CvPCAFaceReg::loadTestImgArray(CvFaceDB *db)
{
  printf("Loading testing images ............ \n");
  int nFace;
  if(db_type == XM2VTS)
  {
    char *filename = new char[50];
    int nClass = ((CvXm2vts*)database)->get_num_sub();
    int nperClass = ((CvXm2vts*)database)->get_num_pic();

    IplImage ** testImgArr = (IplImage **)cvAlloc(200*8*sizeof(IplImage *));
    
    
    
    nFace = 0;
    for (int i = 1; i <= nClass; i++)
    {
      for(int j = 1; j <= 8; j++)
      {
        ((CvXm2vts*)database)->getfilename( i, j, filename );
        testImgArr[nFace] = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
        nFace++;

      }
    }
    delete filename;
    
    
    cvReleaseMat(&projectedTrainFaceMat);
    projectedTrainFaceMat = cvCreateMat(200*8, nEigens, CV_32FC1);
    for (int i = 0; i < nTrainImages; i++)
    {
      cvEigenDecomposite(
	    testImgArr[i],
	    nEigens,
	    eigenVectArr,
	    0, 0,
	    pAvgTrainImg,
	    projectedTrainFaceMat->data.fl + i*nEigens);
    }

    cvFree(testImgArr);
  }
  else if(db_type == FERET)
  {
  }

  return nFace;
}


/*!
    \fn CvPCAFaceReg::svmpredict(const char *imgfilename, int nfeatures)
 */
float CvPCAFaceReg::svmpredict(const char *imgfilename, int nfeatures)
{
  IplImage *img = cvLoadImage(imgfilename, CV_LOAD_IMAGE_GRAYSCALE);
   
  CvMat * coeff = cvCreateMat(1, nEigens, CV_32FC1);
  cvEigenDecomposite(
	  img,
	  nEigens,
	  eigenVectArr,
	  0, 0,
	  pAvgTrainImg,
	  coeff->data.fl ); 
   	  
  CvMat *sample = cvCreateMat(1, nfeatures, CV_32FC1);
  for(int i = 0; i < nfeatures; i++)
  {
    float val = cvGetReal1D( coeff, i);
    cvSetReal1D( sample, i, val );
  }
  
  float r = svm->predict( sample );
   
  cvReleaseMat(&sample);
  cvReleaseMat( &coeff ); 
  cvReleaseImage( &img );
  
  return r;
}
