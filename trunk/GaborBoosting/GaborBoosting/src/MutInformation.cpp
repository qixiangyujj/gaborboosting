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
#define NDEBUG
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define CV_VERBOSE 1

#include <iostream>
#include <cstdlib>
//#include "cvgaborfeature.h"
//#include "cvgaborfeaturepool.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
//#include <ml.h>
#include <sys/stat.h>
#include <vector>
//#include "cvtrainingdata.h"
//#include "cvmultitrainingdata.h"
//#include "cvweaklearner.h"
//#include "cvadaboost.h"
//#include "cvgabadafeatureselect.h"
//#include "cvgaborfeaturepairpool.h"
//#include "cventropy.h"
#include "cvgaborfeaturepairselect.h"


using namespace std;
using namespace AdaGabor;

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    const char * dir_name = "/mnt/export/rexm2vts/";
    const char *output_name = "mut_table";
    printf( "Usage: %s\n  -src <data source (default path = %s)>\n"
            "  -job <NO. of job e.g. 1>\n"
            "  -output or -o <output filename (default = %s)>\n"
            "  -start <starting feature e.g. 1 1 0 -1>\n"
            "  -end <ending feature e.g. 2 1 0 -1>\n",
            argv[0], dir_name, output_name);
  }
  else
  {
    int width = 51;
    int height = 55;
    int minscale = -1;
    int maxscale = 3;
    int norientations = 8;
    int interval = 0;
    int bound = 0;
    bool reduce = true;
  
    char srcpath[30];
    char outputname[30];
    strcpy(srcpath, "/mnt/export/rexm2vts/" );
    strcpy(outputname, "mut_table.txt");
    int x1, x2, y1, y2, Mu1, Mu2, Nu1, Nu2, job;
    for(int i = 1; i < argc; i++ )
    {
      if( !strcmp( argv[i], "-start" ) )
      {
        x1 = atoi( argv[++i] );
        y1 = atoi( argv[++i]);
        Mu1 = atoi( argv[++i] );
        Nu1 = atoi( argv[++i] );
      }
      else if (!strcmp( argv[i], "-end" ) )
      {
        x2 = atoi( argv[++i] );
        y2 = atoi( argv[++i]);
        Mu2 = atoi(argv[++i]);
        Nu2 = atoi(argv[++i]);
      }
      else if( !strcmp( argv[i], "-src" ) )
      {
        strcpy(srcpath, argv[++i]);
      }
      else if( (!strcmp( argv[i], "-output" ))||(!strcmp( argv[i], "-o" )) )
      {
        strcpy(outputname, argv[++i]);
      }
      else if ( !strcmp( argv[i], "-job" ) )
      {
        job = atoi( argv[++i] );
      }
    }
  
  
  
    CvXm2vts xm2vts( srcpath );  
    xm2vts.setPicIndex( 1 );
    CvPoolParams param( height, width, minscale, maxscale, norientations, interval, bound, reduce );
    
    CvGaborFeaturePool *pool = new CvGaborFeaturePool;
    pool->Init( &param );
    
    
    //find the parts
    int N = pool->getSize();
    int numParts = 120;
    
    int total = N*(N-1)/2;
    int size_part = total/numParts;
    int *nums = new int[numParts];
    
    int n = 0;
    int sum = 0;
    for(int i = 0; i < N; i++)
    {
      sum = sum + (N-i-1);
      if(sum > size_part)  
      {
        i--;
        nums[n] = i;
        sum = 0;
        //printf("%d\n", nums[n]);
        n++;
      }
    }
    
    int istart, iend;
    if(job == 0)
    {
      istart = 0;
      iend = nums[job];
    }
    else if (job == numParts)
    {
      istart = nums[job-1]+1;
      iend = N-2;
    }
    else
    {
      istart = nums[job-1]+1;
      iend = nums[job];
    }
    
    
    
    CvGaborFeature *fstart = pool->getfeature( istart );
    CvGaborFeature *fend = pool->getfeature( iend );

    CvGaborFeaturePairSelect *pairselec = new CvGaborFeaturePairSelect;
    pairselec->setPool( &param, fstart, fend );
    pairselec->setDB( &xm2vts );
    pairselec->setNumBins( 10 );
    pairselec->setMutFile( outputname );
    
    struct stat dummy;
    if (stat( outputname, &dummy) != 0)
    {
      pairselec->build();
    }
    else
    {
      pairselec->resume();
    }
  
    delete [] nums;
    delete pairselec;
    delete pool;
  }

  return 0;
}
