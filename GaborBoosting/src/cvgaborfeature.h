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
#ifndef CVGABORFEATURE_H
#define CVGABORFEATURE_H
#include "cvgabor.h"
#include "cvtrainingdata.h"
#include "cvbinarytrainingdata.h"
#include "cvxm2vts.h"
#include "cvferet.h"
#include "cvpoolparams.h"
//#include "cvgaborresponsedata.h"
//#include "cvfacedb.h"

/**
@author Mian Zhou
*/



class CvGaborFeature{
public:
    CvGaborFeature();

    ~CvGaborFeature();
    CvGaborFeature(int x, int y, int Mu, int Nu);
    CvGaborFeature(int x, int y, double Phi, int Nu);
    double val(const IplImage *img);
    void Init(int x, int y, double Phi, int Nu);
    double XM2VTSdata(const char *pathname, int subject, int pic) const;
    void clear();
    double val(const char *filename) const;
    double geterror() const;
    void seterror(double e);
    CvTrainingData* _XM2VTSBin(const char *pathname, int possub, const CvMat *index) const;
    CvTrainingData* _XM2VTSBin(const char *pathname, int possub) const;
    CvTrainingData* _XM2VTSBin(const char *pathname, bool reduce) const;
    CvTrainingData* _XM2VTSBin(const char *pathname) const;
    CvTrainingData* _XM2VTSMulti(const CvXm2vts *database) const;
    CvTrainingData* _XM2VTSMulti(const char *pathname, const CvMat* picIndex, const CvMat* subIndex) const;
    CvTrainingData* _XM2VTSMulti(const char *pathname, const CvMat* index) const;
    CvGaborFeature* clone() const;
    void Init(int x, int y, int Mu, int Nu);
    void CreateGabor();
    int getx() const;
    int gety() const;
    int getMu() const;
    int getNu() const;
    double getPhi() const;
    void extractname(char * name, const char * filename) const; 
    bool operator<(const CvGaborFeature& a);
    bool operator>(const CvGaborFeature& a);
    bool operator==(const CvGaborFeature& a);
    CvGaborFeature & operator=(const CvGaborFeature& a);
    CvGaborFeature(const CvGaborFeature& f);
    void write(const char* filename) const;
    bool neighbor(const CvGaborFeature *feature, int num) const;
    CvTrainingData* _XM2VTSBin_F(const char *pathname, int possub, const CvMat *index) const;
    CvTrainingData* _XM2VTSBin_F(const char *pathname, int possub) const;
    CvTrainingData* _XM2VTSMulti_F(const char *pathname, const CvMat* picIndex, const CvMat* subIndex) const;
    CvTrainingData* _XM2VTSMulti_F(const char *pathname, const CvMat* index) const;
    CvTrainingData* _XM2VTSMulti_F(const CvXm2vts *database) const;
    double XM2VTSdata(const char *pathname, int subject, int pic, bool reduce) const;
    double val(const char *filename, int scale) const;
    double val(const IplImage* img, int scale);
    CvTrainingData* _FERETBin(const CvFeret* feret, const CvPoolParams *param) const;
    CvTrainingData* _FERETBin(const char *pathname, bool reduce) const;
    double FERETdata(const char *pathname, int subject, const char* imgname, bool reduce) const;
    double FERETdata(const char *pathname, int subject, const char* imgname) const;
    CvTrainingData* _FERETBin_F(const CvFeret* feret, int possub, const CvMat *index) const;
    CvTrainingData* _XM2VTSGender_F(const CvXm2vts* xm2vts, int ntpic) const;
    CvTrainingData* _XM2VTSGender_F(const CvXm2vts* xm2vts, const CvMat* index) const;
    
protected:
    int ix;
    int iy;
    int iMu;
    int iNu;
    CvGabor* gabor;
    double dPhi;
    bool isInitialised;
    double error;
    bool GaborCreated;
private:
    void writeTXT(const char *filename) const;
    void writeXML(const char* filename) const;

    

    
    
    
    
};

#endif
