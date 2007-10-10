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
#ifndef ADAGABORCVGABORFEATUREPAIRSELECT_H
#define ADAGABORCVGABORFEATUREPAIRSELECT_H
#include "cvpoolparams.h"
#include "cvfacedb.h"
#include "cvgaborfeaturepairpool.h"
#include "cvmutinf.h"

#define DEFAULT_NO_BINS 10

namespace AdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvGaborFeaturePairSelect{
public:
    CvGaborFeaturePairSelect();

    ~CvGaborFeaturePairSelect();
    CvGaborFeaturePairSelect(CvPoolParams *param, CvFaceDB *facedb);
    CvGaborFeaturePairSelect(CvPoolParams *param, CvFaceDB *facedb, int nobins);
    void setPool(CvPoolParams *param);
    void setDB(CvFaceDB *facedb);
    void setNumBins(int b);
    void build();
    void clear();
    void resume();
    void resume(int n);
    void setPool(CvPoolParams *param, CvGaborFeature *start_feature, CvGaborFeature *end_feature);
    void setMutFile(const char* filename);
    

protected:
    CvGaborFeaturePairPool *old_pool;
    CvGaborFeaturePairPool *new_pool;
    CvFaceDB *database;
    int numbins;
    char* mutfile;
};

}

#endif
