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
#include "cvxm2vts.h"

CvXm2vts::CvXm2vts()
 : CvFaceDB()
{
  isSub = false;
  isPic = false;
  name = new char[10];
  pathname = new char[50];
  strcpy(name, "XM2VTS");
  strcpy(pathname, "/mnt/export/xm2vts_feature/"); 
  setNumPic(MAXPIC);
  setNumSub(200);
  isGender = false;
}


CvXm2vts::~CvXm2vts()
{
  clear();
}




/*!
    \fn CvXm2vts::CvXm2vts(const char* where)
 */
 CvXm2vts::CvXm2vts(const char* where)
{
  isSub = false;
  isPic = false;
  name = new char[10];
  pathname = new char[50];
  strcpy(name, "XM2VTS");
  strcpy(pathname, where);
  setNumPic(MAXPIC);
  setNumSub(200);
  isGender = false;
}


/*!
    \fn CvXm2vts::setNumSub(int s)
 */
void CvXm2vts::setNumSub(int s)
{
  if(isSub) cvReleaseMat(&subIndex);
  if(s > MAXSUB)
  {
    perror("CvXm2vts::setNumSub ");
    printf("The number of subjects should not exceed %d.\n", MAXSUB);
    exit(-1);
  }
  numsub = s;
  
  subIndex = cvCreateMat(1, numsub, CV_32SC1);
  for(int i = 1; i <= numsub; i++)
  {
    cvSetReal1D(subIndex, (i-1), i);
    isSub = true;
  }
}


/*!
    \fn CvXm2vts::setNumPic(int p)
 */
void CvXm2vts::setNumPic(int p)
{
  if(isPic) cvReleaseMat(&picIndex);
  if((p <= MAXPIC)&&(p > 0))
  {
    numpic = p;
    picIndex = cvCreateMat(1, numpic, CV_32SC1);
    for(int i = 1; i <= numpic; i++)
    {
      cvSetReal1D(picIndex, (i-1), i);
      isPic = true;
    }
  }
  else
  {
    perror("CvXm2vts::setNumPic ");
    printf("The number of picture per subject should not be exceed %d.\n", MAXPIC);
    exit(-1);
  }
}


/*!
    \fn CvXm2vts::setPicIndex(int i1, int i2, int i3, int i4, int i5, int i6, int i7)
 */
void CvXm2vts::setPicIndex(int i1, int i2, int i3, int i4, int i5, int i6, int i7)
{
  if(picIndex!= NULL) cvReleaseMat(&picIndex);
  numpic = 7;
  picIndex = cvCreateMat(1, 7, CV_32SC1);
 
  cvSetReal1D(picIndex, 0, i1);
  cvSetReal1D(picIndex, 1, i2);
  cvSetReal1D(picIndex, 2, i3);
  cvSetReal1D(picIndex, 3, i4);
  cvSetReal1D(picIndex, 4, i5);
  cvSetReal1D(picIndex, 5, i6);
  cvSetReal1D(picIndex, 6, i7);
}


/*!
    \fn CvXm2vts::setPicIndex(int i1, int i2, int i3, int i4, int i5, int i6)
 */
void CvXm2vts::setPicIndex(int i1, int i2, int i3, int i4, int i5, int i6)
{
  if(picIndex!= NULL) cvReleaseMat(&picIndex);
  numpic = 6;
  picIndex = cvCreateMat(1, 6, CV_32SC1);
  
  cvSetReal1D(picIndex, 0, i1);
  cvSetReal1D(picIndex, 1, i2);
  cvSetReal1D(picIndex, 2, i3);
  cvSetReal1D(picIndex, 3, i4);
  cvSetReal1D(picIndex, 4, i5);
  cvSetReal1D(picIndex, 5, i6);
}


/*!
    \fn CvXm2vts::setPicIndex(int i1, int i2, int i3, int i4, int i5)
 */
void CvXm2vts::setPicIndex(int i1, int i2, int i3, int i4, int i5)
{
  if(picIndex!= NULL) cvReleaseMat(&picIndex);
  numpic = 5;
  picIndex = cvCreateMat(1, 5, CV_32SC1);
  
  cvSetReal1D(picIndex, 0, i1);
  cvSetReal1D(picIndex, 1, i2);
  cvSetReal1D(picIndex, 2, i3);
  cvSetReal1D(picIndex, 3, i4);
  cvSetReal1D(picIndex, 4, i5);
}


/*!
    \fn CvXm2vts::setPicIndex(int i1, int i2, int i3, int i4)
 */
void CvXm2vts::setPicIndex(int i1, int i2, int i3, int i4)
{
  if(picIndex!= NULL) cvReleaseMat(&picIndex);
  numpic = 4;
  picIndex = cvCreateMat(1, 4, CV_32SC1);
  
  cvSetReal1D(picIndex, 0, i1);
  cvSetReal1D(picIndex, 1, i2);
  cvSetReal1D(picIndex, 2, i3);
  cvSetReal1D(picIndex, 3, i4);
}


/*!
    \fn CvXm2vts::setPicIndex(int i1, int i2, int i3)
 */
void CvXm2vts::setPicIndex(int i1, int i2, int i3)
{
  if(picIndex!= NULL) cvReleaseMat(&picIndex);
  numpic = 3;
  picIndex = cvCreateMat(1, 3, CV_32SC1);
  
  cvSetReal1D(picIndex, 0, i1);
  cvSetReal1D(picIndex, 1, i2);
  cvSetReal1D(picIndex, 2, i3);

}


/*!
    \fn CvXm2vts::setPicIndex(int i1, int i2)
 */
