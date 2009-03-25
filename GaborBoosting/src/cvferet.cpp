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
#include "cvferet.h"

CvFeret::CvFeret()
 : CvFaceDB()
{
  strcpy(name, "FERET");
  pathname = new char[30];
  fa_path = new char[30];
  fb_path = new char[30];
}


CvFeret::~CvFeret()
{
  clear();
}







/*!
    \fn CvFeret::setPath(const char* mainpath, const char*fapath, const char* fbpath)
 */
void CvFeret::setPath(const char* mainpath, const char*fapath, const char* fbpath)
{
  strcpy( this->pathname, mainpath );
  strcpy( this->fa_path, fapath );
  strcpy( this->fb_path, fbpath );
}


/*!
    \fn CvFeret::openPath(const char* pathname)
 */
void CvFeret::openPath(const char* pathname)
{
   // clear the subjects first
  subjects.clear();
  
  CvSubject **templist;
  templist = new CvSubject*[1209];
  for(int i = 1; i <= 1209; i++)
  {
    CvSubject *subject = new CvSubject( i );
    templist[i-1] = subject;

  }
  
  
  DIR *pdir;
  struct dirent *pent;
  
  pdir=opendir( pathname ); //"." refers to the current dir
  if (!pdir)
  {
    perror( pathname );
    exit(-1);
  }
  
  errno=0;
  while ((pent=readdir(pdir)))
  {
    if((strcmp(pent->d_name, "."))&&(strcmp(pent->d_name, "..")))
    {
      string name( pent->d_name );
      string ID = name.substr( 0,5 );
      char *idstring = new char[5];
      strcpy(idstring, ID.c_str());
      int id = atoi( idstring );
      CvSubject *subject = templist[id-1];
      subject->setname( name );
      delete [] idstring;
    }
  }
  
  if (errno){
    printf ("readdir() failure; terminating");
    exit(-1);
  }
  closedir(pdir);
  
  for(int i = 1; i <= 1209; i++)
  {
    CvSubject *subject = templist[i-1];
    subjects.push_back( *subject );
    
    delete subject;
  }
  
  delete [] templist;
  numpic = 0;
  
  
  
  for(int i = 0; i < subjects.size(); i++)
  {
    CvSubject sub = subjects[i];
    numpic = numpic + sub.getnum();
   
    
    numsub = subjects.size();  
   
    if( sub.getnum() > 0 )
    {
      //printf("%d    %d", sub.getId(), sub.getnum());
      for(int i = 0; i < sub.getnum(); i++)
      {
        string name = sub.getname(i);
        //printf("  %s", name.c_str());
      }
      //printf("\n");
    }
    else 
    {
      pos = subjects.begin()+i;
      subjects.erase(pos);
      i--;
    }
    
  }
 
  numsub = subjects.size();
}


/*!
    \fn CvFeret::openPath(const char* faname, const char* fbname)
 */
void CvFeret::openPath(const char* faname, const char* fbname)
{
  openPath( faname );
  opentestingPath( fbname );

  
}


/*!
    \fn CvFeret::CvFeret(const char* mainpath)
 */
 CvFeret::CvFeret(const char* mainpath)
{
  name = new char[10];
  strcpy(name, "FERET");
  pathname = new char[30];
  fa_path = new char[30];
  fb_path = new char[30];
  setPath( mainpath, "", "" );
  calc_stat();
}


/*!
    \fn CvFeret::CvFeret(const char* mainpath, const char* fapath, const char* fbpath)
 */
 CvFeret::CvFeret(const char* mainpath, const char* fapath, const char* fbpath)
{
  name = new char[10];
  strcpy(name, "FERET");
  pathname = new char[30];
  fa_path = new char[30];
  fb_path = new char[30];
  setPath( mainpath, fapath, fbpath );
  openPath( fa_path, fb_path );
  calc_stat();
}


