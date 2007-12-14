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
#include "cvbingabadafeatureselect.h"

CvBinGabAdaFeatureSelect::CvBinGabAdaFeatureSelect()
{
}


CvBinGabAdaFeatureSelect::~CvBinGabAdaFeatureSelect()
{
  clear();
}



/*!
    \fn CvBinGabAdaFeatureSelect::setDB(CvFaceDB* db)
 */
void CvBinGabAdaFeatureSelect::setDB(CvFaceDB* db)
{
  database = db->clone();
  char * dbname = database->getName();
  if( !strcmp(dbname, "XM2VTS")) this->db_type = XM2VTS;
  else if ( !strcmp(dbname, "FERET")) this->db_type = FERET;
  printf("...............a %s database copied!\n\n", dbname);
  
  if(db_type == XM2VTS)
  {
    nClass = ((CvXm2vts*)database)->get_num_sub();
    nperClass = ((CvXm2vts*)database)->get_num_pic();
    classIdx = cvCreateMat(nsamples, 1, CV_32SC1);
    nsamples = nClass*nperClass;
    int n = 0;
    for(int i = 0; i < nClass; i++)
    {
      for(int j = 0; j < nperClass; j++)
      {
        cvSetReal1D(classIdx, n, i+1);
        n++;
      }
    }
  }
  else if(db_type == FERET)
  {
    nClass = ((CvFeret*)database)->getSub();
    nsamples = ((CvFeret*)database)->getNum();
    classIdx = cvCreateMat(nsamples, 1, CV_32SC1);
    int n = 0;
    for(int i = 0; i < nClass; i++)
    {
      CvSubject subject = ((CvFeret*)database)->getSubject(i);
      int num = subject.getnum();
      int id = subject.getId();
      for(int j = 0; j < num; j++)
      {
        cvSetReal1D(classIdx, n, id);
        n++;
      }
    }
  }
}


/*!
    \fn CvBinGabAdaFeatureSelect::setPool(CvPoolParams *param)
 */
void CvBinGabAdaFeatureSelect::setPool(CvPoolParams *param)
{
  old_pool = new CvGaborFeaturePool;
  old_pool->Init( param);
  printf(".................a %d long pool built!\n", old_pool->getSize());
  new_pool = new CvGaborFeaturePool;
}


/*!
    \fn CvBinGabAdaFeatureSelect::train(int nfeatures)
 */
void CvBinGabAdaFeatureSelect::train(int nfeatures)
{
  double error = 0.0; 
  nselecfeatures = 0;
  nexpfeatures = nfeatures;
  char *signfilename = new char[30];
  strcpy(signfilename, "significant.txt");
  FILE * file = fopen(signfilename, "w");
  fclose( file );
  delete [] signfilename;
  
  for (int i = 0; i < nexpfeatures; i++)
  {
    current = i;
    if (old_pool->getSize() < 1) break;
    
    char *allfeaturefilename = new char[30];
    sprintf(allfeaturefilename, "allfeature_%d.txt", current);
    FILE * file1 = fopen(allfeaturefilename, "w");
    fclose( file1 );
    delete [] allfeaturefilename;
    
    char *disfeaturefilename = new char[30];
    sprintf(disfeaturefilename, "discard_%d.txt", current);
    FILE * file2 = fopen(disfeaturefilename, "w");
    fclose( file2 );
    delete [] disfeaturefilename;
    
    char *featurefilename = new char[30];
    sprintf(featurefilename, "feature_%d.txt", current); 
    FILE * file3 = fopen(featurefilename, "w");
    fclose( file3 );
    delete [] featurefilename;
    
    normalise();
    trainallfeatures();
    error = findminerror();
    update(); 
    saveweaks( weaksname.c_str() );
    nselecfeatures++;
  }
}


/*!
    \fn CvBinGabAdaFeatureSelect::init_weights()
 */
void CvBinGabAdaFeatureSelect::init_weights()
{
  weights = cvCreateMat(nsamples, 1, CV_32FC1);
  double posweight, negweight;
  posweight = 1.0/npositive/2.0;
  negweight = 1.0/nnegative/2.0;
  for(int i = 0; i < nsamples; i++)
  {
    int id = (int)cvGetReal1D(classIdx, i);
    if(id == possub) cvSetReal1D(weights, i, posweight);
    else cvSetReal1D(weights, i, negweight);
  }
}


