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
    nsamples = nClass*nperClass;
    classIdx = cvCreateMat(nsamples, 1, CV_32SC1);
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
    
    /* prepare log files */
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
    /* prepare log files */
    
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
  if( possub > 0 )  // face recogntion
  {
    for(int i = 0; i < nsamples; i++)
    {
      int id = (int)cvGetReal1D(classIdx, i);
      if(id == possub) cvSetReal1D(weights, i, posweight);
      else cvSetReal1D(weights, i, negweight);
    }
  }
  else   //gender detection
  {
    for(int i = 0; i < nsamples; i++)
    {
      int id = (int)cvGetReal1D(classIdx, i);
      bool sex = ((CvXm2vts*)database)->getGender( id );
      if(sex) cvSetReal1D(weights, i, posweight);
      else cvSetReal1D(weights, i, negweight);
    }
  }
}


/*!
    \fn CvBinGabAdaFeatureSelect::setSub(int nosub)
 */
void CvBinGabAdaFeatureSelect::setSub(int nosub)
{
  if (nosub > 0)
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
  else
  {
    this->possub = nosub;
    if(db_type == XM2VTS)
    {
      int m = 0;
      int f = 0;
      for (int i = 0; i < nClass; i++)
      {
        bool gen = ((CvXm2vts*)database)->getGender( i+1);
        if( gen ) m++;
        else f++;
      }
      npositive = m*nperClass;
      nnegative = f*nperClass;
    }
  }
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
  time_t start, end;
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
  time ( &start );

  for (int i = 0; i <old_pool->getSize(); i++)
  //for (int i = 0; i < 10; i++)
  {
    
    CvGaborFeature *feature;
    feature = old_pool->getfeature(i);
     
    double error = featureweak(feature);
    
    writefeature( allfeaturefilename, feature, error, falsepositive );
    if ((error > errthreshold)||(falsepositive > 0.5))
//     if (error > 0.5)
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
  printf("%d features remain, and %d features are filtered out.\n", maxfeatures-m, m);
  if(dif >= 3600) printf("\n\n\n This iteration %d takes %.2lf hours.\n\n\n\n", current, dif/3600);
  else if(dif >= 60) printf("\n\n\n This iteration %d takes %.2lf minutes.\n\n\n\n", current, dif/60);
  else if(dif < 60) printf("\n\n\n This iteration %d takes %.2lf seconds.\n\n\n\n", current, dif);
  delete [] allfeaturefilename;
  delete [] disfeaturefilename;
  delete [] featurefilename;
  delete [] weightfilename;
}


/*!
    \fn CvBinGabAdaFeatureSelect::findminerror()
 */
double CvBinGabAdaFeatureSelect::findminerror()
{
  double merror;
  bool isin;
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
      if(new_pool->isIn( sfeature ))
      {
        isin = true;
        sfeature->clear();
        delete sfeature;
      }
      else
      {
        isin = false;
      }
    }
    while( isin );
    
    merror = sfeature->geterror();
    signfeature(sfeature);
    //current = new_pool->getSize() - 1;
    return merror;
    sfeature->clear();
    delete sfeature;
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
      //data = feature->_XM2VTSBin_F( ((CvXm2vts*)database)->getPath(), possub, classIdx);
      if (possub > 0) data = feature->_XM2VTSBin_F( ((CvXm2vts*)database)->getPath(), possub, classIdx);
      else data = feature->_XM2VTSGender_F( (CvXm2vts*)database, 4);
    }
    else if(db_type == FERET)
    {
      data = feature->_FERETBin_F( (CvFeret*)database, possub, classIdx);
    }
    
    data->setweights(weights);
    
    CvWeakLearner *weak = new CvWeakLearner;
    weak->train( data, weaklearner_type );
    weak->update( data );
    //weak->train(data, CvWeakLearner::ann);
    cvReleaseMat( &weights );
    weights = data->getweights();
    error = weak->training_error();
    alpha = weak->importance();
    
    printf("Push a weaklearner .......\n");
    weaks.push_back(*weak);
    alphas.push_back(alpha);
    
    char *signfilename = new char[20];
    strcpy(signfilename, "significant.txt");
    writefeature(signfilename, feature, error, alpha);
    
    if((weaklearner_type == CvWeakLearner::POTSU)
       ||(weaklearner_type == CvWeakLearner::FLD))
      delete weak;
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
    if (possub > 0) data = feature->_XM2VTSBin_F( ((CvXm2vts*)database)->getPath(), possub, classIdx);
    else data = feature->_XM2VTSGender_F( (CvXm2vts*)database, 4);
  }
  else if( db_type == FERET )
  {
    data = feature->_FERETBin_F( (CvFeret*)database, possub, classIdx);
  }
  data->setweights(weights);
  CvWeakLearner *weak = new CvWeakLearner;
  weak->train(data, weaklearner_type);
  //weak->describe();
  // weak->train(data, CvWeakLearner::ANN);
  e = weak->training_error();
  
  //printf( "the error of this weak learner is %f\n", e );
  
  feature->seterror(e);
  falsepositive = weak->myerror();
  //weak->clear();
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
  //CvSize class_size = cvGetSize( classIdx );
  //CvSize weights_size = cvGetSize( weights );
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

  cvReleaseMat(&classIdx);
  cvReleaseMat(&weights);

  delete database;
  delete old_pool;
  delete new_pool;
  weaksname.clear();
  
  weaks.clear();
