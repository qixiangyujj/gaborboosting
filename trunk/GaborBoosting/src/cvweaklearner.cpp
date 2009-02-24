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
#include "cvweaklearner.h"

CvWeakLearner::CvWeakLearner()
{
}


CvWeakLearner::~CvWeakLearner()
{
  clear();
}




/*!
    \fn CvWeakLearner::CvWeakLearner(CvTrainingData *data, int Learner_Type)
 */
 CvWeakLearner::CvWeakLearner(CvTrainingData *data, int Learner_Type)
{
    train(data, Learner_Type);
}


/*!
    \fn CvWeakLearner::train(CvTrainingData *data, int Learner_Type)
 */
bool CvWeakLearner::train(CvTrainingData *data, int Learner_Type)
{
    type = Learner_Type;
    IsTrained = false;
    
    if (!data)
    {
      printf("Training data for this weak learner has not been initilised!\n");
      return false;
    }
    else
    {
      switch(type)
      {
        case BAYES:
          bayeslearning(data);
          break;
        case KNEAR:
          break;
        case SVM:
          svmlearning(data);
          break;
        case ANN:
          annlearning(data);
          break;
        case FLD:
          fldlearning( data );
          break;
        case POTSU:
          thresholdlearning(data);
          break;
      }
      IsTrained = true;
      return true;
    }
}


/*!
    \fn CvWeakLearner::training_error()
 */
double CvWeakLearner::training_error()
{
   if(!IsTrained)
   {
     printf("The weak learner has not been trained!\n");
     exit(-1);
   } 
   else
   {
     //printf("Error of this weak learner is %f. ", error);
     return error;
   }
}


/*!
    \fn CvWeakLearner::save(const char *filename)
 */
void CvWeakLearner::save(const char *filename)
{
   //cvSave( filename, (CvMat*)preClasses, NULL, NULL, cvAttrList(0,0));
   bayes->save(filename, 0); 
}


/*!
    \fn CvWeakLearner::importance()
 */
double CvWeakLearner::importance()
{
   if(!IsTrained)
   {
     printf("The weak learner has not been trained!\n");
     exit(-1);
   } 
   else
   {
     printf("Error of this weak learner is %f. ", error);
     printf("Alpha of this weak learner is %f\n", alpha);
     return alpha;
   }
}


/*!
    \fn CvWeakLearner::predict(CvMat* sample)
 */
double CvWeakLearner::predict(CvMat* sample)
{
    if(!sample)
    {
      perror("Weak Learner: The testing data is NULL.\n");
      exit(-1);
    }
      
    
    double label;
    switch (type)
    {
        case BAYES:
          label = bayespredict(sample);
          break;
        case KNEAR:
          break;
        case SVM:
          break;
        case ANN:
          label = annpredict(sample);
          break;
        case FLD:
          break;
        case POTSU:
          label = thresholdpredict(sample);
          break;

    }
    
    return label;
}   


/*!
    \fn CvWeakLearner::clear()
 */
void CvWeakLearner::clear()
{
    switch(type)
    {
        case BAYES:
          delete bayes;
          break;
        case KNEAR:
          break;
        case SVM:
          delete svm;
          break;
        case ANN:
      //ann->clear();
          delete (CvANN_MLP *)ann;
          break;
        case FLD:
      break;
    case POTSU:
      break;
    }
}


/*!
    \fn CvWeakLearner::resampling(CvTrainingData* origin)
 */
