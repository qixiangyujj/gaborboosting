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
    \fn CvGaborDifferenceDataMaker::getIntraDifference()
 */
CvMat* CvGaborDifferenceDataMaker::getIntraDifference()
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
    \fn CvGaborDifferenceDataMaker::getExtraDifference()
 */
CvMat* CvGaborDifferenceDataMaker::getExtraDifference()
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
