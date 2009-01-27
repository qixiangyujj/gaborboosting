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



#include <iostream>
#include <cstdlib>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <ml.h>
#include <sys/stat.h>
#include <vector>
#include "cvxm2vts.h"
#include "cvpoolparams.h"
#include "cvbingabadafeatureselect.h"

using namespace std;

int main(int argc, char *argv[])
{
  int width = 51;
  int height = 55;
  int bound = 0;
  int interval = 1;
  
  int norientations = 8;
  int minscale = -1;
  int maxscale = 3;
  bool reduced = false;
  
  int nweaks = 10;
  char *srcpath  = new char[50];
  char *output = new char[50];
  char *gweaksname = new char[50];
  int type = 0; // 0 is ensemble and 1 is svm
  
  strcpy( srcpath, "/windows/D/Data/xm2vts_feature/");
  strcpy( output, "gender_results_testing.txt");
  strcpy( gweaksname , "/local/BinEXP/Gender/ANN/genderweaks.xml");
  bool unioutput = false;
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-n" ) )
    {
      nweaks= atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-src" ) )
    {
      strcpy( srcpath, argv[++i] );
    }
    else if ( !strcmp( argv[i], "-output" ) )
    {
      strcpy( output, argv[++i] );
      unioutput = true;
    }
    else if ( !strcmp( argv[i], "-weaks" ) )
    {
      strcpy( gweaksname, argv[++i] );
    }
    else if ( !strcmp( argv[i], "-type" ) )
    {
      if ( !strcmp( "ensemble", argv[++i] )) 
        type = 0;
      else if ( !strcmp("svm", argv[i] )) 
        type = 1;
    }
    else if ( !strcmp(argv[i], "-help"))
    {
      printf( "Usage: %s\n  -n <number_of_features = %d>\n"
              "  -src <database_path = %s>\n"
              "  -output <output_file = %s>\n"
              "  -weaks  <weak_learners_xml_file = %s>\n"
              "  -type <type_of_classifier = ensemble or svm>\n"
              " -help  <Help Information.>\n",
              argv[0], nweaks, srcpath, output, gweaksname, type );
      return EXIT_SUCCESS;
    }
  }
  
  
  if( !unioutput )
  {
    string type_str;
    if (type == 0) 
      type_str = "ensemble";
    else if (type == 1) 
      type_str  = "svm";
    sprintf( output, "%s_%d", type_str.c_str(), nweaks );
  }
  
  CvXm2vts xm2vts( srcpath );
  xm2vts.setNumSub( 200 );
  xm2vts.setNumPic( 4 );
  xm2vts.setGender( "/home/sir/sir02mz/local/FaceDB/XM2VTS/gender.csv" );
  
  CvPoolParams param(height, width, minscale, maxscale, norientations, interval, bound, reduced);
  
  CvBinGabAdaFeatureSelect FeatureSelec( &xm2vts, &param, -1 );
  
  FeatureSelec.loadweaks( gweaksname );
  
  if ( type == 0 ) FeatureSelec.gendertesting( output, nweaks );
  else if ( type == 1 ) FeatureSelec.svmtesting( output, nweaks );
  
  delete [] srcpath;
  delete [] output;
  delete [] gweaksname;
  
  return EXIT_SUCCESS;
}
