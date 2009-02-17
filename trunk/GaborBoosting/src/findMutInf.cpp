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
#include "cvpoolparams.h"
#include "cvgaborfeaturepool.h"
#include "cvgaborfeaturepairselect.h"


using namespace AdaGabor;
void findMinMax(int job, const char* jobpath, double *min, double *max);
int numbelow( int job, const char* jobpath, double value );
int numbelowfrmpool( int job, const char* jobpath, double value );
int numbetween( int job, const char* jobpath, double value1, double value2 );
int numbetweenfrmpool( int job, const char* jobpath, double value1, double value2 );
int returnindex(float mutinf, vector<float> vec);





int main(int argc, char *argv[])
{
  
 
      //fprintf (file, "%d %f\n", i, v);
  const char* jobpath = "/windows/D/mutinf/";
  /*  for(int job = 103; job <= 120; job++)
  {
    FILE * file;
    file = fopen("mutmaxmin.txt", "a");
    assert( file );
  
  
    double min, max;
    findMinMax(job, jobpath, &min, &max);
  
    fprintf(file, "%d %f %f\n",job, min, max);
    fclose( file );
  }
 */
  const char* filename = "/local/mutlocalmaxmin.txt";
  FILE * file;
  if ((file=fopen(filename,"r")) == NULL)
  {
    printf( "Cannot read file %s.\n", filename );
    exit(1);
  }
  int job;
  float min, max;
  vector<float> vec;
  while (!feof(file))
  {
    if (fscanf(file, " %d", &job) == EOF) break;
    if (fscanf(file, " %f", &min) == EOF) break;
    if (fscanf(file, " %f\n", &max) == EOF) break;
    vec.push_back(min);
    vec.push_back(max);
  }
  fclose( file );
  std::sort(vec.begin(), vec.end());
  int s = vec.size()+1;
  
  // double value = vec[0];
  

  /*  CvMat* record = cvCreateMat(s, 121, CV_32FC1);
  cvSetZero( record );
  for(int job = 0; job <= 120; job++)
  {
    char *jobfile = new char[50];
    sprintf(jobfile, "%s/job%d.txt", jobpath, job);
    printf("Processing %s ...........\n", jobfile);
    FILE * file;
    if ((file=fopen(jobfile,"r")) == NULL)
    {
      printf("Cannot read file %s.\n", jobfile);
      exit(1);
    }
    float mutinf;
    while (!feof(file))
    {
      int flag = fscanf(file, " %f\n", &mutinf);
      if ((flag == EOF)||(flag == 0)) break;
      int index = returnindex(mutinf, vec);
      //printf("%d  %f\n", index, mutinf);
      double value = cvGetReal2D( record, index, job);
      value++;
      cvSetReal2D(record, index, job, value);
    }
    fclose( file );
    delete [] jobfile;
  }
  cvSave( "/local/recordmat.xml", record, NULL, NULL, cvAttrList());
  cvReleaseMat(&record);
  */
  int row = 7;
  CvMat * record = (CvMat*)cvLoad("/local/recordmat.xml", NULL, NULL, NULL);
  double value = vec[row];
  CvMat* submat = cvCreateMat(1, 121, CV_32FC1);
  cvGetRow( record, submat, row );
  for (int job = 0; job <=120; job++)
  {
    float v = cvGetReal1D(submat, job);
    if (v != 0.0)
    {
      if (row == 0) numbetweenfrmpool( job, jobpath, vec[0], 0.0 );
      else numbetweenfrmpool( job, jobpath, vec[row], vec[row-1]);
    }
  }
  cvReleaseMat(&submat);
  cvReleaseMat(&record);
  /*  int n = 0;
   for(int job = 0; job <= 120; job++)
   {
     int num = numbelow( job, jobpath, value ); 
     printf("Job%d has %d \n", job, num);
     n = n + num;
   }
   printf("\nTotal %d\n", n); 
   vec.clear();
  */
  
  /*  numbelowfrmpool( 23, jobpath, value );
  numbelowfrmpool( 24, jobpath, value );
  numbelowfrmpool( 84, jobpath, value );
  numbelowfrmpool( 96, jobpath, value );
  numbelowfrmpool( 97, jobpath, value );
*/  
  
 
  
  
  
  return 0; 
}



