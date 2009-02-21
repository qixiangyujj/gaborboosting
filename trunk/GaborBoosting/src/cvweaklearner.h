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
#ifndef CVWEAKLEARNER_H
#define CVWEAKLEARNER_H

#include <ml.h>
#include <algorithm>
#include "cvtrainingdata.h"
/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvWeakLearner{
public:
    CvWeakLearner();

    ~CvWeakLearner();
     CvWeakLearner(CvTrainingData *data, int Learner_Type);
    bool train(CvTrainingData *data, int Learner_Type);
    double training_error();
    void save(const char *filename);
    double importance();
    double predict(CvMat* sample);
    void clear();
    CvTrainingData* resampling(CvTrainingData* origin);
    void bayeslearning(CvTrainingData* data);
    void annlearning(CvTrainingData *data);
    double bayespredict(CvMat *sample);
    double annpredict(const CvMat *sample) const;
    double myerror();
    void thresholdlearning(CvTrainingData *data);
    double thresholderror(double thesld, CvMat *weights, CvMat *data, CvMat *classes);
    double thresholdpredict(double thresld, double input);
    double thresholdpredict(CvMat* sample);
    double calcparity(CvTrainingData *data);
    double getthreshold();
    double getparity();
    void setType(int Learner_Type);
    void setparity(double p);
    void setthreshold(double thres);
    double predict(double value);
    void describe();
    double confident(double value);
    double thresholdconfident( double input);
    void fldlearning(CvTrainingData *data);
    double annpredict(double value) const;
    double bayespredict(double value);
    double fldpredict( double value );
    void update(CvTrainingData *data);
    void svmlearning(CvTrainingData *data);
    double svmpredict(double value);
    double svmpredict( CvMat *sample );
    enum { BAYES = 0, KNEAR = 1, SVM = 2, ANN = 3, FLD = 4, POTSU = 5};
 
protected:
    int type;
    int nsample;
    int nelement;
    //CvNormalBayesClassifier* bayes;
    //CvStatModel* classifier;
    CvNormalBayesClassifier *bayes;
    CvANN_MLP *ann;
    CvSVM *svm;
    bool IsTrained;
    double error;
    double alpha;
    double fprate;
    double thresholding;
    double parity;
    int num_neg;
    int num_pos;
    int fp;
    int fn;
    double w;
    double b;
};

#endif
