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
#include "cvtrainingdata.h"

CvTrainingData::CvTrainingData()
{
    bInitialised = 0;
    maxnum = 0;
    ncls = 0;
    nelement = 0;
}


CvTrainingData::~CvTrainingData()
{
    if (bInitialised == 1)
    {
      cvReleaseMat(&Valcache);
      delete [] cls;
      delete [] clsdist;
      delete [] weight;
    }
}




/*!
    \fn CvTrainingData::numcls() const
 */
int CvTrainingData::getnumcls() const
{
    return ncls;
}



/*!
    \fn CvTrainingData::numsample() const
 */
int CvTrainingData::getnumsample() const
{
    return maxnum;
}


/*!
    \fn CvTrainingData::setnumsample(int num)
 */
void CvTrainingData::setnumsample(int num)
{
    maxnum = num;
}


/*!
    \fn CvTrainingData::setnumcls(int num)
 */
void CvTrainingData::setnumcls(int num)
{
    ncls = num;
}


/*!
    \fn CvTrainingData::loadIris(const char *filename);
 */
bool CvTrainingData::loadIris(const char *filename)
{
//     float e1,e2,e3,e4;
//     char clsname[20];
//     char buffer[80];
//     int n = 0;
//     int nclsdist;
//     /*Initialsed 3 classes, 150 samples and 8 elements per sample*/
//     init(3, 150, 4);   
//     
// 
// 
//     ifstream irisfile;
//     irisfile.open(filename, ios::out);
//     if(irisfile.is_open())
//     {
//       while(!irisfile.eof())
//       {
//         irisfile >> buffer;
//         //printf("%s\n",buffer);
// 
//         sscanf(buffer,"%f,%f,%f,%f,%s",&e1,&e2,&e3,&e4,clsname);
// 
//         if (!strcmp(clsname, "Iris-setosa"))
//         {
//           cls[n] = 1;
//           //printf("Sample No. %d is Iris-setosa.\n",n+1);
//           nclsdist = clsdist[0];
//           nclsdist++;
//           clsdist[0] = nclsdist;
//         }
//         else if (!strcmp(clsname, "Iris-versicolor"))
//         {
//           cls[n] = 2;
//           //printf("Sample No. %d is Iris-versicolor.\n",n+1);
//           nclsdist = clsdist[1];
//           nclsdist++;
//           clsdist[1] = nclsdist;
//         }
//         else if (!strcmp(clsname, "Iris-virginica"))
//         {
//           cls[n] = 3;
//           //printf("Sample No. %d is Iris-virginica.\n",n+1);
//           nclsdist = clsdist[2];
//           nclsdist++; 
//           clsdist[2] = nclsdist;
//         }
//         
//         weight[n] = 0.0;
//         cvSetReal2D( (CvMat*)Valcache, n, 0, (float)e1 );
//         cvSetReal2D( (CvMat*)Valcache, n, 1, (float)e2 );
//         cvSetReal2D( (CvMat*)Valcache, n, 2, (float)e3 );
//         cvSetReal2D( (CvMat*)Valcache, n, 3, (float)e4 );
//         n++;
//       }
//     }
// 
//     
//     irisfile.close(); 
  
  assert(filename != NULL );
  FILE *file = fopen( filename, "r");
  if(file == NULL)
  {
    printf("ERROR: Can not open the file %s.\n", filename);
    exit(-1);
  }
  
  int fpos;
  float feature1, feature2, feature3, feature4;
  char * name = new char[50];

  init(3, 150, 4);
  
  int n = 0;
  int nclsdist;
  while (!feof(file))
  {
    if (fscanf(file, "%3f,%3f,%3f,%3f,%s", &feature1, &feature2, &feature3, &feature4, name) == EOF) break;
    if(!strcmp( name, "Iris-setosa"))
    {
      cls[n] = 1;
      nclsdist = clsdist[0];
      nclsdist++;
      clsdist[0] = nclsdist;
    }
    else if(!strcmp( name, "Iris-versicolor"))
    {
      cls[n] = 2;
      nclsdist = clsdist[1];
      nclsdist++;
      clsdist[1] = nclsdist;
    }
    else if(!strcmp( name, "Iris-virginica"))
    {
      cls[n] = 3;
      nclsdist = clsdist[2];
      nclsdist++; 
      clsdist[2] = nclsdist;
    }
    	
    weight[n] = 0.0;
    cvSetReal2D( (CvMat*)Valcache, n, 0, feature1 );
    cvSetReal2D( (CvMat*)Valcache, n, 1, feature2);
    cvSetReal2D( (CvMat*)Valcache, n, 2, feature3 );
    cvSetReal2D( (CvMat*)Valcache, n, 3, feature4 );
    n++;
  }
  
  
  
  delete [] name;
 
  fclose(file);
  statclsdist();
}










