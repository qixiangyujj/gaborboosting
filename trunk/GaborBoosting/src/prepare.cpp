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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <cxcore.h>
#include "PrepareData.h"
#include "makingfeaturedata.h"
#include "cvgabor.h"


using namespace std;

using namespace PrepareData;


int main(int argc, char *argv[])
{
  if(argc == 1)
  {
    printf( "Usage: %s\n  -src <source path = >\n"
            "  -dest <destination path = >\n"
            "  -w <width = >\n"
            "  -h <height = >\n"
            "  -norients <numbers_of_orientations = >\n"
            "  -minscale <minimal scale = >\n"
            "  -maxscale <maximum scale = >\n",
            argv[0]);
  }
  char *srcpath = new char[100];
  char *dstpath = new char[100];
  bool reduce = false;
  
  
  int width, height, min, max, no; 
  
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-src" ) )
    {
      strcpy(srcpath, argv[++i]);
    }
    else if ( !strcmp( argv[i], "-dest" ) )
    {
      strcpy(dstpath, argv[++i]);
    }
    else if ( !strcmp( argv[i], "-w" ) )
    {
      width = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-h" ) )
    {
      height = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-minscale" ) )
    {
      min = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-maxscale" ) )
    {
      max = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-norients" ) )
    {
      no = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-reduce" ))
    {
      reduce = true;
    }
    else if ( !strcmp( argv[i], "-default" ) )
    {
      strcpy( srcpath, "/mnt/export/xm2vts/" );
      strcpy( dstpath, "/mnt/export/rexm2vts/" );
      width = 51; 
      height = 55; 
      min = 0;
      max = 3; 
      no = 8; 
      reduce = true;
    }
    else if ( !strcmp( argv[i], "-help" ) )
    {
      printf( "Usage: %s\n  -src <source path = >\n"
              "  -dest <destination path = >\n"
              "  -w <width = >\n"
              "  -h <height = >\n"
              "  -norients <numbers_of_orientations = >\n"
              "  -minscale <minimal scale = >\n"
              "  -maxscale <maximum scale = >\n",
              argv[0]);
    }
    else
    {
      printf("Error: parameter wrong!\n");
    }
  }
  
   
  //srcpath = "/local/FaceDB/XM2VTS/NEW/";

  //dstpath = "/local/D/Data/rexm2vts/";
  dstpath = "/home/sir02mz/Meta";
  preparefolder( dstpath, CvGaborResponseData::FERET );
  
  
  
  //makeMetas( srcpath, dstpath, CvGaborResponseData::XM2VTS );

  
  //const char *srcpath = "/mnt/export/xm2vts/";
  //const char *dstpath = "/mnt/export/xm2vts_feature/";
  //MakingFeatureData make(height, width, min, max, no, srcpath, dstpath, reduce);
  //make.run();

  delete [] srcpath;
  delete [] dstpath;
  return EXIT_SUCCESS;
}


