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
#include <ml.h>
#include <cxcore.h>


int main( int argc, char** argv )
{
  
  char *filename = new char[100];
  strcpy(filename, "/home/sir02mz/iris.data");
  
  
  int iflod = 10;  //1-10
  
  // testing number range
  int start = (iflod-1)*5;
  int end = (iflod)*5 -1;
  
  int start1 = start+50;
  int end1 = end+50;
  // training number range 
  printf("The testing examples are: ");
  for(int i = start; i <= end; i++)
  {
    printf("%d ", i);
  }
  for(int i = start1; i <= end1; i++)
  {
    printf("%d ", i);
  }
  printf("\n");

  

  CvTrainingData odata;
  odata.loadIris( filename );
  
  //CvTrainingData *data2 = odata.split(2);
  CvTrainingData *data2 = odata.split(2,3);
  //CvTrainingData *tedata = data2->getTestingSubset(start,end);
  CvTrainingData *tedata = odata.split(2,3);
  //CvTrainingData *trdata = data2->getTrainingSubset(start,end);
  CvTrainingData *trdata = odata.split(2,3);
  
  
  
  
  
  
  
  
  CvTrainingData *trainingdata = trdata;
  
  //
  CvMat *trainData = trainingdata->getdata();
  CvMat *label = trainingdata->getresponse();
  
  //cvSave("label.xml", label, NULL, NULL);
  int nsample = trainingdata->getnumsample();
  int nelement = trainingdata->getnumelement();
  
  // make svm param
  CvSVMParams params;
  
  params.C = 2;
  
  params.svm_type = CvSVM::NU_SVC;
  params.nu = 0.97;   // between 0.0 and 1.0 with 0.1 interval
  
  
  params.kernel_type = CvSVM::POLY;
  params.degree = 0.6;  // between 0.1 and 3.0 with 0.1 interval
  params.gamma = 1.0;   // between 0.1 and 5.0 with 0.1 interval
  params.coef0 = 1.0;   // between 0.1 and 9.0 with 0.1 interval
  
  params.class_weights = 0;
  
  
  params.term_crit.type = CV_TERMCRIT_ITER+CV_TERMCRIT_EPS;
  params.term_crit.max_iter = 200;
  params.term_crit.epsilon = 0.98;
   // prepare for CvParamGrid
  
  //initilize the grid parameters
  CvParamGrid C_grid = CvSVM::get_default_grid(CvSVM::C);
  CvParamGrid gamma_grid = CvSVM::get_default_grid(CvSVM::GAMMA);
  CvParamGrid p_grid = CvSVM::get_default_grid(CvSVM::P);
  CvParamGrid nu_grid = CvSVM::get_default_grid(CvSVM::NU);
  CvParamGrid coef_grid = CvSVM::get_default_grid(CvSVM::COEF);
  CvParamGrid degree_grid = CvSVM::get_default_grid(CvSVM::DEGREE);
  
  //nu_grid.min_val = 0.0; nu_grid.max_val = 1.0; nu_grid.step = 0.1;
  
  //degree_grid.min_val = 0.1; degree_grid.max_val = 3.0; degree_grid.step = 0.1;
  
  //gamma_grid.min_val = 0.0; gamma_grid.max_val = 5.0; gamma_grid.step = 1;
  
  //coef_grid.min_val = 0.0; coef_grid.max_val = 9.0; coef_grid.step = 1;
  
  
  // The number of class C is 2, no need to optimise
  C_grid.step = 0;
  
  // no need to regression
  p_grid.step = 0;

  // set the grid for different case
  switch(params.kernel_type)
  {
    case CvSVM::POLY:
      break;
    case CvSVM::RBF:
      coef_grid.step = 0;
      degree_grid.step = 0;
      break;
    case CvSVM::SIGMOID:
      degree_grid.step = 0;
      break;
  }
  
  CvSVM svm;
  
  int k_fold = 10;
  // train an svm
//   bool success = svm.train_auto( trainData, label, 0, 0, params, k_fold, C_grid, gamma_grid, p_grid, nu_grid, coef_grid, degree_grid);
//   if(!success)
//   {
//     printf("ERROR: Training failed!\n");
//     exit(-1);
//   }
  
  
  
  svm.train( trainData, label, 0, 0, params);
  
  
  
  CvSVMParams params1 = svm.get_params();
  
  
  
  
  
  
  
  
  
  
  CvTrainingData *testdata = odata.split(2,3);
  //testing stage
  nsample = testdata->getnumsample();
  
  CvMat *testingData = testdata->getdata();
  
  int clsidx, preclsidx, n = 0, nte = 0, ntr = 0;
  CvMat *sample = cvCreateMat(1, nelement, CV_32FC1);
  for (int i = 0; i < nsample; i++)
  {
    clsidx = testdata->getclsidxofsample(i);
    for (int j = 0; j < nelement; j++)
    {
      sample->data.fl[j] = cvGetReal2D(testingData, i, j);
      
    }
    preclsidx = svm.predict(sample);
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

  cvReleaseMat( &trainData );
  cvReleaseMat( &label );
  return 0;
}



