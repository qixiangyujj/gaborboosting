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
}


CvPCAFaceReg::~CvPCAFaceReg()
{
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
    \fn CvPCAFaceReg::learn()
 */
void CvPCAFaceReg::learn()
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

  printf("Allocating Memory for Eigen Vectors ..........   \n");
  for(int i = 0; i < nEigens; i++)
  {
    eigenVectArr[i] = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);
  }
  printf("done!\n");
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