/*!
    \fn CvFeret::CvFeret(const char* mainpath, const char* fapath)
 */
 CvFeret::CvFeret(const char* mainpath, const char* fapath)
{
  name = new char[10];
  strcpy(name, "FERET");
  pathname = new char[30];
  fa_path = new char[30];
  fb_path = new char[30];
  setPath( mainpath, fapath, "" );
  openPath( fa_path );
  calc_stat();
}


/*!
    \fn CvFeret::clear()
 */
void CvFeret::clear()
{
  delete [] name;
  delete [] pathname;
  delete [] fa_path;
  delete [] fb_path;
  subjects.clear();
  testsubjects.clear();
  cvReleaseMat(&fa_distrb);
  cvReleaseMat(&fb_distrb);
  cvReleaseMat(&fa_ID_index);
  cvReleaseMat(&fb_ID_index);
}


/*!
    \fn CvFeret::getSub() const
 */
int CvFeret::getSub() const
{
  return numsub;
}


/*!
    \fn CvFeret::getNum() const
 */
int CvFeret::getNum() const
{
  return numpic;
  
}




/*!
    \fn CvFeret::getSubject(int index) const
 */
CvSubject CvFeret::getSubject(int index) const
{
  CvSubject sub = subjects[index];
  return sub;
}


/*!
    \fn CvFeret::getFApath() const
 */
string CvFeret::getFApath() const
{
  string path(fa_path);
  return path;
}


/*!
    \fn CvFeret::getFBpath() const
 */
string CvFeret::getFBpath() const
{
  string path(fb_path);
  return path;
}


/*!
    \fn CvFeret::openorigin(const char* dvd1path, const char* dvd2path)
 */
void CvFeret::openorigin(const char* dvd1path, const char* dvd2path)
{
  readorigin( dvd1path );
  readorigin( dvd2path );
  numpic = 0;
  for(int i = 0; i < subjects.size(); i++)
  {
    CvSubject sub = getSubject(i);
    printf( "%d :  %d", sub.getId(), sub.getnum() );
    for(int j = 0; j < sub.getnum(); j++)
    {
      string name = sub.getname( j );
      printf("  %s", name.c_str());
    }
    printf("\n");
    numpic += sub.getnum();
  }
  numsub = subjects.size();
}




/*!
    \fn CvFeret::readorigin(const char* path)
 */
void CvFeret::readorigin(const char* path)
{
  DIR *pdir;
  struct dirent *pent;
  
  pdir=opendir( path ); 
  if (!pdir)
  {
    perror( path );
    exit(-1);
  }
  errno=0;
  while ((pent=readdir(pdir)))
  {
    if((strcmp(pent->d_name, "."))&&(strcmp(pent->d_name, "..")))
    {
      string name( pent->d_name );
      char *idstring = new char[10];
      strcpy(idstring, name.c_str());
      int id = atoi( idstring );
      //printf("%d:", id);
      CvSubject *subject = new CvSubject( id );
    
      string subpath = path + name;
    
      readfrmsubjectpath( subpath.c_str(), subject );
      subjects.push_back(*subject);
      delete [] idstring;
    }
  }
  if (errno){
    printf ("readdir() failure; terminating");
    exit(-1);
  }
  closedir(pdir);
}


/*!
    \fn CvFeret::readfrmsubjectpath(const char* subpath, CvSubject *subject)
 */
void CvFeret::readfrmsubjectpath(const char* subpath, CvSubject *subject)
{
  DIR *pdir;
  struct dirent *pent;
  
  pdir = opendir( subpath );
  if(!pdir)
  {
    perror( subpath );
    exit(1);
  }
  errno=0;
  while ((pent=readdir(pdir)))
  {
    if((strcmp(pent->d_name, "."))&&(strcmp(pent->d_name, "..")))
    {
      string imgname( pent->d_name );
      size_t found = imgname.find("fb");
      if( found != string::npos )
      {
        subject->setname(imgname);
        //printf( "  %s", imgname.c_str() );
      }
    }
    
  }
  if (errno){
    printf ("readdir() failure; terminating");
    exit(-1);
  }
  //printf("\n");
  closedir(pdir);
}


