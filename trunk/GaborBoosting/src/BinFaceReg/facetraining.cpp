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
  
  int nweaks = 200;;
  bool resume = false;
  int weak_type = CvWeakLearner::POTSU;
  int sub = 1;
  
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-resume" ) )
    {
      resume = true;
    }
    else if( !strcmp( argv[i], "-n" ) )
    {
      nweaks= atoi( argv[++i] );
    }
    else if( !strcmp( argv[i], "-sub" ) )
    {
      sub = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-type" ) )
    {
      if ( !strcmp( "fld", argv[++i] )) 
        weak_type = CvWeakLearner::FLD;
      else if ( !strcmp("svm", argv[i] )) 
        weak_type = CvWeakLearner::SVM;
      else if ( !strcmp("potsu", argv[i] )) 
        weak_type = CvWeakLearner::POTSU;
      else if ( !strcmp("ann", argv[i] )) 
        weak_type = CvWeakLearner::ANN;
    }
  }
  
  const char *srcpath = "/windows/D/Data/xm2vts_feature/";
  CvXm2vts xm2vts( srcpath );
  xm2vts.setNumSub( 200 );
  xm2vts.setNumPic( 4 );
  
  //xm2vts.setGender( "/local/FaceDB/XM2VTS/gender.csv" );
  
  CvPoolParams param(height, width, minscale, maxscale, norientations, interval, bound, reduced);
  
  
  char weakname[200];
  sprintf( weakname, "weaks_%d.xml", sub);
  CvBinGabAdaFeatureSelect FeatureSelec(&xm2vts, &param, sub, "weaks.xml");
  
  FeatureSelec.setTypeWeak(weak_type);
  //FeatureSelec.setTypeWeak(2);
  if( !resume )
    FeatureSelec.train( nweaks );
  else
    FeatureSelec.resume( );

  return EXIT_SUCCESS;
}


