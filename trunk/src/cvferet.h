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
#ifndef CVFERET_H
#define CVFERET_H

#include <cvfacedb.h>
#include <vector>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "cvsubject.h"

using namespace std;

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvFeret : public CvFaceDB
{
public:
    CvFeret();

    ~CvFeret();
    void setPath(const char* mainpath, const char*fapath, const char* fbpath);
    void openPath(const char* pathname);
    void openPath(const char* faname, const char* fbname);
     CvFeret(const char* mainpath);
     CvFeret(const char* mainpath, const char* fapath, const char* fbpath);
     CvFeret(const char* mainpath, const char* fapath);
    void clear();
    int getSub();
    int getNum();
    CvSubject& getSubject(int index);
    string getFApath();
    string getFBpath();
    


protected:
    char* pathname;
    char* fa_path;
    char* fb_path;
    int numsub;
    int numpic;
    vector<CvSubject> subjects;
};

#endif