/*!
    \fn CvFeret::make(const char* dbpath)
 */
void CvFeret::make(const char* dbpath)
{
  string DBpath(dbpath);
  for(int i = 0; i < subjects.size(); i++)
  {
    CvSubject sub = getSubject(i);
    
    // create a directory
    int id = sub.getId();
    char *subpath = new char[50];
    sprintf(subpath, "%s/%d/", dbpath, id);
    int is = mkdir (subpath, S_IRWXU);
    if(is == -1)
    {
      perror( subpath );
      exit(1);
    }
    
    for(int j = 0; j < sub.getnum(); j++)
    {
      string imgname = sub.getname( j );
      string fapath(fa_path);
      string fullimgname = fapath+"/"+imgname;
      
      
      printf( "Processing %s ... ...\n", fullimgname.c_str() );
      //process40Gabor( fullimgname.c_str(), subpath );
      printf("\n");
    }
    
    
    delete [] subpath;
  }
}


/*!
    \fn CvFeret::process40Gabor( const char* filename, const char* subpath )
 */
/*void CvFeret::process40Gabor( const char* filename, const char* subpath )
{
  IplImage *img = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR);
  IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
  
  cvCvtColor( img, grayimg, CV_RGB2GRAY );
  cvReleaseImage(&img);
  
  
  
  time_t start,end;
  double dif;
  time (&start);
  for (int iNu = -1; iNu <= 3; iNu++)
  {
    for(int iMu = 0; iMu < 8; iMu++)
    {
      CvGabor gabor(iMu, iNu);
      printf( "Gabor scale = %d, orientation = %d ....\n", iNu, iMu );
      IplImage *response = cvCreateImage( cvGetSize(grayimg), IPL_DEPTH_32F, 1 );
      gabor.conv_img( grayimg, response, CV_GABOR_MAG );
      
      // reduce if possible
      IplImage *reimg = reducebyscale( response, iNu );
      cvReleaseImage( &response );
      
      string str(filename);
      size_t found;
      found = str.find_last_of("/\\");
      string str1 = str.substr(found+1);
      found = str1.find_last_of(".");
      string str2 = str1.substr(0, found);
      char *fname = new char[50];
      char *xmlname = new char[50];
      strcpy( fname, str2.c_str() );
      sprintf( xmlname, "%s/%s_%d_%d.xml", subpath, fname, iNu, iMu );
      cvSave( xmlname, reimg, "GaborFeatureData", NULL, cvAttrList());
      delete [] fname;
      delete [] xmlname;
      cvReleaseImage(&reimg);
    }
  }
  
  cvReleaseImage( &grayimg );
  time (&end);
  dif = difftime (end,start);
  printf("It took %.2lf seconds \n", dif);
}
*/

/*!
    \fn CvFeret::reducebyscale(IplImage *src, int Nu)
 */
IplImage* CvFeret::reducebyscale(IplImage *src, int Nu)
{
  if(Nu > -1)
  {
    int div = Nu + 1;
    CvSize size = cvGetSize( src );
    int iwidth = size.width;
    int iheight = size.height;
    
    double dwidth = (double)(iwidth)/pow((double)2, (double)div);
    double dheight = (double)(iheight)/pow((double)2,(double)div);
    int rewidth = (int)ceil(dwidth);
    int reheight = (int)ceil(dheight);
    IplImage * img = cvCreateImage(cvSize(rewidth, reheight), IPL_DEPTH_32F, 1);
    cvResize( src, img, CV_INTER_CUBIC );
    return img;
  }
  else
  {
    IplImage* img = cvCloneImage( src );
    return img;
  }
}


/*!
    \fn CvFeret::getMainpath() const
 */
string CvFeret::getMainpath() const
{
  string path(pathname);
  return path;
}





