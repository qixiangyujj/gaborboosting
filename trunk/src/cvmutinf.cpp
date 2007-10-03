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
#include "cvmutinf.h"

namespace AdaGabor {

CvMutInf::CvMutInf()
{
}


CvMutInf::~CvMutInf()
{
}


}


/*!
    \fn AdaGabor::CvMutInf::CvMutInf(CvTrainingData *data1, CvTrainingData *data2)
 */
 AdaGabor::CvMutInf::CvMutInf(CvTrainingData *data1, CvTrainingData *data2)
{
  this->data1 = data1;
  this->data2 = data2;
  build(this->data1, this->data2, DEFAULT_NO_BINS);
  
}


/*!
\fn AdaGabor::CvMutInf::CvMutInf(CvTrainingData *data1, CvTrainingData *data2, int nobins)
 */
AdaGabor::CvMutInf::CvMutInf(CvTrainingData *data1, CvTrainingData *data2, int nobins)
{
  this->data1 = data1;
  this->data2 = data2;
  build(this->data1, this->data2, nobins);
}


/*!
    \fn AdaGabor::CvMutInf::get()
 */
double AdaGabor::CvMutInf::get()
{
  return this->mut;
}



/*!
    \fn AdaGabor::CvMutInf::CvMutInf(CvGaborFeaturePair *pair, CvFaceDB *database, int nobins)
 */
 AdaGabor::CvMutInf::CvMutInf(CvGaborFeaturePair *pair, CvFaceDB *database, int nobins)
{
  CvTrainingData* _data1 = pair->getData(database, 1);
  CvTrainingData* _data2 = pair->getData(database, 2);
  this->data1 = _data1;
  this->data2 = _data2;
  build(this->data1, this->data2, nobins);
  delete _data1;
  delete _data2;
}


/*!
    \fn AdaGabor::CvMutInf::build(CvTrainingData *data1, CvTrainingData *data2, int nobins)
 */
void AdaGabor::CvMutInf::build(CvTrainingData *data1, CvTrainingData *data2, int nobins)
{
  double h1, h2, h12;
  // Entropy for the data1
  CvEntropy *ent1 = new CvEntropy(data1, nobins);
  h1 = ent1->get();
  
  // Entropy for the data2
  CvEntropy *ent2 = new CvEntropy(data2, nobins);
  h2 = ent2->get();
  
  // Joined entropy for data1 and data2
  CvEntropy *ent12 = new CvEntropy(data1, data2, nobins);
  h12 = ent2->get();

  this->mut = h1 + h2 - h12;
  
  delete ent1;
  delete ent2;
  delete ent12;
}


/*!
    \fn AdaGabor::CvMutInf::CvMutInf(CvGaborFeaturePair *pair, CvFaceDB *database)
 */
 AdaGabor::CvMutInf::CvMutInf(CvGaborFeaturePair *pair, CvFaceDB *database)
{
  CvTrainingData* _data1 = pair->getData(database, 1);
  CvTrainingData* _data2 = pair->getData(database, 2);
  this->data1 = _data1;
  this->data2 = _data2;
  build(this->data1, this->data2, DEFAULT_NO_BINS);
  delete _data1;
  delete _data2;
}
