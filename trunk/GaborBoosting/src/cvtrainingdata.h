/***************************************************************************
 *   Copyright (C) 2006 by Mian Zhou   *
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
#ifndef CVTRAININGDATA_H
#define CVTRAININGDATA_H

//#include <vector>
#include <cv.h>
#include <fstream>

using namespace std;

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvTrainingData{
public:
    CvTrainingData();

    ~CvTrainingData();
    int getnumcls() const;
    int getnumsample() const;
    void setnumsample(int num);
    void setnumcls(int num);
    bool loadIris(const char *filename);
    int getnumelement() const;
    void setnumelement(int num);
    void init(int numcls, int numsample, int numelement);
    int getnumsamcls(int clsidx) const;
    void stat();
    CvTrainingData* clone() const;
    void setclsidxofsample(int clsidx, int sampleidx) const;
    int getclsidxofsample(int sampleidx) const;
    void setweightofsample(double weightvalue, int sampleidx);
    double getweightofsample(int sampleidx) const;
    void statclsdist();
    CvTrainingData* split(int clsidx) const;
    CvTrainingData* merge(const CvTrainingData *data) const;
    CvTrainingData* extract(int elementidx) const;
    CvTrainingData* combine(CvTrainingData *data) const;
    CvMat* getdata() const;
    CvMat* getresponse() const;
    void setdata(const CvMat *mat);
    void setresponse(const CvMat* response);
    void saveweight(const char *filename) const;
    void savedata(const char *filename) const;
    bool loadUCI(const char  *filename);
    CvMat* getweights() const;
    void setweights(const CvMat *wmat);
    CvTrainingData* split(int clsidx1, int clsidx2) const;
    CvTrainingData* getsubset(int start, int end) const;
    CvTrainingData* getTestingSubset(int start, int end);
    CvTrainingData* getTrainingSubset(int start, int end);

  
    

protected:
    int maxnum;
    int *cls;
    double *weight;
    CvMat* Valcache;
    int ncls;
    int* clsdist;
    int nelement;
    bool bInitialised;
    
};

#endif
