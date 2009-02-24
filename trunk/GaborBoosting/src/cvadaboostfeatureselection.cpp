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
#include "cvadaboostfeatureselection.h"

CvAdaBoostFeatureSelection::CvAdaBoostFeatureSelection()
{
  m_features = new CvGaborFeaturePool;
  m_selectedfeatures = new CvGaborFeaturePool;
}


CvAdaBoostFeatureSelection::~CvAdaBoostFeatureSelection()
{
  clear();
}




/*!
    \fn CvAdaBoostFeatureSelection::SetFeatures(CvPoolParams *param)
 */
void CvAdaBoostFeatureSelection::SetFeatures(CvPoolParams *param)
{
  assert( param!=NULL );
  m_features = new CvGaborFeaturePool;
  m_features->Init( param);
  printf("%d features are stored in a list\n", m_features->getSize());
}


/*!
    \fn CvAdaBoostFeatureSelection::InitializeWeights(CvMat *labels)
 */
void CvAdaBoostFeatureSelection::InitializeWeights(CvMat *labels)
{
  assert(labels!=NULL);
  int iNumPositives = 0, iNumNegatives = 0;
  CvSize size = cvGetSize(labels);
  int iNumExamples = size.width*size.height;
  double v;
  for(int i = 0; i < iNumExamples; i++)
  {
    v = cvGetReal1D( labels, i );
    if(v ==  1.0)
      iNumPositives++;
    else if(v == 2.0)
      iNumNegatives++;
  }
  double dPostiveWeight = 1.0/2.0/(double)iNumPositives;
  double dNegativeWeight = 1.0/2.0/(double)iNumNegatives;
  m_weights = cvCreateMat(1, iNumExamples, CV_32FC1);
  for(int i = 0; i < iNumExamples; i++)
  {
    v = cvGetReal1D( labels, i );
    if(v ==  1.0)
      cvSetReal1D(m_weights, i, dPostiveWeight);
    else if(v == 2.0)
      cvSetReal1D(m_weights, i, dNegativeWeight);
  }
}


/*!
    \fn CvAdaBoostFeatureSelection::NormalizeWeights()
 */
void CvAdaBoostFeatureSelection::NormalizeWeights()
{
  assert(m_weights!=NULL);
  CvSize size = cvGetSize(m_weights);
  int iNumExamples = size.width*size.height;
  CvScalar s = cvSum(m_weights);
  double dSum = s.val[0];
  double dOldWeight, dNewWeight;
  for(int i = 0; i < iNumExamples; i++)
  {
    dOldWeight = cvGetReal1D( m_weights, i );
    dNewWeight = dOldWeight/dSum;
    cvSetReal1D(m_weights, i, dNewWeight);
  }
}


/*!
    \fn CvAdaBoostFeatureSelection::Select(int numfeatures)
 */
CvGaborFeaturePool* CvAdaBoostFeatureSelection::Select(int numfeatures)
{
  assert(numfeatures > 0);
  for(int i = 0; i < numfeatures; i++)
  {
    printf("Training in the iteration %d:\n", i);
    NormalizeWeights();
    //for(int j = 0; j < m_features->getSize(); j++)
    for(int j = 0; j < 30; j++)
    {
      std::cout << "Learning a weak learner on the feature: " << j<< "\r" << std::flush;
      CvGaborFeature *feature = m_features->getfeature(j);
      double error = TrainWeaklearner( feature, m_learner_type);
      assert(error >= 0.0);
      assert(error <= 1.0);
      feature->seterror( error );
    }
    CvGaborFeature *sfeature = FindSignificantFeature( m_features );
    m_selectedfeatures->add( sfeature );
    SaveWeights( i );
    UpdateWeights( sfeature );
    delete sfeature;
    printf("\n");
  }
  CvGaborFeaturePool *seletedfeatures = m_selectedfeatures->clone();
  return seletedfeatures;
}