/*!
    \fn CvTrainingData::numelement() const
 */
int CvTrainingData::getnumelement() const
{
    return nelement;
}


/*!
    \fn CvTrainingData::setnumelement(int num)
 */
void CvTrainingData::setnumelement(int num)
{
    nelement = num;
}


/*!
    \fn CvTrainingData::init(int numcls, int numsample, int numelement)
 */
void CvTrainingData::init(int numcls, int numsample, int numelement)
{
    setnumcls(numcls);
    setnumsample(numsample);
    setnumelement(numelement);
    cls = new int[maxnum];
    weight = new double[maxnum];
    clsdist = new int[ncls];
    
    for (int i = 0; i < ncls; i++)
    {
      clsdist[i] = 0;
    }

    Valcache = cvCreateMat(maxnum, nelement, CV_32FC1);
    
    cvSetZero(Valcache);
    bInitialised = 1;
}




/*!
    \fn CvTrainingData::int getnumsamcls(int clsidx) const
 */
int CvTrainingData::getnumsamcls(int clsidx) const
{
    if ((clsidx <= 0) || (clsidx > ncls))
    {
       perror("Class index exceeds the number of class, or negative.\n");
       return 0;
    }
    else
    {
       return clsdist[clsidx-1]; 
    }
}



/*!
    \fn CvTrainingData::stat()
 */
void CvTrainingData::stat()
{
    printf("\n");
    printf("%d samples in the training data.\n",getnumsample());
    printf("%d classes in the training data.\n",getnumcls());
    printf("%d elements for each sample in the training data.\n",getnumelement());
    printf("\n");
    statclsdist();
    for (int i = 0; i < getnumcls(); i++)
    {
       printf("Number of samples for Class %d is %d\n", i+1, getnumsamcls(i+1));
    }
}


/*!
    \fn CvTrainingData::clone() const
 */
CvTrainingData* CvTrainingData::clone() const
{
    if (bInitialised == 1)
    {
      CvTrainingData *data = new CvTrainingData;
      data->init(ncls, maxnum, nelement);
      for (int i = 0; i < maxnum; i++)
      {
         data->setclsidxofsample(getclsidxofsample(i),i);
         data->setweightofsample(getweightofsample(i),i);
      }
      //data->Valcache = cvCloneMat(Valcache);
      cvCopy( this->Valcache, data->Valcache, NULL );
      data->statclsdist();
      return data;
    }
    else
    {
      return NULL;
    }
}


/*!
    \fn CvTrainingData::setclsidxofsample(int clsidx, int sampleidx) const
 */
void CvTrainingData::setclsidxofsample(int clsidx, int sampleidx) const
{
    if (clsidx <= 0) 
    {
       perror("Class index exceeds the number of class or negative \n");
       //printf("ncls%d\n", clsidx);
       exit(-1);
    }
    else if ((sampleidx < 0) || (sampleidx >= maxnum))
    {
       perror("Sample index exceeds the number of class or negative \n");
       exit(-1);
    }
    else
    {
       cls[sampleidx] = clsidx;
    }
}


/*!
    \fn CvTrainingData::getclsidxofsample(int sampleidx) const
 */
int CvTrainingData::getclsidxofsample(int sampleidx) const
{
    if ((sampleidx < 0) || (sampleidx >= maxnum))
    {
       perror("Sample index exceeds the number of class or negative \n");
       return 0;
    }
    else
    {
      return cls[sampleidx];
    }
}


