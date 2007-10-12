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
#ifndef MULTIADAGABORCVMULTIGABADAFSM1_H
#define MULTIADAGABORCVMULTIGABADAFSM1_H
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cvpoolparams.h"
#include "cvfacedb.h"
#include "cvgaborfeaturepool.h"
#include "cvmweaklearner.h"
namespace MultiAdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvMultiGabAdaFSM1{
public:
    CvMultiGabAdaFSM1();
    CvMultiGabAdaFSM1(CvFaceDB* facedb, CvGaborFeaturePool* pool);
    CvMultiGabAdaFSM1(CvFaceDB* facedb, CvPoolParams* params);
    ~CvMultiGabAdaFSM1();
    void setPool(CvPoolParams *param);
    void setDB(CvFaceDB *facedb);
    void init_weights();
    void train(int nfeatures);
    void normalise(CvMat *mat);
    void trainallfeature(CvGaborFeaturePool *pool);
    double featureweak(CvGaborFeature* feature);
    double findminerror(CvGaborFeaturePool *pool);
    void signfeature(CvGaborFeature *feature);
    void update(CvMat* mat);
    void writefeature(const char* filename, CvGaborFeature *feature, double error, double alpha);
    void writeweights(const char *filename, CvMat *mat);
    void clear();
    void resume();
    void resume(int n);
    void continue_training(int no, int current);
    void loadweights(const char* filename);
    void trainingsub(int job, int iter);
    void resumesub(int job, int iter);
    double findmin();
    void loadsign(const char* filename);
    void update();
    int  getNumFeatures();
    void setCurrentIter(int c);
    int getCurrentIter();
    CvMat* predict(IplImage *img, int nweaks);
    CvMat* predict(const char *filename, int nweaks);
    CvMat* predict(IplImage *img);
    CvMat* predict(const char *filename);
    void testing(const char* filelist, const char* resultfile, int nofeatures);
    void testing(const char* filelist, const char* resultfile);

    

protected:
    CvGaborFeaturePool* old_pool;
    CvGaborFeaturePool* new_pool;
    CvFaceDB* database;
    vector<CvMWeakLearner> mweaks;
    vector<double> alphas;
    int db_type;
    enum { XM2VTS = 0, FERET = 1};
    int nsamples;
    int nClass;
    int nperClass;
    CvMat* weights;
    int nexpfeatures;
    int nselecfeatures;
    int current;
 
};

}

#endif
