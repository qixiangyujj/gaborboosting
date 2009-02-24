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
#include <highgui.h>

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
    void setPicIndex(const CvMat* mat);
    void setSubIndex(const CvMat* mat);
    void clear();
    char* getPath() const;
    CvMat* getSub() const;
    CvMat* getPic() const;
    CvXm2vts(const CvXm2vts& a);
    CvXm2vts* clone() const;
    int get_num_pic() const;
    int get_num_sub() const;
    void setGender(const char* filename);
    bool getGender(int nsub) const;
    void getfilename(int sub, int pic, char *filename) const;
    CvSize getSize() const;

protected:
    char* pathname;
    CvMat* picIndex;
    int numsub;
    int numpic;
    CvMat* subIndex;
    bool isPic;
    bool isSub;
    CvMat* gender;
    bool isGender;
};

#endif