/*!
    \fn CvTrainingData::setweightofsample(doulbe weightvalue, int sampleidx)
 */
void CvTrainingData::setweightofsample(double weightvalue, int sampleidx)
{
    if ((sampleidx < 0) || (sampleidx >= maxnum))
    {
       perror("Sample index exceeds the number of class or negative \n");
       exit(-1);
    }
    else
    {
      weight[sampleidx] = weightvalue;
    }
}


/*!
    \fn CvTrainingData::getweightofsample(int sampleidx) const
 */
double CvTrainingData::getweightofsample(int sampleidx) const
{
    if ((sampleidx < 0) || (sampleidx >= maxnum))
    {
       perror("Sample index exceeds the number of class or negative \n");
       return 0.0;
    }
    else
    {
      return weight[sampleidx];
    }
}


/*!
    \fn CvTrainingData::statclsdist()
 */
void CvTrainingData::statclsdist()
{
    int clsidx;
    int v;

    for (int i = 0; i < ncls; i++)
    {
      clsdist[i] = 0;
    }

    for (int i = 0; i < maxnum; i++)
    {
      clsidx = cls[i];
      if (clsidx <= 0)
      {
        printf("%d      %d", clsidx, i);
        perror("Class index exceeds the number of class, or negative.\n");
        exit(-1);
      }
      v = clsdist[clsidx-1];
      v++;
      clsdist[clsidx-1] = v;
    }
}


/*!
    \fn CvTrainingData::split(int clsidx) const
 */
CvTrainingData* CvTrainingData::split(int clsidx) const
{
    if ((clsidx <= 0) || (clsidx > ncls))
    {
      perror("Class index exceeds the number of class, or negative.\n");
      return NULL;
    }
    else
    {
      int numsample = getnumsamcls(clsidx);
      CvTrainingData *sdata = new CvTrainingData;
      sdata->init(1, numsample, getnumelement());
      
      int n = 0;
      double v = 0.0;
      for (int i = 0; i < maxnum; i++)
      {
        if (getclsidxofsample(i) == clsidx)
        {
          //printf("ncls = %d\n",sdata->ncls);
          sdata->setclsidxofsample(1,n);
          
          for (int j = 0; j < getnumelement(); j++)
          {
            v = cvGetReal2D(Valcache,i,j);
            cvSetReal2D(sdata->Valcache, n, j, v);
          }
          sdata->setweightofsample(getweightofsample(i),n);
          n++;
        }
      }

      sdata->statclsdist();
      return sdata;
    }
}


/*!
    \fn CvTrainingData::merge(const CvTrainingData *data) const
 */
CvTrainingData* CvTrainingData::merge(const CvTrainingData *data) const
{
    if ((bInitialised == 0)||(data->bInitialised == 0)) 
    {
      perror("Training data has not been initilized.\n");
      return NULL; 
    }
    else if (nelement != data->getnumelement())
    {
      perror("Two training data do not have the same length samples.\n");
      return NULL; 
    }
    else
    {
      CvTrainingData *newdata = new CvTrainingData;
      int samplenum1, samplenum2, samplenum, clsnum1, clsnum2, clsnum, elementnum, clsidx;
      double v, dwei;
      samplenum1 = maxnum;
      samplenum2 = data->getnumsample();
      samplenum = samplenum1 + samplenum2;
      
      clsnum1 = ncls;
      
      
      
      clsnum2 = data->getnumcls();
      
      clsnum = clsnum1 + clsnum2;
      
      elementnum = getnumelement();
      newdata->init(clsnum, samplenum, elementnum);

      for (int i = 0; i < samplenum; i++)
      {
        
        if (i < samplenum1)
        {
          for (int j = 0; j < getnumelement(); j++)
          {
            v = cvGetReal2D(Valcache,i,j);
            cvSetReal2D(newdata->Valcache, i, j, v);
          }
          clsidx = getclsidxofsample(i);
          dwei = getweightofsample(i); 
        }
        else
        {
          for (int j = 0; j < getnumelement(); j++)
          {
            v = cvGetReal2D(data->Valcache, (i-samplenum1), j);
            cvSetReal2D(newdata->Valcache, i, j, v);
          }
          clsidx = data->getclsidxofsample(i-samplenum1)+clsnum1;
          dwei = data->getweightofsample(i - samplenum1);
        }
        /* Class index */
        newdata->setclsidxofsample(clsidx,i);
        newdata->setweightofsample(dwei,i); 
      }
      newdata->statclsdist();
      return newdata;
    }
}