/*!
    \fn CvBinGabAdaFeatureSelect::setSub(int nosub)
 */
void CvBinGabAdaFeatureSelect::setSub(int nosub)
{
  this->possub = nosub;

  npositive = 0;
  for(int i = 0; i < nsamples; i++)
  {
    int id = (int)cvGetReal1D(classIdx, i);
    if(id == possub) npositive++;
  }

  nnegative = nsamples - npositive;
}


/*!
    \fn CvBinGabAdaFeatureSelect::CvBinGabAdaFeatureSelect(CvFaceDB *db, CvPoolParams *param, int possub)
 */
 CvBinGabAdaFeatureSelect::CvBinGabAdaFeatureSelect(CvFaceDB *db, CvPoolParams *param, int possub)
{
  setDB(db);
  setPool( param );
  setSub( possub );
  setWeaksname( "weaks.xml" );
  init_weights();
}


/*!
    \fn CvBinGabAdaFeatureSelect::normalise()
 */
void CvBinGabAdaFeatureSelect::normalise()
{
  CvScalar scalar = cvSum( weights );
  double s = scalar.val[0];
  cvConvertScale(weights, weights, 1.0/s, 0);
}


/*!
    \fn CvBinGabAdaFeatureSelect::trainallfeatures()
 */
void CvBinGabAdaFeatureSelect::trainallfeatures()
{
  // time
  time_t start,end;
  double dif;
  
  int maxfeatures = old_pool->getSize();
  
  char *allfeaturefilename = new char[30];
  sprintf(allfeaturefilename, "allfeature_%d.txt", current);
  char *disfeaturefilename = new char[30];
  sprintf(disfeaturefilename, "discard_%d.txt", current);
  char *featurefilename = new char[30];
  sprintf(featurefilename, "feature_%d.txt", current);
  char *weightfilename = new char[30];
  sprintf(weightfilename, "weight_%d.xml", current);
  
  saveweights( weightfilename );
  double errthreshold = errorthreshold();
  int n = 0, m = 0;
  for (int i = 0; i <old_pool->getSize(); i++)
  {
    
    CvGaborFeature *feature;
    feature = old_pool->getfeature(i);
     
    double error = featureweak(feature);
    
    writefeature( allfeaturefilename, feature, error, falsepositive );
    //if ((error > errthreshold)||(falsepositive > 0.5))
    if (falsepositive > 0.5)
    {
      writefeature( disfeaturefilename, feature, error, falsepositive );
      //printf("Rejecting this feature .............\n");
      old_pool->remove(i);
      i--;
      m++;
    }
    else
    {
      writefeature( featurefilename, feature, error, falsepositive );
    }
   
    std::cout << "Trainging in the iteration: " << n<< "\r" << std::flush;
    n++;
  }
  std::cout << endl;
  time (&end);
  dif = difftime (end,start);
  printf("%d features remain, and %d features are filtered out.\n", m, maxfeatures-m);
  if(dif >= 3600) printf("\n\n\n This iteration %d takes %.2lf hours.\n\n\n\n", current, dif/3600);
  else if(dif >= 60) printf("\n\n\n This iteration %d takes %.2lf minutes.\n\n\n\n", current, dif/60);
  else if(dif < 60) printf("\n\n\n This iteration %d takes %.2lf seconds.\n\n\n\n", current, dif);
  delete [] allfeaturefilename;
  delete [] disfeaturefilename;
  delete [] featurefilename;
}


/*!
    \fn CvBinGabAdaFeatureSelect::findminerror()
 */
double CvBinGabAdaFeatureSelect::findminerror()
{
  double merror;
  if (old_pool->getSize() < 1)
  {
    return -1.0;
  }
  else
  { 
    CvGaborFeature *sfeature;
    do
    {
      sfeature = old_pool->min();
    }
    while(new_pool->isIn( sfeature ));
    
    merror = sfeature->geterror();
    signfeature(sfeature);
    //current = new_pool->getSize() - 1;
    return merror;
  }
}


/*!
    \fn CvBinGabAdaFeatureSelect::update()
 */
