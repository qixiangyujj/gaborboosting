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
#ifndef MULTIADAGABORCVKNNWEAKLEARNER_H
#define MULTIADAGABORCVKNNWEAKLEARNER_H
#include <ml.h>
#include "cvfacedb.h"
#include "cvtrainingdata.h"
#include "cvxm2vts.h"

namespace MultiAdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvKNNWeakLearner{
public:
    CvKNNWeakLearner();

    ~CvKNNWeakLearner();
     CvKNNWeakLearner(CvFaceDB* db);
    void train(CvTrainingData *data);
    void clear();
    double predicterror(int k, CvMat* data, CvMat* weights, CvMat* classes);

protected:
    double error;
    int nClass;
    int nelement;
    int totalsamples;
    CvKNearest* knn;
    int nsamples;
    int k;
    
};

}

#endif