/*!
    \fn CvTrainingData::extract(int elementidx) const
 */
CvTrainingData* CvTrainingData::extract(int elementidx) const
{
    if ((elementidx < 0) || (elementidx >= getnumelement()))
    {
      perror("Element index exceeds the number of elements, or negative.\n");
      return NULL;
    }
    else
    {
      int clsnum, samplenum, clsidx;
      double v, dwei;
      clsnum = getnumcls();
      samplenum = getnumsample();
      CvTrainingData *data = new CvTrainingData;
      data->init(clsnum, samplenum, 1);
      for (int i = 0; i < samplenum; i++)
      {
        clsidx = getclsidxofsample(i);
        data->setclsidxofsample(clsidx, i);

        v = cvGetReal2D(Valcache, i, elementidx);
        cvSetReal1D(data->Valcache, i, v);
        dwei = getweightofsample(i);
        data->setweightofsample( dwei, i);
      }
      data->statclsdist();
      return data;
    } 
}


/*!
    \fn CvTrainingData::combine(CvTrainingData *data) const
 */
CvTrainingData* CvTrainingData::combine(CvTrainingData *data) const
{
    if ((getnumcls() != 1) || (data->getnumcls() != 1))
    {
      perror("Number of class for the training data must be 1!.\n");
      return NULL;
    }
    else
    {
      int elementnum, elementnum1, elementnum2, samplenum;
      double v, dwei1, dwei2;
      elementnum1 = getnumelement();
      elementnum2 = data->getnumelement();
      elementnum = elementnum1 + elementnum2;
      samplenum = getnumsample();
      CvTrainingData *newdata = new CvTrainingData;
      newdata->init(1, samplenum, elementnum);
      for (int i = 0; i < samplenum; i++)
      {
        int clsidx = getclsidxofsample(i);
        newdata->setclsidxofsample(clsidx, i);
        for (int j = 0; j < elementnum; j++)
        {
          if (j < elementnum1) v = cvGetReal2D(Valcache, i, j);
          else v = cvGetReal2D(Valcache, i, j-elementnum1);
          cvSetReal2D(newdata->Valcache, i, j, v);
        }
      dwei1 = getweightofsample(i);
      dwei2 = getweightofsample(i);
      if (dwei1 != dwei2) 
      {  
        printf("The weights for the sample No, %d are not equal!\n", i);
        exit(-1);
      }
      newdata->setweightofsample(dwei1, i);
      }
      newdata->statclsdist();
      return newdata;
    }
}


/*!
    \fn CvTrainingData::getdata() const
 */
CvMat* CvTrainingData::getdata() const
{
    CvMat* mat = cvCloneMat( Valcache );
    return mat;
}


/*!
    \fn CvTrainingData::getresponse() const
 */
CvMat* CvTrainingData::getresponse() const
{
  CvMat* response = cvCreateMat(1, maxnum, CV_32FC1);
    double v;
    for (int i = 0; i < maxnum; i++)
    {
      v = (double)getclsidxofsample(i);
      cvSetReal1D(response, i, v);
    }
    return response;
}


/*!
    \fn CvTrainingData::setdata(const CvMat *mat)
 */
void CvTrainingData::setdata(const CvMat *mat)
{
    if (!bInitialised) 
    {
      perror("Training data has not been initilized.\n");
      exit(-1); 
    }

    //Valcache = cvCloneMat(mat);
    CvSize size1 = cvGetSize(mat);
    CvSize size2 = cvGetSize(Valcache);
    CvMat *tmat = cvCloneMat(Valcache);
    if(size1.width != size2.width)
    {
      cvTranspose(mat, tmat);
      cvCopy(tmat, Valcache);
    }
    else
    {
      cvCopy(mat, Valcache);
    }
    cvReleaseMat(&tmat);
}


