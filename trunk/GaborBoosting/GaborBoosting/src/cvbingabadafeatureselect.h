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
#ifndef CVBINGABADAFEATURESELECT_H
#define CVBINGABADAFEATURESELECT_H
#include "cvfacedb.h"
#include "cvgaborfeaturepool.h"
#include "cvweaklearner.h"
#include "cvgabadafeatureselect.h"

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvBinGabAdaFeatureSelect{
public:
    CvBinGabAdaFeatureSelect();

    ~CvBinGabAdaFeatureSelect();
    void setDB(CvFaceDB* db);
    void setPool(CvPoolParams *param);
    void train(int nfeatures);
    void init_weights();
    void setSub(int nosub);
     CvBinGabAdaFeatureSelect(CvFaceDB *db, CvPoolParams *param, int possub);
    void normalise();
    void trainallfeatures();
    double findminerror();
    void update();
    double featureweak(CvGaborFeature* feature);
    double errorthreshold();
    void signfeature(CvGaborFeature *feature);
    void writefeature(const char *filename, CvGaborFeature *feature, double param1, double param2);
    void saveweaks(const char *filename);
    void saveweights(const char* filename);
    void setWeaksname(const char* filename);
     CvBinGabAdaFeatureSelect(CvFaceDB *db, CvPoolParams *param, int possub, const char* filename);
    void clear();
    void loadweaks(const char* filename);
    void weaknode2tree(CvFileNode *node, CvFileStorage *fs, CvGaborTree *tree);
    void testing(const char* filelist, const char* resultfile, int nofeatures);
    double predict(const char *filename, int nweaks);
    double predict(IplImage *img, int nweaks);
    void testing(const char* filelist, const char* resultfile);
    void gendertesting( const char* resultfile );
    void gendertesting(const char* resultfile, int nweaks);
    double predict_a(const char *filename, int nweaks);
    double predict_a(IplImage *img, int nweaks);
    void resume();
    void resume(int c);
    void svmtesting(const char* resultfile, int nofeatures);
    float svmpredict(IplImage *img, CvSVM *svm, int nfeatures);
    float svmpredict(const char* filename, CvSVM *svm, int nfeatures);
    void svmtesting( const char* resultfile );
    void svmlearning(const char* path, int nofeatures, CvSVM * svm);
    void svm_test( FILE *file, const char *path, int nofeatures, CvSVM * svm);
    void svm_test_a( FILE *file, const char *path, int nofeatures, CvSVM * svm);
    void setTypeWeak(int type);
    enum { XM2VTS = 0, FERET = 1};
  
protected:
    CvFaceDB* database;
    CvGaborFeaturePool* old_pool;
    CvGaborFeaturePool* new_pool;
    vector<CvWeakLearner> weaks;
    vector<double>  alphas;
    int db_type;
    int nClass;
    int nperClass;
    int nsamples;
    CvMat* weights;
    int possub;
    CvMat* classIdx;
    int npositive;
    int nnegative;
    int nexpfeatures;
    double falsepositive;
    int nselecfeatures;
    int current;
    string weaksname;
    int weaklearner_type;
};

#endif
