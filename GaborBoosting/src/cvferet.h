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

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <cvfacedb.h>
#include <vector>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "cvgabor.h"
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
    int getSub() const;
    int getNum() const;
    CvSubject getSubject(int index) const;
    string getFApath() const;
    string getFBpath() const;
    void openorigin(const char* dvd1path, const char* dvd2path);
    void readorigin(const char* path);
    void readfrmsubjectpath(const char* subpath, CvSubject *subject);
    void make(const char* dbpath);
  //void process40Gabor( const char* filename, const char* subpath );
    IplImage* reducebyscale(IplImage *src, int Nu);
    string getMainpath() const;
    void opentestingPath(const char* pathname);
    CvFeret* clone() const;
    int getFbSub() const;
    int getFbNum() const;
    CvSubject getFbSubject(int index) const;
    CvSubject getFaSubject(int index) const;
    int getFaSub() const;
    int  getFaNum() const;
    void calc_stat();
    int getNumFaSubject(int ID) const;
    int getNumFbSubject(int ID) const;
    int getPosFaSubject(int ID) const;
    int getPosFbSubject(int ID) const;
    CvSize getSize() const;
    int getPosFaSubjectIND(int index) const;
    int getPosFbSubjectIND(int index) const;
    
    


protected:
    char* pathname;
    char* fa_path;
    char* fb_path;
    int numsub;
    int numpic;
    vector<CvSubject> subjects;
    vector<CvSubject>::iterator pos;
    vector<CvSubject> testsubjects;
    int testnumpic;
    int testnumsub;
    CvMat* fa_distrb;
    CvMat* fb_distrb;
    CvMat* fa_ID_index;
    CvMat* fb_ID_index;
};

#endif
