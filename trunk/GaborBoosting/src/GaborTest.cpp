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
  
  CvGabor gabor(3,-1);
  IplImage *img = cvLoadImage( "/local/FaceDB/CMU/rotated/217.jpg", CV_LOAD_IMAGE_GRAYSCALE );
  
  IplImage *reimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
  
  time_t start,end;
  double dif;
  time (&start);
  
  for(int i = 0; i < 1000; i++)
  {
    gabor.conv_img(img, reimg, CV_GABOR_REAL);
  }
  
  time (&end);
  dif = difftime (end,start);
  printf("It took %.2lf seconds \n\n", dif);
  
  
  cvReleaseImage(&reimg);
  cvReleaseImage(&img);
  return 0;
}

