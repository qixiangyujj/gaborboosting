/***************************************************************************
 *   Copyright (C) 2006 by Mian Zhou   *
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
#ifndef CVADABOOST_H
#define CVADABOOST_H
#include <vector>
#include "cvtrainingdata.h"
#include "cvweaklearner.h"
#include "cvtestingdata.h"
#include <cmath>

using namespace std;
/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvAdaBoost{
public:
    CvAdaBoost();

    ~CvAdaBoost();
    bool train(CvTrainingData *data, int numweak, int learner_type);
    void init(CvTrainingData *data, int numweak, int learner_type);
    void normalize();
    int predict(CvTestingData *testing);
    int predict(CvMat* sample);
    double perform();
    void clear();
    int predict(CvMat* sample, double bias);

protected:
    CvMat* weight;
    int ncls;
    int nsample;
    int nelement;
    int nweak;
    CvTrainingData* tdata;
    bool bInitialised;
    vector<CvWeakLearner> weaks;
    vector<double>  alphas;
    int niteration;
    int weak_type;
};

#endif
