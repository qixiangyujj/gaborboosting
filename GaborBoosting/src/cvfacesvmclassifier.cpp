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
#include "cvfacesvmclassifier.h"

CvFaceSVMClassifier::CvFaceSVMClassifier()
 : CvFaceClassifier()
{
}


CvFaceSVMClassifier::~CvFaceSVMClassifier()
{
}




/*!
    \fn CvFaceSVMClassifier::Train(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features)
 */
void CvFaceSVMClassifier::Train(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features)
{
  int iNumFeatures = new_features.getSize();
  assert(iNumFeatures > 0);
  
  CvFaceDB *database = gabordata.getDB();
  
  // to get the number of examples
  CvGaborFeature *feature_1 = new_features.getfeature(0);
  CvGaborDifferenceDataMaker datamaker_1(&gabordata, feature_1, database);
  int iNumExamples = datamaker_1.getNumDifference();
  
  // to get the labels
  CvMat *labels = datamaker_1.getLabels();
  
  //initilize the training data for SVM
  CvMat *train_data = cvCreateMat(iNumExamples, iNumFeatures, CV_32FC1);
  
  double value;
  for(int i = 0; i < iNumFeatures; i++)
  {
    CvGaborFeature *feature = new_features.getfeature(i);
    assert(feature != NULL);
    CvGaborDifferenceDataMaker datamaker(&gabordata, feature, database);
    CvTrainingData *data = datamaker.getDifference();
    assert(data != NULL);
    CvMat* featurevalues = data->getdata();
    assert(featurevalues != NULL);
    
    // copy the feature values to training data for SVM
    CvSize size = cvGetSize(featurevalues);
    assert(iNumExamples == (size.width*size.height));
    for(int j = 0; j < iNumExamples; j++)
    {
      value = cvGetReal1D(featurevalues,j);
      cvSetReal2D( train_data, j, i, value ); 
    }
    
    cvReleaseMat( &featurevalues );
    delete data;
  }
  
  // prepare for CvParamGrid
  
  //initilize the grid parameters
  CvParamGrid C_grid = CvSVM::get_default_grid(CvSVM::C);
  CvParamGrid gamma_grid = CvSVM::get_default_grid(CvSVM::GAMMA);
  CvParamGrid p_grid = CvSVM::get_default_grid(CvSVM::P);
  CvParamGrid nu_grid = CvSVM::get_default_grid(CvSVM::NU);
  CvParamGrid coef_grid = CvSVM::get_default_grid(CvSVM::COEF);
  CvParamGrid degree_grid = CvSVM::get_default_grid(CvSVM::DEGREE);
  
  // The number of class C is 2, no need to optimise
  C_grid.step = 0;
  
  // no need to regression
  p_grid.step = 0;

  // set the grid for different case
  switch(params.kernel_type)
  {
    case CvSVM::POLY:
      break;
    case CvSVM::RBF:
      coef_grid.step = 0;
      degree_grid.step = 0;
      break;
    case CvSVM::SIGMOID:
      degree_grid.step = 0;
      break;
  }
  
  int k_fold = 10;
  // train an svm
  svm.train_auto( train_data, labels, 0, 0, 
                  params, k_fold, 
                  C_grid, gamma_grid, p_grid, nu_grid, coef_grid, degree_grid);
  
 // svm.train( train_data, labels, 0, 0, params);
  CvSVMParams params1 = svm.get_params();
  cvCopy(&params1, &params, NULL);
}


/*!
    \fn CvFaceSVMClassifier::clear()
 */
void CvFaceSVMClassifier::clear()
{
  svm.clear();
}


/*!
    \fn CvFaceSVMClassifier::Save(const char * filename)
 */
void CvFaceSVMClassifier::Save(const char * filename) 
{
  assert(filename != NULL);
  svm.save( filename, 0);
}


/*!
    \fn CvFaceSVMClassifier::Load(const char *filename)
 */
void CvFaceSVMClassifier::Load(const char *filename)
{
  assert(filename != NULL);
  svm.load( filename, 0 );
}


/*!
    \fn CvFaceSVMClassifier::SetKernel(int kernel_type)
 */
void CvFaceSVMClassifier::SetKernel(int kernel_type)
{
  params.kernel_type = kernel_type;
}


/*!
    \fn CvFaceSVMClassifier::CvFaceSVMClassifier( int kernel, int numclass, int max_iter, double epsilon)
 */
 CvFaceSVMClassifier::CvFaceSVMClassifier( int kernel, int numclass, int max_iter, double epsilon)
{
  params.svm_type = CvSVM::NU_SVC;
  params.C = numclass;
  params.kernel_type = kernel;
  params.class_weights = 0;
  params.degree = 3;
  params.nu = 0.9;
  if((max_iter > 0) && (epsilon < 1.0) && (epsilon > 0.0))
  {
    params.term_crit.type = CV_TERMCRIT_ITER+CV_TERMCRIT_EPS;
  }
  else if(max_iter > 0)
  {
    params.term_crit.type = CV_TERMCRIT_ITER;
  }
  else if(epsilon > 0.0)
  {
    params.term_crit.type = CV_TERMCRIT_EPS;
  }
  else if(epsilon > 1.0)
  {
    printf("ERROR!  The epsilon should not greater than 1.0.\n");
    exit(-1);
  }
  else
  {
    printf("ERROR!  The parameters for criteria are not correct.\n");
    exit(-1);
  }
  params.term_crit.max_iter = max_iter;
  params.term_crit.epsilon = epsilon;
}
