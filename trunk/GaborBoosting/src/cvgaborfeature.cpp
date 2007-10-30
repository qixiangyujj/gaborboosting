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
#include "cvgaborfeature.h"

CvGaborFeature::CvGaborFeature()
{
}


CvGaborFeature::~CvGaborFeature()
{
}



/*!
    \fn CvGaborFeature::CvGaborFeature(int x, int y, double Phi, int Nu)
 */
CvGaborFeature::CvGaborFeature(int x, int y, double Phi, int Nu)
{
    Init(x, y, Phi, Nu);

  
}


/*!
    \fn CvGaborFeature::CvGaborFeature(int x, int y, int Mu, int Nu)
 */
CvGaborFeature::CvGaborFeature(int x, int y, int Mu, int Nu)
{
    Init(x, y, Mu, Nu);

}


/*!
    \fn CvGaborFeature::val(IplImage *img)
 */
double CvGaborFeature::val(IplImage *img)
{
    CreateGabor();
    
    double ve;
    
    IplImage *reimg = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_32F, 1);
    gabor->conv_img(img, reimg, CV_GABOR_MAG);
    ve = cvGetReal2D((IplImage*)reimg, iy-1, ix-1);
    cvReleaseImage(&reimg);
    clear();
    return ve;
}


/*!
    \fn CvGaborFeature::Init(int x, int y, double Phi, int Nu)
 */
void CvGaborFeature::Init(int x, int y, double Phi, int Nu)
{
    GaborCreated = false;
    error = 1.0;
    ix = x;
    iy = y;
    iNu = Nu;
    dPhi = Phi;
    isInitialised = true;
}





/*!
    \fn CvGaborFeature::XM2VTSdata(const char *pathname, int subject, int pic)
 */
double CvGaborFeature::XM2VTSdata(const char *pathname, int subject, int pic)
{
    //char filename[80];
    char *filename;
    filename = new char[80];
    //char buff[20];
    char *buff;
    buff = new char[20];
    //memset( buff, '\0', 20 );
    int scale = iNu;
    int orientation = iMu;
    sprintf(buff, "%d/%d/%d_%d.xml", subject, pic, scale, orientation);
    strcpy(filename, pathname);
    strcat(filename, buff);
    double ve;
    
    ve = val(filename);

    delete [] buff;
    delete [] filename;
    

    return ve;
}



/*!
\fn CvGaborFeature::XM2VTSdata(const char *pathname, int subject, int pic, bool reduce)
 */
double CvGaborFeature::XM2VTSdata(const char *pathname, int subject, int pic, bool reduce)
{
    //char filename[80];
  char *filename;
  filename = new char[80];
    //char buff[20];
  char *buff;
  buff = new char[20];
    //memset( buff, '\0', 20 );
  int scale = iNu;
  int orientation = iMu;
  sprintf(buff, "%d/%d/%d_%d.xml", subject, pic, scale, orientation);
  strcpy(filename, pathname);
  strcat(filename, buff);
  double ve;
  
  ve = val(filename, scale);
  
  delete [] buff;
  delete [] filename;
  
  
  return ve;
}



/*!
    \fn CvGaborFeature::clear()
 */
void CvGaborFeature::clear()
{
    if (GaborCreated) delete gabor;
}


/*!
    \fn CvGaborFeature::val(const char *filename)
 */
double CvGaborFeature::val(const char *filename)
{
    double ve;
    CvFileStorage *fs;
    fs = cvOpenFileStorage( filename, NULL, CV_STORAGE_READ );
    if (!fs)
    {
       printf("Error: Can not open %s \n", filename);
       exit(-1);
    }
    char *name;
    name = new char[20];
    extractname(name, filename);
    CvFileNode *node;
    node = cvGetFileNodeByName( fs, 0, name );
    CvFileNode *nodew;
    nodew = cvGetFileNodeByName( fs, node, "width" );
    int width = cvReadInt(nodew);
    CvFileNode *nodeh;
    nodeh = cvGetFileNodeByName( fs, node, "height" );
    int height = cvReadInt(nodeh);

    CvFileNode *nodedata;
    nodedata = cvGetFileNodeByName( fs, node, "data" );
    CvSeq* seq = nodedata->data.seq;
    
    CvSeqReader reader;
    cvStartReadSeq( seq, &reader, 0 );

    int num = (iy-1)*width + ix - 1;
 
    CvFileNode* pt = (CvFileNode*)reader.ptr + num;
    
    ve = pt->data.f;
    delete [] name;
    cvReleaseFileStorage(&fs);
    return ve;
}


