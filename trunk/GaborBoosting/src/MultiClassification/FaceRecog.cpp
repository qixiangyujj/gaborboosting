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
  const char * outputname = "testingresults_m.txt";
  if(!isWeakFile)
  {
    fsm1.loadsign( significantfile );
    int nfeatures = fsm1.getNumFeatures();
    //nfeatures = 1;
    for(int i = 0 ; i < nfeatures; i++)
    {
      printf("Weaklearner %d ........\n", i);
      fsm1.setCurrentIter( i );
      if(i > 0)
      {
        char * weightname = new char[50];
        sprintf(weightname, "%s/weight_%d.xml", datapath, i-1);
        fsm1.loadweights( weightname );
        delete [] weightname;
      }
      fsm1.update();
      printf("\n");
    }
    
  
    fsm1.saveweaks("/local/mweaks.xml");
  }
  else
  {
    fsm1.loadweaks("/local/mweaks.xml");
  }
  //CvMat *mat = (CvMat*)cvLoad("/windows/D/Data/xm2vts/100/1/-1_4.xml", NULL, NULL, NULL);
  //float v = cvGetReal2D(mat, 12-1, 25-1);
  fsm1.testing( inputname, outputname );
  
  delete [] significantfile;
  return 0;
}



