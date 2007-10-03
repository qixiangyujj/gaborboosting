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
#include <vector>
#include "cvfacesegment.h"


using namespace std;

vector<CvPoint> points;
void on_mouse( int event, int x, int y, int flags, void* param );

int main( int argc, char** argv )
{
  const char *filename = "/local/FaceDB/MissingFB/00057_931230_fb.ppm";
  IplImage* img = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR );
  
  cvNamedWindow( "image", CV_WINDOW_AUTOSIZE );
  
  cvShowImage( "image", img );
  
  cvSetMouseCallback( "image", on_mouse, 0 );
  
  char c;
  bool nowin = false;
  while(!nowin)
  {
    c = cvWaitKey( 0 );
    switch ( c )
    {
    case 'x':
      {
        printf("exit the window....\n");
        cvDestroyWindow( "image" );
        
        nowin = true;
        break;
      }
    }
  }
  
  CvPoint righteye = points[0];
  CvPoint lefteye = points[1];
  
  CvFaceSegment faceseg(img, lefteye, righteye);
  faceseg.rotate();
  cvReleaseImage( &img );
  return 0;
}

void on_mouse( int event, int x, int y, int flags, void* param )
{
  CvPoint p;
  switch( event )
  {
  case CV_EVENT_LBUTTONDOWN:
    {
      printf(" x = %d, y = %d\n", x, y);
      p = cvPoint(x,y);
      points.push_back(p);
      break;
    }
  }
}

