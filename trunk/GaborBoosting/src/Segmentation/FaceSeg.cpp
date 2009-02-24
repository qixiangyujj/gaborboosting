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
#include <errno.h>
#include <dirent.h>
#include "cvfacesegment.h"


using namespace std;

vector<CvPoint> points;
void on_mouse( int event, int x, int y, int flags, void* param );
void pointeyes(const char* filename);
void pointeyes(IplImage *img);

int main( int argc, char** argv )
{
  char *src = new char[100];
  char *dest = new char[100];
  char *singlename = new char[100];
  bool multi;
  
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-src" ) )
    {
      strcpy( src, argv[++i]);
      multi = true;
    }
    else if ( !strcmp( argv[i], "-dest" ) )
    {
      strcpy( dest, argv[++i]);
      multi = true;
    }
    else if (!strcmp( argv[i], "-img") )
    {
      strcpy(singlename, argv[++i]);
      multi = false;
    }
  }
  if(multi)
  {
    printf("Input directory is : %s\n", src);
    printf("Output directory is : %s\n", src);
  
  
    printf("\n\n");
    DIR *pdir;
    struct dirent *pent;
  
    pdir=opendir( src ); 
    if (!pdir)
    {
      perror( src );
      exit(-1);
    }
  
    errno=0;
    while ((pent=readdir(pdir)))
    {
      if((strcmp(pent->d_name, "."))&&(strcmp(pent->d_name, "..")))
      {
        string name(pent->d_name);
        string srcpath(src);
        string destpath(dest);
        string imagefilename = srcpath +"/"+ name;
        string facefilename = destpath + "/" + name;
        printf("Load %s\n", imagefilename.c_str());
        //printf("Output file is:%s\n", facefilename.c_str());
        {

          IplImage* img = cvLoadImage( imagefilename.c_str(), CV_LOAD_IMAGE_ANYCOLOR );
          pointeyes(img);
          CvPoint righteye = points[0];
          CvPoint lefteye = points[1];
          CvFaceSegment faceseg(img, lefteye, righteye);
          faceseg.saveface( facefilename.c_str() );
          cvReleaseImage( &img );
          points.clear();
        }
      }
      printf("\n");
    }
  
    if (errno){
      printf ("readdir() failure; terminating");
      exit(-1);
    }
    closedir(pdir);
  
  }
  else
  {
    IplImage* img = cvLoadImage( singlename, CV_LOAD_IMAGE_ANYCOLOR );
    pointeyes(img);
    CvPoint righteye = points[0];
    CvPoint lefteye = points[1];
    CvFaceSegment faceseg(img, lefteye, righteye);
    faceseg.saveface( "face.ppm" );
    cvReleaseImage( &img );
    points.clear();
  }
  
  delete [] src;
  delete [] dest;
  delete [] singlename;
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


void pointeyes(const char* filename)
{
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
  cvReleaseImage( &img );
}

void pointeyes(IplImage *img)
{
  
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
}