CvTrainingData* CvWeakLearner::resampling(CvTrainingData* origin)
{
    CvTrainingData *data = new CvTrainingData;
    int numcls = origin->getnumcls();
    int numsample = origin->getnumsample();
    int numelement = origin->getnumelement();

    CvMat *oldTrainingData = cvCreateMat(numsample, numelement, CV_32FC1);
    CvMat *newTrainingData;  
    CvMat *num = cvCreateMat(numsample, 1, CV_32FC1);
    CvMat *sample = cvCreateMat(1, numelement, CV_32FC1);

    oldTrainingData = origin->getdata();

    double v,v1,v2;
    double dsum = 0.0;
    int sum;
    int numpos = origin->getnumsamcls(1);
    int numneg = origin->getnumsamcls(2);
    for (int i = 0; i < numsample; i++)
    {
      v1 = origin->getweightofsample(i);
      int label = origin->getclsidxofsample(i);
      if (label == 1)
      {
        v1 = v1*2*numpos;
        if ((v1-1.0)<0.001) v = (origin->getweightofsample(i))*2*numpos*1;
        else v = (origin->getweightofsample(i))*2*numpos*50;
      }
      else
      {
        v1 = v1*2*numneg;
        if ((v1-1.0)<0.001) v = (origin->getweightofsample(i))*2*numneg*1;
        else v = (origin->getweightofsample(i))*2*numneg*50;
      }

      v = round(v);
      cvSetReal1D(num, i, v);
      dsum = dsum + v;
    }
    sum = (int)dsum;
    data->init(numcls, sum, numelement);
    printf("Resampling: %d samples are generated.\n", sum);
    newTrainingData = cvCreateMat(sum, numelement, CV_32FC1);
    int snum;
    int n = 0;
    int clsidx;
    for (int i = 0; i < numsample; i++)
    {
      cvGetRow( oldTrainingData, sample, i );
      snum = (int)cvGetReal1D(num, i);
      clsidx = origin->getclsidxofsample(i); 
      while(snum > 0)
      {
        if ( n >= sum) break;
        for (int j = 0; j < numelement; j++)
        {
          v = cvGetReal1D(sample, j);
          cvSetReal2D(newTrainingData, n, j, v);
        }
        data->setclsidxofsample(clsidx, n);
        snum--;
        n++;
      }
    }
    data->setdata(newTrainingData);
    //data->stat();

    cvReleaseMat(&sample);
    cvReleaseMat(&num);
    cvReleaseMat(&newTrainingData);
    cvReleaseMat(&oldTrainingData);
    return data;
}


/*!
    \fn CvWeakLearner::bayeslearning(CvTrainingData* data)
 */
void CvWeakLearner::bayeslearning(CvTrainingData* data)
{
      int falsepos = 0;
      nsample = data->getnumsample();
      nelement = data->getnumelement();
      bayes = new CvNormalBayesClassifier;
      //CvNormalBayesClassifier *classifier;
      //this->bayes = classifier;
      
      CvMat* trainData = cvCreateMat(nsample, nelement, CV_32FC1);
      trainData = data->getdata();
      CvMat* response = cvCreateMat(nsample, 1, CV_32FC1);
      response = data->getresponse();
      int numpos = data->getnumsamcls(1);
      /* Resampling the training data*/
      //CvTrainingData *resamdata = new CvTrainingData;
      CvTrainingData *resamdata;
      resamdata = resampling(data);
      CvMat *trainData1 = resamdata->getdata();
      CvMat *response1 = resamdata->getresponse();

      if (!bayes->train(trainData1, response1, 0, 0, false))
      {
        printf("Bayes Weak Learner training failed!\n");
        exit(-1);
      }
      CvMat *sample = cvCreateMat(1, nelement, CV_32FC1);
      //classifier->predict(trainData, preresponse);
      double label1, label2;
      double e = 0;
      int n = 0;
      for (int i = 0; i < nsample; i++)
      {
//         for (int j = 0; j < nelement; j++)
//         {
//           cvSetReal1D(sample, j, ((double*)(trainData->data.ptr + trainData->step*i))[j]);
//         }
        cvGetRow(trainData, sample, i);
        label1 = cvGetReal1D(response, i);
        label2 = bayes->predict(sample, 0);
        if (label1 != label2)
        {
          e = e + data->getweightofsample(i);
          if (label1 == 1.0) falsepos++;
          n++;
        }
      }
      fprate = (double)falsepos/numpos;
      error = e;
  //printf("%d / %d      %d / %d\n", n, nsample, falsepos, numpos);
      printf("%d / %d\n", n, nsample);
      alpha = log((1-error)/error)/2;
      
  // update weights
  /*
      double dwei;
      for (int i = 0; i < nsample; i++)
      {
//         for (int j = 0; j < nelement; j++)
//         {
//           cvSetReal1D(sample, j, ((double*)(trainData->data.ptr + trainData->step*i))[j]);
//         }
        cvGetRow(trainData, sample, i);
        label1 = cvGetReal1D(response, i);
        label2 = bayes->predict(sample, 0);

        if (label1 != label2) dwei = exp(alpha)*data->getweightofsample(i);
        else dwei = exp(-1*alpha)*data->getweightofsample(i);
        data->setweightofsample(dwei, i);
      }
     */ 
      
     
      cvReleaseMat(&sample);
      cvReleaseMat(&response1);
      cvReleaseMat(&trainData1);
      delete resamdata;
      cvReleaseMat(&response);
      cvReleaseMat(&trainData);
}