void findMinMax(int job, const char* jobpath, double *min, double *max)
{
  char * jobfile = new char[30];
  sprintf(jobfile, "%s/job%d.txt", jobpath, job);
  
  int width = 51, height = 55, minscale = -1, maxscale = 3, norientations = 8, interval = 0, bound = 0;
  bool reduce = true;
  CvPoolParams param( height, width, minscale, maxscale, norientations, interval, bound, reduce );
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  pool->Init( &param );
  //find the parts
  int N = pool->getSize();
  int numParts = 120;
  
  int total = N*(N-1)/2;
  int size_part = total/numParts;
  int *nums = new int[numParts];
  
  int n = 0;
  int sum = 0;
  for(int i = 0; i < N; i++)
  {
    sum = sum + (N-i-1);
    if(sum > size_part)  
    {
      i--;
      nums[n] = i;
      sum = 0;
        //printf("%d\n", nums[n]);
      n++;
    }
  }
  
  int istart, iend;
  if(job == 0)
  {
    istart = 0;
    iend = nums[job];
  }
  else if (job == numParts)
  {
    istart = nums[job-1]+1;
    iend = N-2;
  }
  else
  {
    istart = nums[job-1]+1;
    iend = nums[job];
  }
  
  
  
  CvXm2vts xm2vts( "/mnt/export/rexm2vts/" );  
  xm2vts.setPicIndex( 1 );
  
  CvGaborFeature *fstart = pool->getfeature( istart );
  CvGaborFeature *fend = pool->getfeature( iend );
  
  
  CvGaborFeaturePairPool pairpool(&param, fstart, fend );
  pairpool.loadMutFile( jobfile );
  
  pairpool.MinMax( min, max );
  
  
  delete pool;
  delete [] jobfile;
}

int numbelow( int job, const char* jobpath, double value )
{
  char * jobfile = new char[30];
  sprintf(jobfile, "%s/job%d.txt", jobpath, job);
  
  FILE * file;
  if ((file=fopen(jobfile,"r")) == NULL)
  {
    printf("Cannot read file %s.\n", jobfile);
    exit(1);
  }
  float mutinf;
  vector<float> vec;
  while (!feof(file))
  {
    int flag = fscanf(file, " %f\n", &mutinf);
    if ((flag == EOF)||(flag == 0)) break;
    vec.push_back(mutinf);
  }
  fclose( file );
  int n = 0;
  for(int i = 0; i < vec.size(); i++)
  {
    float v = vec[i];
    if (v <= value) n++;
  }
  
  
  vec.clear();
  delete [] jobfile;
  return n;
}


int numbelowfrmpool( int job, const char* jobpath, double value )
{
  char * jobfile = new char[30];
  sprintf(jobfile, "%s/job%d.txt", jobpath, job);
  
  int width = 51, height = 55, minscale = -1, maxscale = 3, norientations = 8, interval = 0, bound = 0;
  bool reduce = true;
  CvPoolParams param( height, width, minscale, maxscale, norientations, interval, bound, reduce );
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  pool->Init( &param );
  //find the parts
  int N = pool->getSize();
  int numParts = 120;
  
  int total = N*(N-1)/2;
  int size_part = total/numParts;
  int *nums = new int[numParts];
  
  int n = 0;
  int sum = 0;
  for(int i = 0; i < N; i++)
  {
    sum = sum + (N-i-1);
    if(sum > size_part)  
    {
      i--;
      nums[n] = i;
      sum = 0;
        //printf("%d\n", nums[n]);
      n++;
    }
  }
  
  int istart, iend;
  if(job == 0)
  {
    istart = 0;
    iend = nums[job];
  }
  else if (job == numParts)
  {
    istart = nums[job-1]+1;
    iend = N-2;
  }
  else
  {
    istart = nums[job-1]+1;
    iend = nums[job];
  }
  
  
  
  CvXm2vts xm2vts( "/mnt/export/rexm2vts/" );  
  xm2vts.setPicIndex( 1 );
  
  CvGaborFeature *fstart = pool->getfeature( istart );
  CvGaborFeature *fend = pool->getfeature( iend );
  
  
  CvGaborFeaturePairPool pairpool(&param, fstart, fend );
  pairpool.loadMutFile( jobfile );
  
  //int num = pairpool.numMutInf(value, "LESS");
  
  CvGaborFeaturePairPool *newpool = pairpool.select(value, "LESS");
  newpool->write("/local/lowmutinf.txt");
  
  //delete newpool;
  printf("dddd\n");
  delete pool;
  delete [] jobfile;
  
  return 0;
}