/*!
    \fn CvGaborFeature::geterror()
 */
double CvGaborFeature::geterror()
{
    return error;
}


/*!
    \fn CvGaborFeature::seterror(double e)
 */
void CvGaborFeature::seterror(double e)
{
    error = e;
}


/*!
\fn CvGaborFeature::_XM2VTSBin(const char *pathname)
 */
CvTrainingData* CvGaborFeature::_XM2VTSBin(const char *pathname)
{
  int nopic = 8;
  int nosub = 200;
  int numsample = nopic*nosub;
  CvTrainingData *bindata = new CvTrainingData; 
  bindata->init(2, numsample, 1);
  CvMat *mat = cvCreateMat(numsample, 1, CV_32FC1);
  double v;
  int n = 0;
  for (int i = 1; i <= nosub; i++)
  {
    for (int j = 1; j <= nopic; j++)
    {
      
      v = XM2VTSdata(pathname, i, j);
      cvSetReal1D(mat, n, v);

      bindata->setclsidxofsample(1, n);
      n++;
    }
  }
  bindata->setdata(mat);
  cvReleaseMat(&mat);
  bindata->statclsdist();
  return bindata;
}


/*!
    \fn CvGaborFeature::_XM2VTSBin(const char *pathname, bool reduce)
 */
CvTrainingData* CvGaborFeature::_XM2VTSBin(const char *pathname, bool reduce)
{
  int nopic = 8;
  int nosub = 200;
  int numsample = nopic*nosub;
  CvTrainingData *bindata = new CvTrainingData; 
  bindata->init(2, numsample, 1);
  CvMat *mat = cvCreateMat(numsample, 1, CV_32FC1);
  double v;
  int n = 0;
  for (int i = 1; i <= nosub; i++)
  {
    for (int j = 1; j <= nopic; j++)
    {
      
      if(reduce) v = XM2VTSdata( pathname, i, j, reduce );
      else v = XM2VTSdata( pathname, i, j );
      cvSetReal1D( mat, n, v );
      
      bindata->setclsidxofsample( 1, n );
      std::cout << " Go : " << n<< "\r" << std::flush;
      n++;
    }
  }
  cout << endl;
  bindata->setdata(mat);
  cvReleaseMat(&mat);
  bindata->statclsdist();
  return bindata;
}




/*!
    \fn CvGaborFeature::_XM2VTSBin(const char *pathname, int possub)
 */
CvTrainingData* CvGaborFeature::_XM2VTSBin(const char *pathname, int possub)
{
    int nopic = 4;
    int nosub = 200;
    int numsample = nopic*nosub;
    CvTrainingData *bindata = new CvTrainingData; 
    bindata->init(2, numsample, 1);
    CvMat *mat = cvCreateMat(numsample, 1, CV_32FC1);
    double v;
    int n = 0;
    for (int i = 1; i <= nosub; i++)
    {
      for (int j = 1; j <= nopic; j++)
      {
        
        v = XM2VTSdata(pathname, i, j);
        cvSetReal1D(mat, n, v);
        if (i == possub) bindata->setclsidxofsample(1, n);
        else bindata->setclsidxofsample(2, n);
        std::cout << " Go : " << n<< "\r" << std::flush;
        n++;
      }
    }
    cout << endl;
    bindata->setdata(mat);
    cvReleaseMat(&mat);
    bindata->statclsdist();
    return bindata;
}




/*!
    \fn CvGaborFeature::operator<(const CvGaborFeature& a)
 */
bool CvGaborFeature::operator<(const CvGaborFeature& a)
{

    if (this->error < a.error) return true;
    else return false;
}


/*!
    \fn CvGaborFeature::operator>(const CvGaborFeature& a)
 */
bool CvGaborFeature::operator>(const CvGaborFeature& a)
{
    if (this->error > a.error) 
    {
      return true;
    }
    else 
    {return false;}
}


/*!
    \fn CvGaborFeature::clone()
 */