/*!
    \fn CvWeakLearner::annlearning(CvTrainingData *data)
 */
void CvWeakLearner::annlearning(CvTrainingData *data)
{
    int niteration;
    nsample = data->getnumsample();
    nelement = data->getnumelement();

    CvMat *trainData = data->getdata();
    CvMat *trainClasses = data->getresponse();

    CvMat *weight = data->getweights();
    
    CvANN_MLP_TrainParams params;
    params.train_method = CvANN_MLP_TrainParams::RPROP;
  

  
    CvMat *layer_sizes = cvCreateMat(1, 3, CV_32SC1);
    cvSetReal1D( layer_sizes, 0, nelement);
    cvSetReal1D( layer_sizes, 1, 2);
    cvSetReal1D( layer_sizes, 2, 1);
  
    ann = new CvANN_MLP;
    ann->create(layer_sizes, CvANN_MLP::SIGMOID_SYM, 1, 1);
    CvSize size_data = cvGetSize( trainData );
    CvSize size_class = cvGetSize( trainClasses );
    if(size_class.height != nsample)
      cvTranspose( trainClasses, trainClasses );
    //CvSize size_t = cvGetSize( trainData );
    //cvSave("weights.xml", weight, NULL, NULL, cvAttrList(0,0));
    niteration = ann->train( trainData, trainClasses, weight, 0, params, 0 );
    if (niteration <= 0)
    {
      printf("ANN Weak Learner training failed!\n");
      exit(-1); 
    }
    //printf("An Neural Network Weak Learner has been trained with %d iterations.\n", niteration);
    

    // computing error
    CvMat* sample = cvCreateMat(1, nelement, CV_32FC1);
    CvMat* output = cvCreateMat(1, 1, CV_32FC1);
    double label1, label2;
    double e = 0;
    int n = 0;
    double v;
    for (int i = 0; i < nsample; i++)
    {
      cvGetRow(trainData, sample, i);
      ann->predict(sample, output);
      label1 = cvGetReal1D(trainClasses, i);
      label2 = round(cvGetReal1D(output, 0));
      //printf("%f vs %f\n", label1, label2);
      if (label1 != label2)
      {
        e = e + data->getweightofsample(i);
        //printf("%d vs  %d\n", (int)label1, (int)label2);
        //printf("the error is %f .\n", e);
        n++;
      }
    }
    error = e;
  //printf("%d / %d\n", n, nsample);
  
    alpha = log((1-error)/error)/2;
  // update the weights
  /*
    double dwei;
    for (int i = 0; i < nsample; i++)
    {
      cvGetRow(trainData, sample, i);
      ann->predict(sample, output);
      label1 = cvGetReal1D(trainClasses, i);
      label2 = round(cvGetReal1D(output, 0));

      if (label1 != label2) dwei = exp(alpha)*data->getweightofsample(i);
      else dwei = exp(-1*alpha)*data->getweightofsample(i);
      data->setweightofsample(dwei, i);
    }
    */
    
  //ann = (CvANN_MLP*) classifier;


    cvReleaseMat(&sample);
    cvReleaseMat(&output);
    cvReleaseMat( &layer_sizes);
    //delete layer;
  
  
    cvReleaseMat( &weight );
    cvReleaseMat(&trainClasses);
    cvReleaseMat(&trainData);
}