int numbetween( int job, const char* jobpath, double value1, double value2 )
{
  char * jobfile = new char[30];
  sprintf(jobfile, "%s/job%d.txt", jobpath, job);
  
  FILE * file;
  if ((file=fopen(jobfile,"r")) == NULL)
  {
    printf("Cannot read file %s.\n", jobfile);
    exit(1);
  }
  float mutinf;
  vector<float> vec;
  while (!feof(file))
  {
    int flag = fscanf(file, " %f\n", &mutinf);
    if ((flag == EOF)||(flag == 0)) break;
    vec.push_back(mutinf);
  }
  fclose( file );
  int n = 0;
  for(int i = 0; i < vec.size(); i++)
  {
    float v = vec[i];
    if ((v <= value1)&&(v > value2)) n++;
  }
  
  
  vec.clear();
  delete [] jobfile;
  return n;
}


int numbetweenfrmpool( int job, const char* jobpath, double value1, double value2 )
{
  char * jobfile = new char[30];
  sprintf(jobfile, "%s/job%d.txt", jobpath, job);
  printf("Processing %s ..................\n", jobfile);
  int width = 51, height = 55, minscale = -1, maxscale = 3, norientations = 8, interval = 0, bound = 0;
  bool reduce = true;
  CvPoolParams param( height, width, minscale, maxscale, norientations, interval, bound, reduce );
  CvGaborFeaturePool *pool = new CvGaborFeaturePool;
  pool->Init( &param );
  //find the parts
  int N = pool->getSize();
  int numParts = 120;
  
  int total = N*(N-1)/2;
  int size_part = total/numParts;
  int *nums = new int[numParts];
  
  int n = 0;
  int sum = 0;
  for(int i = 0; i < N; i++)
  {
    sum = sum + (N-i-1);
    if(sum > size_part)  
    {
      i--;
      nums[n] = i;
      sum = 0;
        //printf("%d\n", nums[n]);
      n++;
    }
  }
  
  int istart, iend;
  if(job == 0)
  {
    istart = 0;
    iend = nums[job];
  }
  else if (job == numParts)
  {
    istart = nums[job-1]+1;
    iend = N-2;
  }
  else
  {
    istart = nums[job-1]+1;
    iend = nums[job];
  }
  
  CvXm2vts xm2vts( "/mnt/export/rexm2vts/" );  
  xm2vts.setPicIndex( 1 );
  
  CvGaborFeature *fstart = pool->getfeature( istart );
  CvGaborFeature *fend = pool->getfeature( iend );
  
  
  CvGaborFeaturePairPool pairpool(&param, fstart, fend );
  pairpool.loadMutFile( jobfile );
  
  //int num = pairpool.numMutInf(value, "LESS");
  
  CvGaborFeaturePairPool *newpool = pairpool.select(value1, value2, "BETWEEN");
  newpool->write("/local/lowmutinf.txt");
  
  //delete newpool;
  printf("dddd\n");
  delete pool;
  delete [] jobfile;
  
  return 0;
}

int returnindex(float mutinf, vector<float> vec)
{
  int index;
  bool done = false;
  int s = vec.size();
  for(int i = 0; i < s; i++)
  {
    if ( i == 0 )
    {
      if ( mutinf <= vec[i])
      {
        index = 0;
        done = true;
      }
    }
    else if( i == (s-1) )
    {
      if ( mutinf > vec[i] )
      {
        index = s;
        done = true;
      }
      else if (mutinf <= vec[i])
      {
        index = i;
        done = true;
      }
    }
    else
    {
      if((mutinf <= vec[i])&&(mutinf > vec[i-1]))
      {
        index = i;
        done = true;
      }
    }
    if(done) break;
  }
  return index;
}


