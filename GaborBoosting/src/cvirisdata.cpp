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
#include "cvirisdata.h"

CvIrisData::CvIrisData()
 : CvTrainingData()
{
}


CvIrisData::~CvIrisData()
{
}




/*!
    \fn CvIrisData::CvIrisData( const char * filename)
 */
 CvIrisData::CvIrisData( const char * filename)
{
  loadIris( filename );
}


/*!
    \fn CvIrisData::Remove(const char * name)
 */
void CvIrisData::Remove(const char * name)
{
  int clsidx;
  if(!strcmp(name, "setosa"))
  {
    clsidx = 1;
  }
  else if(!strcmp(name, "versicolor"))
  {
    clsidx = 2;
  }
  else if(!strcmp(name, "virginica"))
  {
    clsidx = 3;
  }
  
  
  int numresample = getnumsamcls(clsidx);
  int numsample = maxnum - numresample;
  int clsindex;
  CvMat* valmat = cvCreateMat(numsample, nelement, CV_32FC1);
  int *cls_a = new int[maxnum];
  double *weight_a = new double[maxnum];
  
  int n = 0;
  double v;
  for(int i = 0; i < maxnum; i++)
  {
    clsindex = getclsidxofsample(i);
    if(clsindex != clsidx)
    {
      for (int j = 0; j < getnumelement(); j++)
      {
        v = cvGetReal2D(Valcache,i,j);
        cvSetReal2D(valmat, n, j, v);
      }
      n++;
    }
  }
}