/*!
    \fn CvFeret::opentestingPath(const char* pathname)
 */
void CvFeret::opentestingPath(const char* pathname)
{
   // clear the subjects first
  testsubjects.clear();
  
  CvSubject **templist;
  templist = new CvSubject*[1209];
  for(int i = 1; i <= 1209; i++)
  {
    CvSubject *subject = new CvSubject( i );
    templist[i-1] = subject;
    
  }
  
  
  DIR *pdir;
  struct dirent *pent;
  
  pdir=opendir( pathname ); //"." refers to the current dir
  if (!pdir)
  {
    perror( pathname );
    exit(-1);
  }
  
  errno=0;
  while ((pent=readdir(pdir)))
  {
    if((strcmp(pent->d_name, "."))&&(strcmp(pent->d_name, "..")))
    {
      string name( pent->d_name );
      string ID = name.substr( 0,5 );
      char *idstring = new char[5];
      strcpy(idstring, ID.c_str());
      int id = atoi( idstring );
      CvSubject *subject = templist[id-1];
      subject->setname( name );
      delete [] idstring;
    }
  }
  
  if (errno){
    printf ("readdir() failure; terminating");
    exit(-1);
  }
  closedir(pdir);
  
  for(int i = 1; i <= 1209; i++)
  {
    CvSubject *subject = templist[i-1];
    testsubjects.push_back( *subject );
    
    delete subject;
  }
  
  delete [] templist;
  testnumpic = 0;
  
  
  
  for(int i = 0; i < testsubjects.size(); i++)
  {
    CvSubject sub = testsubjects[i];
    testnumpic = testnumpic + sub.getnum();

    testnumsub = testsubjects.size();  
    
    if( sub.getnum() > 0 )
    {
      //printf("%d    %d", sub.getId(), sub.getnum());
      for(int i = 0; i < sub.getnum(); i++)
      {
        string name = sub.getname(i);
        //printf("  %s", name.c_str());
      }
      //printf("\n");
    }
    else 
    {
      pos = testsubjects.begin()+i;
      testsubjects.erase(pos);
      i--;
    } 
  }
  testnumsub = testsubjects.size();
}


/*!
    \fn CvFeret::clone() const
 */
CvFeret* CvFeret::clone() const
{
  CvFeret* db = new CvFeret(pathname, fa_path, fb_path);
  return db;
}


/*!
    \fn CvFeret::getFbNum() const
 */
int CvFeret::getFbNum() const
{
  return testnumpic;
}


/*!
    \fn CvFeret::getFbSub() const
 */
int CvFeret::getFbSub() const
{
  return testnumsub;
}


/*!
    \fn CvFeret::getFbSubject(int index) const
 */
CvSubject CvFeret::getFbSubject(int index) const
{
  CvSubject sub = testsubjects[index];
  return sub;
}


/*!
    \fn CvFeret::getFaNum() const
 */
int  CvFeret::getFaNum() const
{
  return getNum();
}


/*!
    \fn CvFeret::getFaSub() const
 */
int CvFeret::getFaSub() const
{
  return getSub();
}


/*!
    \fn CvFeret::getFaSubject(int index) const
 */
CvSubject CvFeret::getFaSubject(int index) const
{
  return getSubject(index);
}


/*!
    \fn CvFeret::calc_stat()
 */
