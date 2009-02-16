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
#include "cvgaborfeaturepool.h"

CvGaborFeaturePool::CvGaborFeaturePool()
{
  count = 0;
  errors.clear();
  features.clear();
  isInitialised = false;
}


CvGaborFeaturePool::~CvGaborFeaturePool()
{
  clear();
}

/*!
\fn CvGaborFeaturePool::Init(CvPoolParams *param)
 */
int CvGaborFeaturePool::Init(CvPoolParams *param)
{
  CvSize imgsize = param->getImgSize();
  CvMat* scales = param->getScales();
  CvMat* orients = param->getOrients();
  int interval = param->getInterval();
  int bound = param->getBound();
  this->reduce = param->isReduced();
  int c = Init(imgsize, scales, orients, interval, bound);
  return c;
}


/*!
    \fn CvGaborFeaturePool::Init(CvSize imgsize, CvMat *Scale, CvMat *Orientation, int interval, int bound)
 */
int CvGaborFeaturePool::Init( CvSize imgsize, CvMat *Scale, CvMat *Orientation, int interval = 0, int bound =0 )
{
    int minx = 1;
    int miny = 1;
    winsize.width = imgsize.width - bound;
    winsize.height = imgsize.height - bound;
    int maxx = winsize.width;
    int maxy = winsize.height;

    CvSize size = cvGetSize(Scale);
    int nScale = size.width;
    size = cvGetSize(Orientation);
    int nOrientation = size.width;

    int scale;
    double dscale;
    int orient;
    int c = 0;
  if (reduce == false)
  {
    for (int i = 0; i < nScale; i++) 
    {
        scale = (int)cvGetReal1D( Scale, i);
         
        for (int j = 0; j < nOrientation; j++) 
        {
            orient = (int)cvGetReal1D(Orientation, j);
            for (int y = 1; y <= maxy; y = y+1+interval)
            {
                for (int x = 1; x <= maxx; x = x+1+interval)
                {
                  //printf("The Position is (%d, %d) on %d and %d. \n",x,y,i,j);
                  CvGaborFeature feature(x, y, orient, scale);
                  //CvGaborFeature *feature = new CvGaborFeature(x, y, orient, scale);
                  features.push_back(feature);
                  index.push_back(c);
                  errors.push_back(1.0);
                  c++;
                  //printf("%d",feature.getx()); 
                  //delete feature;
                }
            }
        }
    }
    count = c;
  }
  else { // reduce number of gabor features
    for (int i = 0; i < nScale; i++) 
    {
      scale = (int)cvGetReal1D( Scale, i);  
  
      double dwidth = (double)(winsize.width)/pow((double)2, (double)(scale+1));
      double dheight = (double)(winsize.height)/pow((double)2,(double)(scale+1));
      int maxx = (int)ceil(dwidth);
      int maxy = (int)ceil(dheight);
      
      for (int j = 0; j < nOrientation; j++) 
      {
        orient = (int)cvGetReal1D(Orientation, j);
        for (int y = 1; y <= maxy; y = y+1+interval)
        {
          for (int x = 1; x <= maxx; x = x+1+interval)
          {
            CvGaborFeature feature(x, y, orient, scale);
            //printf("Scale %d, Orientation %d, (%d,%d)\n", scale, orient, x, y);
            features.push_back(feature);
            index.push_back(c);
            errors.push_back(1.0);
            c++;
          }
        }
      }
    }
    count = c;
  }
  isInitialised = true;
  return c;
}


/*!
    \fn CvGaborFeaturePool::Init(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound)
 */
int CvGaborFeaturePool::Init(CvSize imgsize, int minscale, int maxscale, int norient, int interval=0, int bound=0)
{
    int nscale = maxscale - minscale + 1;
    CvMat *Scale = cvCreateMat(1, nscale, CV_32SC1);
    for (int i = 0; i < nscale; i++)
    {
        cvSetReal1D( (CvMat*)Scale, i, (double)(minscale+i) );
    }
    
    CvMat *Orientation = cvCreateMat(1, norient, CV_32FC1);
    for (int i = 0; i < norient; i++)
    {
        cvSetReal1D( (CvMat*)Orientation, i, i);
    }
    int c = Init( imgsize, Scale, Orientation, interval, bound);
    cvReleaseMat(&Scale);
    cvReleaseMat(&Orientation);
    if (!(c == count))  return -1;
    return c;
}


