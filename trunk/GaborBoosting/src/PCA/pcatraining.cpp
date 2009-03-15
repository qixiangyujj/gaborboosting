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
#include "cvpcafacereg.h"
#include "cvxm2vts.h"

using namespace std;

int main(int argc, char *argv[])
{


  int client_no = 7;
  int nfeatures = 799;
  
  
  const char *srcpath = "/home/sir02mz/XM2VTS";
  CvXm2vts xm2vts( srcpath );
  xm2vts.setNumSub( 200 );
  xm2vts.setNumPic( 4 );


  CvPCAFaceReg facereg;
  facereg.setDB( &xm2vts );
  
  //facereg.pca();
  //facereg.storeTrainingData("/home/sir02mz/eigen.xml");
  
  
  facereg.loadTrainingData( "/home/sir02mz/eigen.xml" );
  
  facereg.svmlearning( client_no, nfeatures);

  facereg.svmtesting( "/home/sir02mz/XM2VTS/imglist.txt", nfeatures);
  //pca.showAvgImage( );
  //for (int i = 0; i < 70; i++)
  //{
  //	pca.showEigenImage(i);
  //}
  
  
  
  //pca.storeTrainingData("eigen.xml");
  //pca.loadTrainingData("eigen.xml");
  return EXIT_SUCCESS;

}