//   for(int i = 0; i < weaks.size(); i++)
//   {
//     CvWeakLearner * weak = &weaks[i];
//     delete weak;
//   }
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
    weak->setType( weaklearner_type );
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


/*!
    \fn CvBinGabAdaFeatureSelect::gendertesting( const char* resultfile )
 */
void CvBinGabAdaFeatureSelect::gendertesting( const char* resultfile )
{
  gendertesting( resultfile, weaks.size() );
}


/*!
    \fn CvBinGabAdaFeatureSelect::gendertesting(const char* resultfile, int nweaks)
 */
void CvBinGabAdaFeatureSelect::gendertesting(const char* resultfile, int nweaks)
{
  if ( nweaks > weaks.size() )
  {
    exit( -1 );
  }
  else
  {
    //FILE *file = fopen(resultfile,"w");
    string filename_in( resultfile );
    string filename_out( resultfile );
    filename_in = filename_in + "_in";
    filename_out = filename_out + "_out";
    
    FILE *file_in = fopen(filename_in.c_str(),"w");
    FILE *file_out = fopen(filename_out.c_str(),"w");
    
  if ( db_type == XM2VTS )
  {
    const char *xm2vtspath = "/local/FaceDB/XM2VTS/";
    char *filename = new char[50];
    
   //training validation
    double l, t;
    int fal = 0;
    for(int sub = 1; sub <= 200; sub++)
    {
      if (((CvXm2vts*)database)->getGender( sub )) t = 1.0;
      else t = 2.0;
      for(int pic = 1; pic <= 8; pic++)
      {
        sprintf(filename, "%s/%d_%d.bmp", xm2vtspath, sub, pic);
        l = predict_a( filename, nweaks );
        if (l == t)
        {
          fprintf( file_in, "%s   %f\n", filename, 1.0 );
          printf( "%s is Class %d, and classified correctly.\n", filename, (int)l );
        }
        else
        {
          fprintf( file_in, "%s   %f\n", filename, 0.0 );
          printf( "%s is Class %d, and misclassified.\n", filename, (int)l );
          fal++;
        }
      }
    }
    printf("\n    %d   faults out of 1600          \n", fal);
    
    fal = 0;
    for(int sub = 201; sub <= 295; sub++)
    {
      if (((CvXm2vts*)database)->getGender( sub )) t = 1.0;
      else t = 2.0;
      for(int pic = 1; pic <= 8; pic++)
      {
        sprintf(filename, "%s/%d_%d.bmp", xm2vtspath, sub, pic);
        l = predict_a( filename, nweaks );
        if (l == t)
        {
          fprintf( file_out, "%s   %f\n", filename, 1.0 );
          printf( "%s is Class %d, and classified correctly.\n", filename, (int)l );
        }
        else
        {
          fprintf( file_out, "%s   %f\n", filename, 0.0 );
          printf( "%s is Class %d, and misclassified.\n", filename, (int)l );
          fal++;
        }
      }
    }
    printf("\n    %d   faults out of 760          \n", fal);
    
    delete [] filename;
  }
  else if ( db_type == FERET )
  {
    
  }
     
  fclose( file_in );
    fclose( file_out );
  }
}


