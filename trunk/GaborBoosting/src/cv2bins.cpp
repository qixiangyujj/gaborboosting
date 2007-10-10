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
#include "cv2bins.h"

namespace AdaGabor {

Cv2Bins::Cv2Bins()
{
}


Cv2Bins::~Cv2Bins()
{
  clear();
}


}




/*!
    \fn AdaGabor::Cv2Bins::Cv2Bins(CvBin* bin1, CvBin* bin2)
 */
 AdaGabor::Cv2Bins::Cv2Bins(CvBin* bin1, CvBin* bin2)
{
  this->bin1 = bin1->clone();
  this->bin2 = bin2->clone();
}


/*!
    \fn AdaGabor::Cv2Bins::check(double val1, double val2)
 */
bool AdaGabor::Cv2Bins::check(double val1, double val2)
{
  double lower1 = this->bin1->getLower();
  double lower2 = this->bin2->getLower();
  double upper1 = this->bin1->getUpper();
  double upper2 = this->bin2->getUpper();
  if ((val1 > lower1)&&(val1 <= upper1)&&(val2 > lower2)&&(val2 <= upper2))
  {
    this->bin1->add(val1);
    this->bin2->add(val2);
    return true;
  }
  else
  {
    return false;
  }
}


/*!
    \fn AdaGabor::Cv2Bins::getNum()
 */
int AdaGabor::Cv2Bins::getNum()
{
  int num1 = this->bin1->getNum();
  int num2 = this->bin2->getNum();
  if (num1 != num2)
  {
    perror("Cv2Bins\n");
    printf("The numbers of elements in two bins are not same!\n");
    exit(-1);
  }
  return num1;
}


/*!
    \fn AdaGabor::Cv2Bins::Cv2Bins(const Cv2Bins& b)
 */
 AdaGabor::Cv2Bins::Cv2Bins(const Cv2Bins& b)
{
  CvBin b1 = *(b.bin1);
  CvBin b2(*(b.bin2));
  this->bin1 = &b1;
  this->bin2 = &b2;
}


/*!
    \fn AdaGabor::Cv2Bins::operator = (const Cv2Bins& b)
 */
AdaGabor::Cv2Bins AdaGabor::Cv2Bins::operator = (const Cv2Bins& b)
{
  if(this == &b)
    return *this;
  if (!bin1) delete bin1;
  if (!bin2) delete bin2;
  CvBin b1 = *(b.bin1);
  CvBin b2(*(b.bin2));
  bin1 = &b1;
  bin2 = &b2;
  return *this;
}


/*!
    \fn AdaGabor::Cv2Bins::clear()
 */
void AdaGabor::Cv2Bins::clear()
{
  if (bin1!=NULL) delete bin1;
  if (bin2!=NULL) delete bin2;
}