/*!
    \fn CvWeakLearner::bayespredict(CvMat *sample)
 */
double CvWeakLearner::bayespredict(CvMat *sample)
{
    return bayes->predict(sample, 0);
}


/*!
    \fn CvWeakLearner::annpredict(const CvMat *sample) const
 */
double CvWeakLearner::annpredict(const CvMat *sample) const
{
    double label;
    CvMat* output = cvCreateMat(1, 1, CV_32FC1);
    ann->predict(sample, output);
    label = round(cvGetReal1D(output, 0));
    cvReleaseMat(&output);
    return label;
}


/*!
    \fn CvWeakLearner::myerror()
 */
double CvWeakLearner::myerror()
{
    //double m = fprate/error;
    //printf("My error is %f.\n", m);
    //error/fprate;
  fprate = 0.0;
    return fprate;
}


/*!
    \fn CvWeakLearner::thresholdlearning(CvTrainingData *data)
 */

void CvWeakLearner::thresholdlearning(CvTrainingData *data)
{
    int numpos = data->getnumsamcls(1);
    parity = calcparity(data);
    //printf("Parity is %f\n", parity);
    nsample = data->getnumsample();
    nelement = data->getnumelement();
    if (nelement != 1)
    {
      printf("Error: The number of elements in sample must be 1!\n");
      exit(-1);
    }
    CvMat *trainData = data->getdata();
    CvMat *trainClasses = data->getresponse();

    CvMat *weight = data->getweights();

    float* myarr = new float[nsample];
    for (int i = 0; i < nsample; i++)
    {
       myarr[i] = cvGetReal1D(trainData, i);
    }
    std::sort(myarr, myarr + nsample);
 
    int nthresholds = nsample-1;
    float v;
    CvMat *thresholds = cvCreateMat(nthresholds, nelement, CV_32FC1);
    CvMat *theslderrors = cvCreateMat(nthresholds, nelement, CV_32FC1);
    float e;
    for (int i = 0; i < nthresholds; i++)
    {
      v = (myarr[i] + myarr[i+1])/2;
      cvSetReal1D(thresholds, i, v);
      e = thresholderror(v, weight, trainData, trainClasses);
      cvSetReal1D(theslderrors, i, e);
    }
    double min_val, max_val;
    CvPoint min_loc, max_loc;
    cvMinMaxLoc( theslderrors, &min_val, &max_val,&min_loc, &max_loc,NULL );
    error = min_val;
    int loc = min_loc.y; 
    thresholding = cvGetReal1D(thresholds, loc); 

    alpha = log((1-error)/error)/2;
    
  
    double label1, label2, ve, dwei;
    int falsepos = 0;
    int n = 0;
    for (int i = 0; i < nsample; i++)
    {
      ve = cvGetReal1D(trainData,i);
      label1 = cvGetReal1D(trainClasses, i);
      label2 = thresholdpredict(thresholding, ve);
      if (label1 != label2)
      {
        //dwei = exp(alpha)*data->getweightofsample(i);
        if (label1 == 1.0) falsepos++;
        n++;
      }  
      //else dwei = exp(-1*alpha)*data->getweightofsample(i);
      //data->setweightofsample(dwei, i);
    }
    //printf("%d / %d      %d / %d\n", n, nsample, falsepos, numpos);
    
    fprate = (double)falsepos/numpos;
    this->num_pos = numpos;
    this->num_neg = nsample - numpos;
    this->fp = falsepos;
    this->fn = n - falsepos;
    delete [] myarr;
    cvReleaseMat(&theslderrors);
    cvReleaseMat(&thresholds);
    cvReleaseMat(&trainData);
    cvReleaseMat(&trainClasses);
    cvReleaseMat(&weight);

}


/*!
    \fn CvWeakLearner::thresholderror(double thesld, CvMat *weights, CvMat *data, CvMat *classes)
 */
