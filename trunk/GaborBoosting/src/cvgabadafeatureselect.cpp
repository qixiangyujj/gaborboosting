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
#include "cvgabadafeatureselect.h"

CvGabAdaFeatureSelect::CvGabAdaFeatureSelect()
{
//spath = "/windows/D/Data/xm2vts/";
}


CvGabAdaFeatureSelect::~CvGabAdaFeatureSelect()
{
  clear();
}




/*!
    \fn CvGabAdaFeatureSelect::create(CvGaborFeaturePool *pool, int flags, const char *source, int possub)
 */
void CvGabAdaFeatureSelect::create(CvGaborFeaturePool *pool, int flags, const char *source, int possub)
{
    type = flags;
    origpool = pool;
    //origpool = pool->clone();
    //strcpy(spath, source);
    spath = source;
    maxnum = pool->getSize();
    newpool = new CvGaborFeaturePool;
    sub = possub;
    switch (type)
    {
         case XM2VTS:
         //strcpy(path, "/windows/D/Data/xm2vts/");
      //init_weights(0);
      init_weights(0);
         break;
    }
}


/*!
\fn CvGabAdaFeatureSelect::init_weights(int flag)
 */
void CvGabAdaFeatureSelect::init_weights(int flag)
{
  double posweight, negweight;
  int smin, smax;
  if (nopositivesampels == 4)
  {
    nsample = 800;
    posweight = 1.0/2/4; 
    negweight = 1.0/2/(nsample -4);
    smin = (sub-1)*4;
    smax = sub*4;
  }
  else 
  {
    CvSize size = cvGetSize(index);
    int numpos = size.width;
    int numneg = (200 - 1)*4;
    nsample = numpos + numneg;
    posweight = 1.0/2/numpos;
    negweight = 1.0/2/numneg;
    smin = (sub -1)*4;
    smax = (sub -1)*4+numpos; 
  }
  
  weights = cvCreateMat(nsample, 1, CV_32FC1);
  if(flag == 0)
  {
    for (int i = 0; i < nsample; i++)
    {
      if ((i >= smin)&&(i < smax)) cvSetReal1D(weights, i, posweight);
      else cvSetReal1D(weights, i, negweight); 
    }
  }
  else
  {
    for (int i = 0; i < nsample; i++)
    {
      cvSetReal1D(weights, i, 1.0/(double)nsample); 
    }
  }
}



/*!
    \fn CvGabAdaFeatureSelect::train(int nfeatures)
 */
int CvGabAdaFeatureSelect::train(int nfeatures)
{
    nexpfeatures = nfeatures;
    switch(type)
    {
          case XM2VTS:
          trainxm2vts();
          break;
    }
}


/*!
    \fn CvGabAdaFeatureSelect::trainxm2vts()
 */
int CvGabAdaFeatureSelect::trainxm2vts()
{
    double error = 0.0; 
    nselecfeatures = 0;
    char *signfilename = new char[20];
    strcpy(signfilename, "significant.txt");
    FILE * file = fopen(signfilename, "w");
    fclose( file );
    for (int i = 0; i < nexpfeatures; i++)
    {
      current = i;
      if (origpool->getSize() < 1) break;
      
      char *allfeaturefilename = new char[20];
      sprintf(allfeaturefilename, "allfeature_%d.txt", current);
      char *disfeaturefilename = new char[20];
      sprintf(disfeaturefilename, "discard_%d.txt", current);
      char *featurefilename = new char[20];
      sprintf(featurefilename, "feature_%d.txt", current); 
      
      FILE * file1 = fopen(allfeaturefilename, "w");
      fclose( file1 );
      FILE * file2 = fopen(disfeaturefilename, "w");
      fclose( file2 );
      FILE * file3 = fopen(featurefilename, "w");
      fclose( file3 );
      
      
      normalise(weights);
      trainallfeature(origpool);
      error = findminerror(type);
      updateweights();
      delete [] allfeaturefilename;
      delete [] disfeaturefilename;
      delete [] featurefilename;
   }
   delete [] signfilename;
   return nselecfeatures; 
}


/*!
    \fn CvGabAdaFeatureSelect::findminerror(int flag)
 */
