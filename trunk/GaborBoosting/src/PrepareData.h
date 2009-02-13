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
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cvgabor.h"
#include "cvgaborresponsedata.h"


//#define XM2VTS		0
//#define FERET		1




namespace PrepareData {
void preparefolder(const char* pathname, int flag);
void prepareXM2VTSfolder(const char* pathname);
void makeJPEGs(const char* source, const char* destination, int flag);
void makeXM2VTSJPEGs(const char* source, const char* destination);
void makeMetas(const char* source, const char* destination, int flag);
void makeXM2VTSMetas(const char* source, const char* destination);
}


void PrepareData::preparefolder(const char* pathname, int flag)
{
  switch(flag)
  {
  case CvGaborResponseData::XM2VTS:
    {
      prepareXM2VTSfolder(pathname);
      break;
    }
  case CvGaborResponseData::FERET:
    {
      break;
    }
  } 
  
}


void PrepareData::makeJPEGs(const char* source, const char* destination, int flag)
{
  switch(flag)
  {
  case CvGaborResponseData::XM2VTS:
    {
      makeXM2VTSJPEGs(source, destination);
      break;
    }
  case CvGaborResponseData::FERET:
    {
      break;
    }
  } 
}

void PrepareData::makeMetas(const char* source, const char* destination, int flag)
{
  switch(flag)
  {
  case CvGaborResponseData::XM2VTS:
    {
      makeXM2VTSMetas(source, destination);
      break;
    }
  case CvGaborResponseData::FERET:
    {
      break;
    }
  } 
}

void PrepareData::prepareXM2VTSfolder(const char* pathname)
{
  char *subpath = new char[100];
  char *picpath = new char[100];
  char *sub = new char[5];
  char *pic = new char[5];
  int is;
  
  for (int nsub = 1; nsub <= 295; nsub++)
  {
    strcpy (subpath, pathname);
    sprintf(sub, "%d", nsub);
    strcat(subpath, sub);
    strcat(subpath, "/");
    is = chdir(subpath);
    if (is != 0)
    {
      is = mkdir (subpath, S_IRWXU);
      if (is != 0) 
      {
        printf("%s write failed. \n", subpath);
      }
      else
      {
        printf("%s has been created.\n", subpath);
      }
      for (int npic = 1; npic <= 8; npic++)
      {   
        strcpy(picpath, subpath);
        sprintf(pic, "%d", npic);
        strcat(picpath, pic);
        strcat(picpath, "/");
        is = chdir(picpath);
        if ( is != 0)
        {
          is = mkdir (picpath, S_IRWXU);
          if (is != 0)
          { 
            printf("%s write failed.\n", picpath);
          }
          else
          {
            printf("%s has been created.\n", picpath);
          }
        }
      }
    }
  }
  
  delete subpath;
  delete picpath;
  delete sub;
  delete pic; 
}


void PrepareData::makeXM2VTSJPEGs(const char* source, const char* destination)
{
  char *inputfile = new char[100];
  char *outputfile = new char[100];
  char *sub = new char[5];
  char *pic = new char[5];
  char *Mu = new char[5];
  char *Nu = new char[5];
  
  for (int nsub = 1; nsub <= 295; nsub++)
  {
    for (int npic = 1; npic <= 8; npic++)
    {   
      strcpy (inputfile, source);
      sprintf(sub, "%d", nsub);
      strcat(inputfile, sub);
      strcat(inputfile, "_");
      sprintf(pic, "%d", npic);
      strcat(inputfile, pic);
      strcat(inputfile, ".jpg");
      
      IplImage *img = cvLoadImage( inputfile,  CV_LOAD_IMAGE_GRAYSCALE );
      if (img == NULL)
      {
        printf("The image file '%s' can not be read!\n", inputfile);
        break;
      }
      printf("..................The image file '%s' has been read..................\n", inputfile);
      for (int iNu = -3; iNu <= 3; iNu++)
      {
        for(int iMu = 0; iMu < 8; iMu++)
        {
          
          CvGabor gaborfilter(iMu, iNu);
          IplImage *response = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
          gaborfilter.conv_img((IplImage*)img, (IplImage*)response, CV_GABOR_MAG);
          
          strcpy(outputfile, destination);
          strcat(outputfile, sub);
          strcat(outputfile, "/");
          strcat(outputfile, pic);
          sprintf(Nu, "%d", iNu);
          strcat(outputfile, "/");
          strcat(outputfile, Nu);
          sprintf(Mu, "%d", iMu);
          strcat(outputfile, "_");
          strcat(outputfile, Mu);
          strcat(outputfile, ".jpg");
          
          cvSaveImage(outputfile, (IplImage*)response );
          printf("%s has been saved!\n", outputfile); 
          cvReleaseImage(&response);
        }
      }
      cvReleaseImage(&img);
    }
  } 
  
  
  delete sub;
  delete pic;
  delete Mu;
  delete Nu;
  delete inputfile;
  delete outputfile;
}