void CvXm2vts::setPicIndex(int i1, int i2)
{
  if(picIndex!= NULL) cvReleaseMat(&picIndex);
  numpic = 2;
  picIndex = cvCreateMat(1, 2, CV_32SC1);
  
  cvSetReal1D(picIndex, 0, i1);
  cvSetReal1D(picIndex, 1, i2);

}


/*!
    \fn CvXm2vts::setPicIndex(int i1)
 */
void CvXm2vts::setPicIndex(int i1)
{
  if(picIndex!= NULL) cvReleaseMat(&picIndex);
  numpic = 1;
  picIndex = cvCreateMat(1, 1, CV_32SC1);
  
  cvSetReal1D(picIndex, 0, i1);
}


/*!
    \fn CvXm2vts::setPicIndex(CvMat* mat)
 */
void CvXm2vts::setPicIndex(CvMat* mat)
{
  if(picIndex!= NULL) cvReleaseMat(&picIndex);
  picIndex = cvCloneMat(mat);
  CvSize size = cvGetSize(mat);
  numpic = size.width;
}


/*!
    \fn CvXm2vts::setSubIndex(CvMat* mat)
 */
void CvXm2vts::setSubIndex(CvMat* mat)
{
  if(subIndex!= NULL) cvReleaseMat(&subIndex);
  subIndex = cvCloneMat(mat);
  CvSize size = cvGetSize(mat);
  numsub = size.width;
}


/*!
    \fn CvXm2vts::clear()
 */
void CvXm2vts::clear()
{
  delete [] name;
  delete [] pathname;
  if( subIndex!=NULL ) 
    cvReleaseMat( &subIndex );
  if( picIndex!=NULL ) 
    cvReleaseMat( &picIndex );
  if( isGender ) 
    cvReleaseMat( &gender );
}


/*!
    \fn CvXm2vts::getPath()
 */
char* CvXm2vts::getPath()
{
  return (char*)pathname; 
}


/*!
    \fn CvXm2vts::getSub()
 */
CvMat* CvXm2vts::getSub()
{
  return subIndex;
}


/*!
    \fn CvXm2vts::getPic()
 */
CvMat* CvXm2vts::getPic()
{
  return picIndex;
}


/*!
    \fn CvXm2vts::CvXm2vts(const CvXm2vts& a)
 */
 CvXm2vts::CvXm2vts(const CvXm2vts& a)
{
  this->numpic = a.numpic;
  this->numsub = a.numsub;
  this->name = new char[10];
  this->pathname = new char[50];
  strcpy(this->name, a.name);
  strcpy(this->pathname, a.pathname);
  this->picIndex = cvCloneMat(a.picIndex);
  this->subIndex = cvCloneMat(a.subIndex);

}


/*!
    \fn CvXm2vts::clone()
 */
CvXm2vts* CvXm2vts::clone()
{
  CvXm2vts *database= new CvXm2vts;
  strcpy( database->pathname, this->pathname);
  database->numpic = this->numpic;
  database->numsub = this->numsub;
  if( database->picIndex != NULL ) cvReleaseMat(&(database->picIndex));
  if( database->subIndex != NULL ) cvReleaseMat(&(database->subIndex));
  database->picIndex = cvCloneMat(this->picIndex);
  database->subIndex = cvCloneMat(this->subIndex);
  database->isGender = isGender;
  if(this->isGender)database->gender = cvCloneMat(this->gender);
  return database;
}


/*!
    \fn CvXm2vts::get_num_pic()
 */
int CvXm2vts::get_num_pic()
{
  return numpic;
}


/*!
    \fn CvXm2vts::get_num_sub()
 */
int CvXm2vts::get_num_sub()
{
  return numsub;
}


/*!
    \fn CvXm2vts::setGender(const char* filename)
 */
void CvXm2vts::setGender(const char* filename)
{
  if (isGender) cvReleaseMat( &gender );
  
  gender = cvCreateMat( 1, 295, CV_32FC1 );
  isGender = true;
  FILE * file;
  if ((file=fopen(filename,"r")) == NULL)
  {
    printf("Cannot read file %s.\n", filename);
    exit(1);
  }
  
  int nosub;
  char *genderchar = new char[20];
  char malekey[] = "M";
  char femalekey[] = "F";
  while (!feof(file))
  {
    if (fscanf(file, "%d", &nosub) == EOF) break;
    if (nosub > 200) break;
    if (fscanf(file, ",%s\n", genderchar) == EOF) break;
    char * pch;
    pch = strpbrk (genderchar, malekey);
    if (pch != NULL)       // male
    {
      cvSetReal1D( gender, nosub-1, 1.0 );
      //printf( "%d:M\n", nosub );
    }
    else           //female
    {
      cvSetReal1D( gender, nosub-1, 2.0);
      //printf( "%d:F\n", nosub );
    }
  }
  delete [] genderchar;
  fclose(file);
}


/*!
    \fn CvXm2vts::getGender(int nsub)
 */
bool CvXm2vts::getGender(int nsub)
{
  float v = cvGetReal1D( gender, nsub-1);
  if ( v == 1.0 ) return true;
  else return false;
}


/*!
    \fn CvXm2vts::getfilename(int sub, int pic, char *filename)
 */
void CvXm2vts::getfilename(int sub, int pic, char *filename)
{
   sprintf( filename, "%s/%d_%d.jpg", pathname, sub, pic );
}


/*!
    \fn CvXm2vts::getSize()
 */
CvSize CvXm2vts::getSize()
{
    char filename[200];
    getfilename( 1, 1, filename );
    IplImage *img = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE );
    CvSize size;
    size = cvGetSize( img );
    cvReleaseImage( &img );
    return size;
}
