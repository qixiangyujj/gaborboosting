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

/**
@author Mian Zhou
*/


class CvGaborFeature{
public:
    CvGaborFeature();

    ~CvGaborFeature();
    CvGaborFeature(int x, int y, int Mu, int Nu);
    CvGaborFeature(int x, int y, double Phi, int Nu);
    double val(IplImage *img);
    void Init(int x, int y, double Phi, int Nu);
    double XM2VTSdata(const char *pathname, int subject, int pic);
    void clear();
    double val(const char *filename);
    double geterror();
    void seterror(double e);
    CvTrainingData* _XM2VTSBin(const char *pathname, int possub, CvMat *index);
    CvTrainingData* _XM2VTSBin(const char *pathname, int possub);
    CvTrainingData* _XM2VTSBin(const char *pathname, bool reduce);
    CvTrainingData* _XM2VTSBin(const char *pathname);
    CvTrainingData* _XM2VTSMulti(CvXm2vts *database);
    CvTrainingData* _XM2VTSMulti(const char *pathname, CvMat* picIndex, CvMat* subIndex);
    CvTrainingData* _XM2VTSMulti(const char *pathname, CvMat* index);  
    CvGaborFeature* clone();
    void Init(int x, int y, int Mu, int Nu);
    void CreateGabor();
    int getx();
    int gety();
    int getMu();
    int getNu();
    double getPhi();
    void extractname(char * name, const char * filename); 
    bool operator<(const CvGaborFeature& a);
    bool operator>(const CvGaborFeature& a);
    bool operator==(const CvGaborFeature& a);
    CvGaborFeature & operator=(const CvGaborFeature& a);
    CvGaborFeature(const CvGaborFeature& f);
    void write(const char* filename);
    bool neighbor(CvGaborFeature *feature, int num);
    CvTrainingData* _XM2VTSBin_F(const char *pathname, int possub, CvMat *index);
    CvTrainingData* _XM2VTSBin_F(const char *pathname, int possub);
    CvTrainingData* _XM2VTSMulti_F(const char *pathname, CvMat* picIndex, CvMat* subIndex);
    CvTrainingData* _XM2VTSMulti_F(const char *pathname, CvMat* index);
    CvTrainingData* _XM2VTSMulti_F(CvXm2vts *database);
    double XM2VTSdata(const char *pathname, int subject, int pic, bool reduce);
    double val(const char *filename, int scale);
    double val(IplImage* img, int scale);
    CvTrainingData* _FERETBin(CvFeret* feret, CvPoolParams *param);
    CvTrainingData* _FERETBin(const char *pathname, bool reduce);
    double FERETdata(const char *pathname, int subject, const char* imgname, bool reduce);
    double FERETdata(const char *pathname, int subject, const char* imgname);

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
    void writeTXT(const char *filename);
    void writeXML(const char* filename);
    

    
    
    
    
};

#endif