/*!
    \fn CvTrainingData::setresponse(const CvMat* response)
 */
void CvTrainingData::setresponse(const CvMat* response)
{
    if (!bInitialised) 
    {
      perror("Training data has not been initilized.\n");
      exit(-1); 
    }
    int idx;
    for (int i = 0; i < maxnum; i++)
    {
      idx = (int)cvGetReal1D(response, i);
      setclsidxofsample(idx, i);
    }
}


/*!
    \fn CvTrainingData::saveweight(const char *filename) const
 */
void CvTrainingData::saveweight(const char *filename) const
{
    CvMat mat = cvMat( maxnum, 1,
                   CV_32FC1, // 32-bit floating-point, single channel type
                   weight // user data pointer (no data is copied)
                   );
    cvSave( filename, (CvMat*)&mat, NULL, NULL, cvAttrList(0,0));
}     


/*!
    \fn CvTrainingData::savedata(const char *filename) const
 */
void CvTrainingData::savedata(const char *filename) const
{
    cvSave( filename, (CvMat*)Valcache, NULL, NULL, cvAttrList(0,0));
}


/*!
    \fn CvTrainingData::loadUCI(const char  *filename)
 */
bool CvTrainingData::loadUCI(const char  *filename)
{
    float e1,e2,e3,e4,e5,e6,e7,e8;
    int i8;
    char clsname;
    char buffer[80];
    int n = 0;
    int nclsdist;
    /*Initialsed 2 classes,  samples and 8 elements per sample*/
    init(3, 4177, 8);
    
    ifstream UCIfile;
    UCIfile.open(filename, ios::out);
    if(UCIfile.is_open())
    {
      while(!UCIfile.eof())
      {
        UCIfile >> buffer;
        //printf("%s\n",buffer);

        sscanf(buffer,"%c,%f,%f,%f,%f,%f,%f,%f,%d",&clsname,&e1,&e2,&e3,&e4,&e5,&e6,&e7,&i8);
        e8 = (float)i8;

        if (clsname == 'M')
        {
          cls[n] = 1;
          
          nclsdist = clsdist[0];
          nclsdist++;
          clsdist[0] = nclsdist;
        }
        else if (clsname == 'F')
        {
          cls[n] = 2;
          nclsdist = clsdist[1];
          nclsdist++;
          clsdist[1] = nclsdist;
        }
        else if (clsname == 'I')
        {
          cls[n] = 3;
          nclsdist = clsdist[2];
          nclsdist++;
          clsdist[2] = nclsdist;
        }
        
        weight[n] = 0.0;
        cvSetReal2D( (CvMat*)Valcache, n, 0, (float)e1 );
        cvSetReal2D( (CvMat*)Valcache, n, 1, (float)e2 );
        cvSetReal2D( (CvMat*)Valcache, n, 2, (float)e3 );
        cvSetReal2D( (CvMat*)Valcache, n, 3, (float)e4 );
        cvSetReal2D( (CvMat*)Valcache, n, 4, (float)e5 );
        cvSetReal2D( (CvMat*)Valcache, n, 5, (float)e6 );
        cvSetReal2D( (CvMat*)Valcache, n, 6, (float)e7 );
        cvSetReal2D( (CvMat*)Valcache, n, 7, (float)e8 );
        n++;
      }
    }

    
    UCIfile.close(); 
}


/*!
    \fn CvTrainingData::getweights() const
 */
CvMat* CvTrainingData::getweights() const
{
    CvMat* weights = cvCreateMat(maxnum, 1, CV_32FC1);
    double v;
    for (int i = 0; i < maxnum; i++)
    {
      v = getweightofsample(i);
      assert(v >= 0.0);
      cvSetReal1D(weights, i, v);
    }
    return weights;
}


/*!
    \fn CvTrainingData::setweights(const CvMat *wmat)
 */
void CvTrainingData::setweights(const CvMat *wmat)
{
     if (!bInitialised) 
    {
      perror("Training data has not been initilized.\n");
      exit(-1); 
    }
    double w;
    for (int i = 0; i < maxnum; i++)
    {
      w = cvGetReal1D(wmat, i);
      assert(w >= 0.0);
      setweightofsample(w, i);
    }
}