/*!
    \fn CvGaborFeaturePool::clear()
 */
void CvGaborFeaturePool::clear()
{

      /*for (int i = 0; i < features.size(); i++)
      {
        pos = features.begin()+i;
        features.erase(pos);
      }*/
      features.clear();
      index.clear();
      errors.clear();
      count = 0;

}


/*!
    \fn CvGaborFeaturePool::getSize()
 */
int CvGaborFeaturePool::getSize()
{
    int n = features.size();
    return n;
}


/*!
    \fn CvGaborFeaturePool::add(CvGaborFeature *gabor)
 */
void CvGaborFeaturePool::add(CvGaborFeature *gabor)
{
    features.push_back(*gabor);
    //errors.push_back(gabor->geterror());
    //index.push_back(count);
    count++;
}


/*!
    \fn CvGaborFeaturePool::remove(int idex)
 */
void CvGaborFeaturePool::remove(int idex)
{
    pos = features.begin()+idex;
    features.erase(pos);
    //errors.erase(idex);
    //index.erase(idex);
    count--;
}


/*!
    \fn CvGaborFeaturePool::getfeature(int idex)
 */
CvGaborFeature* CvGaborFeaturePool::getfeature(int idex)
{
    if (idex < count)
    {
      CvGaborFeature *feature;
      feature = &(features[idex]);
      //printf("Get a feature: Scale %d and Orientation %d at (%d,%d)\n", feature->getNu(), feature->getMu(), feature->getx(), feature->gety());
      return feature;
    }
    else 
    {
      printf("The index of the Gabor Feature Pool is out of range!\n");
      exit(-1); 
    }
}


/*!
    \fn CvGaborFeaturePool::sort()
 */
void CvGaborFeaturePool::sort()
{
   //std::sort(features.begin(), features.end(), SortPredicate); 
   //std::sort(features.begin(), features.end());
}


/*!
    \fn CvGaborFeaturePool::SortPredicate(const CvGaborFeature& lhs, const CvGaborFeature& rhs)
 */
bool CvGaborFeaturePool::SortPredicate(const CvGaborFeature& lhs, const CvGaborFeature& rhs)
{
    //return lhs.geterror() < rhs.geterror();
}


/*!
    \fn CvGaborFeaturePool::min()
 */
CvGaborFeature* CvGaborFeaturePool::min()
{
    pos = std::min_element(features.begin(), features.end());
    CvGaborFeature *tmpfeature;
    tmpfeature = &(*pos);
    CvGaborFeature *feature;// = new CvGaborFeature(tmpfeature->getx(),
    //                   tmpfeature->gety(),
    //                                        tmpfeature->getMu(),
    //                                        tmpfeature->getNu());
    //feature->seterror(tmpfeature->geterror());
    feature = tmpfeature->clone();
    printf("Get a feature: Scale %d and Orientation %d at (%d,%d) with the minimum error.\n", feature->getNu(), feature->getMu(), feature->getx(), feature->gety());
    features.erase(pos);
    return feature;
}


/*!
    \fn CvGaborFeaturePool::load(const char *filename)
 */
void CvGaborFeaturePool::load(const char *filename)
{
    clear();

    FILE * file;
    //file = fopen (filename,"r");
    if ((file=fopen(filename,"r")) == NULL)
    {
      printf("Cannot read file %s.\n", filename);
      exit(1);
    }
    int x;
    int y;
    int Mu;
    int Nu;
    float merror;
    float alpha;
    count = 0;
    isInitialised = true;    

    int fpos;

    while (!feof(file))
    {
       if (fscanf(file, " %d", &x) == EOF) break;
       if (fscanf(file, " %d", &y) == EOF) break;
       if (fscanf(file, " %d", &Nu) == EOF) break;
       if (fscanf(file, " %d", &Mu) == EOF) break;
       if (fscanf(file, " %f", &merror) == EOF) break;
       if (fscanf(file, " %f\n", &alpha) == EOF) break;
       
      //if (merror < 0.25) 
       {
         CvGaborFeature feature(x,y,Mu,Nu);
         feature.seterror(merror);
         add(&feature);

       }

    }

    fclose(file);
}


/*!
    \fn CvGaborFeaturePool::clone()
 */