CvGaborFeature* CvGaborFeature::clone()
{
    CvGaborFeature *feature = new CvGaborFeature;
    if(GaborCreated)
    {
      feature->Init(ix, iy, iMu, iNu);
      feature->seterror(error);
    }
    else
    {  
      feature->Init(ix, iy, iMu, iNu);
      feature->seterror(error);
    }
      return feature; 
}


/*!
    \fn CvGaborFeature::Init(int x, int y, int Mu, int Nu)
 */
void CvGaborFeature::Init(int x, int y, int Mu, int Nu)
{
    GaborCreated = false;
    error = 1.0;
    ix = x;
    iy = y;
    iNu = Nu;
    iMu = Mu;
    dPhi = iMu*PI/8;
    isInitialised = true; 
}


/*!
    \fn CvGaborFeature::CreateGabor()
 */
void CvGaborFeature::CreateGabor()
{
    double Sigma = 2*PI;
    double F = sqrt(2.0);
    //CvGabor *tmpgabor = new CvGabor;
    gabor = new CvGabor;
    gabor->Init(dPhi, iNu, Sigma, F);
    //gabor = tmpgabor;
    GaborCreated = true;
}


/*!
    \fn CvGaborFeature::getx()
 */
int CvGaborFeature::getx()
{
    return ix;
}


/*!
    \fn CvGaborFeature::gety()
 */
int CvGaborFeature::gety()
{
    return iy;
}


/*!
    \fn CvGaborFeature::getMu()
 */
int CvGaborFeature::getMu()
{
    return iMu;
}


/*!
    \fn CvGaborFeature::getNu()
 */
int CvGaborFeature::getNu()
{
    return iNu;
}


/*!
    \fn CvGaborFeature::getPhi()
 */
double CvGaborFeature::getPhi()
{
    return dPhi;
}


/*!
    \fn CvGaborFeature::extractname(char * name, const char * filename)
 */
void CvGaborFeature::extractname(char * name, const char * filename)
{
    string str(filename);
    size_t found;
    found = str.find_last_of("/\\");
    string str1 = str.substr(found+1);
    found = str1.find_last_of(".");
    string str2 = str1.substr(0, found);
    str2.insert(0,"_");
  
    const char *test = str2.c_str();
    strcpy(name, test);
}


/*!
    \fn CvGaborFeature::_XM2VTSBin(const char *pathname, int possub, CvMat *index)
 */
CvTrainingData* CvGaborFeature::_XM2VTSBin(const char *pathname, int possub, CvMat *index)
{
    int nopic = 4;
    int nosub = 200; 
    CvSize size = cvGetSize(index);
    int numpos = size.width;
    int numneg = (nosub-1)*4;
    int numsample = numneg + numpos;
    CvTrainingData *bindata = new CvTrainingData; 
    bindata->init(2, numsample, 1);
    CvMat *mat = cvCreateMat(numsample, 1, CV_32FC1);
    double v;
    int n = 0;
    int begin = (possub -1)*4;
    int end = (possub -1)*4+numpos;

    while(n < numsample)
    {
      int i, j;
      if ( n < begin )
      {
	i = (int)(n/4) + 1;
        j = (int)fmod((double)n,(double)4) + 1;
        v = XM2VTSdata(pathname, i, j);
        cvSetReal1D(mat, n, v);
	bindata->setclsidxofsample(2, n);
      }
      else if ((n >= begin) && ( n < end))   //postive samples
      {
	i = possub;
        j = n - begin + 1;
	v = XM2VTSdata(pathname, i, (int)cvGetReal1D(index,(j-1)));
	cvSetReal1D(mat, n, v);
	bindata->setclsidxofsample(1, n);

      }
      else  //negative
      {
	i = (int)((n - end)/4) + possub + 1;
        j = (int)fmod((double)(n-end),(double)4)+1;
	v = XM2VTSdata(pathname, i, j);
        cvSetReal1D(mat, n, v);
	bindata->setclsidxofsample(2, n);

      }
      n++;
    }
    bindata->setdata(mat);
    cvReleaseMat(&mat);
    bindata->statclsdist();
    return bindata;


}




/*!
\fn CvGaborFeature::_XM2VTSBin_F(const char *pathname, int possub, CvMat *index)
 */