/*!
    \fn CvBinGabAdaFeatureSelect::predict_a(const char *filename, int nweaks)
 */
double CvBinGabAdaFeatureSelect::predict_a(const char *filename, int nweaks)
{
  IplImage *img = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR );
  IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
  if ( img->nChannels == 1 )
    cvCopy( img, grayimg, NULL );
  else if (img->nChannels == 3) 
    cvCvtColor( img, grayimg, CV_RGB2GRAY );
  double l = predict_a( grayimg, nweaks );
  cvReleaseImage(&img);
  return l;
}


/*!
    \fn CvBinGabAdaFeatureSelect::predict_a(IplImage *img, int nweaks)
 */
double CvBinGabAdaFeatureSelect::predict_a(IplImage *img, int nweaks)
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
    


    vfeature = feature->val( img );
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
    \fn CvBinGabAdaFeatureSelect::resume()
 */
void CvBinGabAdaFeatureSelect::resume()
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
  printf("start from iteration %d\n", c);
  resume(c);
}


/*!
    \fn CvBinGabAdaFeatureSelect::resume(int c)
 */
void CvBinGabAdaFeatureSelect::resume(int c)
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
  pool->load_a( allfeaturefilename );
  old_pool = pool;
  
  // load the significant features
  loadweaks(weaksname.c_str() );
  
  
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
  while(current < 1000)
  {
    printf( " Iteration %d ............\n", current );
    normalise( );
    trainallfeatures();
    double error = findminerror();
    update();
    saveweaks( weaksname.c_str() );
    current++;
  }

}




/*!
    \fn CvBinGabAdaFeatureSelect::svmtesting(const char* resultfile, int nofeatures)
 */
void CvBinGabAdaFeatureSelect::svmtesting(const char* resultfile, int nofeatures)
{
  if( nofeatures > new_pool->getSize())
  {
    printf("Number of features exceeds the maximal!\n");
    nofeatures = new_pool->getSize();
    printf(" The number of features is changed to %d\n", nofeatures);
  }
  string filename_in( resultfile );
  string filename_out( resultfile );
  filename_in = filename_in + "_in";
  filename_out = filename_out + "_out";
  
  FILE *file_in = fopen(filename_in.c_str(),"w");
  FILE *file_out = fopen(filename_out.c_str(),"w");
  CvSVM *svm = new CvSVM;
  const char *xm2vtspath = "/local/FaceDB/XM2VTS/";
  
  if ( db_type == XM2VTS )
  {
    
    svmlearning( xm2vtspath, nofeatures, svm);
    
    // testing 

    svm_test( file_in, xm2vtspath, nofeatures, svm );
    svm_test_a( file_out, xm2vtspath, nofeatures, svm );
  }
  delete svm;
  fclose( file_in );
  fclose( file_out );
}


/*!
    \fn CvBinGabAdaFeatureSelect::svmpredict(IplImage *img, CvSVM *svm, int nfeatures)
 */
float CvBinGabAdaFeatureSelect::svmpredict(IplImage *img, CvSVM *svm, int nfeatures)
{
  if( nfeatures > new_pool->getSize())
  {
    perror("Number of features exceeds the maximal!\n");
    exit(-1);
  }
  
  CvMat *test_sample = cvCreateMat(1, nfeatures, CV_32FC1);
  double vfeature;
  for (int i = 0; i < nfeatures; i++)
  {
      /* load feature value */
    CvGaborFeature *feature;
    feature = new_pool->getfeature( i );
    vfeature = feature->val( img );
    cvSetReal1D( test_sample, i, vfeature );
  }
  
  float result = svm->predict( test_sample );
  //printf("The class number is %f\n", result);
  
  return result;
}


