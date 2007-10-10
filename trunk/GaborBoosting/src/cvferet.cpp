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
      printf("%d    %d", sub.getId(), sub.getnum());
      for(int i = 0; i < sub.getnum(); i++)
      {
        string name = sub.getname(i);
        printf("  %s", name.c_str());
      }
      printf("\n");
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
  
  
  // input FA part
  pdir=opendir( faname ); 
  if (!pdir)
  {
    perror( faname );
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
  
  
   // input FB part
  pdir=opendir( fbname ); 
  if (!pdir)
  {
    perror( fbname );
    exit(-1);
  }
  errno=0;
  while ((pent=readdir(pdir)))
  {
    if((strcmp(pent->d_name, "."))&&(strcmp(pent->d_name, "..")))
    {
      string name( pent->d_name );
      //printf("%s\n", pent->d_name);
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
    if( sub.getnum() > 0 )
    {
      string name = sub.getname(0);
      //printf("%d    %d    %s\n", sub.getId(), sub.getnum(), name.c_str());
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
    \fn CvFeret::CvFeret(const char* mainpath)
 */
 CvFeret::CvFeret(const char* mainpath)
{
  pathname = new char[30];
  fa_path = new char[30];
  fb_path = new char[30];
  setPath( mainpath, "", "" );
}


/*!
    \fn CvFeret::CvFeret(const char* mainpath, const char* fapath, const char* fbpath)
 */
 CvFeret::CvFeret(const char* mainpath, const char* fapath, const char* fbpath)
{
  pathname = new char[30];
  fa_path = new char[30];
  fb_path = new char[30];
  setPath( mainpath, fapath, fbpath );
  openPath( fa_path, fb_path );
}


/*!
    \fn CvFeret::CvFeret(const char* mainpath, const char* fapath)
 */
 CvFeret::CvFeret(const char* mainpath, const char* fapath)
{
  pathname = new char[30];
  fa_path = new char[30];
  fb_path = new char[30];
  setPath( mainpath, fapath, "" );
  openPath( fa_path );
}


/*!
    \fn CvFeret::clear()
 */
void CvFeret::clear()
{
  delete [] pathname;
  delete [] fa_path;
  delete [] fb_path;
  subjects.clear();
}


/*!
    \fn CvFeret::getSub()
 */
int CvFeret::getSub()
{
  return numsub;
}


/*!
    \fn CvFeret::getNum()
 */
int CvFeret::getNum()
{
  return numpic;
  
}




/*!
    \fn CvFeret::getSubject(int index)
 */
CvSubject CvFeret::getSubject(int index)
{
  CvSubject sub = subjects[index];
  return sub;
}


/*!
    \fn CvFeret::getFApath()
 */
string CvFeret::getFApath()
{
  string path(fa_path);
  return path;
}


/*!
    \fn CvFeret::getFBpath()
 */
string CvFeret::getFBpath()
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
    \fn CvFeret::getMainpath()
 */
string CvFeret::getMainpath()
{
  string path(pathname);
  return path;
}