double CvWeakLearner::thresholderror(double thesld, CvMat *weights, CvMat *data, CvMat *classes)
{
   double label1, label2;
   double weightvalue;
   double input;
   double e = 0.0;

   for (int i = 0; i < nsample; i++)
   {
     input = cvGetReal1D(data, i);
     weightvalue = cvGetReal1D(weights, i);
     label1 = cvGetReal1D(classes, i);
     label2 = thresholdpredict(thesld, input);
     if (label1 != label2)
     {
        e = e + weightvalue;
     } 
     if (e >= 0.5) break; 
   }
   return e;
}


/*!
    \fn CvWeakLearner::thresholdpredict(double thresld, double input)
 */
double CvWeakLearner::thresholdpredict(double thresld, double input)
{
    double i, t;
    i = input * parity;
    t = thresld * parity;

    if (i < t) return 1.0;
    else 
       return 2.0;
}


/*!
    \fn CvWeakLearner::calcparity(CvTrainingData *data)
 */
double CvWeakLearner::calcparity(CvTrainingData *data)
{
    int num = data->getnumsample();
    int nele = data->getnumelement();
    if (nele != 1)
    {
      printf("Error: The number of elements in sample must be 1!\n");
      exit(-1);
    }
    CvMat *trainData = data->getdata();
    CvMat *trainClasses = data->getresponse();
    double sum1 = 0.0, sum2 = 0.0, mean1, mean2;
    double label, v;
    int n1 = 0, n2 = 0;
    for (int i = 0; i < num; i++)
    {
      v = cvGetReal1D(trainData,i);
      label = cvGetReal1D(trainClasses, i);
      if (label == 1.0)
      {
         sum1 = sum1 + v;
         n1++;
      }
      else if(label == 2.0)
      {
         sum2 = sum2 + v;
         n2++;
      }
    }
    mean1 = sum1/n1;
    mean2 = sum2/n2;

    cvReleaseMat(&trainData);
    cvReleaseMat(&trainClasses);
    if (mean1 < mean2) return 1.0;
    else return -1.0;
}


/*!
    \fn CvWeakLearner::thresholdpredict(CvMat* sample)
 */
double CvWeakLearner::thresholdpredict(CvMat* sample)
{
    CvSize s;
    s = cvGetSize( sample );
    if ((s.width != 1)||(s.height != 1))
    {
       printf("No of elements of the example testing data must be in 1 when the thresholding method is chosen.\n");
       exit(-1);
    }
    double v = cvGetReal1D(sample, 0);
    double l = thresholdpredict(thresholding, v);
    return l;
}


/*!
    \fn CvWeakLearner::getthreshold()
 */
double CvWeakLearner::getthreshold()
{
    return thresholding;
}


/*!
    \fn CvWeakLearner::getparity()
 */
double CvWeakLearner::getparity()
{
    return parity;
}



/*!
    \fn CvWeakLearner::setType(int Learner_Type)
 */
void CvWeakLearner::setType(int Learner_Type)
{
    type = Learner_Type;
}


/*!
    \fn CvWeakLearner::setparity(double p)
 */
void CvWeakLearner::setparity(double p)
{
    parity = p;
}


/*!
    \fn CvWeakLearner::setthreshold(double thres)
 */
void CvWeakLearner::setthreshold(double thres)
{
    thresholding = thres;
}


/*!
    \fn CvWeakLearner::predict(double value)
 */
double CvWeakLearner::predict(double value)
{
    double label;
    switch (type)
    {
        case BAYES:
          label = bayespredict(value);
          break;
        case KNEAR:
          break;
        case SVM:
          label = svmpredict( value );
          break;
        case ANN:
          label = annpredict(value);
          break;
        case FLD:
          label = fldpredict( value);
          break;
        case POTSU:
          label = thresholdpredict( thresholding, value );
          break;

    }
    
    return label;
}



/*!
    \fn CvWeakLearner::describe()
 */
