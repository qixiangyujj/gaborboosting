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
#ifndef MULTIADAGABORCVMWEAKLEARNER_H
#define MULTIADAGABORCVMWEAKLEARNER_H
#include "cvtrainingdata.h"
#include <ml.h>
#include <vector>
#include "cvweaklearner.h"
#include "cvfacedb.h"
#include "cvxm2vts.h"
#include "MatUsage.h"

using namespace MatFunctions;

namespace MultiAdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/



class CvMWeakLearner{
public:
    CvMWeakLearner();

    ~CvMWeakLearner();
    void train( CvTrainingData *data );
    CvMWeakLearner(int nclass, int type);
    CvMWeakLearner(CvFaceDB *db, int Type);
    void Mul2Bin(CvTrainingData *data, int clabel);
    double predict(double value);
    void set_error(double e);
    double get_error();
    void updateweight(CvTrainingData *data);
    CvMat* mpredict(double val);
    void clear();
    double getTheshold(int clsIdx);
    double getParity(int clsIdx);
    CvWeakLearner* getWeakLearner(int index);
    void init(int nclass, int type);
    void init(CvFaceDB *db, int Type);
   

protected:
    int type;
    int nClass;
    int nsample;
    int nelement;
    vector<CvWeakLearner> classifiers;
    int totalsamples;
    double error;
};

}

#endif