CvGaborFeaturePool* CvGaborFeaturePool::clone()
{
    CvGaborFeaturePool *newpool = new CvGaborFeaturePool;
    int x, y, Mu, Nu;
    double error;
    for (int i = 0; i < count; i++)
    {
      CvGaborFeature *feature = getfeature( i );
      x = feature->getx();
      y = feature->gety();
      Mu = feature->getMu();
      Nu = feature->getNu();
      error = feature->geterror();
      CvGaborFeature newfeature( x, y, Mu, Nu ); 
      newfeature.seterror( error );
      //newfeature->Init(x,y,Mu,Nu);
      //newfeature->seterror(error);
      newpool->add(&newfeature);
    }
    return newpool;
}




/*!
    \fn CvGaborFeaturePool::write(const char* filename)
 */
void CvGaborFeaturePool::write(const char* filename)
{
  int x, y, Mu, Nu;
  double error;
  FILE *file;
  if ((file=fopen(filename,"w")) == NULL)
  {
    perror(filename);
    exit(-1);
  }
  for (int i = 0; i < features.size(); i++)
  {
    CvGaborFeature *feature = getfeature(i);
    x = feature->getx();
    y = feature->gety();
    Mu = feature->getMu();
    Nu = feature->getNu();
    error = feature->geterror();
    fprintf (file, "%d %d %d %d %f %f\n",x, y, Nu, Mu, error, 0.0);
  }
  fclose( file );
}


/*!
    \fn CvGaborFeaturePool::isReduced()
 */
bool CvGaborFeaturePool::isReduced()
{
  return reduce;
}


/*!
    \fn CvGaborFeaturePool::getIndex(CvGaborFeature *feature)
 */
int CvGaborFeaturePool::getIndex(CvGaborFeature *feature)
{
  if(FAST_INDEX)
  {
    int numfeatures = getSize();
    int n = 0;
    for(int i = 0; i < numfeatures; i++)
    {
      CvGaborFeature *pfeature = getfeature( i );
      if(*pfeature == *feature) break;
      n++;
    }
    return n;
  }
  else
  {
    int index, total0, total1, total2, total3;
    int x = feature->getx();
    int y = feature->gety();
    int Mu = feature->getMu();
    int Nu = feature->getNu();
    if(reduce)
    {
      double dwidth, dheight;
      int maxx, maxy, step;
      if(Nu == -1)
      {
        dwidth = (double)(winsize.width)/pow((double)2, (double)(Nu+1));
        dheight = (double)(winsize.height)/pow((double)2,(double)(Nu+1));
        maxx = (int)ceil(dwidth);
        maxy = (int)ceil(dheight); 
        step = maxy*maxx;
        total0 = maxy*maxx*8;
        index = step*Mu + (y-1)*maxx + (x-1);
      }
      else if (Nu == 0)
      {
        dwidth = (double)(winsize.width)/pow((double)2, (double)(Nu+1));
        dheight = (double)(winsize.height)/pow((double)2,(double)(Nu+1));
        maxx = (int)ceil(dwidth);
        maxy = (int)ceil(dheight); 
        step = maxy*maxx;
        total1 = maxy*maxx*8;
        index = step*Mu + (y-1)*maxx + (x-1);
        index = index + total0;
      }
      else if (Nu == 1)
      {
        dwidth = (double)(winsize.width)/pow((double)2, (double)(Nu+1));
        dheight = (double)(winsize.height)/pow((double)2,(double)(Nu+1));
        maxx = (int)ceil(dwidth);
        maxy = (int)ceil(dheight); 
        step = maxy*maxx;
        total2 = maxy*maxx*8;
        index = step*Mu + (y-1)*maxx + (x-1);
        index = index + total0 + total1;
      }
      else if (Nu == 2)
      {
        dwidth = (double)(winsize.width)/pow((double)2, (double)(Nu+1));
        dheight = (double)(winsize.height)/pow((double)2,(double)(Nu+1));
        maxx = (int)ceil(dwidth);
        maxy = (int)ceil(dheight); 
        step = maxy*maxx;
        total3 = maxy*maxx*8;
        index = step*Mu + (y-1)*maxx + (x-1);
        index = index + total0 + total1 + total2;
      }
      else if (Nu == 3)
      {
        dwidth = (double)(winsize.width)/pow((double)2, (double)(Nu+1));
        dheight = (double)(winsize.height)/pow((double)2,(double)(Nu+1));
        maxx = (int)ceil(dwidth);
        maxy = (int)ceil(dheight); 
        step = maxy*maxx;
        total1 = maxy*maxx*8;
        index = step*Mu + (y-1)*maxx + (x-1);
        index = index + total0+ total1 + total2 + total3;
      }
    }
    else
    { 
      if(count == winsize.width*winsize.height*8*5)
      {
        int step = winsize.width*winsize.height;
        index = step*Mu + (y-1)*winsize.height + (x-1) + step*8*(Nu+1);
      }
      else
      {
        int step = (int)ceil(winsize.width/pow(2.0, 1.0))*(int)ceil(winsize.height/pow(2.0, 1.0));
        index = step*Mu + (y-1)*(int)ceil(winsize.height/pow(2.0, 1.0)) + (x-1) + step*8*(Nu+1);
      }
    }
    return index;
  }
}


