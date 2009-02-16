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
    int getnumcls();
    int getnumsample();
    void setnumsample(int num);
    void setnumcls(int num);
    bool loadIris(const char *filename);
    int getnumelement();
    void setnumelement(int num);
    void init(int numcls, int numsample, int numelement);
    int getnumsamcls(int clsidx);
    void stat();
    CvTrainingData* clone();
    void setclsidxofsample(int clsidx, int sampleidx);
    int getclsidxofsample(int sampleidx);
    void setweightofsample(double weightvalue, int sampleidx);
    double getweightofsample(int sampleidx);
    void statclsdist();
    CvTrainingData* split(int clsidx);
    CvTrainingData* merge(CvTrainingData *data);
    CvTrainingData* extract(int elementidx);
    CvTrainingData* combine(CvTrainingData *data);
    CvMat* getdata();
    CvMat* getresponse();
    void setdata(CvMat *mat);
    void setresponse(CvMat* response);
    void saveweight(const char *filename);
    void savedata(const char *filename);
    bool loadUCI(const char  *filename);
    CvMat* getweights();
    void setweights(CvMat *wmat);
    CvTrainingData* split(int clsidx1, int clsidx2);
  
    

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