void CvBinGabAdaFeatureSelect::update()
{
  if (old_pool->getSize() >= 1)
  {
    double error, alpha;
    CvGaborFeature *feature;
    feature = new_pool->getfeature(current);
    
    CvTrainingData *data;
     
    if(db_type == XM2VTS)
    {
      data = feature->_XM2VTSBin_F( ((CvXm2vts*)database)->getPath(), possub, classIdx);
    }
    else if(db_type == FERET)
    {
      data = feature->_FERETBin_F( (CvFeret*)database, possub, classIdx);
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
    
    
    delete [] signfilename;
    delete data;
  }
}


/*!
    \fn CvBinGabAdaFeatureSelect::featureweak(CvGaborFeature* feature)
 */
double CvBinGabAdaFeatureSelect::featureweak(CvGaborFeature* feature)
{
  double e;
  CvTrainingData *data;
 
  if( db_type == XM2VTS )
  {
    data = feature->_XM2VTSBin_F( ((CvXm2vts*)database)->getPath(), possub, classIdx);
  }
  else if( db_type == FERET )
  {
    data = feature->_FERETBin_F( (CvFeret*)database, possub, classIdx);
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
    \fn CvBinGabAdaFeatureSelect::errorthreshold()
 */
double CvBinGabAdaFeatureSelect::errorthreshold()
{

  double nsum = 0.0, psum = 0.0;
  for(int i = 0; i < nsamples; i++)
  {
    int id = (int)cvGetReal1D( classIdx, i );
    double w = cvGetReal1D( weights, i );
    if( id == possub ) psum = psum + w;
    else nsum = nsum + w;
  }    
  
  return nsum/2.0;

}


/*!
    \fn CvBinGabAdaFeatureSelect::signfeature(CvGaborFeature *feature)
 */
void CvBinGabAdaFeatureSelect::signfeature(CvGaborFeature *feature)
{
  double merror = feature->geterror();
  new_pool->add(feature);
  nselecfeatures++;
}


/*!
    \fn CvBinGabAdaFeatureSelect::writefeature(const char *filename, CvGaborFeature *feature, double param1, double param2)
 */
void CvBinGabAdaFeatureSelect::writefeature(const char *filename, CvGaborFeature *feature, double param1, double param2)
{
  FILE * file;
  file = fopen (filename,"a");
  
  int x = feature->getx();
  int y = feature->gety();
  int Mu = feature->getMu();
  int Nu = feature->getNu();
  
  fprintf (file, "%d %d %d %d %f %f\n",x, y, Mu, Nu, param1, param2);

  fclose(file);
}


/*!
    \fn CvBinGabAdaFeatureSelect::saveweaks(const char *filename)
 */
void CvBinGabAdaFeatureSelect::saveweaks(const char *filename)
{
  CvMemStorage* storage = cvCreateMemStorage(0);
  CvSeq* seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvGaborTree), storage );
  int num = weaks.size();
  for(int i = 0; i < num; i++ )
  {
    CvGaborFeature *feature;
    feature = new_pool->getfeature(i);
    
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
    \fn CvBinGabAdaFeatureSelect::saveweights(const char* filename)
 */
void CvBinGabAdaFeatureSelect::saveweights(const char* filename)
{
  cvSave( filename, weights, NULL, NULL, cvAttrList());
}


/*!
    \fn CvBinGabAdaFeatureSelect::setWeaksname(const char* filename)
 */
void CvBinGabAdaFeatureSelect::setWeaksname(const char* filename)
{
  weaksname = filename;
}


/*!
    \fn CvBinGabAdaFeatureSelect::CvBinGabAdaFeatureSelect(CvFaceDB *db, CvPoolParams *param, int possub, const char* filename))
 */
 CvBinGabAdaFeatureSelect::CvBinGabAdaFeatureSelect(CvFaceDB *db, CvPoolParams *param, int possub, const char* filename)
{
  setDB(db);
  setPool( param );
  setSub( possub );
  setWeaksname( filename );
  init_weights();
}


/*!
    \fn CvBinGabAdaFeatureSelect::clear()
 */
void CvBinGabAdaFeatureSelect::clear()
{
  alphas.clear();
  weaks.clear();
  cvReleaseMat(&classIdx);
  cvReleaseMat(&weights);
  delete database;
  delete old_pool;
  delete new_pool;
  weaksname.clear();
}


/*!
    \fn CvBinGabAdaFeatureSelect::loadweaks(const char* filename)
 */
void CvBinGabAdaFeatureSelect::loadweaks(const char* filename)
{
  //clear();
  {
    delete new_pool;
    weaks.clear();
  }
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
  new_pool = new CvGaborFeaturePool;
  for (int i = 0; i <seq->total; i++)
  {
    CvGaborTree *atree = (CvGaborTree*)cvGetSeqElem(seq, i);
    CvWeakLearner *weak = new CvWeakLearner;
    weak->setType( CvWeakLearner::THRESHOLD );
    weak->setthreshold(atree->threshold);
    weak->setparity(atree->parity);
    weaks.push_back(*weak);
    CvGaborFeature *feature = new CvGaborFeature(atree->x, atree->y, atree->Mu, atree->Nu);
    new_pool->add(feature);
    alphas.push_back(atree->alpha);
    delete weak;
    delete feature;
  } 
  
  cvReleaseMemStorage( &storage );
  cvReleaseFileStorage(&fs);
  cvReleaseMemStorage( &fstorage );
  delete [] weakname;
  
    /* set member variables */
  current = new_pool->getSize();
  falsepositive = 0.0;
  nexpfeatures = new_pool->getSize();
  nselecfeatures = new_pool->getSize();
  printf(" %d weak classifiers have been loaded!\n", nselecfeatures);
}


/*!
    \fn CvBinGabAdaFeatureSelect::weaknode2tree(CvFileNode *node, CvFileStorage *fs, CvGaborTree *tree)
 */
void CvBinGabAdaFeatureSelect::weaknode2tree(CvFileNode *node, CvFileStorage *fs, CvGaborTree *tree)
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
    \fn CvBinGabAdaFeatureSelect::testing(const char* filelist, const char* resultfile, int nofeatures)
 */
void CvBinGabAdaFeatureSelect::testing(const char* filelist, const char* resultfile, int nofeatures)
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
    \fn CvBinGabAdaFeatureSelect::predict(const char *filename, int nweaks)
 */
double CvBinGabAdaFeatureSelect::predict(const char *filename, int nweaks)
{
  IplImage *img = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR );
  IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
  cvCvtColor( img, grayimg, CV_RGB2GRAY );
  double l = predict( grayimg, nweaks );
  cvReleaseImage(&img);
  return l;
}


/*!
    \fn CvBinGabAdaFeatureSelect::predict(IplImage *img, int nweaks)
 */
double CvBinGabAdaFeatureSelect::predict(IplImage *img, int nweaks)
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
    feature = new_pool->getfeature(i);
    
//     IplImage *aimg;
//     if(img->depth == IPL_DEPTH_8U)
//     {
//       aimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
//       for(int i = 0; i < img->width; i++)
//       {
//         for(int j = 0; j < img->height; j++)
//         {
//           
//           double pix = cvGetReal2D(img, j, i);
//           
//           cvSetReal2D(aimg, j, i, pix);
//           //printf("%d %d\n", i, j);
//         }
//       }
//     }
//     else
//     {
//       aimg = cvCloneImage(img);
//     }
    //
    int s = feature->getNu();
    vfeature = feature->val( img, s );
    //printf("%f\n ", vfeature);
    
    /* get label from weak learner*/
    label = weaks[i].predict( vfeature );
    //printf("The label that Weak Learner No.%d predicted is %d.\n", i, (int)label);
    //printf("\n");
    label = -1*(label - 2);
    alpha = alphas[i];
    flabel = flabel + label*alpha;
    criterion = criterion + alpha;            //features.begin()
    //cvReleaseImage(&aimg);
  }
  criterion = criterion / 2;
  
  if ( flabel >= criterion ) return 1.0;
  else return 2.0;
}


/*!
    \fn CvBinGabAdaFeatureSelect::testing(const char* filelist, const char* resultfile)
 */
void CvBinGabAdaFeatureSelect::testing(const char* filelist, const char* resultfile)
{
  int num = weaks.size();
  testing( filelist, resultfile, num );
}
