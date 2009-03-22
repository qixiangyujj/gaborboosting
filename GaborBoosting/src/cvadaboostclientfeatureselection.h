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
#ifndef CVADABOOSTCLIENTFEATURESELECTION_H
#define CVADABOOSTCLIENTFEATURESELECTION_H

#include <cvadaboostfeatureselection.h>
#include <cvgaborclientdatamaker.h>

using namespace PrepareData;
/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvAdaBoostClientFeatureSelection : public CvAdaBoostFeatureSelection
{
public:
    CvAdaBoostClientFeatureSelection();

    ~CvAdaBoostClientFeatureSelection();
     CvAdaBoostClientFeatureSelection(CvGaborResponseData *memdata, CvMat *labels, CvPoolParams *param, int learner_type, int client);
    CvTrainingData* GetDataforWeak(CvGaborFeature *feature, CvGaborResponseData *memdata);

protected:
    int client_no;
};

#endif
