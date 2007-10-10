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
#ifndef CVXM2VTS_H
#define CVXM2VTS_H

#include <cvfacedb.h>
#include <cxcore.h>
#include <iostream>

#define MAXPIC  8
#define MAXSUB  295

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvXm2vts : public CvFaceDB
{
public:
    CvXm2vts();

    ~CvXm2vts();
     CvXm2vts(const char* where);
    void setNumSub(int s);
    void setNumPic(int p);
    void setPicIndex(int i1, int i2, int i3, int i4, int i5, int i6, int i7);
    void setPicIndex(int i1, int i2, int i3, int i4, int i5, int i6);
    void setPicIndex(int i1, int i2, int i3, int i4, int i5);
    void setPicIndex(int i1, int i2, int i3, int i4);
    void setPicIndex(int i1, int i2, int i3);
    void setPicIndex(int i1, int i2);
    void setPicIndex(int i1);
    void setPicIndex(CvMat* mat);
    void setSubIndex(CvMat* mat);
    void clear();
    char* getPath();
    CvMat* getSub();
    CvMat* getPic();
    CvXm2vts(const CvXm2vts& a);
    CvXm2vts* clone();
    int get_num_pic();
    int get_num_sub();

protected:
    char* pathname;
    CvMat* picIndex;
    int numsub;
    int numpic;
    CvMat* subIndex;
    bool isPic;
    bool isSub;
};

#endif