/*!
    \fn CvTrainingData::split(int clsidx1, int clsidx2) const
 */
CvTrainingData* CvTrainingData::split(int clsidx1, int clsidx2) const
{
  CvTrainingData * data1 = this->split(clsidx1);
  CvTrainingData * data2 = this->split(clsidx2);
  CvTrainingData * newdata = data1->merge( data2 );
  delete data1;
  delete data2;
  return newdata;

}


/*!
    \fn CvTrainingData::getsubset(int start, int end) const
 */
CvTrainingData* CvTrainingData::getsubset(int start, int end) const
{
  int num_samples = end - start + 1;
  int num_elements = getnumelement();
  CvTrainingData *newdata = new CvTrainingData;
  newdata->init(1, num_samples, num_elements);
  
  CvMat *omat = getdata();
  CvMat *submat = cvCreateMat(num_samples, num_elements, CV_32FC1);
  cvGetRows( omat, submat, start, end+1, 1);
  //cvSave("data.xml", submat, NULL, NULL, cvAttrList(0,0) );
  newdata->setdata( submat );
  
  for(int i = 0; i < num_samples; i++)
  {
    int clsidx = getclsidxofsample(start + i);
    newdata->setclsidxofsample(clsidx, i);
    newdata->setweightofsample( 0.0 , i);
  }
  cvReleaseMat( &submat );
  cvReleaseMat( &omat );
  return newdata;
}





/*!
    \fn CvTrainingData::getTestingSubset(int start, int end)
 */
CvTrainingData* CvTrainingData::getTestingSubset(int start, int end)
{
  int start1 = start+50;;
  int end1 = end+50;
  int num_elements = getnumelement();
  int num_samples = (end - start + 1)*2;
  int num_class = getnumcls();
  CvTrainingData *newdata = new CvTrainingData;
  newdata->init(num_class, num_samples, num_elements);
  
  CvMat *submat = cvCreateMat(num_samples, num_elements, CV_32FC1);
  
  double v;
  int clsidx, n = 0;
  for(int i = 0; i < maxnum; i++)
  {
    if(((i >= start)&&(i <= end)) || ((i >= start1)&&(i <= end1)))
    {
      clsidx = getclsidxofsample(i);
      newdata->setclsidxofsample(clsidx, n);
      newdata->setweightofsample(0.0, n);
      for(int j = 0; j < getnumelement(); j++)
      {
        v = cvGetReal2D(Valcache, i, j);
        cvSetReal2D(submat, n, j, v);
      }
      n++;
    }
  }
  
  newdata->setdata( submat );
  cvReleaseMat(&submat);
  newdata->statclsdist();
  return newdata;
}


/*!
    \fn CvTrainingData::getTrainingSubset(int start, int end)
 */
CvTrainingData* CvTrainingData::getTrainingSubset(int start, int end)
{
  int start1 = start+50;;
  int end1 = end+50;
  
  int num_elements = getnumelement();
  int num_samples = maxnum - (end - start + 1)*2;
  int num_class = getnumcls();
  
  CvTrainingData *newdata = new CvTrainingData;
  newdata->init(num_class, num_samples, num_elements);
  
  CvMat *submat = cvCreateMat(num_samples, num_elements, CV_32FC1);
  
  double v;
  int clsidx, n = 0;
  for(int i = 0; i < maxnum; i++)
  {
    if(((i >= start)&&(i <= end)) || ((i >= start1)&&(i <= end1)))
    {
    }
    else
    {
      clsidx = getclsidxofsample(i);
      newdata->setclsidxofsample(clsidx, n);
      newdata->setweightofsample(0.0, n);
      for(int j = 0; j < getnumelement(); j++)
      {
        v = cvGetReal2D(Valcache, i, j);
        cvSetReal2D(submat, n, j, v);
      }
      n++;
    }
  }
  
  newdata->setdata( submat );
  cvReleaseMat(&submat);
  newdata->statclsdist();
  return newdata;
}
