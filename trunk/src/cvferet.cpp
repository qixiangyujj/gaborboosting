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
  for(int i = 1; i <= 1209; i++)
  {
    CvSubject sub = subjects[i-1];
    numpic = numpic + sub.getnum();
    if( sub.getnum() > 0 )
    {
      string name = sub.getname(0);
      
      printf("%d    %d    %s\n", sub.getId(), sub.getnum(), name.c_str());
    }
  }
 
  
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
  for(int i = 1; i <= subjects.size(); i++)
  {
    CvSubject sub = subjects[i-1];
    numpic = numpic + sub.getnum();
    if( sub.getnum() > 0 )
    {
      string name = sub.getname(0);
      
      printf("%d    %d    %s\n", sub.getId(), sub.getnum(), name.c_str());
    }
  }
  numsub = subjects.size();
  //printf("%d    %d\n", numsub, numpic);
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
CvSubject& CvFeret::getSubject(int index)
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