/*!
    \fn CvAdaBoostFeatureSelection::FindSignificantFeature(CvGaborFeaturePool* features)
 */
CvGaborFeature* CvAdaBoostFeatureSelection::FindSignificantFeature(CvGaborFeaturePool* features)
{
  assert(features!=NULL);
  CvGaborFeature *sfeature = features->min();
  return sfeature;
}


/*!
    \fn CvAdaBoostFeatureSelection::UpdateWeights(CvGaborFeature *feature)
 */
void CvAdaBoostFeatureSelection::UpdateWeights(CvGaborFeature *feature)
{
  assert(feature!=NULL);
  CvTrainingData *data = GetDataforWeak( feature, m_memdata );
  CvWeakLearner weak( data, m_learner_type);
  weak.update( data );
  CvMat *newWeights = data->getresponse();
  cvCopy( newWeights, m_weights, NULL );
  delete data;
  cvReleaseMat(&newWeights);
}


/*!
    \fn CvAdaBoostFeatureSelection::TrainWeaklearner(CvGaborFeature *feature, int  learner_type)
 */
double CvAdaBoostFeatureSelection::TrainWeaklearner(CvGaborFeature *feature, int  learner_type)
{
  assert(feature!=NULL);
  CvTrainingData *data = GetDataforWeak( feature, m_memdata );
  CvWeakLearner weak( data, learner_type);
  double error = weak.training_error();
  delete data;
  return error;
}


/*!
    \fn CvAdaBoostFeatureSelection::SetType(int learner_type)
 */
void CvAdaBoostFeatureSelection::SetType(int learner_type)
{
  m_learner_type = learner_type;
}


/*!
    \fn CvAdaBoostFeatureSelection::GetDataforWeak(CvGaborFeature *feature, CvGaborResponseData *memdata)
 */
CvTrainingData* CvAdaBoostFeatureSelection::GetDataforWeak(CvGaborFeature *feature, CvGaborResponseData *memdata)
{
  assert(feature != NULL);
  assert(memdata != NULL);
  CvFaceDB *database = memdata->getDB();
  CvGaborDifferenceDataMaker maker( memdata, feature, database );
  CvTrainingData *data = maker.getDifference(m_labels);
  data->setweights(m_weights);
  return data;
}


/*!
    \fn CvAdaBoostFeatureSelection::CvAdaBoostFeatureSelection(CvGaborResponseData *memdata, CvMat *labels, CvPoolParams *param, int learner_type)
 */
 CvAdaBoostFeatureSelection::CvAdaBoostFeatureSelection(CvGaborResponseData *memdata, CvMat *labels, CvPoolParams *param, int learner_type)
{
  m_features = new CvGaborFeaturePool;
  m_selectedfeatures = new CvGaborFeaturePool;
  m_memdata = memdata;
  m_labels = cvCloneMat(labels);
  SetFeatures( param );
  SetType(learner_type);
  InitializeWeights(labels);
}


/*!
    \fn CvAdaBoostFeatureSelection::clear()
 */
void CvAdaBoostFeatureSelection::clear()
{
  delete m_features;
  delete m_selectedfeatures;
  cvReleaseMat(&m_labels);
  cvReleaseMat(&m_weights);
}


/*!
    \fn CvAdaBoostFeatureSelection::SaveWeights(const char *filename) const
 */
void CvAdaBoostFeatureSelection::SaveWeights(const char *filename) const
{
  assert(filename!=NULL);
  cvSave( filename, m_weights, NULL, NULL, cvAttrList(0,0));
}


/*!
    \fn CvAdaBoostFeatureSelection::SaveWeights(int Iter)
 */
void CvAdaBoostFeatureSelection::SaveWeights(int Iter)
{
  assert(Iter >= 0);
  char *filename = new char[200];
  sprintf(filename, "weights_%d.xml", Iter);
  SaveWeights(filename);
  delete [] filename;
}
