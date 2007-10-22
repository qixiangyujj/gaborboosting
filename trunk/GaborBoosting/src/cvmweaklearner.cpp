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
#include "cvmweaklearner.h"

namespace MultiAdaGabor {

CvMWeakLearner::CvMWeakLearner()
{
}


CvMWeakLearner::~CvMWeakLearner()
{
  clear();
}


}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::CvMWeakLearner(CvFaceDB *db, int Type)
 */
MultiAdaGabor::CvMWeakLearner::CvMWeakLearner(CvFaceDB *db, int Type)
{
  this->type = Type;
  CvXm2vts *xdb = (CvXm2vts*)db;
  this->nClass = xdb->get_num_sub();
  this->nsample = xdb->get_num_pic();
  this->totalsamples = nClass*nsample;
  for (int i = 0; i < nClass; i++)
  {
    CvWeakLearner weaklearner;
    classifiers.push_back( weaklearner );
  }
}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::CvMWeakLearner(int nclass, int type)
 */
MultiAdaGabor::CvMWeakLearner::CvMWeakLearner(int nclass, int type)
{
  this->nClass = nclass;
  this->type = type;
  for (int i = 0; i < nClass; i++)
  {
    CvWeakLearner weaklearner;
    classifiers.push_back( weaklearner );
  }
}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::train(CvTrainingData *data)
 */
void MultiAdaGabor::CvMWeakLearner::train(CvTrainingData *data)
{
  this->nelement = data->getnumelement();
  if(totalsamples != data->getnumsample())
  {
    perror("number of samples should be equal!");
    exit(-1);
  }
 
  // train the data
  printf("Training the weak classifier ............\n ");
  for(int i = 0; i < nClass; i++)
  {
    CvTrainingData *tmpdata = data->clone();
    int c = i + 1;
    Mul2Bin( tmpdata, c );
    CvWeakLearner *weaklearner = &(classifiers[i]);
    weaklearner->train( tmpdata, type );
    delete tmpdata;
  }
  
  // evaluation on the training data
  printf("Checking the performance on the training data ..............\n");
  CvMat* values = data->getdata();
  double e = 0.0;
  CvMat *table = cvCreateMat( 1, totalsamples, CV_32FC1 );
  for(int i = 0; i < totalsamples; i++)
  {
    double value = cvGetReal1D( values, i );
   
    CvMat* pred_c = mpredict( value );
    double c = (double)data->getclsidxofsample( i );
    if(!isInMat(pred_c, c)) 
    {
      e = e + data->getweightofsample( i );
      cvSetReal1D(table, i, 1.0);
    }
    else 
    {
      cvSetReal1D(table, i, 0.0);
      printf("Sample_ID%d ( Class_ID%d ) is correctly classified!\n", i, data->getclsidxofsample(i));
    }
    cvReleaseMat(&pred_c);
  }
  cvReleaseMat( &values );
  set_error(e);
  
  double beta = error/(1-error);
  for(int i = 0; i < totalsamples; i++)
  {
    double weight = data->getweightofsample( i );
    double e = cvGetReal1D(table, i);
    double new_weight = weight * pow( beta, 1-e);
    data->setweightofsample( new_weight, i );
  }
  cvReleaseMat(&table);
}








/*!
    \fn MultiAdaGabor::CvMWeakLearner::Mul2Bin(CvTrainingData *data, int clabel)
 */
void MultiAdaGabor::CvMWeakLearner::Mul2Bin(CvTrainingData *data, int clabel)
{
  int clsidx;
  for(int i = 0; i < totalsamples; i++)
  {
    clsidx = data->getclsidxofsample( i );
    if(clsidx == clabel) data->setclsidxofsample(1, i);
    else data->setclsidxofsample(2, i);
  }
}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::predict(double value)
 */
double MultiAdaGabor::CvMWeakLearner::predict(double value)
{
  CvMat *confid = cvCreateMat(1, nClass, CV_64FC1);
  double co;
  for(int i = 0; i < nClass; i++)
  {
    CvWeakLearner *weaklearner = &(classifiers[i]);
    co = weaklearner->confident( value );
    //printf("the confident is %f\n", co);
    cvSetReal1D(confid, i, co);
  }
  double min_val, max_val;
  CvPoint min_loc, max_loc;
  cvMinMaxLoc( confid, &min_val, &max_val, &min_loc, &max_loc, NULL );
  if((max_val == 0.0)&&(max_loc.x == 0)) 
  {
    cvReleaseMat( &confid );
    return 0.0;
  }
  else
  { 
    int c = max_loc.x;
    int n = 0;
    for(int i = 0; i < nClass; i++)
    {
      if(max_val == cvGetReal1D(confid, i)) n++;
    }
    //if( n>1 ) printf("There is %d maximums.\n", n);
    cvReleaseMat( &confid );
    return (double)(c+1.0);
  }
}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::set_error(double e)
 */
void MultiAdaGabor::CvMWeakLearner::set_error(double e)
{
  this->error = e;
}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::get_error()
 */
double MultiAdaGabor::CvMWeakLearner::get_error()
{
  return this->error;
}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::updateweight(CvTrainingData *data)
 */
void MultiAdaGabor::CvMWeakLearner::updateweight( CvTrainingData *data )
{
  
}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::mpredict(double val)
 */
CvMat* MultiAdaGabor::CvMWeakLearner::mpredict(double val)
{
  CvMat *confid = cvCreateMat(1, nClass, CV_64FC1);
  double co;
  for(int i = 0; i < nClass; i++)
  {
    CvWeakLearner *weaklearner = &(classifiers[i]);
    co = weaklearner->confident( val );
    cvSetReal1D(confid, i, co);
  }
  double min_val, max_val;
  CvPoint min_loc, max_loc;
  cvMinMaxLoc( confid, &min_val, &max_val, &min_loc, &max_loc, NULL );
  if((max_val == 0.0)&&(max_loc.x == 0)) 
  {
    cvReleaseMat( &confid );
    CvMat *cls = cvCreateMat(1, 1, CV_64FC1);
    cvSetReal1D( cls, 0, 0.0);
    return cls;
  }
  else
  { 
    vector<double> table;
    for(int i = 0; i < nClass; i++)
    {
      if(max_val == cvGetReal1D(confid, i)) 
      {
        table.push_back((double)i+1.0);
      }
    }
    //if( table.size() > 1 ) printf("There is %d maximums.\n", table.size());
    cvReleaseMat( &confid );
    CvMat *cls = cvCreateMat(1, table.size(), CV_64FC1);
    for(int i = 0; i < table.size(); i++)
    {
      cvSetReal1D(cls, i, table[i]);
    }
    return cls;
  }
}


/*!
    \fn MultiAdaGabor::CvMWeakLearner::clear()
 */
void MultiAdaGabor::CvMWeakLearner::clear()
{
  classifiers.clear();
}
