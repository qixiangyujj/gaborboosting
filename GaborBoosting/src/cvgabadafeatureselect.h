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
#ifndef CVGABADAFEATURESELECT_H
#define CVGABADAFEATURESELECT_H
#include "cvgaborfeaturepool.h"
#include "cvweaklearner.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
struct CvGabAdaFeatureParams
{
   char sourcepath[50];
   CvGaborFeaturePool *featurepool;
   int flag;
   int nosubject;
   int nofeatures;
   int *testIndex;
   int numindex;
};




struct CvGaborTree
{
   int x;
   int y;
   int Nu;
   int Mu;
   double threshold;
   double parity;
   double alpha;
};

class CvGabAdaFeatureSelect{
public:
    CvGabAdaFeatureSelect();

    ~CvGabAdaFeatureSelect();
    void create(CvGaborFeaturePool *pool, int flags, const char *source, int possub);
    int train(int nfeatures);
    int trainxm2vts();
    double findminerror(int flag);
    double featureweak(CvGaborFeature *feature);
    double signfeature(CvGaborFeature *feature);
    int trainallfeature(CvGaborFeaturePool *pool);
    void updateweights();
    void normalise(CvMat *weight);
    void writefeature(const char *filename, CvGaborFeature *feature, double merror, double fprate);
    void writeweights(const char *filename, CvMat *mat);
    void loadweights(const char *filename);
    void resume(int c);
    double errorthreshold();
    void save(const char *filename);
    void loadweaks(const char* filename);
    void weaknode2tree(CvFileNode *node, CvFileStorage *fs, CvGaborTree *tree);
    double predict(IplImage *img);
    double predict(const char *filename);
    void create(CvGabAdaFeatureParams *params);
    void testing(const char* filelist, const char* resultfile);
    void resume();
    void loadsignfeatures(const char *filename);
    double predict(IplImage *img, int nweaks);
    void testing(const char* filelist, const char* resultfile, int nofeatures);
    double predict(const char *filename, int nweaks);
    void init_weights(int flag);
    void sign2weaks(const char* signfile, const char* weaksname);
    void loadparams(const char *filename);
    void loadindex(const char *filename);
    

    enum { XM2VTS = 0, FERET = 1};

protected:
    CvGaborFeaturePool* origpool;
    CvGaborFeaturePool* newpool;
    vector<CvWeakLearner> weaks;
    vector<double>  alphas;
    int type;
    const char *spath;
    int current;
    int maxnum;
    int nsample;
    CvMat* weights;
    int sub;
    int nselecfeatures;
    double falsepositive;
    int nexpfeatures;
    CvMat *index;
    int nopositivesampels;

private:
    bool isInArray(int i, int *array, int n);
    void clear();
};

#endif
