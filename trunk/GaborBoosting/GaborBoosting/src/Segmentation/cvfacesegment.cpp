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
#include "cvfacesegment.h"

CvFaceSegment::CvFaceSegment()
{
}


CvFaceSegment::~CvFaceSegment()
{
  clear();
}


/*!
    \fn CvFaceSegment::CvFaceSegment(IplImage *img, CvPoint left, CvPoint right)
 */
 CvFaceSegment::CvFaceSegment(IplImage *img, CvPoint left, CvPoint right)
{
  image = cvCloneImage( img );
  face = cvCreateImage(cvSize(128,128), IPL_DEPTH_8U, 3);
  lefteye = left;
  righteye = right;
  process();
}





/*!
    \fn CvFaceSegment::scale(IplImage *img)
 */
IplImage * CvFaceSegment::scale(IplImage *img)
{
  int xl = lefteye.x;
  int yl = lefteye.y;
  int xr = righteye.x;
  int yr = righteye.y;
  
  double distance = sqrt((double)(pow((xl-xr),2)+pow((yl-yr),2)));
  double ratio = 64/distance;
  
  CvSize size = cvGetSize( img );
  int width = size.width;
  int height = size.height;
  
  int new_width = (int)round(width*ratio);
  int new_height = (int)round(height*ratio);
  
  IplImage *aimg = cvCreateImage(cvSize(new_width,new_height), IPL_DEPTH_8U, 3);
  cvResize( img, aimg, CV_INTER_LINEAR );
  
  
  righteye.x = (int)round(righteye.x*ratio);
  righteye.y = (int)round(righteye.y*ratio);
  lefteye.x = (int)round(lefteye.x*ratio);
  lefteye.y = (int)round(lefteye.y*ratio);
  
  
  /*  IplImage * cimg = cvCloneImage(aimg);
  cvCircle( cimg, righteye, 4, cvScalar(0, 255, 0, 0), 1, 8, 0 );
  cvCircle( cimg, lefteye, 4, cvScalar(0, 255, 0, 0), 1, 8, 0 );
  cvNamedWindow("Scale", CV_WINDOW_AUTOSIZE);
  cvShowImage("Scale", cimg);
  cvWaitKey(0);
  cvDestroyWindow("Scale");
  cvReleaseImage(&cimg);
  */
  return aimg;
}


/*!
    \fn CvFaceSegment::rotate(IplImage *img)
 */
IplImage* CvFaceSegment::rotate(IplImage *img)
{
  int xl = lefteye.x;
  int yl = lefteye.y;
  int xr = righteye.x;
  int yr = righteye.y;
  
  double angle = atan((double)(yr-yl)/(double)(xr-xl));
  angle = 180*angle/CV_PI;
  
  double distance = sqrt((double)(pow((xl-xr),2)+pow((yl-yr),2)));
  int dis = (int)round(distance);
  
  CvMat* map_matrix = cvCreateMat(2,3,CV_32FC1);
  cv2DRotationMatrix( cvPointTo32f( righteye ), angle, 1.0, map_matrix);
  
  IplImage* newimg = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
  cvWarpAffine( img, newimg, map_matrix, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
  
  lefteye.y = righteye.y;
  lefteye.x = righteye.x+dis;
  cvReleaseMat( &map_matrix );
  
  /*
  cvCircle( newimg, righteye, 4, cvScalar(0, 255, 0, 0), 1, 8, 0 );
  cvCircle( newimg, lefteye, 4, cvScalar(0, 255, 0, 0), 1, 8, 0 );
  cvNamedWindow("Scale", CV_WINDOW_AUTOSIZE);
  cvShowImage("Scale", newimg);
  cvWaitKey(0);
  */
  return newimg;
}





/*!
    \fn CvFaceSegment::process()
 */
void CvFaceSegment::process()
{
  IplImage *roimage = rotate( this->image);
  IplImage *scaimage = scale( roimage );
  int x = righteye.x - 32;
  int y = righteye.y - 32;
  
  CvRect rect = cvRect(x, y, 128, 128);
  cvSetImageROI(scaimage, rect);
  cvCopy(scaimage, face);
  
  cvReleaseImage(&roimage);
  cvReleaseImage(&scaimage);
  
  cvNamedWindow("Face", CV_WINDOW_AUTOSIZE);
  cvShowImage("Face", face);
  char c = cvWaitKey(0);
  
  if(c == 'x') cvDestroyWindow("Face");
  
}


/*!
    \fn CvFaceSegment::saveface(const char* filename)
 */
void CvFaceSegment::saveface(const char* filename)
{
  cvSaveImage(filename, face);
  printf("%s saved.\n", filename);
}


/*!
    \fn CvFaceSegment::clear()
 */
void CvFaceSegment::clear()
{
  cvReleaseImage(&image);
  cvReleaseImage(&face);
}


/*!
    \fn CvFaceSegment::CvFaceSegment(const char* filename, CvPoint left, CvPoint right)
 */
 CvFaceSegment::CvFaceSegment(const char* filename, CvPoint left, CvPoint right)
{
  image = cvLoadImage(filename, CV_LOAD_IMAGE_ANYCOLOR );
  face = cvCreateImage(cvSize(128,128), IPL_DEPTH_8U, 3);
  lefteye = left;
  righteye = right;
  process();
}
