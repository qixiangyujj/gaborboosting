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
#include <cvgaborresponsedata.h>
#include <cvfacedb.h>
#include <cxcore.h>
#include <cvfacesvmclientclassifier.h>


using namespace PrepareData;

int main(int argc, char *argv[])
{
  char dbpath[100] = "";
  char datapath[100] = "";
  char selectfeaturefilename[100] = "";
  
  strcpy( dbpath, "/home/sir02mz/FERET");
  strcpy( datapath, "/home/sir02mz/Meta");
  strcpy( selectfeaturefilename, "/home/sir02mz/EXP/FERET/29/significant.txt");
  
  
  // default setting 
  int height = 0;
  int width = 0;
  int minscale = -1;
  int maxscale = 3;
  int norientations = 8;
  int interval = 0;
  int bound = 0;
  bool reduced = false;

  int nweaks = 100;
  int db_type = CvGaborResponseData::FERET;
  
  int svm_type = CvSVM::NU_SVC;
  double nu = 0.99;
  
  int kernel_type = CvSVM::POLY;
  double degree = 3.0;
  double gamma = 1.0;
  double coef0 = 1.0;
  
  double C = 1.0;
  double p = 1.0;
  
  CvTermCriteria term_crit;
  term_crit.type = CV_TERMCRIT_ITER+CV_TERMCRIT_EPS;
  term_crit.max_iter = 200;
  term_crit.epsilon = 0.99;

  
  
  int client = 29;
  
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
    else if ( !strcmp( argv[i], "-client" ) )
    {
      client = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-XM2VTS" ))
    {
      db_type = CvGaborResponseData::XM2VTS;
      if( !strcmp( dbpath, "" ) )
        strcpy( dbpath, "/home/sir02mz/XM2VTS");
      if( !strcmp( datapath, "" ) )
        strcpy( datapath, "/home/sir02mz/OUTPUT");
    }
    else if ( !strcmp( argv[i], "-FERET" ))
    {
      db_type = CvGaborResponseData::FERET;
      if( !strcmp( dbpath, "" ) )
        strcpy( dbpath, "/home/sir02mz/FERET");
      if( !strcmp( datapath, "" ) )
        strcpy( datapath, "/home/sir02mz/Meta");
    }
    else if ( !strcmp( argv[i], "-features" ))
    {
      strcpy( selectfeaturefilename, argv[++i] );
    }
  }

  CvFaceDB *database;
  CvSize size;
  if( db_type == CvGaborResponseData::XM2VTS )
  {
    CvXm2vts * xm2vts = new CvXm2vts( dbpath );
    xm2vts->setNumSub( 200 );
    xm2vts->setNumPic( 4 );
    size = xm2vts->getSize();
    height = size.height;
    width = size.width;
    database = xm2vts;
    

  }
  else if( db_type == CvGaborResponseData::FERET)
  {
    sprintf(datapath, "%s/FA", datapath);
    char fapath[100];
    char fbpath[100];
    sprintf(fapath, "%s/FA", dbpath);
    sprintf(fbpath, "%s/FB", dbpath);
    CvFeret *feret = new CvFeret("", fapath, fbpath);
    size = feret->getSize();
    height = size.height;
    width = size.width;

    database = feret;
  }
  
  CvPoolParams param(size, minscale, maxscale, norientations, interval, bound, reduced);
  
  //CvGaborResponseData GaborData( database, &param, datapath );
  
  CvGaborFeaturePool SelectFeatures;
  SelectFeatures.load(selectfeaturefilename);
  
  CvFaceSVMClientClassifier classifier( client );
  classifier.SetParams( svm_type, kernel_type, degree, gamma, coef0, C, nu, p, term_crit);
  
  //classifier.AutoTrain( GaborData, SelectFeatures );
  
  CvMat *train_data = (CvMat*)cvLoad( "/home/sir02mz/data.xml", NULL, NULL, NULL );
  CvMat *labels = (CvMat*)cvLoad( "/home/sir02mz/labels.xml", NULL, NULL, NULL );
  
  classifier.train_svm( train_data, labels );
  
  CvScalar scalar = classifier.Training_error( train_data, labels );
  
  printf("The total error rate is %f.\n", scalar.val[0]);
  printf("The true positive rate is %f.\n", scalar.val[1]);
  printf("The false positive rate is %f.\n", scalar.val[2]);


  
  
  
  return EXIT_SUCCESS; 
}


