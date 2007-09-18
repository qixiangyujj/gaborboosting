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

#define CV_VERBOSE 1
#define _XM2VTS     0
#define _FERET      1

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
#include "cvweaklearner.h"
#include "cvadaboost.h"
#include "cvgabadafeatureselect.h"
using namespace std;

int main(int argc, char *argv[])
{
  CvGabAdaFeatureParams training_params;
  int sub = 4;
  int nfeatures = 200;
  const char *xm2vtspath= "/mnt/export/rexm2vts/";
  strcpy(training_params.sourcepath, xm2vtspath);
  int DB = _XM2VTS; // 1 is XM2VTS; and 2 is FERET
  bool resume = false;
  /* The image properties*/
  int width = 51;
  int height = 55;
  int bound = 0;
  int interval = 0;

  int norientations = 8;
  int minscale = -1;
  int maxscale = 3;

  char *outputfile = new char [50];
  strcpy(outputfile, "weaks.xml");
  
  char *testingfile = new char [50];
  strcpy(testingfile, "/local/FaceDB/XM2VTS/imglist.txt");
  int *testIdx = new int[8];
  int numIdx = 4;
  bool reduced = false;
  
  for(int i = 1; i < argc; i++ )
  {
     if( !strcmp( argv[i], "-subject" ) )
     {
         sub = atoi( argv[++i] );
     }
     else if( !strcmp( argv[i], "-nfeature" ) )
     {
         nfeatures = atoi( argv[++i] );
     }
     else if( !strcmp( argv[i], "-db") )
     {
         if (!strcmp( argv[++i], "XM2VTS" )) DB = _XM2VTS;
         else if(!strcmp( argv[++i], "FERET")) DB = _FERET; 
     }
     else if (!strcmp( argv[i], "-data"))
     {
         strcpy(training_params.sourcepath, argv[++i]);
     } 
     else if (!strcmp(argv[i], "-resume"))
     {
         resume = true;
     }
     else if (!strcmp(argv[i], "-w"))
     { 
         width = atoi( argv[++i] ); 
     }
     else if (!strcmp(argv[i], "-h"))
     {
         height = atoi( argv[++i] );
     }
     else if (!strcmp(argv[i], "-bound"))
     {
         bound = atoi( argv[++i] );
     }
     else if (!strcmp(argv[i], "-interval"))
     {
         interval = atoi( argv[++i] );
     }
     else if (!strcmp(argv[i], "-norients"))
     {
         norientations = atoi( argv[++i] );
     }
     else if (!strcmp(argv[i], "-minscale"))
     {
         minscale = atoi( argv[++i] );
     }
     else if (!strcmp(argv[i], "-maxscale"))
     {
         maxscale = atoi( argv[++i] );
     }
     else if (!strcmp(argv[i], "-outputfile"))
     {
         strcpy(outputfile, argv[++i]);
     }
     else if (!strcmp(argv[i], "-testing"))
     {
         strcpy(testingfile, argv[++i]);
     }
     else if (!strcmp(argv[i], "-cross"))
     {
       int r;
       int *idx = testIdx;
       numIdx = 0;
       while(i < argc-1)
       {
         r = atoi( argv[++i] );
         if(r == 0)
         {
           i = i - 1;
           break;
         }
         else 
         {
           *idx = r;
           idx++;
           numIdx++;
         }
       }
         //testIdx = atoi( argv[++i] );
         //numIdx = 1;
     }
     else if (!strcmp(argv[i], "-fullsample"))
     {
       numIdx = 0; 
     }
     else if (!strcmp(argv[i], "-reduce"))
     {
       reduced = true;
       interval = 0;
     }
     else if (!strcmp(argv[i], "-help"))
     {
         const char *rflag = (resume == true)?"YES":"NO";
         printf( "Usage: %s\n  -data <dir_name = %s>\n"
                "  -subject <subject_name = %d>\n"
                "  -nfeature <number_of_features = %d>\n"
                "  -db <database = %d>\n"
                "  -resume <resume the program = %s>\n"
                "  -w <width = %d>\n"
                "  -h <height = %d>\n"
                "  -bound <bound = %d>\n"
                "  -interval <interval = %d>\n"
                "  -norients <numbers_of_orientations = %d>\n"
                "  -minscale <minimal scale = %d>\n"
                "  -maxscale <maximum scale = %d>\n",
                argv[0], training_params.sourcepath, sub, nfeatures, DB, rflag,
                width, height, bound, interval, norientations,
                minscale, maxscale );
     }
  }

  training_params.flag = DB;
  training_params.nosubject =  sub;
  training_params.nofeatures = nfeatures;
  training_params.testIndex = testIdx;
  training_params.numindex = numIdx;
  if(reduced) interval = 0;
  const char *rflag = (resume == true)?"YES":"NO";
  printf( "Program Name: %s\n  <dir_name = %s>\n"
          "  <subject_name = %d>\n"
          "  <number_of_features = %d>\n"
          "  <database = %d>\n"
          "  <resume the program = %s>\n"
          "  <width = %d>\n"
          "  <height = %d>\n"
          "  <bound = %d>\n"
          "  <interval = %d>\n"
          "  <numbers_of_orientations = %d>\n"
          "  <minimal scale = %d>\n"
          "  <maximum scale = %d>\n",
          argv[0], training_params.sourcepath, sub, nfeatures, DB, rflag,
          width, height, bound, interval, norientations,
          minscale, maxscale );
/*  ------------------------------------Feature-----------------------------------   */
  //CvGaborFeature feature(43, 11, 4, 1);
  //double value = feature.XM2VTSdata(xm2vtspath, 1, 1);
  //printf("%f\n", value);
  
  CvPoolParams param(height, width, minscale, maxscale, norientations, interval, bound, reduced);
  
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  //pool->Init(cvSize( width, height ), minscale, maxscale, norientations, interval, bound);
  //pool->load("/local/EXP/THESLD/Tallfeature_0.txt");
  pool->Init(&param);
  printf("%d features are loaded.\n", pool->getSize());
  training_params.featurepool = pool;
  CvGabAdaFeatureSelect *featureSelec = new CvGabAdaFeatureSelect;
  if (!resume)
  {
     featureSelec->create( &training_params );
  }
  else
  {
    featureSelec->resume();
  }

  featureSelec->save(outputfile);

/*  testing the whole set */

  featureSelec->testing( testingfile, "testingresults.txt" );
/*  testing the whole set */

  delete [] testIdx;
  delete [] testingfile;
  delete [] outputfile;
  delete featureSelec;
  delete pool;
  return EXIT_SUCCESS;
}