double CvGabAdaFeatureSelect::findminerror(int flag)
{
     double merror;
     if (origpool->getSize() < 1)
     {
       return -1.0;
     }
     else
     { 
       CvGaborFeature *sfeature;
       sfeature = origpool->min();
       merror = sfeature->geterror();
       signfeature(sfeature);
       return merror;
     }
}






/*!
    \fn CvGabAdaFeatureSelect::featureweak(CvGaborFeature *feature)
 */
double CvGabAdaFeatureSelect::featureweak(CvGaborFeature *feature)
{
    double e;
    CvTrainingData *data;
  //if (nopositivesampels == 4)
  //{
      //data = feature->_XM2VTSBin(spath, sub);
  //  data = feature->_XM2VTSBin_F( spath, sub);
  //}
  //else 
    {
      //data = feature->_XM2VTSBin(spath, sub, index);
      data = feature->_XM2VTSBin_F( spath, sub, index);
    }
    data->setweights(weights);
    CvWeakLearner *weak = new CvWeakLearner;
    weak->train(data, CvWeakLearner::THRESHOLD);
  //weak->describe();
    //weak->train(data, CvWeakLearner::ANN);
    e = weak->training_error();
    weak->clear();
    feature->seterror(e);
    falsepositive = weak->myerror();
    delete data;
    delete weak;

    return e;
}



/*!
    \fn CvGabAdaFeatureSelect::signfeature(CvGaborFeature *feature)
 */
double CvGabAdaFeatureSelect::signfeature(CvGaborFeature *feature)
{
    double merror = feature->geterror();
    newpool->add(feature);
    nselecfeatures++;
    return merror;
}


/*!
    \fn CvGabAdaFeatureSelect::trainallfeature(CvGaborFeaturePool *pool)
 */
int CvGabAdaFeatureSelect::trainallfeature(CvGaborFeaturePool *pool)
{
    // time
    time_t start,end;
    double dif;
  
    int maxfeatures = pool->getSize();
    double error;
    /* For each feature, trainiging a weaklearner, and get the error */
    int n = 0;
  
    double errthreshold = errorthreshold();
    
    char *allfeaturefilename = new char[20];
    sprintf(allfeaturefilename, "allfeature_%d.txt", current);
    char *disfeaturefilename = new char[20];
    sprintf(disfeaturefilename, "discard_%d.txt", current);
    char *featurefilename = new char[20];
    sprintf(featurefilename, "feature_%d.txt", current); 
    time (&start);
    for (int i = 0; i <pool->getSize(); i++)
    {
      
       CvGaborFeature *feature;
       feature = pool->getfeature(i);
      //printf("Get a feature: Scale %d and Orientation %d at (%d,%d)\n", feature->getNu(), feature->getMu(), feature->getx(), feature->gety());
       /* recored each error */
       error = featureweak(feature);
       writefeature( allfeaturefilename, feature, error, falsepositive );
       if ((error > errthreshold)||(falsepositive > 0.5))
       //if(error > 0.5)
       {
          writefeature( disfeaturefilename, feature, error, falsepositive );
         //printf("Rejecting this feature .............\n");
          pool->remove(i);
          i--;
         n++;
       }
       else
       {
          writefeature( featurefilename, feature, error, falsepositive );
       }
       maxfeatures = pool->getSize();
      //int p = i/maxfeatures;
      std::cout << "Trainging in the iteration: " << i<< "\r" << std::flush;
    }
    std::cout << endl;
    time (&end);
    dif = difftime (end,start);
    printf("%d features remain, and %d features are filtered out.\n", n, maxfeatures-n);
    if(dif >= 3600) printf("\n\n\n This iteration %d takes %.2lf hours.\n\n\n\n", current+1, dif/3600);
    else if(dif >= 60) printf("\n\n\n This iteration %d takes %.2lf minutes.\n\n\n\n", current+1, dif/60);
    else if(dif < 60) printf("\n\n\n This iteration %d takes %.2lf seconds.\n\n\n\n", current+1, dif);
  
    delete [] disfeaturefilename;
    delete [] allfeaturefilename;
    delete [] featurefilename;
    return n;
}


/*!
    \fn CvGabAdaFeatureSelect::updateweights()
 */
