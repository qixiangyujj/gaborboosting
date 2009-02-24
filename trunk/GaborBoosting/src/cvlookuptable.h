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
#ifndef ADAGABORCVLOOKUPTABLE_H
#define ADAGABORCVLOOKUPTABLE_H
#include "cvgaborfeature.h"
#include "cvgaborfeaturepair.h"
#include <vector>
#include "cvbin.h"
#include "cv2bins.h"

namespace AdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvLookUpTable{
public:
    CvLookUpTable();

    ~CvLookUpTable();


    void setNumBins(int num);
    void setType(int flag);
    void clear();
    void build();
    int numsample();
    int retrieve(int binindex);
    int retrieve(int binindex1, int binindex2);
     CvLookUpTable(CvTrainingData *data);
     CvLookUpTable(CvTrainingData *data1, CvTrainingData *data2);
  

protected:
    CvTrainingData *data1;
    CvTrainingData *data2;
    CvTrainingData *data;
    int num_bins;
    int type;
    const char  *pathname;
    CvMat* index;
    vector<CvBin*> binlist;
    vector<CvBin*>::iterator pos;
    vector<Cv2Bins*> twinbinlist;
    vector<Cv2Bins*>::iterator twinpos;
    int num;
    int num_bintwins;
    int num_data;
};

}

#endif
