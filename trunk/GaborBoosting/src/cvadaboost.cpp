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
#include "cvadaboost.h"

CvAdaBoost::CvAdaBoost()
{
}


CvAdaBoost::~CvAdaBoost()
{
   clear();
}




/*!
    \fn CvAdaBoost::train(CvTrainingData *data, int numweak, int learner_type)
 */
bool CvAdaBoost::train(CvTrainingData *data, int numweak, int learner_type)
{
    if (!data)
    {
      perror("Training data is null!\n");
      return false;
    }
    else if (numweak <= 0)
    {
      perror("Number of weak learners can not be zero or negative.\n");
      return false;
    }
    else
    {
      init(data, numweak, learner_type);
      double error, alpha;
      nweak = 0;
      for (int i = 0; i < niteration; i++)
      {
        CvWeakLearner *weak = new CvWeakLearner;
        printf("\n");
        printf("--------- The Iteration %d/%d ----------\n", i, niteration);

        //char filename[80];
        //sprintf(filename,"/local/%d_weight.xml",i); 
        //tdata->saveweight(filename);
        weak->train(tdata, weak_type);
        error = weak->training_error();
        /*if (error >= 0.5) 
        {
          printf("Learning quit from the %d th from %d due to the error exceeds the random guessing, Only %d weak learners are built.\n", nweak, niteration, nweak);
          break;
        }
        */
        weaks.push_back(*weak);

        
        alpha = weak->importance();
        alphas.push_back(alpha);
        normalize();
        double total_error = perform();
        if((error == 0.0)||(total_error == 0.0))
        {
          printf("Learning finished at the %d th round, due to the error is zero.\n", nweak);
          break;
        }
        weak->update(tdata);

        nweak++;
        //delete weak;
      }
      return true;

    }
}


/*!
    \fn CvAdaBoost::init(CvTrainingData *data, int numiteration, int learner_type)
 */
void CvAdaBoost::init(CvTrainingData *data, int numiteration, int learner_type)
{
    weak_type = learner_type;
    tdata = data->clone();
    niteration = numiteration;
    ncls = tdata->getnumcls();
    nelement = tdata->getnumelement();
    nsample = tdata->getnumsample();
    nweak = 0;
    for (int i = 0; i < nsample; i++)
    {
       tdata->setweightofsample((double)1/nsample, i);
    }
    bInitialised = true;
}


/*!
    \fn CvAdaBoost::normalize()
 */
void CvAdaBoost::normalize()
{
    double sumweight;
    double dwei = 0.0;
    for(int i = 0; i < nsample; i++)
    {
      dwei = dwei + tdata->getweightofsample(i);
    }
    sumweight = dwei;
    for(int i = 0; i < nsample; i++)
    {
      dwei = tdata->getweightofsample(i)/sumweight;
      tdata->setweightofsample(dwei, i);
    }
}


/*!
    \fn CvAdaBoost::predict(CvTestingData *testing)
 */
int CvAdaBoost::predict(CvTestingData *testing)
{
    if(!testing)
    {
      perror("The testing is NULL.\n");
      exit(-1);
    }
    int numsample, numelement;
    numsample = testing->getnumsample();
    numelement = testing->getnumelement();
    if (numelement != nelement)
    {
      perror("The number of element for each sample in the testing data is not same as in the training data!\n");
      exit(-1);
    }
    CvMat *data;
    data = testing->getdata();
    CvMat *sample = cvCreateMat(1, nelement, CV_32FC1);

    int clsidx, preclsidx;
    for (int i = 0; i < numsample; i++)
    {
      /* filling the sample */
      for (int j = 0; j < numelement; j++)
      {
        sample->data.fl[j] = cvGetReal2D(data, i, j);
      }
      /*for (int ii = 0; ii < weaks.size(); ii++)
      {
         weak = &weaks[ii];
         label = weak->predict(sample);
         label = label*2 -3;     /*  change the 1 ~ 2 scale to -1 ~ 1 scale */
/*         alpha = alphas[ii];
         result = result + alpha*label;
      } 
      if (result > 0) clsidx = 2;
      else if (result < 0) clsidx = 1;
      else clsidx = 0;
      */
      clsidx = predict(sample);
      preclsidx = testing->getclsidxofsample(i);

    }
    cvReleaseMat(&sample);
    cvReleaseMat(&data);
}


/*!
    \fn CvAdaBoost::predict(CvMat* sample)
 */
int CvAdaBoost::predict(CvMat* sample)
{
    double label,alpha;
    double result = 0.0;
    int clsidx;
   
    int num = weaks.size();
    for (int i = 0; i < weaks.size(); i++)
    {
       label = weaks[i].predict(sample); 
       label = label*2 -3;     /*  change the 1 ~ 2 scale to -1 ~ 1 scale */
       alpha = alphas[i];
       result = result + alpha*label;
    }
    if (result > 0) clsidx = 2;
    else if (result < 0) clsidx = 1;
    else clsidx = 0;
 
    return clsidx;
}


/*!
    \fn CvAdaBoost::perform()
 */
double CvAdaBoost::perform()
{
    int preclsidx, clsidx;
    int n = 0;
    double training_error;
    CvMat *data = cvCreateMat(nsample, nelement, CV_32FC1);
    data = tdata->getdata();
    CvMat *sample = cvCreateMat(1, nelement, CV_32FC1);
    for (int i = 0; i < nsample; i++)
    {
      clsidx = tdata->getclsidxofsample(i);
      for (int j = 0; j < nelement; j++)
      {
        sample->data.fl[j] = cvGetReal2D(data, i, j);
      }
      preclsidx = predict(sample);
      if (preclsidx != clsidx) n++; 
    }
    cvReleaseMat(&sample);
    cvReleaseMat(&data);
    training_error= (double)n/nsample;
    
    printf("\n");
    printf("The general error for the AdaBoosting Classifier is %f. It contains %d weak learners.\n", training_error, weaks.size());
    printf("\n");
    return training_error;
}


/*!
    \fn CvAdaBoost::clear()
 */
void CvAdaBoost::clear()
{
    if(bInitialised)
    {
       
       for (int i = 0; i < weaks.size(); i++)
       {
         weaks[i].clear();
       }
       //cvReleaseMat(&weight);
       delete tdata;
       
    }
}