void CvGabAdaFeatureSelect::updateweights()
{
    if (origpool->getSize() >= 1)
    {
      double error, alpha;
      CvGaborFeature *feature;
      feature = newpool->getfeature(current);
       
      CvTrainingData *data;
      //if (nopositivesampels == 4)
      //{
      //  data = feature->_XM2VTSBin_F(spath, sub);
      //}
      //else 
      {
        data = feature->_XM2VTSBin_F(spath, sub, index);
      }
      
      data->setweights(weights);
    
      CvWeakLearner *weak = new CvWeakLearner;
      weak->train(data, CvWeakLearner::THRESHOLD);
    
      weights = data->getweights();
      error = weak->training_error();
      alpha = weak->importance();

      printf("Push a weaklearner .......\n");
      weaks.push_back(*weak);
      alphas.push_back(alpha);

      char *signfilename = new char[20];
      strcpy(signfilename, "significant.txt");
      writefeature(signfilename, feature, error, alpha);
    

      char *weightfilename = new char[30];
      sprintf(weightfilename, "upweight_%d.xml", current+1);
      writeweights(weightfilename, weights);

      delete [] signfilename;
      delete [] weightfilename;
      delete data;
    }
}


/*!
    \fn CvGabAdaFeatureSelect::normalise()
 */
void CvGabAdaFeatureSelect::normalise(CvMat *weight)
{
    int num = nsample;
    double v ;
    double sum = 0.0;
    for (int i = 0; i < num; i++)
    {
      v = cvGetReal1D(weight, i);
      sum = sum + v;
    }
    for (int i = 0; i < num; i++)
    {
      v = cvGetReal1D(weight, i);
      cvSetReal1D(weight, i, v/sum);
    }
    char *weightfilename = new char[30];
    sprintf(weightfilename, "weight_%d.xml", current);
    writeweights(weightfilename, weights);
    delete [] weightfilename;
}


/*!
    \fn CvGabAdaFeatureSelect::writefeature(const char *filename, CvGaborFeature *feature, double merror, double fprate)
 */
void CvGabAdaFeatureSelect::writefeature(const char *filename, CvGaborFeature *feature, double merror, double fprate)
{
    FILE * file;
    file = fopen (filename,"a");

    int x = feature->getx();
    int y = feature->gety();
    int Mu = feature->getMu();
    int Nu = feature->getNu();

    fprintf (file, "%d %d %d %d %f %f\n",x, y, Nu, Mu, merror, fprate);
    

    fclose(file);
}


/*!
    \fn CvGabAdaFeatureSelect::writeweights(const char *filename, CvMat *mat)
 */
void CvGabAdaFeatureSelect::writeweights(const char *filename, CvMat *mat)
{
    cvSave( filename, (CvMat*)mat, NULL, NULL); 
}


/*!
    \fn CvGabAdaFeatureSelect::loadweights(const char *filename)
 */
void CvGabAdaFeatureSelect::loadweights(const char *filename)
{
    CvMat *mat = (CvMat*)cvLoad(filename, NULL, NULL, NULL);
    cvCopy(mat, weights);
    cvReleaseMat(&mat);
}


/*!
    \fn CvGabAdaFeatureSelect::resume(int c)
 */
void CvGabAdaFeatureSelect::resume(int c)
{   
  // Load weights 
  current = c;
  char *weightfilename = new char[20];
  sprintf(weightfilename, "weight_%d.xml", current);
  weights = (CvMat*)cvLoad(weightfilename, NULL, NULL, NULL);
  delete [] weightfilename;
  
  // Load features
  char *allfeaturefilename = new char[20];
  sprintf(allfeaturefilename, "feature_%d.txt", current - 1);
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  pool->load( allfeaturefilename );
  origpool = pool;
  
  // load the significant features
  loadsignfeatures("significant.txt");
  
  
  sprintf(allfeaturefilename, "allfeature_%d.txt", current);
  char *disfeaturefilename = new char[20];
  sprintf(disfeaturefilename, "discard_%d.txt", current);
  char *featurefilename = new char[20];
  sprintf(featurefilename, "feature_%d.txt", current);
  
  // rename the current iteration file
  char *allfeaturefilename_bkp = new char[20];
  char *disfeaturefilename_bkp = new char[20];
  char *featurefilename_bkp = new char[20];
  sprintf(allfeaturefilename_bkp, "allfeature_%d.txt~", current);
  sprintf(disfeaturefilename_bkp, "discard_%d.txt~", current);
  sprintf(featurefilename_bkp, "feature_%d.txt~", current);
  
  rename( allfeaturefilename, allfeaturefilename_bkp );
  rename( disfeaturefilename, disfeaturefilename_bkp );
  rename( featurefilename, featurefilename_bkp );
  
  delete [] allfeaturefilename_bkp;
  delete [] disfeaturefilename_bkp;
  delete [] featurefilename_bkp;

  // run loop
  while(current < nexpfeatures)
  {
    normalise(weights);
    trainallfeature(origpool);
    double error = findminerror(type);
    updateweights();
    current++;
  }
}


