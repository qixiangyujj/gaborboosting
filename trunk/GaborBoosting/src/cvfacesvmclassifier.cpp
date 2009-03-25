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
  
  
  // to get the labels
  CvMat *labels = GetLabelsFromFeatures( gabordata, new_features );
  //cvSave("labels.xml", labels, NULL, NULL, cvAttrList(0,0));
  
  //initilize the training data for SVM
  CvMat *train_data = GetDataFromFeatures( gabordata, new_features );
  //cvSave("data.xml", train_data, NULL, NULL, cvAttrList(0,0));
  
  // train a SVM
  train_svm( train_data, labels );
  
  cvReleaseMat(&train_data);
  cvReleaseMat(&labels);
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


/*!
    \fn CvFaceSVMClassifier::GetDataFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
 */
CvMat* CvFaceSVMClassifier::GetDataFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
{
}


/*!
    \fn CvFaceSVMClassifier::train_svm_auto(CvMat *train_data, CvMat * labels)
 */
void CvFaceSVMClassifier::train_svm_auto(CvMat *train_data, CvMat * labels)
{
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
  bool is = svm.train_auto( train_data, labels, 0, 0, 
                  params, k_fold, 
                  C_grid, gamma_grid, p_grid, nu_grid, coef_grid, degree_grid);
  
  int num_sv = svm.get_support_vector_count();
  
  if( is )
    printf("An SVM is trained and %d support vectors are obtained.\n", num_sv);
  else
    printf("Training an SVM is failed.\n");
}


/*!
    \fn CvFaceSVMClassifier::train_svm( CvMat * train_data, CvMat * labels)
 */
void CvFaceSVMClassifier::train_svm( CvMat * train_data, CvMat * labels)
{
  bool is = svm.train( train_data, labels, 0, 0, params);
  int num_sv = svm.get_support_vector_count();
  if( is )
    printf("An SVM is trained and %d support vectors are obtained.\n", num_sv);
  else
    printf("Training an SVM is failed.\n");

}


/*!
    \fn CvFaceSVMClassifier::GetLabelsFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
 */
CvMat* CvFaceSVMClassifier::GetLabelsFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
{
}


/*!
    \fn CvFaceSVMClassifier::AutoTrain(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features)
 */
void CvFaceSVMClassifier::AutoTrain(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features)
{
  // to get the labels
  CvMat *labels = GetLabelsFromFeatures( gabordata, new_features );
  
  //initilize the training data for SVM
  CvMat *train_data = GetDataFromFeatures( gabordata, new_features );
  
  
  // train a SVM auto
  train_svm_auto( train_data, labels );
  
  cvReleaseMat(&train_data);
  cvReleaseMat(&labels);

}


/*!
    \fn CvFaceSVMClassifier::SetParams(CvSVMParams svmParams)
 */
void CvFaceSVMClassifier::SetParams(CvSVMParams svmParams)
{
  params.svm_type = svmParams.svm_type;
  params.kernel_type = svmParams.kernel_type;
  params.degree = svmParams.degree;
  params.gamma = svmParams.gamma;
  params.coef0 = svmParams.coef0;
  params.C = svmParams.C;
  params.nu = svmParams.nu;
  params.p = svmParams.p;
  params.class_weights = 0;
  
  params.term_crit.type = svmParams.term_crit.type;
  params.term_crit.max_iter = svmParams.term_crit.max_iter;
  params.term_crit.epsilon = svmParams.term_crit.epsilon;
  
}


/*!
    \fn CvFaceSVMClassifier::SetParams(int _svm_type, int _kernel_type, double _degree, double _gamma, double _coef0, double _C, double _nu, double _p, CvTermCriteria _term_crit)
 */
void CvFaceSVMClassifier::SetParams(int _svm_type, int _kernel_type, double _degree, double _gamma, double _coef0, double _C, double _nu, double _p, CvTermCriteria _term_crit)
{
  params.svm_type = _svm_type;
  params.kernel_type = _kernel_type;
  params.degree = _degree;
  params.gamma = _gamma;
  params.coef0 = _coef0;
  params.C = _C;
  params.nu = _nu;
  params.p = _p;
  params.class_weights = 0;
}





