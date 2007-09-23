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
  
  char * srcpath = new char[30];
  char * weightfilename = new char[30];
  int width = 51;
  int height = 55;
  int minscale = -1;
  int maxscale = 3;
  int norientations = 8;
  int interval = 0;
  int bound = 0;
  bool reduce = true;
  int job, iter;
  
  if(argc < 2)
  {
    printf( "Usage: %s\n  -src <data source>\n"
            "  -job <NO. of Job, e.g. 0>\n"
            "  -iter <NO. of Iteration, e.g. 0>\n"
            "  -weight or -w <weight filename>\n",
            argv[0] );
  }
  else
  {
    for(int i = 1; i < argc; i++ )
    {
      if( !strcmp( argv[i], "-src" ) )
      {
        strcpy(srcpath, argv[++i]);
      }
      else if( (!strcmp( argv[i], "-weight" ))||(!strcmp( argv[i], "-w" )) )
      {
        strcpy(weightfilename, argv[++i]);
      }
      else if ( !strcmp( argv[i], "-job" ) )
      {
        job = atoi( argv[++i] );
      }
      else if ( !strcmp( argv[i], "-iter" ) )
      {
        iter = atoi( argv[++i] );
      }
      else
      {
        printf( "Usage: %s\n  -src <data source>\n"
                "  -job <NO. of Job, e.g. 0>\n"
                "  -iter <NO. of Iteration, e.g. 0>\n"
                "  -weight or -w <weight filename>\n",
                argv[0] );
      }
    }
    
    CvXm2vts xm2vts( srcpath );
    xm2vts.setNumSub( 200 );
    xm2vts.setNumPic( 4 );
    
    CvPoolParams param( height, width, minscale, maxscale, norientations, interval, bound, reduce );
    CvGaborFeaturePool *pool = new CvGaborFeaturePool;
    pool->Init( &param );
    
    
    int N = pool->getSize();
    int numParts = 120;
    int istart, iend;
    findRange( N, numParts, job, &istart, &iend );
    CvGaborFeaturePool* subpool = pool->range( istart, iend );
    
    CvMultiGabAdaFSM1 *mfsm1 = new CvMultiGabAdaFSM1( &xm2vts, subpool );
    if(iter > 0) mfsm1->loadweights( weightfilename );
    //mfsm1->trainingsub( job, iter );
    
    struct stat dummy;
    char * outputname = new char[30];
    sprintf(outputname, "feature_%d_%d.txt", iter, job); 
    if (stat( outputname, &dummy) != 0)
    {
      mfsm1->trainingsub( job, iter );
    }
    else
    {
      mfsm1->resumesub( job, iter );
    }
    delete [] outputname;
    
    
    delete mfsm1;
    delete subpool;
    delete pool;
    
  }

  
  delete [] weightfilename;
  delete [] srcpath;
  return EXIT_SUCCESS;
}

