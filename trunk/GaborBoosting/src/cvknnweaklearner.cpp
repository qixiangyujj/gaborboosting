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
#include "cvknnweaklearner.h"

namespace MultiAdaGabor {

CvKNNWeakLearner::CvKNNWeakLearner()
{
}


CvKNNWeakLearner::~CvKNNWeakLearner()
{
  clear();
}


}


/*!
    \fn MultiAdaGabor::CvKNNWeakLearner::CvKNNWeakLearner(CvFaceDB* db)
 */
 MultiAdaGabor::CvKNNWeakLearner::CvKNNWeakLearner(CvFaceDB* db)
{
  char* dbname = db->getName();
  if(!strcmp(dbname, "XM2VTS"))
  {
    CvXm2vts *xdb = (CvXm2vts*)db;
    nClass = xdb->get_num_sub();
    nsamples = xdb->get_num_pic();
    totalsamples = nClass * nsamples;
  }
  else
  {
  }
  knn = new CvKNearest;
}


/*!
    \fn MultiAdaGabor::CvKNNWeakLearner::train(CvTrainingData *data)
 */
void MultiAdaGabor::CvKNNWeakLearner::train(CvTrainingData *data)
{
  int max_k = totalsamples-1;
  CvMat *trainData = data->getdata();
  CvMat *trainClasses = data->getresponse();
  CvMat *weights = data->getweights();
  knn->train( trainData, trainClasses, 0, false, max_k, false );
  
  double weighterror;
  CvMat* errors = cvCreateMat(max_k-nClass+1, 1, CV_32FC1);
  int n = 0;
  for(int ik = nClass; ik <= max_k; ik++)
  {
    weighterror = predicterror( ik, trainData, weights, trainClasses );
    cvSetReal1D(errors, n, weighterror);
    n++;
  }
  
  double min_val, max_val;
  CvPoint min_loc, max_loc;
  cvMinMaxLoc( errors, &min_val, &max_val, &min_loc, &max_loc, NULL );
  
  k = nClass + min_loc.y; 
  
  error = min_val;
  cvReleaseMat(&errors);
  cvReleaseMat(&weights);
  cvReleaseMat(&trainData);
  cvReleaseMat(&trainClasses);
}


/*!
    \fn MultiAdaGabor::CvKNNWeakLearner::clear()
 */
void MultiAdaGabor::CvKNNWeakLearner::clear()
{
  knn->clear();
}


/*!
    \fn MultiAdaGabor::CvKNNWeakLearner::predicterror(int k, CvMat* data, CvMat* weights, CvMat* classes)
 */
double MultiAdaGabor::CvKNNWeakLearner::predicterror(int k, CvMat* data, CvMat* weights, CvMat* classes)
{
  CvSize size = cvGetSize( data );
  int num = size.height;
  CvMat *results = cvCreateMat(num, 1, CV_32FC1);
  knn->find_nearest( data, k, results, 0, 0, 0);
  
  float c, pc, w, e = 0.0;
  for(int i = 0; i < num; i++)
  {
    c = cvGetReal1D(classes, i);
    pc = cvGetReal1D(results, i);
    w = cvGetReal1D(weights, i);
    if(c != pc) e = e + w;
  }
  
  cvReleaseMat(&results);
  return e;
}
