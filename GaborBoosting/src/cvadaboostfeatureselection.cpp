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
  if(isResume())
  {
    Resume();
  }
  else
  {
    m_total_iter = numfeatures;
    printf("\n");
    assert(m_total_iter > 0);
    
    // create a discard file
    CreateDiscardFile( DISCARD_FILE );
    for(int i = 0; i < m_total_iter; i++)
    {
      m_current_iter = i;
      
      WriteDiscardFile( DISCARD_FILE, m_current_iter );
    
      OutputState( STATE_FILE );
    
      //save weights
      SaveWeights( m_current_iter );
      time_t start, end;
      double dif;
      time (&start);
      printf("Training in the iteration %d:\n", m_current_iter);
      NormalizeWeights();
      int count = 0;
      for(int j = 0; j < m_features->getSize(); j++)
      //for(int j = 0; j < 30; j++)
      {
        std::cout << "Learning a weak learner on the feature: " << count << "\r" << std::flush;
      
        // get a feature from the large set of features
        CvGaborFeature *feature = m_features->getfeature(j);
      
        // train a weak learner with respect to the feature
        double error = TrainWeaklearner( feature, m_learner_type);
        assert(error >= 0.0 && error <= 1.0);
        feature->seterror( error );
        
        if(error > 0.5)
        {
          WriteDiscardFile( DISCARD_FILE, feature, error );
          m_features->remove(j);
          j--;
        }
        
        count++;
      }
      // find the significant feature with minimal error
      CvGaborFeature *sfeature = FindSignificantFeature( m_features );
      sfeature->write(SIGN_FILE);
      // save the selected feature
      m_selectedfeatures->add( sfeature );
    
      //update weights
      UpdateWeights( sfeature );
      delete sfeature;
    
      printf("\n");
      // display time consumed
      time (&end);
      dif = difftime (end,start);
      if(dif >= 3600) printf("The iteration takes %.2lf hours.\n", dif/3600);
      else if(dif >= 60) printf("The iteration takes %.2lf minutes.\n", dif/60);
      else if(dif < 60) printf("The iteration takes %.2lf seconds.\n", dif);
      printf("\n");
    }
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
  CvMat *newWeights = data->getweights();
  

  CvSize size1 = cvGetSize(newWeights);
  CvSize size2 = cvGetSize(m_weights);
  
  assert(size1.width*size1.height == size2.width*size2.height);
  if((size1.width != size2.width)||(size1.height != size2.height))
  {
    cvTranspose(newWeights, newWeights);
  }


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
  if(m_learner_type == CvWeakLearner::POTSU)
    printf("The weak learner is set as POTSU.\n");
  else if(m_learner_type == CvWeakLearner::ANN)
    printf("The weak learner is set as ANN.\n");
  else if(m_learner_type == CvWeakLearner::FLD)
    printf("The weak learner is set as FLD.\n");
  else if(m_learner_type == CvWeakLearner::SVM)
    printf("The weak learner is set as SVM.\n");
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
  data->statclsdist();
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
  remove( STATE_FILE );
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
    \fn CvAdaBoostFeatureSelection::SaveWeights(int Iter) const
 */
void CvAdaBoostFeatureSelection::SaveWeights(int Iter) const
{
  assert(Iter >= 0);
  char *filename = new char[200];
  sprintf(filename, "weights_%d.xml", Iter);
  SaveWeights(filename);
  delete [] filename;
}


/*!
    \fn CvAdaBoostFeatureSelection::isResume() const
 */
bool CvAdaBoostFeatureSelection::isResume() const
{
  bool result = false;
  FILE * file = fopen( STATE_FILE, "r");
  if( file != NULL)
  {
    int iter, alliter, type;
    while (!feof(file))
    {
      if (fscanf(file, "Current Iteration = %d\n", &iter) == EOF) break;
      if (fscanf(file, "Total Iteration = %d\n", &alliter) == EOF) break;
      if (fscanf(file, "Learner Type = %d\n", &type) == EOF) break;
    }
    if(iter > 0)
      result = true;
    fclose(file);
  }

  return result;
}


/*!
    \fn CvAdaBoostFeatureSelection::OutputState(const char * filename) const
 */
void CvAdaBoostFeatureSelection::OutputState(const char * filename) const
{
  assert(filename != NULL);
  FILE *file = fopen(filename, "w");
  if(file == NULL)
  {
    printf("ERROR: The file: %s can not be created!\n", filename);
    exit(-1);
  }
  fprintf( file, "Current Iteration = %d\n", m_current_iter);
  fprintf( file, "Total Iteration = %d\n", m_total_iter);
  fprintf( file, "Learner Type = %d\n", m_learner_type);
  fclose(file);
}


/*!
    \fn CvAdaBoostFeatureSelection::Resume()
 */
void CvAdaBoostFeatureSelection::Resume()
{
  ReadState( STATE_FILE );
  
  // load the selected features
  m_selectedfeatures->load( SIGN_FILE );
  
  // load the weights
  LoadWeights( m_current_iter );
  
  // Remove selected features from the original list
  for(int i = 0; i < m_selectedfeatures->getSize(); i++)
  {
    CvGaborFeature *feature = m_selectedfeatures->getfeature( i );
    m_features->remove( feature );
  }
  
  // remove discard features
  ReadDiscardFile( DISCARD_FILE );
  
  
  
  // start the selection
  for(int i = m_current_iter; i < m_total_iter; i++)
  {
    m_current_iter = i;
    
    OutputState( STATE_FILE );
    
    //save weights
    SaveWeights( m_current_iter );
    time_t start, end;
    double dif;
    time (&start);
    printf("Training in the iteration %d:\n", m_current_iter);
    NormalizeWeights();
    for(int j = 0; j < m_features->getSize(); j++)
    //for(int j = 0; j < 30; j++)
    {
      std::cout << "Learning a weak learner on the feature: " << j<< "\r" << std::flush;
      
      // get a feature from the large set of features
      CvGaborFeature *feature = m_features->getfeature(j);
      
      // train a weak learner with respect to the feature
      double error = TrainWeaklearner( feature, m_learner_type);
      assert(error >= 0.0 && error <= 1.0);
      feature->seterror( error );
    }
    // find the significant feature with minimal error
    CvGaborFeature *sfeature = FindSignificantFeature( m_features );
    sfeature->write(SIGN_FILE);
    // save the selected feature
    m_selectedfeatures->add( sfeature );
    
    //update weights
    UpdateWeights( sfeature );
    delete sfeature;
    
    printf("\n");
    // display time consumed
    time (&end);
    dif = difftime (end,start);
    if(dif >= 3600) printf("The iteration takes %.2lf hours.\n", dif/3600);
    else if(dif >= 60) printf("The iteration takes %.2lf minutes.\n", dif/60);
    else if(dif < 60) printf("The iteration takes %.2lf seconds.\n", dif);
    printf("\n");
  }
}


/*!
    \fn CvAdaBoostFeatureSelection::ReadState(const char *filename)
 */
void CvAdaBoostFeatureSelection::ReadState(const char *filename)
{
  assert( filename != NULL );
  FILE * file = fopen( filename, "r");
  if(file == NULL)
  {
    printf("ERROR: The file: %s does not exist!\n", filename);
    exit(-1);
  }
  
  int iter, alliter, type;
  while (!feof(file))
  {
    if (fscanf(file, "Current Iteration = %d\n", &iter) == EOF) break;
    if (fscanf(file, "Total Iteration = %d\n", &alliter) == EOF) break;
    if (fscanf(file, "Learner Type = %d\n", &type) == EOF) break;
  }
  assert(iter > 0);
  m_current_iter = iter;
  
  assert(alliter > iter);
  m_total_iter = alliter;
  
  switch(type)
  {
    case CvWeakLearner::ANN:
      m_learner_type = CvWeakLearner::ANN;
      printf("The type of weak learner is CvWeakLearner::ANN.\n");
      break;
    case CvWeakLearner::POTSU:
      m_learner_type = CvWeakLearner::POTSU;
      printf("The type of weak learner is CvWeakLearner::POTSU.\n");
      break;
    case CvWeakLearner::FLD:
      m_learner_type = CvWeakLearner::FLD;
      printf("The type of weak learner is CvWeakLearner::FLD.\n");
      break;
    case CvWeakLearner::SVM:
      m_learner_type = CvWeakLearner::SVM;
      printf("The type of weak learner is CvWeakLearner::SVM.\n");
      break;
  }
  fclose(file);
}


/*!
    \fn CvAdaBoostFeatureSelection::LoadWeights(int Iter)
 */
void CvAdaBoostFeatureSelection::LoadWeights(int Iter)
{
  assert(Iter >= 0);
  char *filename = new char[200];
  sprintf(filename, "weights_%d.xml", Iter);
  LoadWeights(filename);
  delete [] filename;
}


/*!
    \fn CvAdaBoostFeatureSelection::LoadWeights(const char *filename)
 */
void CvAdaBoostFeatureSelection::LoadWeights(const char *filename)
{
  assert(filename!=NULL);
  cvReleaseMat( &m_weights );
  m_weights = (CvMat*)cvLoad( filename, NULL);
}


/*!
    \fn CvAdaBoostFeatureSelection::CreateDiscardFile( const char * filename ) const
 */
void CvAdaBoostFeatureSelection::CreateDiscardFile( const char * filename ) const
{
  assert( filename != NULL );
  FILE *file = fopen( filename, "w" );
  if(file == NULL)
  {
    printf("ERROR: The file: %s can not be created!\n", filename);
    exit(-1);
  }
  fclose( file );
}


/*!
    \fn CvAdaBoostFeatureSelection::WriteDiscardFile( const char * filename, CvGaborFeature *feature, double error) const
 */
void CvAdaBoostFeatureSelection::WriteDiscardFile( const char * filename, CvGaborFeature *feature, double error) const
{
  assert( filename != NULL );
  FILE *file = fopen(filename, "a+");
  if(file == NULL)
  {
    printf("ERROR: The file: %s does not exist!\n", filename);
    exit(-1);
  }
  int x = feature->getx();
  int y = feature->gety();
  int Mu = feature->getMu();
  int Nu = feature->getNu();
  
  fprintf (file, "%d %d %d %d %f\n",x, y, Mu, Nu, error);
  fclose( file );
}


/*!
    \fn CvAdaBoostFeatureSelection::WriteDiscardFile( const char * filename, int Iter ) const
 */
void CvAdaBoostFeatureSelection::WriteDiscardFile( const char * filename, int Iter ) const
{
  assert( filename != NULL );
  FILE *file = fopen(filename, "a+");
  if(file == NULL)
  {
    printf("ERROR: The file: %s does not exist!\n", filename);
    exit(-1);
  }
  
  fprintf( file, "----------------------- %d -----------------------\n", Iter);
  fclose( file );
}


/*!
    \fn CvAdaBoostFeatureSelection::ReadDiscardFile(const char * filename)
 */
void CvAdaBoostFeatureSelection::ReadDiscardFile(const char * filename)
{
  assert( filename != NULL );
  FILE *file = fopen(filename, "r+");
  if(file == NULL)
  {
    printf("ERROR: Cannot read file %s.\n", filename);
    exit(-1);
  }
  
  int nIter, x, y, Mu, Nu;
  double merror;
  int n = 0;
  while (!feof(file))
  {
    if (fscanf(file, "----------------------- %d -----------------------\n", &nIter) == EOF)
      break;
    if (nIter == m_current_iter)
      break;
    if (fscanf(file, " %d", &x) == EOF)
      break;
    if (fscanf(file, " %d", &y) == EOF)
      break;
    if (fscanf(file, " %d", &Mu) == EOF)
      break;
    if (fscanf(file, " %d", &Nu) == EOF)
      break;
    if (fscanf(file, " %f\n", &merror) == EOF)
      break;
    
    CvGaborFeature feature(x,y,Mu,Nu);
    m_features->remove( &feature );
    n++;
  }
  
  printf("\n %d features are discard from the original feature list.\n");
  fclose( file );
}
