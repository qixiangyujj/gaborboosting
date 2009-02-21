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
#include "cvbindiffgabadafeatureselect.h"

CvBinDiffGabAdaFeatureSelect::CvBinDiffGabAdaFeatureSelect()
 : CvBinGabAdaFeatureSelect()
{
}


CvBinDiffGabAdaFeatureSelect::~CvBinDiffGabAdaFeatureSelect()
{
}




/*!
    \fn CvBinDiffGabAdaFeatureSelect::CvBinDiffGabAdaFeatureSelect(CvFaceDB *db, CvPoolParams *param, CvGaborResponseData *data)
 */
 CvBinDiffGabAdaFeatureSelect::CvBinDiffGabAdaFeatureSelect(CvFaceDB *db, CvPoolParams *param, CvGaborResponseData *data)
{
  setMemdata( data );
  setDB( db );
  setPool( param );
  //setSub( possub );
  setWeaksname( "weaks.xml" );
  init_weights();
}


/*!
    \fn CvBinDiffGabAdaFeatureSelect::setMemdata(CvGaborResponseData *data)
 */
void CvBinDiffGabAdaFeatureSelect::setMemdata(CvGaborResponseData *data)
{
  memdata = data;
}


/*!
    \fn CvBinDiffGabAdaFeatureSelect::featureweak(CvGaborFeature* feature)
 */
double CvBinDiffGabAdaFeatureSelect::featureweak(CvGaborFeature* feature)
{
  double e;
  CvTrainingData *data;
 
  CvGaborDifferenceDataMaker DataMaker(memdata, feature, database);
  data = DataMaker.getDifference();
  data->setweights(weights);
  CvWeakLearner *weak = new CvWeakLearner;
  weak->train(data, weaklearner_type);
  //weak->describe();
  // weak->train(data, CvWeakLearner::ANN);
  e = weak->training_error();
  
  //printf( "the error of this weak learner is %f\n", e );
  
  feature->seterror(e);
  falsepositive = weak->myerror();
  //weak->clear();
  delete data;
  delete weak;
  
  return e;
}