CvTrainingData* CvGaborFeature::_XM2VTSBin_F(const char *pathname, int possub, CvMat *index)
{
  int nosub = 200;
  int nopic = 8;
  int ntpic = 4;
  /*    Generate filename   */
  char *filename = new char[100];
  char *ch_scale = new char[5];
  char *ch_orient = new char[5];
  char *ch_name = new char[10];
  strcpy( filename, pathname );
  sprintf( ch_scale, "%d", iNu );
  strcat(filename, ch_scale);
  strcat(filename, "/");
  
  sprintf( ch_orient, "%d", iMu );
  strcat( filename, ch_orient );
  strcat(filename, "/");
  sprintf(ch_name, "%d_%d.xml", ix, iy);
  strcat(filename, ch_name);
  delete [] ch_scale;
  delete [] ch_orient;
  delete [] ch_name;
  /*  Generate filename   */
  
  CvMat* mat = (CvMat*)cvLoad( filename, NULL, NULL, NULL );
   
  CvSize size = cvGetSize( index );
  CvSize size1 = cvGetSize( mat );
  int num = size1.height;
  int numpos = size.width;
  int numneg = ( nosub - 1 )*4;
  int numsample = numneg + numpos;
  CvTrainingData *bindata = new CvTrainingData;
  CvMat* tmpmat = cvCreateMat(numsample, 1, CV_32FC1);
  bindata->init(2, numsample, 1);
  
  int n = 0;
  double v;
  for(int sub = 0; sub < nosub; sub++)
  {
    if(sub == (possub-1))
    {
      for(int i = 0; i < numpos; i++)
      {
        double ind = cvGetReal1D(index, i);
        v = cvGetReal1D(mat, sub*nopic+(int)(ind-1));
        cvSetReal1D(tmpmat, sub*ntpic+i, v);
        bindata->setclsidxofsample( 1, sub*ntpic+i );
        n++;
      }
    }
    else if (sub < (possub -1))
    {
      for(int pic = 0; pic < nopic; pic++) 
      {
        if(pic < 4) 
        {
          v = cvGetReal1D(mat, sub*nopic+pic);
          cvSetReal1D(tmpmat, sub*ntpic+pic, v);
          bindata->setclsidxofsample( 2, sub*ntpic+pic );
        }
        n++;
      }
    }
    else if (sub > (possub -1))
    {
      for(int pic = 0; pic < nopic; pic++) 
      {
        if(pic < 4) 
        {
          v = cvGetReal1D(mat, sub*nopic+pic);
          cvSetReal1D(tmpmat, (sub-1)*ntpic+numpos+pic, v);
          bindata->setclsidxofsample( 2, (sub-1)*ntpic+numpos+pic );
        }
        n++;
      }
    }
  }
  bindata->setdata(tmpmat);
  
  cvReleaseMat(&tmpmat);
  cvReleaseMat(&mat);
  delete [] filename;
  bindata->statclsdist();
  return bindata;
}



/*!
    \fn CvGaborFeature::operator==(const CvGaborFeature& a)
 */
bool CvGaborFeature::operator==(const CvGaborFeature& a)
{
  if((this->ix==a.ix)&&(this->iy==a.iy)&&(this->iMu==a.iMu)&&(this->iNu==a.iNu)) return true;
  else return false;
}


/*!
    \fn CvGaborFeature::_XM2VTSMulti(const char *pathname, CvMat* index)
 */
CvTrainingData* CvGaborFeature::_XM2VTSMulti(const char *pathname, CvMat* index)
{
  CvSize size = cvGetSize(index);
  int npic = size.width;
  int nosub = 200;
  int nsample = nosub*npic;
  int n = 0;
  CvTrainingData *data = new CvTrainingData; 
  data->init(nosub, nsample, 1);
  CvMat *mat = cvCreateMat(nsample, 1, CV_32FC1);
  double v;
  for (int i = 1; i <= nosub; i++)
  {
    for (int j = 1; j <= npic; j++)
    {
      v = XM2VTSdata( pathname, i, (int)cvGetReal1D(index, (j-1)) );
      cvSetReal1D(mat, n, v);
      data->setclsidxofsample(i, n);
      n++;
    }
  }
  data->setdata(mat);
  cvReleaseMat(&mat);
  data->statclsdist();
  return data;
}


/*!
\fn CvGaborFeature::_XM2VTSMulti(const char *pathname, CvMat* picIndex, CvMat* subIndex)
 */
