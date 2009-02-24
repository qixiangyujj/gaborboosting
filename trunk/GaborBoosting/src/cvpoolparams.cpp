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
#include "cvpoolparams.h"

CvPoolParams::CvPoolParams()
{
  CvSize imgsize;
  imgsize.height = 55;
  imgsize.width = 51;
  int minscale = -1;
  int maxscale = 3;
  int norient = 8;
  int _interval = 1;
  int _bound = 0;
  init( imgsize, minscale, maxscale, norient, _interval, _bound);
}


CvPoolParams::~CvPoolParams()
{
  clear();
}




/*!
    \fn CvPoolParams::CvPoolParams(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound)
 */
 CvPoolParams::CvPoolParams(CvSize imgsize, 
                            int minscale, int maxscale, 
                            int norient, 
                            int interval, int bound)
{
  init(imgsize, minscale, maxscale, norient, interval, bound);
}


/*!
    \fn CvPoolParams::CvPoolParams(int height, int width, int minscale, int maxscale, int norient, int interval, int bound)
 */
 CvPoolParams::CvPoolParams(int height, int width, 
                            int minscale, int maxscale, 
                            int norient, 
                            int interval, int bound)
{
  this->imgSize.height = height;
  this->imgSize.width = width;
  init(this->imgSize, minscale, maxscale, norient, interval, bound);
}


/*!
    \fn CvPoolParams::init(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound)
 */
void CvPoolParams::init(CvSize imgsize, 
                        int minscale, int maxscale, 
                        int norient, 
                        int interval, int bound)
{
  this->imgSize = imgsize;
  int nscale = maxscale - minscale + 1;
  scales = cvCreateMat(1, nscale, CV_32SC1);
  for (int i = 0; i < nscale; i++)
  {
    cvSetReal1D( scales, i, (double)(minscale+i) );
  }
  
  orients = cvCreateMat(1, norient, CV_32FC1);
  for (int i = 0; i < norient; i++)
  {
    cvSetReal1D( orients, i, i);
  }
  this->interval = interval;
  this->bound = bound;
  setReduce(false);
}


/*!
    \fn CvPoolParams::setInterval(int i)
 */
void CvPoolParams::setInterval(int i)
{
  this->interval = i;
}


/*!
    \fn CvPoolParams::setBound(int b)
 */
void CvPoolParams::setBound(int b)
{
  this->bound = b;
}


/*!
    \fn CvPoolParams::getInterval() const
 */
int CvPoolParams::getInterval() const
{
  return interval;
}


/*!
    \fn CvPoolParams::getBound() const
 */
int CvPoolParams::getBound() const
{
  return bound;
}


/*!
    \fn CvPoolParams::getScales() const
 */
CvMat* CvPoolParams::getScales() const
{
  return scales;
}


/*!
    \fn CvPoolParams::getOrients() const
 */
CvMat* CvPoolParams::getOrients() const
{
  return orients;
}


/*!
    \fn CvPoolParams::getImgSize() const
 */
CvSize CvPoolParams::getImgSize() const
{
  return imgSize;
}


/*!
    \fn CvPoolParams::clear()
 */
void CvPoolParams::clear()
{
  if(scales != NULL) cvReleaseMat(&scales);
  if(orients != NULL) cvReleaseMat(&orients);
}


/*!
    \fn CvPoolParams::setReduce(bool key)
 */
void CvPoolParams::setReduce(bool key)
{
  this->reduce = key;
}


/*!
    \fn CvPoolParams::init(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound, bool reduced)
 */
void CvPoolParams::init(CvSize imgsize, 
                        int minscale, int maxscale, 
                        int norient, 
                        int interval, int bound, 
                        bool reduced)
{
  init(imgsize, minscale, maxscale, norient, interval, bound);
  setReduce(reduced);
}


/*!
    \fn CvPoolParams::CvPoolParams(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound, bool reduced)
 */
 CvPoolParams::CvPoolParams(CvSize imgsize, 
                            int minscale, int maxscale, 
                            int norient, 
                            int interval, int bound, 
                            bool reduced)
{
  init(imgsize, minscale, maxscale, norient, interval, bound, reduced);
}


/*!
    \fn CvPoolParams::CvPoolParams(int height, int width, int minscale, int maxscale, int norient, int interval, int bound, bool reduced)
 */
 CvPoolParams::CvPoolParams(int height, int width, 
                            int minscale, int maxscale, 
                            int norient, 
                            int interval, int bound, 
                            bool reduced)
{
  this->imgSize.height = height;
  this->imgSize.width = width;
  init(this->imgSize, minscale, maxscale, norient, interval, bound, reduced);
}


/*!
    \fn CvPoolParams::isReduced() const
 */
bool CvPoolParams::isReduced() const
{
  return reduce;
}