/*!
    \fn CvGabAdaFeatureSelect::errorthreshold()
 */
double CvGabAdaFeatureSelect::errorthreshold()
{
    int smin = (sub-1)*4;
    int smax = sub*4; 
    double psum = 0.0, nsum = 0.0, v;
    for (int i = 0; i < nsample; i++)
    {
      v = cvGetReal1D(weights, i);
      if ((i >= smin)&&(i < smax)) psum = psum + v; 
      else nsum = nsum + v;
    }
    return nsum/2;
}


/*!
    \fn CvGabAdaFeatureSelect::save(const char *filename)
 */
void CvGabAdaFeatureSelect::save(const char *filename)
{
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvGaborTree), storage );
    int num = weaks.size();
    for(int i = 0; i < num; i++ )
    {
      CvGaborFeature *feature;
      feature = newpool->getfeature(i);
            
      CvGaborTree tree;

      tree.x = feature->getx();
      tree.y = feature->gety();
      tree.Mu = feature->getMu();
      tree.Nu = feature->getNu();
      tree.alpha = alphas[i];      
      tree.threshold = weaks[i].getthreshold();
      tree.parity = weaks[i].getparity();
      
      cvSeqPush( seq, &tree );
    }
    
    char *weakname = new char[20];
    CvMemStorage* xstorage = cvCreateMemStorage( 0 );
    CvFileStorage *fs = cvOpenFileStorage( filename, xstorage, CV_STORAGE_WRITE );
    for (int i = 0; i <seq->total; i++)
    {
      CvGaborTree *atree = (CvGaborTree*)cvGetSeqElem(seq, i);
      
      sprintf(weakname, "weak_%d", i);
      cvStartWriteStruct( fs, weakname, CV_NODE_MAP, "CvGaborTree", cvAttrList(0,0) );
      cvWriteInt(fs, "x", atree->x);
      cvWriteInt(fs, "y", atree->y);
      cvWriteInt(fs, "Mu", atree->Mu);
      cvWriteInt(fs, "Nu", atree->Nu);
      cvWriteReal(fs, "alpha", atree->alpha);
      cvWriteReal(fs, "threshold", atree->threshold);
      cvWriteReal(fs, "parity", atree->parity);
      cvEndWriteStruct( fs ); 
    } 

    /* release memory storage in the end */
    delete [] weakname;
    cvReleaseMemStorage( &xstorage );
    cvReleaseMemStorage( &storage );
    cvReleaseFileStorage(&fs);
}


/*!
    \fn CvGabAdaFeatureSelect::loadweaks(const char* filename)
 */
void CvGabAdaFeatureSelect::loadweaks(const char* filename)
{
    clear();
    CvMemStorage* fstorage = cvCreateMemStorage( 0 );
    CvFileStorage *fs;
    fs = cvOpenFileStorage( filename, fstorage, CV_STORAGE_READ );
    CvFileNode *root = cvGetRootFileNode( fs, 0);
    char *weakname = new char[20];
    int i = 0;

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvGaborTree), storage );
    while(1)
    {
	sprintf( weakname, "weak_%d", i);
        CvFileNode *weaknode = cvGetFileNodeByName( fs, root, weakname);
        if (!weaknode) break;
        CvGaborTree tree;
        weaknode2tree(weaknode, fs, &tree);
        cvSeqPush( seq, &tree );
        i++;	
    }
    
    /* from squence to vector weaks*/
    if (newpool == NULL) newpool = new CvGaborFeaturePool;
    for (int i = 0; i <seq->total; i++)
    {
      CvGaborTree *atree = (CvGaborTree*)cvGetSeqElem(seq, i);
      CvWeakLearner *weak = new CvWeakLearner;
      weak->setType( CvWeakLearner::THRESHOLD );
      weak->setthreshold(atree->threshold);
      weak->setparity(atree->parity);
      weaks.push_back(*weak);
      CvGaborFeature *feature = new CvGaborFeature(atree->x, atree->y, atree->Mu, atree->Nu);
      newpool->add(feature);
      alphas.push_back(atree->alpha);
      delete weak;
      delete feature;
    } 
 
    cvReleaseMemStorage( &storage );
    cvReleaseFileStorage(&fs);
    cvReleaseMemStorage( &fstorage );
    delete [] weakname;

    /* set member variables */
    current = newpool->getSize();
    falsepositive = 0.0;
    nexpfeatures = newpool->getSize();
    nselecfeatures = newpool->getSize();
    printf(" %d weak classifiers have been loaded!\n", nselecfeatures);
}