/*!
    \fn CvBinGabAdaFeatureSelect::svmpredict(const char* filename, CvSVM *svm, int nfeatures)
 */
float CvBinGabAdaFeatureSelect::svmpredict(const char* filename, CvSVM *svm, int nfeatures)
{
  IplImage *img = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR );
  IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
  if ( img->nChannels == 1 )
    cvCopy( img, grayimg, NULL );
  else if (img->nChannels == 3) 
    cvCvtColor( img, grayimg, CV_RGB2GRAY );
  double l = svmpredict( grayimg, svm, nfeatures );
  cvReleaseImage(&img);
  cvReleaseImage(&grayimg);
  return l;
}


/*!
    \fn CvBinGabAdaFeatureSelect::svmtesting( const char* resultfile )
 */
void CvBinGabAdaFeatureSelect::svmtesting( const char* resultfile )
{
  svmtesting( resultfile, weaks.size() );
}


/*!
    \fn CvBinGabAdaFeatureSelect::svmlearning(const char* path, int nofeatures, CvSVM * svm)
 */
void CvBinGabAdaFeatureSelect::svmlearning(const char* path, int nofeatures, CvSVM * svm)
{
  if( db_type == XM2VTS )
  {
    printf("Training an SVM classifier  ................\n");
    CvXm2vts *xm2vts = (CvXm2vts*)database;
    int nTrainingExample = 200*4;
    CvMat* trainData = cvCreateMat(nTrainingExample, nofeatures, CV_32FC1);
    CvMat* response = cvCreateMat(nTrainingExample, 1, CV_32FC1);
    
    for (int i = 0; i < nofeatures; i++)
    {
      /* load feature value */
      CvGaborFeature *feature;
      feature = new_pool->getfeature(i);
      printf("Getting the %d feature ............\n", i+1);
      
      char *filename = new char[50];
      //training validation
      double l, t;
      int fal = 0;
      for(int sub = 1; sub <= 200; sub++)
      {
        if (((CvXm2vts*)database)->getGender( sub )) t = 1.0;
        else t = 2.0;
        
        for(int pic = 1; pic <= 4; pic++)
        {
          sprintf(filename, "%s/%d_%d.bmp", path, sub, pic);
          IplImage *img = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR );
          IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
          if ( img->nChannels == 1 )  cvCopy( img, grayimg, NULL );
          else if (img->nChannels == 3)   cvCvtColor( img, grayimg, CV_RGB2GRAY );
          
          double vfeature = feature->val( img );
          cvSetReal2D( trainData, ((sub-1)*4+(pic-1)), i, vfeature );
          cvSetReal1D( response, ((sub-1)*4+(pic-1)), t );
          cvReleaseImage(&img);
          cvReleaseImage(&grayimg);
        }
      }
      delete [] filename;
    }
    
    
    printf("building the svm classifier .........................\n");
    CvTermCriteria term_crit = cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 200, 0.8);
  /*Type of SVM, one of the following types:
    CvSVM::C_SVC - n-class classification (n>=2), allows imperfect separation of classes with penalty multiplier C for outliers.
    CvSVM::NU_SVC - n-class classification with possible imperfect separation. Parameter nu (in the range 0..1, the larger the value, the smoother the decision boundary) is used instead of C.
    CvSVM::ONE_CLASS - one-class SVM. All the training data are from the same class, SVM builds a boundary that separates the class from the rest of the feature space.
    CvSVM::EPS_SVR - regression. The distance between feature vectors from the training set and the fitting hyperplane must be less than p. For outliers the penalty multiplier C is used.
    CvSVM::NU_SVR - regression; nu is used instead of p. */
    int _svm_type = CvSVM::NU_SVC;
  /*The kernel type, one of the following types:
    CvSVM::LINEAR - no mapping is done, linear discrimination (or regression) is done in the original feature space. It is the fastest option. d(x,y) = x•y == (x,y)
    CvSVM::POLY - polynomial kernel: d(x,y) = (gamma*(x•y)+coef0)degree
    CvSVM::RBF - radial-basis-function kernel; a good choice in most cases: d(x,y) = exp(-gamma*|x-y|2)
    CvSVM::SIGMOID - sigmoid function is used as a kernel: d(x,y) = tanh(gamma*(x•y)+coef0) */
    
    int _kernel_type = CvSVM::POLY;
    
    double _degree = 3.0;
    double _gamma = 1.0;
    double _coef0 = 0.0;
    double _C = 1.0;
    double _nu = 1.0;
    double _p = 1.0;
    
    CvSVMParams  params( CvSVM::C_SVC, CvSVM::POLY, _degree, _gamma, _coef0, _C, _nu, _p,
                         0, term_crit );
    
    svm->train( trainData, response, 0, 0, params );
    
    svm->save( "svm.xml", "svm" );
    cvReleaseMat(&response);
    cvReleaseMat(&trainData);
  }
}


