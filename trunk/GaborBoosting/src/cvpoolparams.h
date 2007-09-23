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
#ifndef CVPOOLPARAMS_H
#define CVPOOLPARAMS_H
#include <cxcore.h>

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvPoolParams{
public:
    CvPoolParams();

    ~CvPoolParams();
     CvPoolParams(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound);
     CvPoolParams(int height, int width, int minscale, int maxscale, int norient, int interval, int bound);
    void setInterval(int i);
    void setBound(int b);
    int getInterval();
    int getBound();
    CvMat* getScales();
    CvMat* getOrients();
    CvSize getImgSize();
    void clear();
    void setReduce(bool key);
     CvPoolParams(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound, bool reduced);
     CvPoolParams(int height, int width, int minscale, int maxscale, int norient, int interval, int bound, bool reduced);
    bool isReduced();

protected:
    CvSize imgSize;
    CvMat* scales;
    CvMat* orients;
    int interval;
    int bound;
    bool reduce;
private:
    void init(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound);
    void init(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound, bool reduced);
};

#endif