/*!
    \fn CvGabAdaFeatureSelect::weaknode2tree(CvFileNode *node, CvFileStorage *fs, CvGaborTree *tree)
 */
void CvGabAdaFeatureSelect::weaknode2tree(CvFileNode *node, CvFileStorage *fs, CvGaborTree *tree)
{
    CvFileNode *xnode = cvGetFileNodeByName( fs, node, "x");
    CvFileNode *ynode = cvGetFileNodeByName( fs, node, "y");
    CvFileNode *Munode = cvGetFileNodeByName( fs, node, "Mu");
    CvFileNode *Nunode = cvGetFileNodeByName( fs, node, "Nu");
    CvFileNode *anode = cvGetFileNodeByName( fs, node, "alpha");
    CvFileNode *tnode = cvGetFileNodeByName( fs, node, "threshold");
    CvFileNode *pnode = cvGetFileNodeByName( fs, node, "parity");
    int x = cvReadInt( xnode, 0);
    int y = cvReadInt( ynode, 0);
    int Mu = cvReadInt( Munode, 0);
    int Nu = cvReadInt( Nunode, 0);
    double alpha = cvReadReal( anode, 0);
    double threshold = cvReadReal( tnode, 0);
    double parity = cvReadReal( pnode, 0);
    tree->x = x;
    tree->y = y;
    tree->Mu = Mu;
    tree->Nu = Nu;
    tree->alpha = alpha;
    tree->threshold = threshold;
    tree->parity = parity;
}




/*!
    \fn CvGabAdaFeatureSelect::predict(IplImage *img, int nweaks)
 */
double CvGabAdaFeatureSelect::predict(IplImage *img, int nweaks)
{
  if( nweaks > weaks.size() )
  {
    perror("Number of weak learners exceeds the maximal!\n");
    exit(-1);
  }
  double vfeature, label, criterion = 0.0, flabel=0.0, alpha;
  for (int i = 0; i < nweaks; i++)
  {
      /* load feature value */
    
    CvGaborFeature *feature;
    feature = newpool->getfeature(i);
    vfeature = feature->val( img );
    
      /* get label from weak learner*/
    label = weaks[i].predict( vfeature );
      //printf("The label that Weak Learner No.%d predicted is %d.\n", i, (int)label);
      //printf("\n");
    label = -1*(label - 2);
    alpha = alphas[i];
    flabel = flabel + label*alpha;
    criterion = criterion + alpha;            //features.begin()
  }
  criterion = criterion / 2;
  if ( flabel >= criterion ) return 1.0;
  else return 2.0;
}



/*!
    \fn CvGabAdaFeatureSelect::predict(IplImage *img)
 */
double CvGabAdaFeatureSelect::predict(IplImage *img)
{
  double v = predict(img, weaks.size());
  return v;
  /* double vfeature, label, criterion = 0.0, flabel=0.0, alpha;
    for (int i = 0; i < weaks.size(); i++)
    {
      /* load feature value */
      
  /*    CvGaborFeature *feature;
      feature = newpool->getfeature(i);
      vfeature = feature->val( img );
      
      /* get label from weak learner*/
  /*  label = weaks[i].predict( vfeature );
      //printf("The label that Weak Learner No.%d predicted is %d.\n", i, (int)label);
      //printf("\n");
      label = -1*(label - 2);
      alpha = alphas[i];
      flabel = flabel + label*alpha;
      criterion = criterion + alpha;            //features.begin()
    }
    criterion = criterion / 2;
    if ( flabel >= criterion ) return 1.0;
    else return 2.0;
*/
}



