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

int main( int argc, char *argv[] )
{
  
  
  
  //IplImage *img = cvLoadImage( "/home/sir/sir02mz/local/FaceDB/FERET/fa/PPMS/00001_930831_fa_a.ppm", CV_LOAD_IMAGE_ANYCOLOR );
  IplImage *img = cvLoadImage( "/home/sir/sir02mz/local/00001_930831_fb_a.ppm", CV_LOAD_IMAGE_ANYCOLOR );
  IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
  cvCvtColor( img, grayimg, CV_RGB2GRAY );
  cvReleaseImage(&img);
  
  for(int i = -1; i <= 3; i++)
  {
    for(int j = 0; j <8 ; j++)
    {
      CvGabor gabor(j,i);
      IplImage *reimg = cvCreateImage(cvGetSize(grayimg), IPL_DEPTH_8U, 1);
      gabor.conv_img(grayimg, reimg, CV_GABOR_IMAG);
      
      char *filename = new char[30];
      sprintf(filename, "/local/imag_%d_%d.jpg", i, j);
      cvSaveImage( filename, (IplImage*)reimg );
      delete [] filename;
      cvReleaseImage(&reimg);
    }
  }
  
  
  
 
  
  
  
  cvReleaseImage(&grayimg);
  


  return 0;
}