/*!
    \fn CvBinGabAdaFeatureSelect::svm_test( FILE *file, const char *path, int nofeatures, CvSVM * svm)
 */
void CvBinGabAdaFeatureSelect::svm_test( FILE *file, const char *path, int nofeatures, CvSVM * svm)
{
  if( db_type == XM2VTS )
  {
    char *filename = new char[50];
  
    //training validation
    double l, t;
    int fal = 0;
    for(int sub = 1; sub <= 200; sub++)
    {
      if (((CvXm2vts*)database)->getGender( sub )) t = 1.0;
      else t = 2.0;
      for(int pic = 1; pic <= 8; pic++)
      {
        sprintf(filename, "%s/%d_%d.bmp", path, sub, pic);
        l = svmpredict( filename, svm, nofeatures );
        if (l == t)
        {
          fprintf( file, "%s   %f\n", filename, 1.0 );
          printf( "%s is Class %d, and classified correctly.\n", filename, (int)l );
        }
        else
        {
          fprintf( file, "%s   %f\n", filename, 0.0 );
          printf( "%s is Class %d, and misclassified.\n", filename, (int)l );
          fal++;
        }
      }
    }
    printf("\n    %d   faults out of 2360          \n", fal);
    delete [] filename;
  }

}


/*!
    \fn CvBinGabAdaFeatureSelect::svm_test_a( FILE *file, const char *path, int nofeatures, CvSVM * svm)
 */
void CvBinGabAdaFeatureSelect::svm_test_a( FILE *file, const char *path, int nofeatures, CvSVM * svm)
{
  if( db_type == XM2VTS )
  {
    char *filename = new char[50];
    
    //training validation
    double l, t;
    int fal = 0;
    for(int sub = 201; sub <= 295; sub++)
    {
      if (((CvXm2vts*)database)->getGender( sub )) t = 1.0;
      else t = 2.0;
      for(int pic = 1; pic <= 8; pic++)
      {
        sprintf(filename, "%s/%d_%d.bmp", path, sub, pic);
        l = svmpredict( filename, svm, nofeatures );
        if (l == t)
        {
          fprintf( file, "%s   %f\n", filename, 1.0 );
          printf( "%s is Class %d, and classified correctly.\n", filename, (int)l );
        }
        else
        {
          fprintf( file, "%s   %f\n", filename, 0.0 );
          printf( "%s is Class %d, and misclassified.\n", filename, (int)l );
          fal++;
        }
      }
    }
    printf("\n    %d   faults out of 2360          \n", fal);
    delete [] filename;
  }
}

/*!
    \fn CvBinGabAdaFeatureSelect::setTypeWeak(int type)
 */
void CvBinGabAdaFeatureSelect::setTypeWeak(int type)
{
  weaklearner_type = type;
  if(type == CvWeakLearner::ANN)
  printf("The weak learner is set as ANN.\n");
  else if(type == CvWeakLearner::FLD)
    printf("The weak learner is set as FLD.\n");
  else if(type == CvWeakLearner::POTSU)
    printf("The weak learner is set as POTSU.\n");
  else if(type == CvWeakLearner::SVM)
    printf("The weak learner is set as SVM.\n");
}
