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
#ifndef CVPCAFACEREG_H
#define CVPCAFACEREG_H
#include "cvfacedb.h"
#include "cvxm2vts.h"
#include <cvaux.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <ml.h>

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvPCAFaceReg{
public:
    CvPCAFaceReg();

    ~CvPCAFaceReg();
    void setDB(CvFaceDB* db);
    void pca();
    void recognize();
    void doPCA();
    void storeTrainingData( const char * filename );
    int loadFaceImgArray(CvFaceDB *db);
    void showAvgImage();
    void saveAvgImage(const char* filename);
    void showEigenImage(int n);
    void loadTrainingData(const char * filename);
    void clear();
    void svmlearning(int client_no, int nfeatures);
    void svmtesting( const char* filelist, int nfeatures );
    int loadTestImgArray(CvFaceDB *db);
    float svmpredict(const char *imgfilename, int nfeatures);


protected:
    int nTrainImages;
    int nEigens;
    IplImage ** faceImgArr;
    IplImage * pAvgTrainImg;
    IplImage ** eigenVectArr;
    CvMat * eigenValMat;
    CvMat * projectedTrainFaceMat;
    CvFaceDB * database;
    int db_type;
    bool isImgLoaded;

enum { XM2VTS = 0, FERET = 1};
    CvSVM * svm;
    
};

#endif
