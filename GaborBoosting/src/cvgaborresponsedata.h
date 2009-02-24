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
#ifndef PREPAREDATACVGABORRESPONSEDATA_H
#define PREPAREDATACVGABORRESPONSEDATA_H

#include "cvfacedb.h"
#include "cvxm2vts.h"
#include "cvpoolparams.h"
#include "cvgabor.h"
#include "cvgaborfeature.h"

namespace PrepareData {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvGaborResponseData{
public:
    CvGaborResponseData();

    ~CvGaborResponseData();
     CvGaborResponseData(const CvFaceDB *db, const CvPoolParams *param);
    void setDB(const CvFaceDB *db);
    void setParam(const CvPoolParams *param);
    void clear();
    void generate();
    void loadData(const char* datapath);
    double getfeaturefrominstance(const CvGaborFeature *feature, int client_index, int picture_index) const;
    CvMat* getfeaturefromall(const CvGaborFeature *feature) const;
     CvGaborResponseData(CvFaceDB *db, CvPoolParams *param, const char *saved_data_path);
    CvFaceDB* getDB() const;
    enum { XM2VTS = 0,FERET = 1};


protected:
    IplImage **responses;
    int nRespones;
    int nOrientations;
    int nScales;
    int nImages;
    int dbtype;
    CvFaceDB *database;
    CvMat *Orients;
    CvMat *Scales;


};

}

#endif


