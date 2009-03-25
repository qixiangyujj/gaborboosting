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
#ifndef CVFACESVMCLASSIFIER_H
#define CVFACESVMCLASSIFIER_H

#include <ml.h>
#include <cvfaceclassifier.h>
#include <cvgaborfeaturepool.h>
#include <cvgaborresponsedata.h>
#include <cvxm2vts.h>
#include <cvfacedb.h>
#include <cvgabordifferencedatamaker.h>


using namespace PrepareData;

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvFaceSVMClassifier : public CvFaceClassifier
{
public:
    CvFaceSVMClassifier();

    ~CvFaceSVMClassifier();
    void Train(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features);
    void clear();
    void Load(const char *filename);
    void Save(const char * filename);
    void SetKernel(int kernel_type);
    CvFaceSVMClassifier( int kernel, int numclass, int max_iter, double epsilon);
    virtual CvMat* GetDataFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const;
    void train_svm_auto(CvMat *train_data, CvMat * labels);
    void train_svm( CvMat * train_data, CvMat * labels);
    virtual CvMat* GetLabelsFromFeatures(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const;
    void AutoTrain(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features);
    void SetParams(CvSVMParams svmParams);
    void SetParams(int _svm_type, int _kernel_type, double _degree, double _gamma, double _coef0, double _C, double _nu, double _p, CvTermCriteria _term_crit);
    double Predict( const CvMat * sample) const;
    virtual CvScalar Training_error(CvGaborResponseData & gabordata, CvGaborFeaturePool & new_features) const;
    virtual CvScalar Training_error(CvMat * train_data, CvMat * labels) const;

protected:
    CvSVM svm;
    CvSVMParams params;
};

#endif
