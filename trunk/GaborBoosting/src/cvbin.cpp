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
#include "cvbin.h"

namespace AdaGabor {

CvBin::CvBin()
{
}


CvBin::~CvBin()
{
  clear();
}


}


/*!
    \fn AdaGabor::CvBin::setBounds(double lower, double upper)
 */
void AdaGabor::CvBin::setBounds(double lower, double upper)
{
  this->lowerBound = lower;
  this->upperBound = upper; 
}


/*!
    \fn AdaGabor::CvBin::getUpper()
 */
double AdaGabor::CvBin::getUpper()
{
  return upperBound;
}


/*!
    \fn AdaGabor::CvBin::getLower()
 */
double AdaGabor::CvBin::getLower()
{
  return lowerBound;
}


/*!
    \fn AdaGabor::CvBin::CvBin(double lower, double upper)
 */
 AdaGabor::CvBin::CvBin(double lower, double upper)
{
  this->setBounds(lower, upper);
}


/*!
    \fn AdaGabor::CvBin::check(double val)
 */
bool AdaGabor::CvBin::check(double val)
{
  if ((val > lowerBound) && (val <= upperBound))
  {
    this->add(val);
    return true;
  }
  else return false;
}


/*!
    \fn AdaGabor::CvBin::add(double val)
 */
void AdaGabor::CvBin::add(double val)
{
  elements.push_back(val);
}


/*!
    \fn AdaGabor::CvBin::getNum()
 */
int AdaGabor::CvBin::getNum()
{
  return elements.size();
}




/*!
    \fn AdaGabor::CvBin::CvBin(const CvBin& b)
 */
 AdaGabor::CvBin::CvBin(const CvBin& b)
{
  this->lowerBound = b.lowerBound;
  this->upperBound = b.upperBound;
  for (int i = 0; i < b.elements.size(); i++)
  {
    double v = b.elements[i];
    this->elements.push_back(v);
  }
}





/*!
    \fn AdaGabor::CvBin::operator = (const CvBin& b)
 */
AdaGabor::CvBin& AdaGabor::CvBin::operator = (const CvBin& b)
{
  if(this == &b)
    return *this;
  this->lowerBound = b.lowerBound;
  this->upperBound = b.upperBound;
  this->elements.clear();
  for (int i = 0; i < b.elements.size(); i++)
  {
    double v = b.elements[i];
    this->elements.push_back(v);
  }
  return *this;
}


/*!
    \fn AdaGabor::CvBin::clone()
 */
AdaGabor::CvBin* AdaGabor::CvBin::clone()
{
  CvBin* bin = new CvBin;
  bin->lowerBound = lowerBound;
  bin->upperBound = upperBound;
  for (int i = 0; i < elements.size(); i++)
  {
    double v = elements[i];
    bin->elements.push_back(v);
  }
  return bin; 
}


/*!
    \fn AdaGabor::CvBin::clear()
 */
void AdaGabor::CvBin::clear()
{
  elements.clear();
}