CvTrainingData* CvGaborFeature::_XM2VTSMulti(const char *pathname, CvMat* picIndex, CvMat* subIndex)
{
  CvSize size = cvGetSize(picIndex);
  int npic = size.width;
  size = cvGetSize(subIndex);
  int nosub = size.width;
  int nsample = nosub*npic;
  int n = 0;
  CvTrainingData *data = new CvTrainingData;
  data->init(nosub, nsample, 1);
  CvMat *mat = cvCreateMat(nsample, 1, CV_32FC1);
  double v;

  for (int i = 1; i <= nosub; i++)
  {
    for (int j = 1; j <= npic; j++)
    {
      v = XM2VTSdata( pathname, (int)cvGetReal1D(subIndex, (i-1)), (int)cvGetReal1D(picIndex, (j-1)) );
      cvSetReal1D(mat, n, v);
      data->setclsidxofsample(i, n);
      n++;
    }
  }
  data->setdata(mat);
  cvReleaseMat(&mat);
  data->statclsdist();
  return data; 
}


/*!
\fn CvGaborFeature::_XM2VTSMulti(CvXm2vts *database)
 */
CvTrainingData* CvGaborFeature::_XM2VTSMulti(CvXm2vts *database)
{
  char * pathname = database->getPath();
  CvMat *picIndex = database->getPic();
  CvMat *subIndex = database->getSub();
  CvTrainingData * data = _XM2VTSMulti(pathname, picIndex, subIndex);
  return data;
}


/*!
    \fn CvGaborFeature::operator=(const CvGaborFeature& a)
 */
CvGaborFeature & CvGaborFeature::operator = (const CvGaborFeature& a)
{
  if (this == &a)      // Same object?
    return *this;
  this->dPhi = a.dPhi;
  this->error = a.error;
  this->isInitialised = a.isInitialised;
  this->GaborCreated = a.GaborCreated;
  this->iMu = a.iMu;
  this->iNu = a.iNu;
  this->ix = a.ix;
  this->iy = a.iy;
  if(GaborCreated)
  {
    delete this->gabor;
    this->gabor = new CvGabor(this->dPhi, this->iNu);
  } 
  return *this;
}


/*!
    \fn CvGaborFeature::CvGaborFeature(const CvGaborFeature& f)
 */
 CvGaborFeature::CvGaborFeature(const CvGaborFeature& f)
{
  this->dPhi = f.dPhi;
  this->error = f.error;
  this->isInitialised = f.isInitialised;
  this->GaborCreated = f.GaborCreated;
  this->iMu = f.iMu;
  this->iNu = f.iNu;
  this->ix = f.ix;
  this->iy = f.iy;
  if(GaborCreated) this->gabor = new CvGabor(this->dPhi, this->iNu);
}






/*!
    \fn CvGaborFeature::write(const char* filename)
 */
void CvGaborFeature::write(const char* filename)
{
  string str(filename);
  size_t found;
  found = str.find_last_of(".");
  string ext = str.substr(found+1);
  
  if ((ext.compare("xml")==0)||(ext.compare("XML")==0))  writeXML( filename );
  else if (ext.compare("txt")==0) writeTXT( filename );
  else writeTXT( filename );
}


/*!
    \fn CvGaborFeature::writeXML(const char* filename)
 */
void CvGaborFeature::writeXML(const char* filename)
{
  CvMemStorage* storage = cvCreateMemStorage( 0 );
  CvFileStorage* fs = cvOpenFileStorage( filename, storage, CV_STORAGE_WRITE);
 
  cvStartWriteStruct( fs, "CvGaborFeature",
                      CV_NODE_MAP, NULL,
                      cvAttrList(0,0));
  cvWriteInt(fs, "x", getx());
  cvWriteInt(fs, "y", gety());
  cvWriteInt(fs, "Mu",getMu());
  cvWriteInt(fs, "Nu",getNu());
  cvEndWriteStruct(fs); 
  cvEndWriteStruct( fs );
  
  cvReleaseFileStorage(&fs);
  cvReleaseMemStorage(&storage);
}


/*!
    \fn CvGaborFeature::writeTXT(const char *filename)
 */
void CvGaborFeature::writeTXT(const char *filename)
{
  FILE * file;
  file = fopen (filename,"a");
  
  int x = getx();
  int y = gety();
  int Mu = getMu();
  int Nu = getNu();
  double merror = geterror();
  
  fprintf (file, "%d %d %d %d %f\n",x, y, Nu, Mu, merror);

  fclose(file);
}