/*!
    \fn CvFaceSVMClassifier::Predict( const CvMat * sample) const
 */
double CvFaceSVMClassifier::Predict( const CvMat * sample) const
{
  double label = svm.predict( sample );
  return label;
}


/*!
    \fn CvFaceSVMClassifier::Training_error(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
 */
CvScalar CvFaceSVMClassifier::Training_error(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
{
  CvMat * train_data = GetDataFromFeatures( gabordata, new_features );
  CvMat * labels = GetLabelsFromFeatures( gabordata, new_features );
   
  CvSize size = cvGetSize(labels);
  int nsamples = size.width * size.height;
  
  int numpositive = 0, numnegative = 0;
  for(int i = 0; i < nsamples; i++)
  {
    double value = cvGetReal1D( labels, i );
    if ( value == 1.0 )
      numpositive++;
    else if( value == 2.0 )
      numnegative++;
  }
  
  assert((numpositive+numnegative) == nsamples );
  
  int nelements = new_features.getSize();
  
  CvMat * sample = cvCreateMat(1, nelements, CV_32FC1);
  
  int numerror = 0, numtruepositive = 0, numfalsepositive = 0;
  for(int i = 0; i < nsamples; i++)
  {
    cvGetRow( train_data, sample, i );
    double pre_label = Predict( sample );
    double label = cvGetReal1D( labels, i );
    if((pre_label == 1.0)&&(label == 1.0))
    {
      numtruepositive++;
    }
    if(pre_label != label)
    {
      if((pre_label == 1.0)&&(label == 2.0))
        numfalsepositive++;
      numerror++;
    }
  }
  
  double error = (double)numerror/(double)nsamples;
  double tp_rate = (double)numtruepositive/(double)numpositive;
  double fp_rate = (double)numfalsepositive/(double)numnegative;
  
  cvReleaseMat(&sample);
  cvReleaseMat(&train_data);
  cvReleaseMat(&labels);
  
  CvScalar scalar = cvScalar(error, tp_rate, fp_rate, 0);
  
  return scalar;
}


/*!
    \fn CvFaceSVMClassifier::Training_error(CvMat * train_data, CvMat * labels) const
 */
CvScalar CvFaceSVMClassifier::Training_error(CvMat * train_data, CvMat * labels) const
{
  CvSize size = cvGetSize(labels);
  int nsamples = size.width * size.height;
  
  int numpositive = 0, numnegative = 0;
  for(int i = 0; i < nsamples; i++)
  {
    double value = cvGetReal1D( labels, i );
    if ( value == 1.0 )
      numpositive++;
    else if( value == 2.0 )
      numnegative++;
  }
  
  assert((numpositive+numnegative) == nsamples );
  
  size = cvGetSize( train_data );
  
  int nelements = size.height;
  
  CvMat * sample = cvCreateMat(1, nelements, CV_32FC1);
  
  int numerror = 0, numtruepositive = 0, numfalsepositive = 0;
  for(int i = 0; i < nsamples; i++)
  {
    cvGetRow( train_data, sample, i );
    double pre_label = Predict( sample );
    double label = cvGetReal1D( labels, i );
    if((pre_label == 1.0)&&(label == 1.0))
    {
      numtruepositive++;
    }
    if(pre_label != label)
    {
      if((pre_label == 1.0)&&(label == 2.0))
        numfalsepositive++;
      numerror++;
    }
  }
  
  double error = (double)numerror/(double)nsamples;
  double tp_rate = (double)numtruepositive/(double)numpositive;
  double fp_rate = (double)numfalsepositive/(double)numnegative;
  
  cvReleaseMat(&sample);
  
  CvScalar scalar = cvScalar(error, tp_rate, fp_rate, 0);
  
  return scalar;
}
