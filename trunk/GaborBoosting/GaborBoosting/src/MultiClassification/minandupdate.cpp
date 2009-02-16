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
  int iter;
  char * srcpath = new char[30];
  char * weightfilename = new char[30];
  char * signfilename = new char[30];
  if(argc < 2)
  {
    printf( "Usage: %s\n -src <data source>\n"
            "  -iter <NO. of Iteration, e.g. 0>\n"
            "  -weight or -w <weight filename>\n"
            "  -sign or -s <significant feature filename>\n",
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
      else if( (!strcmp( argv[i], "-sign" ))||(!strcmp( argv[i], "-s" )) )
      {
        strcpy(signfilename, argv[++i]);
      }
      else if ( !strcmp( argv[i], "-iter" ) )
      {
        iter = atoi( argv[++i] );
      }
      else
      {
        printf( "Usage: %s\n -src <data source>\n"
                "  -iter <NO. of Iteration, e.g. 0>\n"
                "  -weight or -w <weight filename>\n"
                "  -sign or -s <significant feature filename>\n",
                argv[0] );
      }
    }
  
    CvXm2vts xm2vts( srcpath );
    xm2vts.setNumSub( 200 );
    xm2vts.setNumPic( 4 );
  
    CvGaborFeaturePool * pool = new CvGaborFeaturePool;
    poolloadfromsub("", "feature", iter, 120, pool);
    char *featurefilename = new char[30];
    sprintf(featurefilename, "feature_%d.txt", iter);
    pool->write( featurefilename );
    delete [] featurefilename;
    
 
    CvMultiGabAdaFSM1 *mfsm1 = new CvMultiGabAdaFSM1( &xm2vts, pool );
    if(iter > 0) 
    {
      mfsm1->loadweights( weightfilename );
      mfsm1->loadsign( signfilename );
    }
    double min = mfsm1->findmin();
    printf("The minimum is %f .\n", min);
    mfsm1->update();
  
  
  
    delete mfsm1;
  
    delete pool;
    
    
  }
  delete [] srcpath;
  delete [] weightfilename;
  delete [] signfilename;
    
  return 0;
}
