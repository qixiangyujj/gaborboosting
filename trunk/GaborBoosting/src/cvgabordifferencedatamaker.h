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
#ifndef CVGABORDIFFERENCEDATAMAKER_H
#define CVGABORDIFFERENCEDATAMAKER_H
#include "cvfacedb.h"
#include "cvgaborfeature.h"
#include "cvgaborresponsedata.h"
#include "cvgabordatamaker.h"
//#include "GaborBoosting.h"
using namespace PrepareData;
/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvGaborDifferenceDataMaker : public CvGaborDataMaker{
public:
    CvGaborDifferenceDataMaker();

    ~CvGaborDifferenceDataMaker();
     CvGaborDifferenceDataMaker(CvGaborResponseData *data, CvGaborFeature *gaborfeature, CvFaceDB *db);
    CvMat* getIntraDifference() const;
    CvMat* getExtraDifference() const;
    CvTrainingData* getDifference() const;
    int getNumIntraDifference() const;
    int getNumExtraDifference() const;
    int getNumDifference() const;
    CvTrainingData* getDifference(CvMat *labels) const;
    CvMat* getLabels() const;
    CvTrainingData* getData() const;
    int getNumExamples() const;

protected:
    //CvFaceDB *database;
    //CvGaborFeature *feature;
    //CvGaborResponseData *gabordata;
};

#endif
