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
}


/*!
    \fn CvFaceSegment::CvFaceSegment(IplImage *img, CvPoint left, CvPoint right)
 */
 CvFaceSegment::CvFaceSegment(IplImage *img, CvPoint left, CvPoint right)
{
  image = cvCloneImage( img );
  lefteye = left;
  righteye = right;
}





/*!
    \fn CvFaceSegment::scale()
 */
IplImage * CvFaceSegment::scale()
{
  int xl = lefteye.x;
  int yl = lefteye.y;
  int xr = righteye.x;
  int yr = righteye.y;
  
  double distance = sqrt((double)(pow((xl-xr),2)+pow((yl-yr),2)));
  double ratio = 64/distance;
  
  CvSize size = cvGetSize( image );
  int width = size.width;
  int height = size.height;
  
  int new_width = (int)round(width*ratio);
  int new_height = (int)round(height*ratio);
  
  IplImage *aimg = cvCreateImage(cvSize(new_width,new_height), IPL_DEPTH_8U, 3);
  cvResize( image, aimg, CV_INTER_LINEAR );
  
  cvNamedWindow("Scale", CV_WINDOW_AUTOSIZE);
  cvShowImage("Scale", aimg);
  cvWaitKey(0);
  
  return aimg;
}


/*!
    \fn CvFaceSegment::rotate()
 */
IplImage* CvFaceSegment::rotate()
{
  int xl = lefteye.x;
  int yl = lefteye.y;
  int xr = righteye.x;
  int yr = righteye.y;
  
  double angle = atan((double)(yr-yl)/(double)(xr-xl));
  angle = 180*angle/CV_PI;
  
  
  
  
  CvMat* map_matrix = cvCreateMat(2,3,CV_32FC1);
  cv2DRotationMatrix( cvPointTo32f( righteye ), angle, 1.0, map_matrix);
  
  IplImage* newimg = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3 );
  cvWarpAffine( image, newimg, map_matrix, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
  
  
  cvNamedWindow("Scale", CV_WINDOW_AUTOSIZE);
  cvShowImage("Scale", newimg);
  cvWaitKey(0);
}
