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

#include <iostream>
#include <cstdlib>
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include "cvferet.h"

void resizeimg(const char * inputfilename, const char * outputfilename, double scale);

int main( int argc, char** argv )
{
  
  string dpath("/local/FaceDB/FERET/fb/PPMS/");
  CvFeret feret( "/windows/D/Data/oferet/", "/local/FaceDB/FERET/fb/PPM/" );
  printf("%d/%d\n", feret.getNum(), feret.getSub());
  
  string spath = feret.getFApath();
  for(int i = 0; i < feret.getSub(); i++)
  {
    CvSubject sub = feret.getSubject( i );
    for(int j = 0; j < sub.getnum(); j++)
    {
      string imgname = sub.getname( j );
      string inputname = spath + "/" + imgname;
      string outputname = dpath + "/" + imgname;
      resizeimg( inputname.c_str(), outputname.c_str(), 0.5 );
    }
  }
  return 0;
}


void resizeimg(const char * inputfilename, const char * outputfilename, double scale)
{
  IplImage *img = cvLoadImage(inputfilename, CV_LOAD_IMAGE_ANYCOLOR);
  CvSize size = cvGetSize(img);
  int width = size.width;
  int height = size.height;
  
  int newwidth = (int)round((double)width*scale);
  int newheight = (int)round((double)height*scale);
 
  IplImage *reimg = cvCreateImage( cvSize(newwidth,newheight), img->depth, img->nChannels );
  
  cvResize(img, reimg, CV_INTER_CUBIC);
  printf( "saving %s ......\n", outputfilename );
  cvSaveImage( outputfilename, reimg );
  cvReleaseImage( &img );
  cvReleaseImage( &reimg );
}