/*!
    \fn CvGaborFeature::neighbor(CvGaborFeature *feature, int num)
 */
bool CvGaborFeature::neighbor(CvGaborFeature *feature, int num)
{
  if(fmod((double)num,(double)2) == 0)
  {
    perror("CvGaborFeature::neighbor ");
    printf("The width of neighborhood must be odd! \n");
    exit(-1);
  }
  int x1 = ix;
  int y1 = iy;
  int Mu1 = iMu;
  int Nu1 = iNu;
  int x2 = feature->getx();
  int y2 = feature->gety();
  int Mu2 = feature->getMu();
  int Nu2 = feature->getNu();
  
  int w, minx, miny, maxx, maxy;
  if((Mu1 == Mu2)&&(Nu1 == Nu2))
  {
    w = (num-1)/2;
    minx = x1 - w;
    maxx = x1 + w;
    miny = y1 - w;
    maxy = y1 + w;
    if((x2<=maxx)&&(x2>=minx)&&(y2<=maxy)&&(y2>=miny)) return true;
    else return false;
  }
  else return false;
}



/*!
    \fn CvGaborFeature::_XM2VTSBin_F(const char *pathname, int possub)
 */
CvTrainingData* CvGaborFeature::_XM2VTSBin_F(const char *pathname, int possub)
{
  CvMat *index = cvCreateMat(1, 4, CV_32FC1);
  cvSetReal1D(index,0,1);
  cvSetReal1D(index,1,2);
  cvSetReal1D(index,2,3);
  cvSetReal1D(index,3,4);
  
  CvTrainingData *bindata = _XM2VTSBin_F(pathname, possub, index);
  cvReleaseMat(&index);
  return bindata;
}


/*!
    \fn CvGaborFeature::_XM2VTSMulti_F(CvXm2vts *database)
 */
CvTrainingData* CvGaborFeature::_XM2VTSMulti_F(CvXm2vts *database)
{
  char * pathname = database->getPath();
  CvMat *picIndex = database->getPic();
  CvMat *subIndex = database->getSub();
  CvTrainingData * data = _XM2VTSMulti_F(pathname, picIndex, subIndex);
  return data;
}


/*!
    \fn CvGaborFeature::_XM2VTSMulti_F(const char *pathname, CvMat* picIndex, CvMat* subIndex)
 */
CvTrainingData* CvGaborFeature::_XM2VTSMulti_F(const char *pathname, CvMat* picIndex, CvMat* subIndex)
{
  int nosub = 200;
  int nopic = 8;
  
  /*    Generate filename   */
  char *filename = new char[100];
  char *ch_scale = new char[5];
  char *ch_orient = new char[5];
  char *ch_name = new char[10];
  strcpy( filename, pathname );
  sprintf( ch_scale, "%d", iNu );
  strcat(filename, ch_scale);
  strcat(filename, "/");
  
  sprintf( ch_orient, "%d", iMu );
  strcat( filename, ch_orient );
  strcat(filename, "/");
  sprintf(ch_name, "%d_%d.xml", ix, iy);
  strcat(filename, ch_name);
  delete [] ch_scale;
  delete [] ch_orient;
  delete [] ch_name;
  /*  Generate filename   */
  
  CvMat* mat = (CvMat*)cvLoad( filename, NULL, NULL, NULL );
  //printf("Open %s \n", filename);
  //assert( mat );
  if(!CV_IS_MAT( mat )) 
  {
    //retry 10 times
    for(int i = 0; i < 10; i++)
    {
      time_t rawtime;
      struct tm * timeinfo;
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      perror( asctime (timeinfo) );
      perror("Can not get a matrix from ");
      perror(filename);
      sleep( 10 );   // sleep for 10 sec
      mat = (CvMat*)cvLoad( filename, NULL, NULL, NULL );
      if (CV_IS_MAT( mat )) break;
    }
  }
  CvSize size = cvGetSize( picIndex );
  CvSize size1 = cvGetSize( mat );
  CvSize size2 = cvGetSize(subIndex);
  
  int num = size1.height;
  int ntpic = size.width;
  int ntsub = size2.width;
  int numsample = ntsub*ntpic;
  
  CvTrainingData *bindata = new CvTrainingData;
  CvMat* tmpmat = cvCreateMat(numsample, 1, CV_32FC1);
  bindata->init(ntsub, numsample, 1);
  
  int n = 0;
  int m = 0;
  double v;
  for(int sub = 1; sub <= nosub; sub++)
  {
    for(int pic = 1; pic <= nopic; pic++)
    {
      for(int j = 0; j < ntsub; j++)
      {
        for(int i = 0; i < ntpic; i++)
        {
          if((pic == cvGetReal1D(picIndex, i))&&(sub == cvGetReal1D(subIndex, j))) 
          {
            v = cvGetReal1D(mat, n);
            cvSetReal1D(tmpmat, m, v);
            bindata->setclsidxofsample(sub, m);
            m++;
          }
        }
      }
      n++;
    }
  }
  bindata->setdata(tmpmat);
  
  cvReleaseMat(&tmpmat);
  cvReleaseMat(&mat);
  delete [] filename;
  bindata->statclsdist();
  return bindata;
}