void CvWeakLearner::describe()
{
  printf("%d / %d      %d / %d\n", fp, num_pos, fn, num_neg);
}


/*!
    \fn CvWeakLearner::confident(double value)
 */
double CvWeakLearner::confident(double value)
{
  double confid;
  switch (type)
  {
  case BAYES:
          //label = bayespredict(sample);
    break;
  case KNEAR:
    break;
  case SVM:
    break;
  case ANN:
          //label = annpredict(sample);
    break;
  case FLD:
    break;
  case POTSU:
    confid = thresholdconfident( value );
    break;
    
  }
  
  return confid;
}


/*!
    \fn CvWeakLearner::thresholdconfident( double input)
 */
double CvWeakLearner::thresholdconfident( double input)
{
  double v = input*parity - thresholding*parity;
  if( v < 0 ) v = fabs(v);
  else //v = 0.0;
  {
    v = v*(-1.0);
  }
  return v;
}


/*!
    \fn CvWeakLearner::fldlearning(CvTrainingData *data)
 */
void CvWeakLearner::fldlearning(CvTrainingData *data)
{
  num_pos = data->getnumsamcls( 1 );
  num_neg = data->getnumsamcls( 2 );
  nsample = data->getnumsample(); 
  nelement = data->getnumelement();
  
  CvMat* trainData = data->getdata();
  CvMat *trainClasses = data->getresponse();
  
  double mean_pos = 0.0, mean_neg = 0.0, sum_pos = 0.0, sum_neg = 0.0;
  
  for(int i = 0; i < nsample; i++)
  {
    if ( cvGetReal1D(trainClasses, i) == 1.0)
    {
      double value = cvGetReal1D( trainData, i );
      sum_pos += value;
    }
    else
    {
      double value = cvGetReal1D( trainData, i );
      sum_neg += value;
    }
  }
  mean_neg = sum_neg / (double)num_neg;
  mean_pos = sum_pos / (double)num_pos;
  
  double S_pos = 0.0, S_neg = 0.0;
 
  for(int i = 0; i < nsample; i++)
  {
    if ( cvGetReal1D(trainClasses, i) == 1.0)
    {
      double value = cvGetReal1D( trainData, i );
      S_pos += pow((value -mean_pos),2.0);
    }
    else
    {
      double value = cvGetReal1D( trainData, i );
      S_neg += pow((value -mean_neg),2.0);
    }
  }
  double Sw = S_pos+S_neg;
  w = pow(Sw,-1.0)*(mean_pos-mean_neg);
  double proj_neg = w*mean_neg;
  double proj_pos = w*mean_pos;
  b = (proj_neg+proj_pos)*(-0.5);
  
  double ve, dwei = 0.0;
  double label1, label2;
  fn = 0;
  fp = 0;
  for (int i = 0; i < nsample; i++)
  {
    ve = cvGetReal1D( trainData,i );
    label1 =  cvGetReal1D(trainClasses, i);
    label2 = predict( ve );
    if (label1 != label2)
    {
      dwei += data->getweightofsample( i );
      
      /* 
      if (label1 == 1)
        fn++;
      if (label2 == 2)
        fp++;
      */
    }  
  }
  error = dwei;
  alpha = log((1-error)/error)/2;
  //printf("The error rate is %f\n", error);
  //printf("The false negative rate is %f\n", (double)fp/(double)num_pos);
  //printf("The false positive rate is %f\n", (double)fn/(double)num_neg);
  //printf("The false negative rate is %d   %d\n", fp, num_pos);
  //printf("The false positive rate is %d    %d\n", fn, num_neg);
  
  
  
  cvReleaseMat( &trainClasses );
  cvReleaseMat( &trainData );
}


/*!
    \fn CvWeakLearner::annpredict(double value) const
 */
double CvWeakLearner::annpredict(double value) const
{
  double label;
  CvMat* sample = cvCreateMat(1, 1, CV_32FC1);
  cvSetReal1D( sample, 0, value);
  CvMat* output = cvCreateMat(1, 1, CV_32FC1);
  ann->predict(sample, output);
  label = round(cvGetReal1D(output, 0));
  cvReleaseMat(&output);
  cvReleaseMat(&sample);
  return label;
}