/*!
    \fn CvGaborFeaturePool::range(int start, int end)
 */
CvGaborFeaturePool* CvGaborFeaturePool::range(int start, int end)
{
  CvGaborFeaturePool* pool = new CvGaborFeaturePool;
  
  for(int i = start; i <= end; i++)
  {
    CvGaborFeature *feature = this->getfeature( i );
    pool->add( feature );
  }
  return pool;
}


/*!
    \fn CvGaborFeaturePool::addfile(const char *filename)
 */
void CvGaborFeaturePool::addfile(const char *filename)
{
  FILE * file;
    
  if ((file=fopen(filename,"r")) == NULL)
  {
    perror(filename);
    exit(-1);
  }
  int x;
  int y;
  int Mu;
  int Nu;
  float merror;
  float alpha;
  count = 0;
  isInitialised = true;    
  
  int fpos;
  
  while (!feof(file))
  {
    if (fscanf(file, " %d", &x) == EOF) break;
    if (fscanf(file, " %d", &y) == EOF) break;
    if (fscanf(file, " %d", &Nu) == EOF) break;
    if (fscanf(file, " %d", &Mu) == EOF) break;
    if (fscanf(file, " %f", &merror) == EOF) break;
    if (fscanf(file, " %f\n", &alpha) == EOF) break;
 
    CvGaborFeature feature(x,y,Mu,Nu);
    feature.seterror(merror);
    add(&feature);
    
    
  }
  count = getSize();
  fclose(file);
}


/*!
    \fn CvGaborFeaturePool::isIn(CvGaborFeature *feature)
 */
bool CvGaborFeaturePool::isIn(CvGaborFeature *feature)
{
  bool in = false;
  for(int i = 0; i < getSize(); i++)
  {
    CvGaborFeature *mfeature = getfeature( i );
    
    if((feature->getx()==mfeature->getx())
       &&(feature->gety()==mfeature->gety())
       &&(feature->getMu()==mfeature->getMu())
       &&(feature->getNu()==mfeature->getNu()))
    {
      in = true;
      break;
    }
  }
  return in;
}


/*!
    \fn CvGaborFeaturePool::load_a(const char* filename)
 */
void CvGaborFeaturePool::load_a(const char* filename)
{
  clear();
  
  FILE * file;
    //file = fopen (filename,"r");
  if ((file=fopen(filename,"r")) == NULL)
  {
    printf("Cannot read file %s.\n", filename);
    exit(1);
  }
  int x;
  int y;
  int Mu;
  int Nu;
  float merror;
  float alpha;
  count = 0;
  isInitialised = true;    
  
  int fpos;
  int n = 0;
  while (!feof(file))
  {
    if (fscanf(file, " %d", &x) == EOF) break;
    if (fscanf(file, " %d", &y) == EOF) break;
    if (fscanf(file, " %d", &Mu) == EOF) break;
    if (fscanf(file, " %d", &Nu) == EOF) break;
    if (fscanf(file, " %f", &merror) == EOF) break;
    if (fscanf(file, " %f\n", &alpha) == EOF) break;
    
      //if (merror < 0.25) 
    {
      CvGaborFeature feature(x,y,Mu,Nu);
      feature.seterror(merror);
      add(&feature);
      n++;
    }
    
  }
  printf(" %d features been loaded\n", n);
  fclose(file);
}
