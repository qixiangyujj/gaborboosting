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
#include "cvgaborfeaturepairpool.h"

namespace AdaGabor {

CvGaborFeaturePairPool::CvGaborFeaturePairPool()
{
}


CvGaborFeaturePairPool::~CvGaborFeaturePairPool()
{
  this->clear();
}


}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvGaborFeaturePool* pool)
 */
 AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvGaborFeaturePool* pool)
{
  if(pool == NULL)
  {
    printf("The Gabor feature pool is empty! \n");
    exit(-1);
  }
  this->build(pool);
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvSize imgsize, CvMat *Scale, CvMat *Orientation, int interval, int bound)
 */
 AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvSize imgsize, CvMat *Scale, CvMat *Orientation, int interval, int bound )
{
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  pool->Init( imgsize, Scale, Orientation, interval, bound);
  //printf("successfully!\n");
  features = pool;
  this->build(pool);
  //delete pool;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::getPair(long index)
 */
AdaGabor::CvGaborFeaturePair* AdaGabor::CvGaborFeaturePairPool::getPair(long index)
{
  if (index < pairs.size())
  {  
    //CvGaborFeaturePair *pair = (pairs[index]);
    CvGaborFeaturePair *pair = pairs.at(index);
    return pair;
  }
  else
  {
    perror("CvGaborFeaturePairPool::getPair ");
    printf("The index of the Gabor Feature Pair Pool is out of range!\n");
    exit(-1);
  }
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::size()
 */
long AdaGabor::CvGaborFeaturePairPool::size()
{
  return (long)pairs.size();
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::remove(long index)
 */
void AdaGabor::CvGaborFeaturePairPool::remove(long index)
{
  if(index < pairs.size())
  {
    CvGaborFeaturePair *pair = pairs.at(index);
    delete pair;
    pos = pairs.begin()+index;
    pairs.erase(pos);
  }
  else
  {
    perror("CvGaborFeaturePairPool::remove ");
    printf("The index of the Gabor Feature Pair Pool is out of range!\n");
    exit(-1);
  }
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::clone()
 */
AdaGabor::CvGaborFeaturePairPool* AdaGabor::CvGaborFeaturePairPool::clone()
{
  CvGaborFeaturePairPool *newpool = new CvGaborFeaturePairPool;
  newpool->features = features->clone();
  long count = pairs.size();
  for (long i = 0; i < count; i++)
  {
    CvGaborFeaturePair *pair = this->getPair(i);
    CvGaborFeaturePair *newpair = pair->clone(); 
    newpool->push(newpair);
  }
  return newpool;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::push(CvGaborFeaturePair *pair)
 */
void AdaGabor::CvGaborFeaturePairPool::push(CvGaborFeaturePair *pair)
{
  pairs.push_back(pair);
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::write(const char* filename)
 */
void AdaGabor::CvGaborFeaturePairPool::write(const char* filename)
{
  string str(filename);
  size_t found;
  found = str.find_last_of(".");
  string ext = str.substr(found+1);
  
  printf("Writing the Gabor Feature Pairs into %s\n", filename);
  
  if ((ext.compare("xml")==0)||(ext.compare("XML")==0)) 
  {
    if(size() < 2000000) writeXML( filename );
    else  
    {
      //size_t s = str.size();
      //str.replace(, "txt");
      writeTXT( filename );
    }
  }
  else if (ext.compare("txt")==0) writeTXT( filename );
  else writeTXT( filename );
  
  
  
}



/*!
    \fn AdaGabor::CvGaborFeaturePairPool::build(CvGaborFeaturePool* pool)
 */
void AdaGabor::CvGaborFeaturePairPool::build(CvGaborFeaturePool* pool)
{
  int numfeatures = pool->getSize();
  printf("num of features is %d.\n", numfeatures);
  long n = 0;
  long total = numfeatures*(numfeatures-1)/2;
  printf("Number of pairs is %ld\n",total);
  for (int i = 0; i < numfeatures; i++)
  {
    CvGaborFeature* feature1 = pool->getfeature(i);  //not create a new feature
    for (int j = (i+1); j < numfeatures; j++)
    {
      CvGaborFeature* feature2 = pool->getfeature(j); //not create a new feature
      CvGaborFeaturePair *pair = new CvGaborFeaturePair(feature1, feature2);
      pair->setMutInf(9999.0);
      //if(!feature1->neighbor(feature2, 5)) 
       pairs.push_back(pair);
      //pair->describe();
      int m = n*100/total;
      std::cout << "Building the pair list: " << m << "%\r" << std::flush;
      n++;
      
    }
  }
  cout << endl;
  printf("\nNumber of pairs is %d\n.", pairs.size());
  //printf("The training will take %f hours.\n",6.0*pairs.size()/60/60);
}


/*!
\fn AdaGabor::CvGaborFeaturePairPool::build(CvGaborFeaturePool* pool, CvGaborFeature* start_feature, CvGaborFeature* end_feature)
 */
void AdaGabor::CvGaborFeaturePairPool::build(CvGaborFeaturePool* pool, CvGaborFeature* start_feature, CvGaborFeature* end_feature)
{
  int start = pool->getIndex( start_feature );
  int end = pool->getIndex( end_feature );
  if(start > end)
  {
    int tmp = start;
    start = end;
    end = tmp;
  }
  
  int numfeatures = pool->getSize();
  printf("num of features is %d.\n", numfeatures);
  long n = 0;
  for (int i = start; i <= end; i++)
  {
    CvGaborFeature* feature1 = pool->getfeature(i);  //not create a new feature
    for (int j = (i+1); j < numfeatures; j++)
    {
      CvGaborFeature* feature2 = pool->getfeature(j); //not create a new feature
      CvGaborFeaturePair *pair = new CvGaborFeaturePair(feature1, feature2);
      pair->setMutInf(9999.0);
     
      pairs.push_back(pair);
      std::cout << "Building the pair list: " << n << "\r" << std::flush;
      n++;
    }
  }
  cout << endl;
  printf("\nNumber of pairs is %d\n.", pairs.size());
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::clear()
 */
void AdaGabor::CvGaborFeaturePairPool::clear()
{
  for (long i = 0; i < pairs.size(); i++)
  {
    CvGaborFeaturePair *pair = pairs.at(i);
    delete pair;
  }
  pairs.clear();
  delete features;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvSize imgsize, int minscale, int maxscale, int norient, int interval=0, int bound=0)
 */
 AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvSize imgsize, int minscale, int maxscale, int norient, int interval=0, int bound=0)
{
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  pool->Init( imgsize, minscale, maxscale, norient, interval, bound);
  features = pool;
  this->build(pool);
  //delete pool;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvPoolParams *param)
 */
 AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvPoolParams *param)
{
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  pool->Init(param);
  this->features = pool;
  //pool->write( "/local/features.txt" );
  this->build( this->features ); 
  //delete pool;
}


/*!
\fn AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvPoolParams *param, CvGaborFeature* start_feature, CvGaborFeature *end_feature)
 */
AdaGabor::CvGaborFeaturePairPool::CvGaborFeaturePairPool(CvPoolParams *param, CvGaborFeature* start_feature, CvGaborFeature *end_feature)
{
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  pool->Init(param);
  this->features = pool;
  //pool->write( "/local/features.txt" );
  this->build( this->features, start_feature, end_feature ); 
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::sort()
 */
void AdaGabor::CvGaborFeaturePairPool::sort()
{
  printf("Start sorting ...........\n");
  std::sort(pairs.begin(), pairs.end(), SortPredicate);
  printf("\n\n");
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::writeXML(const char *filename)
 */
void AdaGabor::CvGaborFeaturePairPool::writeXML(const char *filename)
{
  CvMemStorage* storage = cvCreateMemStorage( 0 );
  CvFileStorage* fs = cvOpenFileStorage( filename, storage, CV_STORAGE_WRITE);
  for(long i = 0; i < pairs.size(); i++)
  {
    CvGaborFeaturePair *pair = getPair(i);
    cvStartWriteStruct( fs, "CvGaborFeaturePair",
                        CV_NODE_MAP, NULL,
                        cvAttrList(0,0));
    for(int j = 1; j <= 2; j++)
    {
      CvGaborFeature *feature = pair->getFeature(j);
      cvStartWriteStruct( fs, "CvGaborFeature",
                          CV_NODE_MAP, NULL,
                          cvAttrList(0,0));
      cvWriteInt(fs, "x", feature->getx());
      cvWriteInt(fs, "y", feature->gety());
      cvWriteInt(fs, "Mu", feature->getMu());
      cvWriteInt(fs, "Nu", feature->getNu());
      cvEndWriteStruct(fs); 
    }
    cvWriteReal(fs, "MutInf", pair->getMutInf());
    cvEndWriteStruct( fs );
  }
  
  cvReleaseFileStorage(&fs);
  cvReleaseMemStorage(&storage);
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::writeTXT(const char* filename)
 */
void AdaGabor::CvGaborFeaturePairPool::writeTXT(const char* filename)
{
  FILE * file;
  file = fopen (filename,"a");
  for(long i = 0; i < pairs.size(); i++)
  {
    //printf("%d\n",i);
    CvGaborFeaturePair *pair = getPair(i);
    CvGaborFeature *feature1 = pair->getFeature(1);
    CvGaborFeature *feature2 = pair->getFeature(2);
    int x = feature1->getx();
    int y = feature1->gety();
    int Mu = feature1->getMu();
    int Nu = feature1->getNu();
    fprintf (file, "(%d, %d, %d, %d) ",x, y, Mu, Nu);
    x = feature2->getx();
    y = feature2->gety();
    Mu = feature2->getMu();
    Nu = feature2->getNu();
    fprintf (file, "(%d, %d, %d, %d) ",x, y, Mu, Nu);
    double mut = pair->getMutInf();
    fprintf (file, "%f\n", mut);
  }

  fclose(file);
}


/*!
\fn AdaGabor::CvGaborFeaturePairPool::load(const char *featurepoolname, const char *pairpoolname)
 */
void AdaGabor::CvGaborFeaturePairPool::load(const char *featurepoolname, const char *pairpoolname)
{
  features->load(featurepoolname);
  
}




/*!
    \fn AdaGabor::CvGaborFeaturePairPool::loadMutFile(const char* filename)
 */
void AdaGabor::CvGaborFeaturePairPool::loadMutFile(const char* filename)
{
  FILE * file;
  if ((file=fopen(filename,"r")) == NULL)
  {
    perror( filename );
    exit(1);
  }
  
  float mutinf;
  int count = 0;
  printf("Load from the file %s   ........", filename);
  while (!feof(file))
  {
    int flag = fscanf(file, " %f\n", &mutinf);
    if ((flag == EOF)||(flag == 0)) break;
    //printf("%d\n",count);
    CvGaborFeaturePair *pair = getPair(count);
    pair->setMutInf( mutinf );
    count++;
  }
  printf("\n%d load from the file.\n", count);
  if(count != size())
  {
    perror("the number of entry in the file are not same with the PairPool.");
    exit(1);
  }
  fclose(file);
  printf("\n\n");
}





bool AdaGabor::SortPredicate( CvGaborFeaturePair *pair1, CvGaborFeaturePair *pair2 )
{
  double mutinf1 = pair1->getMutInf();
  double mutinf2 = pair2->getMutInf();
  if(mutinf1 < mutinf2) return true;
  else false;
}




/*!
    \fn AdaGabor::CvGaborFeaturePairPool::MinMax(double *min, double *max)
 */
void AdaGabor::CvGaborFeaturePairPool::MinMax(double *min, double *max)
{
  int num = size();
  double *array = new double[num];
  
  for(int i = 0; i < num ; i++)
  {
    CvGaborFeaturePair *pair = getPair(i);
    double mutinf = pair->getMutInf();
    array[i] = mutinf;
  }
  
  
  std::sort(array, array+num);
  *min = array[0];
  *max = array[num-1];
  delete [] array;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::numMutInf(double mutinf, const char* mode)
 */
int AdaGabor::CvGaborFeaturePairPool::numMutInf(double mutinf, const char* mode)
{
  int n = 0;
  if(!strcmp(mode, "LESS"))
  {
    for(int i = 0; i < pairs.size(); i++)
    {
      CvGaborFeaturePair *pair = getPair( i );
      double MutInf = pair->getMutInf();
      if( MutInf <= mutinf )
      {
        pair->describe();
        n++;
      }
    }
  }
  return n;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::select(double mutinf, const char* mode)
 */
AdaGabor::CvGaborFeaturePairPool* AdaGabor::CvGaborFeaturePairPool::select(double mutinf, const char* mode)
{
  CvGaborFeaturePairPool* pool = this->clone();
  pool->clearPairs();
  
  int n = 0;
  if(!strcmp(mode, "LESS"))
  {
    for(int i = 0; i < pairs.size(); i++)
    {
      // printf("%d", i);
      CvGaborFeaturePair *pair = this->getPair( i );
      double MutInf = pair->getMutInf();
      if( MutInf <= mutinf )
      {
        pair->describe();
        n++;
        pool->push( pair );
      }
      else 
      {
        //printf("  : nothing ");
        
      }
      //printf("\n");
    }
  }
  printf("%d pair selected\n", n);
  
  return pool;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::clearPairs()
 */
void AdaGabor::CvGaborFeaturePairPool::clearPairs()
{
  pairs.clear();
}


/*!
    \fn AdaGabor::CvGaborFeaturePairPool::select(double mutinf1, double mutinf2, const char* mode)
 */
AdaGabor::CvGaborFeaturePairPool* AdaGabor::CvGaborFeaturePairPool::select(double mutinf1, double mutinf2, const char* mode)
{
  CvGaborFeaturePairPool* pool = this->clone();
  pool->clearPairs();
  
  int n = 0;
  if(!strcmp(mode, "BETWEEN"))
  {
    for(int i = 0; i < pairs.size(); i++)
    {
      // printf("%d", i);
      CvGaborFeaturePair *pair = this->getPair( i );
      double MutInf = pair->getMutInf();
      if(( MutInf <= mutinf1 ) && (MutInf > mutinf2))
      {
        pair->describe();
        n++;
        pool->push( pair );
      }
      else 
      {
        //printf("  : nothing ");
        
      }
      //printf("\n");
    }
  }
  printf("%d pair selected\n", n);
  
  return pool;
}
