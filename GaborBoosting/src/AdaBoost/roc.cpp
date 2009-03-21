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
  
  double bias_start = -8;
  double bias_end = 8;
  double interval = 0.000001;
  int weak_type = CvWeakLearner::ANN;
  
  char *filename = new char[100];
  strcpy(filename, "/home/sir02mz/iris.data");
 
  CvTrainingData odata;
  odata.loadIris( filename );
  
  
  CvTrainingData *bdata = odata.split(2,3);
  //CvTrainingData *trainingdata = bdata->getTrainingSubset(start, end);
  CvTrainingData *trainingdata = odata.split(2,3);
  CvTrainingData *testingdata = odata.split(2,3);
   
      
  CvAdaBoost AdaBoost;
  AdaBoost.train( trainingdata, 200, weak_type );
  
  
  int nsample = testingdata->getnumsample();
  int nelement = testingdata->getnumelement();
  
  CvMat *testingData = testingdata->getdata();
  

  FILE *file = fopen("roc.txt", "w");
  
  
  
  for(double bias = bias_start; bias < bias_end; bias=bias+interval)
  {
    int clsidx, preclsidx, n = 0, tp = 0, fp = 0;
    CvMat *sample = cvCreateMat(1, nelement, CV_32FC1);
    for (int i = 0; i < nsample; i++)
    {
      clsidx = testingdata->getclsidxofsample(i);
      for (int j = 0; j < nelement; j++)
      {
        sample->data.fl[j] = cvGetReal2D(testingData, i, j);
      
      }
      preclsidx = AdaBoost.predict(sample, bias);
      if(preclsidx != clsidx)
      {
        n++;
      }
      if((clsidx == 1)&&(preclsidx == 1))
      {
        tp++;
      }
      else if((clsidx == 2)&&(preclsidx == 1))
      {
        fp++;
      }
    }
    printf("%f   %f            %f\n", (double)tp/50.0, (double)fp/50.0, bias);
    fprintf(file, "%f %f %f\n", (double)tp/50.0, (double)fp/50.0, bias );
    //fprintf(file, "%d %d %d %f\n", tp, fp, n, bias);
  }
  fclose(file);
  
  delete [] filename;

  return 0; 
}

