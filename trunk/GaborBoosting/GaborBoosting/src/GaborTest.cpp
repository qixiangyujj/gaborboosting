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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include "cvgabor.h"
#include "cvtrainingdata.h"
#include "cvadaboost.h"
#include "cvweaklearner.h"

int main( int argc, char *argv[] )
{
  
  
  
  //IplImage *img = cvLoadImage( "/home/sir/sir02mz/local/FaceDB/FERET/fa/PPMS/00001_930831_fa_a.ppm", CV_LOAD_IMAGE_ANYCOLOR );
//   IplImage *img = cvLoadImage( "/local/1_1_2.jpg", CV_LOAD_IMAGE_ANYCOLOR );
//   IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
//   cvCvtColor( img, grayimg, CV_RGB2GRAY );
//   cvReleaseImage(&img);
//   
//   for(int i = -1; i <= 3; i++)
//   {
//     for(int j = 3; j <4 ; j++)
//     {
//       CvGabor gabor(j,i);
//       IplImage *reimg = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
//       gabor.conv_img(grayimg, reimg, CV_GABOR_MAG);
//       
//       char *filename = new char[30];
//       sprintf(filename, "/local/eteser_%d_%d.jpg", i, j);
//       cvSaveImage( filename, (IplImage*)reimg );
//       delete [] filename;
//       cvReleaseImage(&reimg);
//     }
//   }
// 
//   cvReleaseImage(&grayimg);
//   
  
  
  /*
  CvTrainingData data;
  data.loadIris("/local/iris.data");
  
  CvTrainingData *bindata = data.split(2,3);
  
  bindata->stat();
  
  CvAdaBoost boost;
  //boost.train(bindata, 20, CvWeakLearner::ANN);
  boost.train(bindata, 20, CvWeakLearner::BAYES);
  delete bindata;
  */
  
  
  CvGabor gabor(3,3);
  CvMat* remat = gabor.get_matrix(CV_GABOR_REAL);
  CvMat* immat = gabor.get_matrix(CV_GABOR_IMAG);
  cvSave( "/local/rematrix.xml", (CvMat*)remat,
          NULL,
          NULL,
          cvAttrList());
  cvSave( "/local/immatrix.xml", (CvMat*)immat,
          NULL,
          NULL,
          cvAttrList());
  IplImage *img = cvLoadImage( "/home/sir/sir02mz/local/FaceDB/FERET/fa/PPMS/00001_930831_fa_a.ppm", CV_LOAD_IMAGE_ANYCOLOR );
  
  IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1); 
  cvCvtColor( img, grayimg, CV_RGB2GRAY );
  
  IplImage *reimg = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_32F, 1);
  cvSave( "/local/gragimage.xml", (IplImage*)grayimg,
          NULL,
          NULL,
          cvAttrList());
  gabor.conv_img_a(grayimg, reimg, CV_GABOR_MAG);
  cvSave( "/local/magimage.xml", (IplImage*)reimg,
          NULL,
          NULL,
          cvAttrList());
  
  cvReleaseImage(&reimg);
  cvReleaseImage(&img);
  
  cvReleaseImage(&grayimg);
  
  return 0;
}

