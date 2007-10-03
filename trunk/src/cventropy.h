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
#ifndef ADAGABORCVENTROPY_H
#define ADAGABORCVENTROPY_H
#include "cvgaborfeaturepair.h"
#include "cvgaborfeature.h"
#include "cvlookuptable.h"

#define DEFAULT_NO_BINS 10

namespace AdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvEntropy{
public:
    CvEntropy();

    ~CvEntropy();
    void setNumBins(int num);
    double get();
    void build();
    void clear();
     CvEntropy(CvTrainingData *data, int nobins);
     CvEntropy(CvTrainingData *data1, CvTrainingData *data2, int nobins);
     CvEntropy(CvGaborFeaturePair *pair, CvFaceDB *database, int nobins);
     CvEntropy(CvGaborFeature *feature, CvFaceDB *database, int nobins);
     CvEntropy(CvGaborFeature *feature, CvFaceDB *database);
     CvEntropy(CvGaborFeaturePair *pair, CvFaceDB *database);

protected:
    int num_bins;
    double entropy;
    CvTrainingData* data1;
    CvTrainingData* data2;
    CvTrainingData* data;
    int num_data;
};

}

#endif
