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
#include "cvgabordifferencedatamaker.h"

CvGaborDifferenceDataMaker::CvGaborDifferenceDataMaker()
{
}


CvGaborDifferenceDataMaker::~CvGaborDifferenceDataMaker()
{
}





/*!
    \fn CvGaborDifferenceDataMaker::CvGaborDifferenceDataMaker(CvGaborResponseData *data, CvGaborFeature *gaborfeature, CvFaceDB *db)
 */
 CvGaborDifferenceDataMaker::CvGaborDifferenceDataMaker(CvGaborResponseData *data, CvGaborFeature *gaborfeature, CvFaceDB *db)
{
    gabordata = data;
    feature = gaborfeature;
    database = db;
}


/*!
    \fn CvGaborDifferenceDataMaker::getIntraDifference() const
 */
CvMat* CvGaborDifferenceDataMaker::getIntraDifference() const
{
    CvMat* examples;
    if(database->is_xm2vts())
    {
	int nClients = ((CvXm2vts *)database)->get_num_sub();
	int nPics = ((CvXm2vts *)database)->get_num_pic();
	int nDiff = nPics *(nPics-1)/2*nClients;
	examples = cvCreateMat( 1, nDiff, CV_32FC1);
	double v1, v2, v;
	int n = 0;
	for(int c=1; c <= nClients; c++)
	{
	   for(int i = 1; i <= nPics; i++)
	   {
	      v1 = gabordata->getfeaturefrominstance(feature, c, i);
	      for(int j = i+1; j<= nPics; j++)
	      {
		 v2 = gabordata->getfeaturefrominstance(feature, c, j);
		 v = fabs(v1-v2);
		 cvSetReal1D(examples, n, v);
		 n++;  
	      }
	   }
	}
    }
    else if(database->is_feret())
    {
	    
    }
    return examples;
}


/*!
    \fn CvGaborDifferenceDataMaker::getExtraDifference() const
 */
CvMat* CvGaborDifferenceDataMaker::getExtraDifference() const
{
	CvMat* examples;
	if(database->is_xm2vts())
	{
		int nClients = ((CvXm2vts *)database)->get_num_sub();
		int nPics = ((CvXm2vts *)database)->get_num_pic();
		int nDiff = nClients*(nClients-1)/2;
		examples = cvCreateMat(1, nDiff, CV_32FC1);
		double v1,v2,v;
		int n = 0;
		for(int i = 1; i <= nClients; i++)
		{
			v1 = gabordata->getfeaturefrominstance(feature, i, 1);
			for(int j = i+1; j <= nClients; j++)
			{
				v2 = gabordata->getfeaturefrominstance(feature, j, 1);
				v = fabs(v1-v2);
				cvSetReal1D(examples, n, v);
				n++;
			}
		}
	}
	else if(database->is_feret())
	{
	}
	return examples;
}


/*!
    \fn CvGaborDifferenceDataMaker::getDifference() const
 */
CvTrainingData* CvGaborDifferenceDataMaker::getDifference() const
{
  CvMat *intra_mat = getIntraDifference();
  CvMat *extra_mat = getExtraDifference();
  
  CvSize intra_size = cvGetSize(intra_mat);
  CvSize extra_size = cvGetSize(extra_mat);
  
  int nIntraDiff = intra_size.width*intra_size.height;
  int nExtraDiff = extra_size.width*extra_size.height;
  int numsample = nIntraDiff+nExtraDiff;
  CvTrainingData *bindata = new CvTrainingData; 
  bindata->init( 2, numsample, 1 );
  CvMat *mat = cvCreateMat(1, numsample, CV_32FC1);
  
  double v;
  for(int i = 0; i < numsample; i++)
  {
    if(i < nIntraDiff)
    {
      v = cvGetReal1D(intra_mat, i);
      cvSetReal1D(mat, i, v);
      bindata->setclsidxofsample(1, i);
    }
    else
    {
      v = cvGetReal1D(extra_mat, i);
      cvSetReal1D(mat, i, v);
      bindata->setclsidxofsample(2, i);
    }
  }
  bindata->setdata(mat);
  cvReleaseMat(&mat);
  cvReleaseMat(&intra_mat);
  cvReleaseMat(&extra_mat);
  return bindata;
}


/*!
    \fn CvGaborDifferenceDataMaker::getNumIntraDifference() const
 */
int CvGaborDifferenceDataMaker::getNumIntraDifference() const
{
  int nDiff;
  if(database->is_xm2vts())
  {
    int nClients = ((CvXm2vts *)database)->get_num_sub();
    int nPics = ((CvXm2vts *)database)->get_num_pic();
    nDiff = nPics *(nPics-1)/2*nClients;
  }
  else if(database->is_feret())
  {
  }
  return nDiff;
}


/*!
    \fn CvGaborDifferenceDataMaker::getNumExtraDifference() const
 */
int CvGaborDifferenceDataMaker::getNumExtraDifference() const
{
  int nDiff;
  if(database->is_xm2vts())
  {
    int nClients = ((CvXm2vts *)database)->get_num_sub();
    int nPics = ((CvXm2vts *)database)->get_num_pic();
    nDiff = nClients*(nClients-1)/2;
  }
  else if(database->is_feret())
  {
  }
  return nDiff;
}


/*!
    \fn CvGaborDifferenceDataMaker::getNumDifference() const
 */
int CvGaborDifferenceDataMaker::getNumDifference() const
{
    int nDiffs = getNumExtraDifference()+getNumIntraDifference();
    return nDiffs;
}


/*!
    \fn CvGaborDifferenceDataMaker::getDifference(CvMat *labels) const
 */
CvTrainingData* CvGaborDifferenceDataMaker::getDifference(CvMat *labels) const
{
  assert(labels != NULL);
  CvMat *intra_mat = getIntraDifference();
  CvMat *extra_mat = getExtraDifference();
  
  CvSize intra_size = cvGetSize(intra_mat);
  CvSize extra_size = cvGetSize(extra_mat);
  
  int nIntraDiff = intra_size.width*intra_size.height;
  int nExtraDiff = extra_size.width*extra_size.height;
  int numsample = nIntraDiff+nExtraDiff;
  
  
  CvSize size = cvGetSize(labels);
  int iNumExamples = size.width*size.height;
  
  assert(iNumExamples == numsample);
  
  
  CvTrainingData *bindata = new CvTrainingData; 
  bindata->init( 2, numsample, 1 );
  CvMat *mat = cvCreateMat(1, numsample, CV_32FC1);
  
  double v;
  for(int i = 0; i < numsample; i++)
  {
    if(i < nIntraDiff)
    {
      v = cvGetReal1D(intra_mat, i);
      cvSetReal1D(mat, i, v);
    }
    else
    {
      v = cvGetReal1D(extra_mat, i);
      cvSetReal1D(mat, i, v);
    }
  }
  bindata->setdata(mat);
  bindata->setresponse(labels);
  cvReleaseMat(&mat);
  cvReleaseMat(&intra_mat);
  cvReleaseMat(&extra_mat);
  return bindata;
}