void CvFeret::calc_stat()
{
  int num_fa_sub = getFaSub();
  int sum = 0;
  if(num_fa_sub > 0)
  {
    fa_distrb = cvCreateMat(1, num_fa_sub, CV_32SC1);
    cvSetReal1D(fa_distrb, 0, 0);
    fa_ID_index = cvCreateMat(1, num_fa_sub, CV_32SC1);
    for(int i = 0; i < num_fa_sub; i++)
    {
      CvSubject subject = getFaSubject(i);
      int num_fa_pic = subject.getnum();
      int ID = subject.getId();
      if( i < (num_fa_sub - 1) )
        cvSetReal1D(fa_distrb, i+1, num_fa_pic);
      cvSetReal1D(fa_ID_index, i, ID);
    }
  }
 
  int num_fb_sub = getFbSub();
  if(num_fb_sub > 0)
  {
    fb_distrb = cvCreateMat(1, num_fb_sub, CV_32SC1);
    cvSetReal1D(fb_distrb, 0, 0);
    fb_ID_index = cvCreateMat(1, num_fb_sub, CV_32SC1);
    for(int i = 0; i < num_fb_sub; i++)
    {
      CvSubject subject = getFaSubject(i);
      int num_fb_pic = subject.getnum();
      int ID = subject.getId();
      if( i < (num_fb_sub - 1) )
        cvSetReal1D(fb_distrb, i, num_fb_pic);
      cvSetReal1D(fb_ID_index, i, ID);
    }
  }
}


/*!
    \fn CvFeret::getNumFaSubject(int ID) const
 */
int CvFeret::getNumFaSubject(int ID) const
{
  CvSize size = cvGetSize(fa_ID_index);
  int numsamples = size.width*size.height;
  int client_index;
  for(int i = 0; i < numsamples; i++)
  {
    int id = cvGetReal1D(fa_ID_index, i);
    if(id == ID)
      client_index = i;
  }
  CvSubject subject = getFaSubject(client_index);
  int num_fa_subject = subject.getnum();
  return num_fa_subject;
}


/*!
    \fn CvFeret::getNumFbSubject(int ID) const
 */
int CvFeret::getNumFbSubject(int ID) const
{
  CvSize size = cvGetSize(fb_ID_index);
  int numsamples = size.width*size.height;
  int client_index;
  for(int i = 0; i < numsamples; i++)
  {
    int id = cvGetReal1D(fb_ID_index, i);
    if(id == ID)
      client_index = i;
  }
  CvSubject subject = getFbSubject(client_index);
  int num_fb_subject = subject.getnum();
  return num_fb_subject;
}


/*!
    \fn CvFeret::getPosFaSubject(int ID) const
 */
int CvFeret::getPosFaSubject(int ID) const
{
  CvSize size = cvGetSize(fb_ID_index);
  int numsamples = size.width*size.height;
  int client_index;
  for(int i = 0; i < numsamples; i++)
  {
    int id = cvGetReal1D(fb_ID_index, i);
    if(id == ID)
      client_index = i;
  }
  int pos = cvGetReal1D(fa_distrb, client_index);
  return pos;
}


/*!
    \fn CvFeret::getPosFbSubject(int ID) const
 */
int CvFeret::getPosFbSubject(int ID) const
{
  CvSize size = cvGetSize(fb_ID_index);
  int numsamples = size.width*size.height;
  int client_index;
  for(int i = 0; i < numsamples; i++)
  {
    int id = cvGetReal1D(fb_ID_index, i);
    if(id == ID)
      client_index = i;
  }
  int pos = cvGetReal1D(fb_distrb, client_index);
  return pos;
}


/*!
    \fn CvFeret::getSize() const
 */
CvSize CvFeret::getSize() const
{
  string fapath = getFApath();
  CvSubject subject = getFaSubject( 0 );
  string name = subject.getname( 0 );
  char filename[100];
  sprintf(filename, "%s/%s", fapath.c_str(), name.c_str());
  IplImage * img = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR);
  CvSize size = cvGetSize( img );
  cvReleaseImage(&img);
  
  return size;
}


/*!
    \fn CvFeret::getPosFaSubjectIND(int index) const
 */
int CvFeret::getPosFaSubjectIND(int index) const
{
  int pos = cvGetReal1D(fa_distrb, index);
  return pos;
}


/*!
    \fn CvFeret::getPosFbSubjectIND(int index) const
 */
int CvFeret::getPosFbSubjectIND(int index) const
{
  int pos = cvGetReal1D(fb_distrb, index);
  return pos;
}
