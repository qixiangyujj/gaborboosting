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
#include <string>
#include "cvtrainingdata.h"
#include "cvadaboost.h"


int main( int argc, char** argv )
{
  char *filename = new char[100];
  strcpy(filename, "/home/sir02mz/iris.data");
  
  int iflod = 5;  //1-10
  
  // testing number range
  int start = (iflod-1)*5;
  int end = (iflod)*5 -1;
  
  int start1 = start+50;
  int end1 = end+50;
  // training number range 
  

  
  
  
  CvTrainingData odata;
  odata.loadIris( filename );
  
  
  CvTrainingData *bdata = odata.split(2,3);
  //CvTrainingData *trainingdata = bdata->getTrainingSubset(start, end);
  CvTrainingData *trainingdata = odata.split(2,3);
  CvTrainingData *testingdata = odata.split(2,3);
   
      
  CvAdaBoost AdaBoost;
  AdaBoost.train( trainingdata, 200, CvWeakLearner::BAYES);
  
  
  
  printf("\nThe testing examples are: ");
  for(int i = start; i <= end; i++)
  {
    printf("%d ", i);
  }
  for(int i = start1; i <= end1; i++)
  {
    printf("%d ", i);
  }
  printf("\n");
  
  
  int nsample = testingdata->getnumsample();
  int nelement = testingdata->getnumelement();
  
  CvMat *testingData = testingdata->getdata();
  
  int clsidx, preclsidx, n = 0, nte = 0, ntr = 0;
  CvMat *sample = cvCreateMat(1, nelement, CV_32FC1);
  for (int i = 0; i < nsample; i++)
  {
    clsidx = testingdata->getclsidxofsample(i);
    for (int j = 0; j < nelement; j++)
    {
      sample->data.fl[j] = cvGetReal2D(testingData, i, j);
      
    }
    preclsidx = AdaBoost.predict(sample, 0.0);
    if (preclsidx != clsidx) 
    {
      if(((i >= start)&&(i <= end)) || ((i >= start1)&&(i <= end1)))
      {
        printf("(%d) ", i);
        nte++;
      }
      else
      {
        printf("%d  ",i);
        ntr++;
      }

      n++;
    } 
    //printf("%d  %d\n", clsidx, preclsidx);
  }
  printf("\n");
  cvReleaseMat(&sample);
  printf("The training error is %f.\n", (double)ntr/90.0);
  printf("The testing error is %f.\n", (double)nte/10.0);
  printf("The total error is %f.\n", (double)n/100.0);
  delete [] filename;

  return 0;
}

