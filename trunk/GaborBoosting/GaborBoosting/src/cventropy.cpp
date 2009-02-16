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
#include "cventropy.h"

namespace AdaGabor {

CvEntropy::CvEntropy()
{
}


CvEntropy::~CvEntropy()
{
}


}


/*!
\fn AdaGabor::CvEntropy::CvEntropy(CvTrainingData *data, int nobins)
 */
AdaGabor::CvEntropy::CvEntropy(CvTrainingData *data, int nobins)
{
  this->data = data;
  num_data = 1;
  setNumBins(nobins);
  build();
}


/*!
\fn AdaGabor::CvEntropy::CvEntropy(CvTrainingData *data1, CvTrainingData *data2, int nobins)
 */
AdaGabor::CvEntropy::CvEntropy(CvTrainingData *data1, CvTrainingData *data2, int nobins)
{
  this->data1 = data1;
  this->data2 = data2;
  num_data = 2;
  setNumBins(nobins);
  build();
}


/*!
    \fn AdaGabor::CvEntropy::setNumBins(int num)
 */
void AdaGabor::CvEntropy::setNumBins(int num)
{
  this->num_bins = num;
}


/*!
    \fn AdaGabor::CvEntropy::get()
 */
double AdaGabor::CvEntropy::get()
{
  return entropy;
}


/*!
    \fn AdaGabor::CvEntropy::build()
 */
void AdaGabor::CvEntropy::build()
{
  if (num_data == 1)
  {
    CvLookUpTable *lut = new CvLookUpTable(data);
    lut->setNumBins(this->num_bins);
    lut->build();
    int t = lut->numsample();
    double p = 0.0;
    for (int i = 0; i < num_bins; i++)
    {
      int x = lut->retrieve(i);
      if (x != 0) p += (double)x*log((double)x)/(double)t;
      else p += 0;
    }
    this->entropy = log((double)t) - p;
    delete lut;
  }
  else if (num_data == 2)
  {
    CvLookUpTable *lut = new CvLookUpTable(data1, data2);
    lut->setNumBins(this->num_bins);
    lut->build();
    int t = lut->numsample();
    double p = 0.0;
    for (int i = 0; i < num_bins; i++)
    {
      for (int j = 0; j < num_bins; j++)
      {
        int x = lut->retrieve(i,j);
        p += (double)x*log((double)x)/(double)t;
      }
    }
    this->entropy = log((double)t) - p;
    delete lut;
  }
  else
  {
    perror("CvEntropy::build\n");
  }
}


/*!
    \fn AdaGabor::CvEntropy::clear()
 */
void AdaGabor::CvEntropy::clear()
{
}



/*!
    \fn AdaGabor::CvEntropy::CvEntropy(CvGaborFeaturePair *pair, CvFaceDB *database, int nobins)
 */
 AdaGabor::CvEntropy::CvEntropy(CvGaborFeaturePair *pair, CvFaceDB *database, int nobins)
{
  CvTrainingData *_data1 = pair->getData(database, 1);
  CvTrainingData *_data2 = pair->getData(database, 2);
  this->data1 = _data1;
  this->data2 = _data2;
  num_data = 2;
  setNumBins(nobins);
  build();
  delete _data1;
  delete _data2;
}


/*!
    \fn AdaGabor::CvEntropy::CvEntropy(CvGaborFeature *feature, CvFaceDB *database, int nobins)
 */
 AdaGabor::CvEntropy::CvEntropy(CvGaborFeature *feature, CvFaceDB *database, int nobins)
{
  CvTrainingData *_data = feature->_XM2VTSMulti_F((CvXm2vts*)database);
  this->data = _data;
  num_data = 1;
  setNumBins(nobins);
  build();
  delete _data;
}


/*!
    \fn AdaGabor::CvEntropy::CvEntropy(CvGaborFeature *feature, CvFaceDB *database)
 */
 AdaGabor::CvEntropy::CvEntropy(CvGaborFeature *feature, CvFaceDB *database)
{
  CvTrainingData *_data = feature->_XM2VTSMulti_F((CvXm2vts*)database);
  this->data = _data;
  num_data = 1;
  setNumBins(DEFAULT_NO_BINS);
  build();
  delete _data;
}


/*!
    \fn AdaGabor::CvEntropy::CvEntropy(CvGaborFeaturePair *pair, CvFaceDB *database)
 */
 AdaGabor::CvEntropy::CvEntropy(CvGaborFeaturePair *pair, CvFaceDB *database)
{
  CvTrainingData *_data1 = pair->getData(database, 1);
  CvTrainingData *_data2 = pair->getData(database, 2);
  this->data1 = _data1;
  this->data2 = _data2;
  num_data = 2;
  setNumBins(DEFAULT_NO_BINS);
  build();
  delete _data1;
  delete _data2;
}