/*!
    \fn CvGabAdaFeatureSelect::predict(const char *filename, int nweaks)
 */
double CvGabAdaFeatureSelect::predict(const char *filename, int nweaks)
{
  IplImage *img = cvLoadImage( filename, CV_LOAD_IMAGE_GRAYSCALE);
  double l = predict(img, nweaks);
  return l;
}





/*!
    \fn CvGabAdaFeatureSelect::predict(const char *filename)
 */
double CvGabAdaFeatureSelect::predict(const char *filename)
{
    IplImage *img = cvLoadImage( filename, CV_LOAD_IMAGE_GRAYSCALE);
    double l = predict(img);
    return l;
}


/*!
    \fn CvGabAdaFeatureSelect::create(CvGabAdaFeatureParams *params)
 */
void CvGabAdaFeatureSelect::create(CvGabAdaFeatureParams *params)
{
    if (params->flag == XM2VTS)
    {
      if (params->numindex != 4)
      {
        index = cvCreateMat(1, (8-params->numindex), CV_32SC1);
        int n = 0;
        for (int i = 1; i <= 8; i++)
        {
          //if(!isInArray(i, params->testIndex, params->numindex))
          bool in = false;
          for(int j = 0; j < params->numindex; j++)
          {
            if(i == params->testIndex[j]) in = true;
          }
          if(!in) 
          {
            cvSetReal1D(index, n, i);
            n++;
          }
        }
      }
      else 
      {
        nopositivesampels = 4;
        index = cvCreateMat(1, (8-params->numindex), CV_32SC1);
        for (int i = 1; i <= 4; i++)
        {
          cvSetReal1D(index, i-1, i);
        }
      }
    }
    cvSave("index.xml", (CvMat*)index, "index", NULL, cvAttrList());
    FILE *file = fopen("params.txt", "w");
    fprintf(file, "%d %s %d %d", params->flag, params->sourcepath, params->nosubject, params->nofeatures);
    fclose(file);
    create( params->featurepool, params->flag, params->sourcepath, params->nosubject );
    train( params->nofeatures );
 
}


/*!
    \fn CvGabAdaFeatureSelect::isInArray(int i, int *array, int n)
 */
bool CvGabAdaFeatureSelect::isInArray(int i, int *array, int n)
{
    for(int j = 0; j < n; j++)
    {
      if (array[j] == i) return true;
    }
    return false;
}


/*!
    \fn CvGabAdaFeatureSelect::clear()
 */
void CvGabAdaFeatureSelect::clear()
{
  if(newpool != NULL) delete newpool;
  //delete origpool;
  if( weights != NULL ) cvReleaseMat(&weights);
  if( index != NULL ) cvReleaseMat(&index);
  alphas.clear();
  
  for (int i = 0; i < weaks.size(); i++)
  {
    weaks[i].clear();
  }
}


/*!
    \fn CvGabAdaFeatureSelect::testing(const char* filelist, const char* resultfile)
 */
void CvGabAdaFeatureSelect::testing(const char* filelist, const char* resultfile)
{
  /*char *imgname = new char[50];
  FILE *fs = fopen(filelist, "r");
  assert(fs);
  FILE *file = fopen(resultfile,"w");
  assert(file);
  double l;
  while(feof(fs) == 0)
  {
    if (fscanf(fs, "%s", imgname) == EOF) break;
    l = predict( imgname );
    fprintf( file, "%s   %f\n", imgname, l );
    printf( "%s is Class %d\n", imgname, (int)l );
  }
  delete [] imgname;
  fclose(file);
  fclose(fs);
  */
  testing( filelist, resultfile, weaks.size() );
}



/*!
\fn CvGabAdaFeatureSelect::testing(const char* filelist, const char* resultfile, int nofeatures)
 */
