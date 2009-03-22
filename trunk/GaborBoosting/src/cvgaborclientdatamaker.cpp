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
#include "cvgaborclientdatamaker.h"

CvGaborClientDataMaker::CvGaborClientDataMaker()
 : CvGaborDataMaker()
{
}


CvGaborClientDataMaker::~CvGaborClientDataMaker()
{
}



/*!
    \fn CvGaborClientDataMaker::CvGaborClientDataMaker(CvGaborResponseData *data, CvGaborFeature *gaborfeature, CvFaceDB *db, int client_no)
 */
 CvGaborClientDataMaker::CvGaborClientDataMaker(CvGaborResponseData *data, CvGaborFeature *gaborfeature, CvFaceDB *db, int client_no)
{
  gabordata = data;
  feature = gaborfeature;
  database = db;
  m_iClientNo = client_no;
}


CvMat* CvGaborClientDataMaker::getLabels() const
{
  CvMat* labels;
  if(database->is_xm2vts())
  {
    int nClients = ((CvXm2vts *)database)->get_num_sub();
    int nPics = ((CvXm2vts *)database)->get_num_pic();

    int lowerbound = (getClientNo() - 1)*nPics;
    int upperbound = getClientNo()*nPics - 1;
    
    int nsamples = nClients * nPics;
    labels = cvCreateMat( 1, nsamples, CV_32FC1);
    for(int i = 0; i < nsamples; i++)
    {
      if((i<upperbound )&&(i>lowerbound))
      {
        cvSetReal1D(labels, i, 1.0);
      }
      else
      {
        cvSetReal1D(labels, i, 2.0);
      }
    }
  }
  else if(database->is_feret())
  {
    /// @todo implement me
  }
  return labels;
}

CvTrainingData* CvGaborClientDataMaker::getData() const
{
  CvTrainingData* data;
  int nsamples = 0;
  int nelements = 0;
  if(database->is_xm2vts())
  {
    int nClients = ((CvXm2vts *)database)->get_num_sub();
    int nPics = ((CvXm2vts *)database)->get_num_pic();
    nsamples = nClients * nPics;
    nelements = 1;
    int nclass = 2;
    data = new CvTrainingData;
    data->init( nclass, nsamples, nelements);
    CvMat* mat = gabordata->getfeaturefromall( feature );
    CvMat* labels = getLabels();
    data->setdata( mat );
    data->setresponse( labels );
    data->statclsdist();
    cvReleaseMat(&mat);
    cvReleaseMat(&labels);
  }
  else if(database->is_feret())
  {
    
  }

  return data;
}





/*!
    \fn CvGaborClientDataMaker::getClientNo() const
 */
int CvGaborClientDataMaker::getClientNo() const
{
  return m_iClientNo;
}
