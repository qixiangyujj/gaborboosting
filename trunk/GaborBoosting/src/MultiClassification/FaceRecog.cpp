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
  
  const char* srcpath = "/mnt/export/rexm2vts/";
  const char* significantfile = "/home/sir/sir02mz/local/EXP/M1TEST/signficant.txt";
  CvXm2vts xm2vts( srcpath );
  xm2vts.setNumSub( 200 );
  xm2vts.setNumPic( 4 );
  
  int height =55, width = 51, minscale=-1, maxscale=3,norientations=8,interval=0,bound=0,reduce=true;
  CvPoolParams param( height, width, minscale, maxscale, norientations, interval, bound, reduce );
  
  
  CvMultiGabAdaFSM1 fsm1(&xm2vts, &param);
  fsm1.loadsign( significantfile );
  
  int nfeatures = fsm1.getNumFeatures();
  
  for(int i = 0 ; i < nfeatures; i++)
  {
    printf("Weaklearner %d ........\n", i);
    fsm1.setCurrentIter( i );
    if(i > 0)
    {
      char * weightname = new char[50];
      sprintf(weightname, "/home/sir/sir02mz/local/EXP/M1TEST/weight_%d.xml", i-1);
      fsm1.loadweights( weightname );
      delete [] weightname;
    }
    fsm1.update();
    printf("\n");
  }
  
  
  return 0;
}



