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
#include "cvadaboostclientfeatureselection.h"

CvAdaBoostClientFeatureSelection::CvAdaBoostClientFeatureSelection()
 : CvAdaBoostFeatureSelection()
{
}


CvAdaBoostClientFeatureSelection::~CvAdaBoostClientFeatureSelection()
{
}




/*!
    \fn CvAdaBoostClientFeatureSelection::CvAdaBoostClientFeatureSelection(CvGaborResponseData *memdata, CvMat *labels, CvPoolParams *param, int learner_type)
 */
 CvAdaBoostClientFeatureSelection::CvAdaBoostClientFeatureSelection(CvGaborResponseData *memdata, CvMat *labels, CvPoolParams *param, int learner_type, int client)
{
  init( memdata, labels, param, learner_type );
  client_no = client;
  
}


/*!
    \fn CvAdaBoostClientFeatureSelection::GetDataforWeak(CvGaborFeature *feature, CvGaborResponseData *memdata)
 */
CvTrainingData* CvAdaBoostClientFeatureSelection::GetDataforWeak(CvGaborFeature *feature, CvGaborResponseData *memdata)
{
  assert(feature != NULL);
  assert(memdata != NULL);
  CvFaceDB *database = memdata->getDB();
  CvGaborClientDataMaker maker( memdata, feature, database, client_no );
  //CvTrainingData *data = maker.getDifference(m_labels);
  
  CvTrainingData *data = maker.getData();
  CvMat* labels = maker.getLabels();
  data->setresponse(labels);
  cvReleaseMat(&labels);
  data->setweights(m_weights);
  data->statclsdist();

  return data;
}
