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
#include "cvgaborfeature.h"
#include "cvgaborfeaturepool.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <ml.h>
#include <sys/stat.h>
#include <vector>
#include "cvtrainingdata.h"
#include "cvmultitrainingdata.h"
#include "cvmweaklearner.h"
#include "cvmultigabadafeatureselect.h"
#include "cvmultigabadafsm1.h"


using namespace std;
using namespace MultiAdaGabor;

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf( "Usage: %s\n  -src <data source>\n"
            "  -NumSub <NO. of Subjects>\n"
            "  -NumPic <NO. of Picture each Subjects>\n"
            "  -nofeatures <NO. of features to select>\n",
            argv[0]);
  }
  else
  {
  char *srcpath = new char[30];
  int NumSub, NumPic, nfeatures;
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-src" ) )
    {
      strcpy( srcpath, argv[++i] );
    }
    else if( !strcmp( argv[i], "-NumSub" ))
    {
      NumSub = atoi( argv[++i] );
    }
    else if( !strcmp( argv[i], "-NumPic" ))
    {
      NumPic = atoi( argv[++i] );
    }
    else if( !strcmp( argv[i], "-nofeatures" ))
    {
      nfeatures = atoi( argv[++i] );
    }
    else
    {
      printf( "Usage: %s\n  -src <data source>\n"
              "  -NumSub <NO. of Subjects>\n"
              "  -NumPic <NO. of Picture each Subjects>\n"
              "  -nofeatures <NO. of features to select>\n",
              argv[0]);
    }
  }
  
  int width = 51;
  int height = 55;
  int minscale = -1;
  int maxscale = 3;
  int norientations = 8;
  int interval = 0;
  int bound = 0;
  bool reduce = true;
  
  CvXm2vts xm2vts( srcpath );
  xm2vts.setNumSub( NumSub );
  xm2vts.setNumPic( NumPic );
  CvPoolParams param( height, width, minscale, maxscale, norientations, interval, bound, reduce );
  
  
  CvMultiGabAdaFSM1 *mfsm1 = new CvMultiGabAdaFSM1( &xm2vts, &param );
  mfsm1->train( nfeatures );
  //mweak->train( data, 3, xm2vts.get_num_sub());
  //CvGaborFeature feature( 1,1,1,1 );
  //mfsm1->featureweak( &feature );
  
  delete mfsm1;
  delete srcpath;
  }
  
  
  return EXIT_SUCCESS;
}

