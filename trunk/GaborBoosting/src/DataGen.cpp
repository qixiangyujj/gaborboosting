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
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "cvgabor.h"
#include "cvpoolparams.h"
#include "cvgaborfeaturepool.h"
#include "cvgabadafeatureselect.h"
#include "makingfeaturedata.h"
#include "cvferet.h"

using namespace PrepareData;

int main(int argc, char *argv[])
{
  
  int sub = 4;
  int nfeatures = 200;
  const char *xm2vtspath= "/mnt/export/xm2vts_feature/";
  
  
  
  
  /* The image properties*/
  
  int width = 64;
  int height = 64;
  int bound = 0;
  int interval = 0;
  
  int norientations = 8;
  int minscale = -1;
  int maxscale = 3;
  int nScale = maxscale - minscale + 1;
  bool reduced = true;
  CvPoolParams poolParam( height, width, minscale, maxscale, norientations, interval, bound, reduced );
  CvFeret feret( "/windows/D/Data/oferet/", "/local/FaceDB/FERET/fa/PPMS/" );
  printf( "Number of images: %d/%d\n", feret.getNum(), feret.getSub() );
  //feret.make("/windows/D/Data/oferet/");
  MakingFeatureData featuremaking( &poolParam, "", "/windows/D/Data/feret/");
  //featuremaking.preparefile( &feret );
  featuremaking.runferet( &feret, &poolParam );
  //featuremaking.run(&feret);
  /*int scale, orient;
  for (int i = 0; i < nScale; i++) 
  {
    scale = minscale + i;
    
    double dwidth = (double)(width)/pow((double)2, (double)i);
    double dheight = (double)(height)/pow((double)2,(double)i);
    int maxx = (int)ceil(dwidth);
    int maxy = (int)ceil(dheight);
    
    for (int j = 0; j < norientations; j++) 
    {
      orient = j;
      for (int y = 1; y <= height; y++)
      {
        for (int x = 1; x <= width; x++)
        {
          char * inputname = new char[50];
          strcpy( inputname, xm2vtspath );
          char * name = new char[20];
          sprintf( name, "%d/%d/%d_%d.xml", scale, orient, x, y);
          strcat( inputname, name );
          
          CvMat * mat = (CvMat*)cvLoad(inputname, NULL, NULL, NULL);

          cvReleaseMat( &mat );
          delete [] name;
          delete [] inputname;
        }
      }
    }
  }
  */

  
  return EXIT_SUCCESS;
}