/*!
    \fn CvGaborFeature::_XM2VTSMulti_F(const char *pathname, CvMat* index)
 */
CvTrainingData* CvGaborFeature::_XM2VTSMulti_F(const char *pathname, CvMat* index)
{
  int nosub = 200;
  int nopic = 8;
  
  /*    Generate filename   */
  char *filename = new char[100];
  char *ch_scale = new char[5];
  char *ch_orient = new char[5];
  char *ch_name = new char[10];
  strcpy( filename, pathname );
  sprintf( ch_scale, "%d", iNu );
  strcat(filename, ch_scale);
  strcat(filename, "/");
  
  sprintf( ch_orient, "%d", iMu );
  strcat( filename, ch_orient );
  strcat(filename, "/");
  sprintf(ch_name, "%d_%d.xml", ix, iy);
  strcat(filename, ch_name);
  delete [] ch_scale;
  delete [] ch_orient;
  delete [] ch_name;
  /*  Generate filename   */
  
  CvMat* mat = (CvMat*)cvLoad( filename, NULL, NULL, NULL );
  
  CvSize size = cvGetSize( index );
  CvSize size1 = cvGetSize( mat );
  int num = size1.height;
  int ntpic = size.width;
  int numsample = nosub*ntpic;
  
  CvTrainingData *bindata = new CvTrainingData;
  CvMat* tmpmat = cvCreateMat(numsample, 1, CV_32FC1);
  bindata->init(nosub, numsample, 1);
  
  int n = 0;
  int m = 0;
  double v;
  for(int sub = 1; sub <= nosub; sub++)
  {
    for(int pic = 1; pic <= nopic; pic++)
    {
      for(int i = 0; i < ntpic; i++)
      {
        if(pic == cvGetReal1D(index, i)) 
        {
          v = cvGetReal1D(mat, n);
          cvSetReal1D(tmpmat, m, v);
          bindata->setclsidxofsample(sub, m);
          m++;
        }
      }
      n++;
    }
  }
  bindata->setdata(tmpmat);
  
  cvReleaseMat(&tmpmat);
  cvReleaseMat(&mat);
  delete [] filename;
  bindata->statclsdist();
  return bindata;
}





/*!
    \fn CvGaborFeature::val(const char *filename, int scale)
 */
double CvGaborFeature::val(const char *filename, int scale)
{
  IplImage * img = (IplImage*)cvLoad(filename, NULL, NULL, NULL);
  CvSize size = cvGetSize( img );
  int width = size.width;
  int height = size.height;
  
  int div = scale + 1;
  double dwidth = (double)(width)/pow((double)2, (double)div);
  double dheight = (double)(height)/pow((double)2,(double)div);
  int rewidth = (int)ceil(dwidth);
  int reheight = (int)ceil(dheight);
  IplImage * reimg = cvCreateImage(cvSize(rewidth, reheight), IPL_DEPTH_32F, 1);
  cvResize( img, reimg, CV_INTER_CUBIC );
  double ve = cvGetReal2D((IplImage*)reimg, iy-1, ix-1);
  cvReleaseImage( &reimg );
  cvReleaseImage( &img );
  return ve;
}


/*!
    \fn CvGaborFeature::val(IplImage* img, int scale)
 */
