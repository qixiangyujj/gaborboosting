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
#ifndef CVADABOOSTFEATURESELECTION_H
#define CVADABOOSTFEATURESELECTION_H
#include "cvfacedb.h"
#include "cvgaborresponsedata.h"
#include "cvgaborfeaturepool.h"
#include "cvweaklearner.h"
#include "cvgabordifferencedatamaker.h"

#define SIGN_FILE	"significant.txt"
#define STATE_FILE	"state.txt"


using namespace PrepareData;

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvAdaBoostFeatureSelection{
public:
    CvAdaBoostFeatureSelection();

    ~CvAdaBoostFeatureSelection();
    void SetFeatures(CvPoolParams *param);
    void InitializeWeights(CvMat *labels);
    CvGaborFeaturePool* Select(int numfeatures);
    void NormalizeWeights();
    CvGaborFeature* FindSignificantFeature(CvGaborFeaturePool* features);
    void UpdateWeights(CvGaborFeature *feature);
    double TrainWeaklearner(CvGaborFeature *feature, int learner_type);
    void SetType(int learner_type);
    CvTrainingData* GetDataforWeak(CvGaborFeature *feature, CvGaborResponseData *memdata);
    CvAdaBoostFeatureSelection(CvGaborResponseData *memdata, CvMat *labels, CvPoolParams *param, int learner_type);
    void clear();
    void SaveWeights(const char *filename) const;
    void SaveWeights(int Iter) const;
    bool isResume() const;
    void Resume();
    void LoadWeights(const char *filename);
    void LoadWeights(int Iter);
    

protected:
    CvMat *m_labels;
    CvGaborResponseData *m_memdata;
    CvGaborFeaturePool *m_features;
    CvGaborFeaturePool *m_selectedfeatures;
    CvMat* m_weights;
    int m_learner_type;
    int m_current_iter;
    int m_total_iter;
    void OutputState(const char * filename) const;
    void ReadState(const char *filename);
};

#endif