void CvGabAdaFeatureSelect::testing(const char* filelist, const char* resultfile, int nofeatures)
{
  char *imgname = new char[50];
  FILE *fs = fopen(filelist, "r");
  assert(fs);
  FILE *file = fopen(resultfile,"w");
  assert(file);
  double l;
  while(feof(fs) == 0)
  {
    if (fscanf(fs, "%s", imgname) == EOF) break;
    l = predict( imgname, nofeatures );
    fprintf( file, "%s   %f\n", imgname, l );
    printf( "%s is Class %d\n", imgname, (int)l );
  }
  delete [] imgname;
  fclose(file);
  fclose(fs);
}



/*!
    \fn CvGabAdaFeatureSelect::resume()
 */
void CvGabAdaFeatureSelect::resume()
{
  int n = 0;
  char *allfeaturefilename = new char[20];
  struct stat dummy;
  while(1)
  {
    sprintf(allfeaturefilename, "allfeature_%d.txt", n);
    if (stat(allfeaturefilename, &dummy) != 0) break;
    n++;
  }
  delete [] allfeaturefilename;
  int c = n - 1;
  resume(c);
}


/*!
    \fn CvGabAdaFeatureSelect::loadsignfeatures(const char *filename)
 */
void CvGabAdaFeatureSelect::loadsignfeatures(const char *filename)
{
    // Load params
  loadparams( "params.txt" );
  
  // Load sample index
  loadindex( "index.xml" );
  
  if( newpool == NULL ) newpool = new CvGaborFeaturePool;
  FILE * file = fopen(filename, "r");
  assert(file);
  int x, y, Mu, Nu;
  double error, alpha;
  
  char *weightname = new char[20];
  int c = 0;
  while(feof(file) == 0)
  {
    //if (fscanf( file, "%d %d %d %d %f %f\n", x, y, Nu, Mu, error, alpha ) == EOF) break;
    //load the parameter of each gabor feature
    if (fscanf(file, " %d", &x) == EOF) break;
    if (fscanf(file, " %d", &y) == EOF) break;
    if (fscanf(file, " %d", &Nu) == EOF) break;
    if (fscanf(file, " %d", &Mu) == EOF) break;
    if (fscanf(file, " %f", &error) == EOF) break;
    if (fscanf(file, " %f\n", &alpha) == EOF) break;
    
    CvGaborFeature *feature = new CvGaborFeature(x, y, Mu, Nu);
    feature->seterror( error );
    newpool->add( feature );
    
    CvTrainingData *data;
    if (nopositivesampels == 4)
    {
      data = feature->_XM2VTSBin_F(spath, sub);
    }
    else 
    {
      data = feature->_XM2VTSBin_F(spath, sub, index);
    }
    sprintf(weightname, "weight_%d.xml", c);
    CvMat *weight = (CvMat*)cvLoad( weightname, NULL, NULL, NULL );
    data->setweights(weight);
    
    CvWeakLearner *weak = new CvWeakLearner;
    weak->train(data, CvWeakLearner::THRESHOLD);
    alpha = weak->importance();
    
    printf("Push a weaklearner .......\n");
    weaks.push_back(*weak);
    alphas.push_back(alpha);
    c++;
    delete data;
  }
  
  delete [] weightname;
  fclose( file );
}




/*!
    \fn CvGabAdaFeatureSelect::sign2weaks(const char* signfile, const char* weaksname)
 */
void CvGabAdaFeatureSelect::sign2weaks(const char* signfile, const char* weaksname)
{
  loadsignfeatures( signfile );
  save( weaksname );
}


/*!
    \fn CvGabAdaFeatureSelect::loadparams(const char *filename)
 */
void CvGabAdaFeatureSelect::loadparams(const char *filename)
{
  int f, nosub, nof;
  char *pa = new char[100];
  // Load params.

  FILE *file = fopen(filename, "r");
  assert( file );
  fscanf(file, "%d", &f);
  fscanf(file, "%s", pa);
  fscanf(file, "%d", &nosub);
  fscanf(file, "%d", &nof);
  fclose(file);
  type = f;
  spath = pa;
  sub = nosub;
  nexpfeatures = nof;
}


/*!
    \fn CvGabAdaFeatureSelect::loadindex(const char *filename)
 */
void CvGabAdaFeatureSelect::loadindex(const char *filename)
{
  index = (CvMat*)cvLoad(filename, NULL, NULL, NULL);
  CvSize size = cvGetSize( index );
  nopositivesampels = size.width;
}


