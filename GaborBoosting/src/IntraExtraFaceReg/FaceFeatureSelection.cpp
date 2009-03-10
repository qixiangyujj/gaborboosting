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
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>
#include <ml.h>
#include "cvxm2vts.h"
#include "cvpoolparams.h"
#include "PrepareData.h"
#include "cvgaborresponsedata.h"
#include "cvbindiffgabadafeatureselect.h"
#include "cvadaboostfeatureselection.h"
//#include "GaborBoosting.h"

using namespace std;
using namespace PrepareData;

int main(int argc, char *argv[])
{
  char dbpath[100];
  char datapath[100];
  
  // default setting
  strcpy( dbpath, "/home/sir02mz/XM2VTS");
  strcpy( datapath, "/home/sir02mz/OUTPUT");
  
  int height = 0;
  int width = 0;
  int minscale = -1;
  int maxscale = 3;
  int norientations = 8;
  int interval = 0;
  int bound = 0;
  bool reduced = false;

  int nweaks = 500;
  int weak_type = CvWeakLearner::SVM;
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-n" ) )
    {
      nweaks= atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-dbpath" ) )
    {
      strcpy( dbpath, argv[++i] );
    }
    else if ( !strcmp( argv[i], "-datapath" ) )
    {
      strcpy( datapath, argv[++i] );
    }
    else if ( !strcmp( argv[i], "-interval" ) )
    {
      interval = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-type" ) )
    {
      if ( !strcmp("fld", argv[++i])) 
        weak_type = CvWeakLearner::FLD;
      else if ( !strcmp("svm", argv[i] )) 
        weak_type = CvWeakLearner::SVM;
      else if ( !strcmp("potsu", argv[i] )) 
        weak_type = CvWeakLearner::POTSU;
      else if ( !strcmp("ann", argv[i] )) 
        weak_type = CvWeakLearner::ANN;
    }
  }

  CvXm2vts xm2vts( dbpath );
  xm2vts.setNumSub( 200 );
  xm2vts.setNumPic( 4 );
  CvSize size = xm2vts.getSize();

  height = size.height;
  width = size.width;
  CvPoolParams param(size, minscale, maxscale, norientations, interval, bound, reduced);
  
  //const CvGaborResponseData GaborData( &xm2vts, &param );
  

  CvGaborResponseData GaborData( &xm2vts, &param, datapath );

  // get the labels of the training data
  CvGaborFeature feature(1,1,1,1);
  CvGaborDifferenceDataMaker maker(&GaborData, &feature, &xm2vts);
  CvMat* labels = maker.getLabels();



  CvAdaBoostFeatureSelection fs( &GaborData, labels, &param, weak_type );
  CvGaborFeaturePool *newfeatures = fs.Select( nweaks );
  newfeatures->write("newfeatures.txt");
  delete newfeatures;


  return EXIT_SUCCESS;

}