double CvGaborFeature::val(IplImage* img, int scale)
{
  CvSize size = cvGetSize( img );
  int width = size.width;
  int height = size.height;
  
  int div = scale + 1;
  double dwidth = (double)(width)/pow((double)2, (double)div);
  double dheight = (double)(height)/pow((double)2,(double)div);
  int rewidth = (int)ceil(dwidth);
  int reheight = (int)ceil(dheight);
  IplImage * reimg = cvCreateImage(cvSize(rewidth, reheight), IPL_DEPTH_32F, 1);
  cvResize( img, reimg, CV_INTER_CUBIC );
  double ve = val(reimg);
  cvReleaseImage( &reimg );
  cvReleaseImage( &img );
  return ve;
}


/*!
    \fn CvGaborFeature::_FERETBin(CvFeret* feret, CvPoolParams* param)
 */
CvTrainingData* CvGaborFeature::_FERETBin(CvFeret* feret, CvPoolParams* param )
{
  int numsample = feret->getNum();
  CvTrainingData *bindata = new CvTrainingData; 
  bindata->init(2, numsample, 1);
  CvMat *mat = cvCreateMat(numsample, 1, CV_32FC1);
  double v;
  int n = 0;
  string mainpath = feret->getMainpath();
  for (int i = 0; i < feret->getSub(); i++)
  {
    CvSubject sub = feret->getSubject(i);
    for (int j = 0; j < sub.getnum(); j++)
    {
      
      string imgname = sub.getname(j);;
      int id = sub.getId();
      if( param->isReduced() ) v = FERETdata( mainpath.c_str(), id, imgname.c_str(), param->isReduced() );
      else v = FERETdata( mainpath.c_str(), id, imgname.c_str());
      cvSetReal1D( mat, n, v );
      
      bindata->setclsidxofsample( 1, n );
      std::cout << " Go : " << n<< "\r" << std::flush;
      n++;
    }
  }
  cout << endl;
  bindata->setdata(mat);
  cvReleaseMat(&mat);
  bindata->statclsdist();
  return bindata;
}




/*!
    \fn CvGaborFeature::_FERETBin(const char *pathname, bool reduce)
 */
CvTrainingData* CvGaborFeature::_FERETBin(const char *pathname, bool reduce)
{
    /// @todo implement me
}


/*!
    \fn CvGaborFeature::FERETdata(const char *pathname, int subject, const char* imgname, bool reduce)
 */
double CvGaborFeature::FERETdata(const char *pathname, int subject, const char* imgname, bool reduce)
{
 
  char *filename = new char[50];
  
  string str(imgname);
  size_t found = str.find_last_of(".");
  string shortname = str.substr(0, found);
  
  sprintf(filename, "%s/%d/%s_%d_%d.xml", pathname, subject, shortname.c_str(), iNu, iMu);
  
  double ve;
  int scale = iNu;
  ve = val(filename, -1);
 
  delete [] filename;
  
  
  return ve;
}


/*!
    \fn CvGaborFeature::FERETdata(const char *pathname, int subject, const char* imgname)
 */
double CvGaborFeature::FERETdata(const char *pathname, int subject, const char* imgname)
{
    /// @todo implement me
}




/*!
    \fn CvGaborFeature::_FERETBin_F(CvFeret* feret, int possub, CvMat *index)
 */
CvTrainingData* CvGaborFeature::_FERETBin_F(CvFeret* feret, int possub, CvMat *index)
{
  int nosub = feret->getSub();
  int nsamples = feret->getNum();
  /*    Generate filename   */
  string path = feret->getMainpath();
  char * filename = new char[50];
  sprintf(filename, "%s/%d/%d/%d_%d.xml", path.c_str(), iNu, iMu, ix, iy);
  
  
  CvMat* mat = (CvMat*)cvLoad( filename, NULL, NULL, NULL );
  //cvTranspose( mat, mat );
  CvTrainingData *bindata = new CvTrainingData;
  bindata->init(2, nsamples, 1);
  bindata->setdata( mat );
  
  for(int i = 0; i < nsamples; i++)
  {
    int id = (int)cvGetReal1D(index, i);
    if(id == possub) bindata->setclsidxofsample( 1, i);
    else bindata->setclsidxofsample( 2, i);
  }
  bindata->statclsdist();
  delete [] filename;
  return bindata;
}
