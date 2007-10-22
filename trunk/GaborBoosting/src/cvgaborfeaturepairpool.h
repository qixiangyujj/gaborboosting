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
#ifndef ADAGABORCVGABORFEATUREPAIRPOOL_H
#define ADAGABORCVGABORFEATUREPAIRPOOL_H
#include <vector>
#include "cvgaborfeaturepair.h"
#include "cvgaborfeaturepool.h"
#include "cvpoolparams.h"
#include <unistd.h>

namespace AdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvGaborFeaturePairPool{
public:
    CvGaborFeaturePairPool();
    ~CvGaborFeaturePairPool();
     CvGaborFeaturePairPool(CvPoolParams *param);
     CvGaborFeaturePairPool(CvGaborFeaturePool* pool);
     CvGaborFeaturePairPool(CvSize imgsize, CvMat *Scale, CvMat *Orientation, int interval, int bound);
     CvGaborFeaturePairPool(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound);
    AdaGabor::CvGaborFeaturePair* getPair(long index);
    long size();
    void remove(long index);
    AdaGabor::CvGaborFeaturePairPool* clone();
    void push(CvGaborFeaturePair *pair);
    void write(const char* filename);
    void clear();
    void sort();
    void load(const char *featurepoolname, const char *pairpoolname);
    void build(CvGaborFeaturePool* pool, CvGaborFeature* start_feature, CvGaborFeature* end_feature);
    void build(CvGaborFeaturePool* pool);
     CvGaborFeaturePairPool(CvPoolParams *param, CvGaborFeature* start_feature, CvGaborFeature *end_feature);
    void loadMutFile(const char* filename);
    void MinMax(double *min, double *max);

    

protected:
    vector<CvGaborFeaturePair*> pairs;
    vector<CvGaborFeaturePair*>::iterator pos;
    CvGaborFeaturePool* features;

     
private:
    void writeXML(const char *filename);
    void writeTXT(const char* filename);

};

bool SortPredicate( CvGaborFeaturePair *pair1, CvGaborFeaturePair *pair2 );

}

#endif
