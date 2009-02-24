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
#include "cvgabadafeatureselect.h"


int main(int argc, char *argv[])
{
  char *weaksname = new char[100];
  char *inputname = new char[100];
  char *outputname = new char[100];
  strcpy(weaksname, "weaks.xml");
  strcpy(inputname, "/local/FaceDB/XM2VTS/imglist.txt");
  strcpy(outputname, "testingresults.txt");
  bool allfeatures = true;
  int nofeatures;
  
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-weaks" ) )
    {
      strcpy(weaksname, argv[++i]);
    }
    else if( !strcmp( argv[i], "-input" ) )
    {
      strcpy(inputname, argv[++i]);
    }
    else if( !strcmp( argv[i], "-output" ) )
    {
      strcpy(outputname, argv[++i]);
    }
    else if( !strcmp( argv[i], "-nofeatures" ) )
    {
      nofeatures = atoi( argv[++i] );
      allfeatures = false;
    }
  }
  
  CvGabAdaFeatureSelect *featureSelec = new CvGabAdaFeatureSelect;
  
  featureSelec->loadweaks( weaksname );
  
  if (allfeatures) featureSelec->testing( inputname, outputname );
  else featureSelec->testing( inputname, outputname, nofeatures);
  
  delete featureSelec;
  delete [] weaksname;
  delete [] inputname;
  delete [] outputname;
  return EXIT_SUCCESS;
}
