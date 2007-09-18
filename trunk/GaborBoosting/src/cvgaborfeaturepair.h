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
#ifndef ADABOOSTINGCVGABORFEATUREPAIR_H
#define ADABOOSTINGCVGABORFEATUREPAIR_H

#include "cvgaborfeature.h"

namespace AdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvGaborFeaturePair{
public:
    CvGaborFeaturePair();

    ~CvGaborFeaturePair();
     CvGaborFeaturePair(CvGaborFeature* feature1, CvGaborFeature* feature2);
    bool addFeature(CvGaborFeature* feature);
    AdaGabor::CvGaborFeaturePair* clone();
    CvGaborFeature* getFeature1();
    CvGaborFeature* getFeature2();
    CvGaborFeaturePair(const CvGaborFeaturePair& p);
    AdaGabor::CvGaborFeaturePair& operator=(const AdaGabor::CvGaborFeaturePair& a);
    void setMutInf(double inf);
    double getMutInf();
    void describe();
    CvTrainingData* getData(CvFaceDB *database, int n);
    bool operator < (const CvGaborFeaturePair& a);
    bool operator > (const CvGaborFeaturePair& a);
    CvGaborFeature* getFeature(int n);
    void clear();
    void write(const char* filename);


protected:
    CvGaborFeature* feature1;
    CvGaborFeature* feature2;
    int num;
    double MutInf;
private:
    void writeXML(const char* filename);
    void writeTXT(const char *filename);
};

}

#endif
