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

#include "cvmultigabadafsm1.h"
#include "cvxm2vts.h"
using namespace MultiAdaGabor;

int main(int argc, char *argv[])
{
  bool isWeakFile = false;
  int nfeatures = 0;
  
  if(argc < 2)
  {
    printf( "Usage: %s\n   -f <number of features>\n",
            argv[0] );
  }
  else
  {
    for(int i = 1; i < argc; i++ )
    {
       if ( !strcmp( argv[i], "-f" ) )
      {
        nfeatures = atoi( argv[++i] );
      }
      else
      {
        printf( "Usage: %s\n    -f <number of features>\n",
                argv[0] );
      }
    }
  }
  
  const char* srcpath = "/mnt/export/rexm2vts/";
  const char *datapath = "/home/sir/sir02mz/local/BinEXP/M1/";
  char* significantfile = new char[50];
  sprintf(significantfile, "%s/signficant.txt", datapath);
  CvXm2vts xm2vts( srcpath );
  xm2vts.setNumSub( 200 );
  xm2vts.setNumPic( 4 );
  
  int height =55, width = 51, minscale=-1, maxscale=3,norientations=8,interval=0,bound=0,reduce=true;
  CvPoolParams param( height, width, minscale, maxscale, norientations, interval, bound, reduce );
  
  
  CvMultiGabAdaFSM1 fsm1(&xm2vts, &param);
  const char * inputname = "/local/FaceDB/XM2VTS/imglist.txt";
  const char * baseoutputname = "/local/testingresults_svm";
  char *outputname = new char[50];
  if(!isWeakFile)
  {
    fsm1.loadsign( significantfile );
    if (nfeatures == 0) nfeatures = fsm1.getNumFeatures();
    fsm1.svmtraining(nfeatures);
    //fsm1.knnsave( "/local/32nn.xml" );
    sprintf( outputname, "%s_%d.txt", baseoutputname, nfeatures);
    fsm1.svmtesting(outputname, nfeatures );
  }
  else
  {
    
  }
  
  
  delete [] outputname;
  delete [] significantfile;
  return 0;
}


