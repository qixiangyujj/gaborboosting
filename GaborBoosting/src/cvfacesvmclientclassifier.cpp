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
#include "cvfacesvmclientclassifier.h"

CvFaceSVMClientClassifier::CvFaceSVMClientClassifier()
 : CvFaceSVMClassifier()
{
}


CvFaceSVMClientClassifier::~CvFaceSVMClientClassifier()
{
}


/*!
    \fn CvFaceSVMClientClassifier::CvFaceSVMClientClassifier(int Client_no)
 */
 CvFaceSVMClientClassifier::CvFaceSVMClientClassifier(int Client_no)
{
  SetClientNo( Client_no );
}


/*!
    \fn CvFaceSVMClientClassifier::GetLabelsFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
 */
CvMat* CvFaceSVMClientClassifier::GetLabelsFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
{
  CvFaceDB *database = gabordata.getDB();
  
  // to get the number of examples
  CvGaborFeature *feature_1 = new_features.getfeature(0);
  CvGaborClientDataMaker datamaker_1(&gabordata, feature_1, database, client_no);
  
  // to get the labels
  CvMat *labels = datamaker_1.getLabels();
  
  return labels;
}


/*!
    \fn CvFaceSVMClientClassifier::GetDataFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
 */
CvMat* CvFaceSVMClientClassifier::GetDataFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const
{
  int iNumFeatures = new_features.getSize();
  assert(iNumFeatures > 0);
  
  CvFaceDB *database = gabordata.getDB();
  
  // to get the number of examples
  CvGaborFeature *feature_1 = new_features.getfeature(0);
  CvGaborClientDataMaker datamaker_1(&gabordata, feature_1, database, client_no);
  int iNumExamples = datamaker_1.getNumExamples();
  

  //initilize the training data for SVM
  CvMat *train_data = cvCreateMat(iNumExamples, iNumFeatures, CV_32FC1);
  
  double value;
  for(int i = 0; i < iNumFeatures; i++)
  {
    CvGaborFeature *feature = new_features.getfeature(i);
    assert(feature != NULL);
    CvGaborClientDataMaker datamaker(&gabordata, feature, database, client_no);
    CvTrainingData *data = datamaker.getData();
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
  
  return train_data;
}


/*!
    \fn CvFaceSVMClientClassifier::SetClientNo(int clientno)
 */
void CvFaceSVMClientClassifier::SetClientNo(int clientno)
{
  client_no = clientno;
}