void PrepareData::makeXM2VTSMetas(const char* source, const char* destination)
{
  char *inputfile = new char[100];
  char *outputfile = new char[100];
  char *sub = new char[5];
  char *pic = new char[5];
  char *Mu = new char[5];
  char *Nu = new char[5];
  
  // Making XML Metafiles
  for (int nsub = 1; nsub <= 295; nsub++)
  {
    for (int npic = 1; npic <= 8; npic++)
    {   
      strcpy (inputfile, source);
      sprintf(sub, "%d", nsub);
      strcat(inputfile, sub);
      strcat(inputfile, "_");
      sprintf(pic, "%d", npic);
      strcat(inputfile, pic);
      strcat(inputfile, ".jpg");
      
      IplImage *img = cvLoadImage( inputfile,  CV_LOAD_IMAGE_GRAYSCALE );
      if (img == NULL)
      {
        printf("The image file '%s' can not be read!\n", inputfile);
        break;
      }
      int width = img->width;
      int height = img->height;
      printf("..................The image file '%s' has been read..................\n", inputfile);
      int NuMin = -1;
      int NuMax = 3;
      int MuMin = 0;
      int MuMax = 8;
      int base;
      for (int iNu = NuMin; iNu <= NuMax; iNu++)
      {
        for(int iMu = MuMin; iMu < MuMax; iMu++)
        {
        /*Create a Gabor*/
          CvGabor gaborfilter(iMu, iNu);
        /*Convolve with Image and mag response*/
          
          IplImage *response = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_32F, 1);
          gaborfilter.conv_img((IplImage*)img, (IplImage*)response, CV_GABOR_MAG);
          
          /*  scale image according to scales  */
          
	/*
	  base = -NuMin;
          double dwidth = (double)width/pow(2,(iNu + base));
          double dheight = (double)height/pow(2,(iNu + base));
          int rewidth = (int)ceil(dwidth);
          int reheight = (int)ceil(dheight);
          IplImage *reduced = cvCreateImage(cvSize(rewidth,reheight), IPL_DEPTH_32F, 1);
          cvResize( response, reduced, CV_INTER_CUBIC );
          */
          strcpy(outputfile, destination);
          strcat(outputfile, sub);
          strcat(outputfile, "/");
          strcat(outputfile, pic);
          sprintf(Nu, "%d", iNu);
          strcat(outputfile, "/");
          strcat(outputfile, Nu);
          sprintf(Mu, "%d", iMu);
          strcat(outputfile, "_");
          strcat(outputfile, Mu);
          strcat(outputfile, ".xml");
          
          //cvSave( outputfile, (IplImage*)reduced, NULL, NULL, cvAttrList(0,0));
          cvSave( outputfile, (IplImage*)response, NULL, NULL, cvAttrList(0,0));
          printf("%s has been saved!\n", outputfile); 
          cvReleaseImage(&response);
          //cvReleaseImage(&reduced);
        }
      }
      cvReleaseImage(&img);
    }
  } 
  
  
  delete sub;
  delete pic;
  delete Mu;
  delete Nu;
  delete inputfile;
  delete outputfile;
}