/*!
    \fn CvWeakLearner::bayespredict(double value)
 */
double CvWeakLearner::bayespredict(double value)
{
  double label;
  CvMat* sample = cvCreateMat(1, 1, CV_32FC1);
  cvSetReal1D( sample, 0, value);
  label = bayes->predict(sample, 0);
  cvReleaseMat(&sample);
  return label;
}


/*!
    \fn CvWeakLearner::fldpredict( double value )
 */
double CvWeakLearner::fldpredict( double value )
{
  double y;
  y = w*value + b;
  
  double label;
  if(y >= 0.0) label = 1.0;
  else label = 2.0;
  return label;
}


/*!
    \fn CvWeakLearner::update(CvTrainingData *data)
 */
void CvWeakLearner::update(CvTrainingData *data)
{
if (error > 0.0)
{
  double label1, label2, ve, dwei;
  
  CvMat* trainData = data->getdata();
  CvMat *trainClasses = data->getresponse();
  
  for (int i = 0; i < nsample; i++)
  {
    ve = cvGetReal1D(trainData,i);
    label1 = cvGetReal1D(trainClasses, i);
    label2 = predict( ve );
    if ( label1 != label2 )
      dwei = exp(alpha)*data->getweightofsample( i );
    else 
      dwei = exp(-1*alpha)*data->getweightofsample( i );
    assert(dwei > 0.0);
    data->setweightofsample( dwei, i );
  }
  
  cvReleaseMat( &trainClasses );
  cvReleaseMat( &trainData );
}
}


/*!
    \fn CvWeakLearner::svmlearning(CvTrainingData *data)
 */
void CvWeakLearner::svmlearning(CvTrainingData *data)
{

  nsample = data->getnumsample();
  nelement = data->getnumelement();
  
  CvMat *trainData = data->getdata();
  CvMat *trainClasses = data->getresponse();
  
  CvMat *weight = data->getweights();
  
  svm = new CvSVM;
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
  
  int _kernel_type = CvSVM::LINEAR;
  
  double _degree = 3.0;
  double _gamma = 1.0;
  double _coef0 = 0.0;
  double _C = 1.0;
  double _nu = 1.0;
  double _p = 1.0;
  
  CvSVMParams  params( CvSVM::C_SVC, CvSVM::POLY, _degree, _gamma, _coef0, _C, _nu, _p,
                       0, term_crit );
  svm->train( trainData, trainClasses, 0, 0, params );

    // computing error
  CvMat* sample = cvCreateMat(1, nelement, CV_32FC1);
  CvMat* output = cvCreateMat(1, 1, CV_32FC1);
  double label1, label2;
  double e = 0;
  int n = 0;
  double v;
  float preclass;
  for (int i = 0; i < nsample; i++)
  {
    cvGetRow(trainData, sample, i);
    preclass = svm->predict(sample);
    label1 = cvGetReal1D(trainClasses, i);
    label2 =preclass;
    if (label1 != label2)
    {
      e = e + data->getweightofsample(i);

      n++;
    }
  }
  error = e;

  
  alpha = log((1-error)/error)/2;

  
  
  cvReleaseMat(&sample);
  cvReleaseMat(&output);
 
  
  
  cvReleaseMat( &weight );
  cvReleaseMat(&trainClasses);
  cvReleaseMat(&trainData);
}


/*!
    \fn CvWeakLearner::svmpredict(double value)
 */
double CvWeakLearner::svmpredict(double value)
{
  CvMat* sample = cvCreateMat(1,1, CV_32FC1);
  cvSetReal1D( sample, 0, value);
  
  float label = svm->predict( sample );
  cvReleaseMat(&sample);
  return label;
}


/*!
    \fn CvWeakLearner::svmpredict( CvMat *sample )
 */
double CvWeakLearner::svmpredict( CvMat *sample )
{
  float label = svm->predict( sample );
  return label;
}
