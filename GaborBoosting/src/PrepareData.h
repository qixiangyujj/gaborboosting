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
#include <cvferet.h>


//#define XM2VTS		0
//#define FERET		1




namespace PrepareData {
void preparefolder(const char* pathname, int flag);
void prepareXM2VTSfolder(const char* pathname);
void makeJPEGs(const char* source, const char* destination, int flag);
void makeXM2VTSJPEGs(const char* source, const char* destination);
void makeMetas(const char* source, const char* destination, int flag);
void makeXM2VTSMetas(const char* source, const char* destination);
void prepareFERETfolder(const char * pathname);
void processGabors(const char * filename);
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
      prepareFERETfolder(pathname);
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

void PrepareData::prepareFERETfolder(const char * pathname)
{
  char * fa_path = new char[100];
  char * fb_path = new char[100];
  
  sprintf(fa_path, "%s/FA", pathname);
  sprintf(fb_path, "%s/FB", pathname);
  int is;
  
  
  const char * fapathname = "/home/sir02mz/FERET/FA/";
  const char * fbpathname = "/home/sir02mz/FERET/FB/";
  CvFeret feret( "", fapathname, fbpathname);
  
  
  //make FA directory
  
//   is = mkdir (fa_path, S_IRWXU);
//   if( is )
//   {
//     printf("ERROR: can not make %s\n", fa_path);
//     exit(-1);
//   }
//   is = chdir(fa_path);
//   int num_fa = feret.getSub();
//   for(int i = 0; i < num_fa; i++)
//   {
//     CvSubject subject = feret.getSubject(i);
//     int num_pic = subject.getnum();
//     for(int j = 0; j < num_pic; j++)
//     {
//       string filename = subject.getname(j);
//       char * filenamepath = new char[100];
//       sprintf( filenamepath, "%s/%s",fa_path, filename.c_str());
//       is = chdir( filenamepath );
//       if (is != 0)
//       {
//         is = mkdir(filenamepath, S_IRWXU);
//         if( is )
//         {
//           printf("ERROR: can not make %s\n", filenamepath);
//           exit(-1);
//         }
//         chdir(filenamepath);
//         char * imgfilename = new char[100];
//         sprintf(imgfilename, "%s/%s", fapathname, filename.c_str() );
//         processGabors( imgfilename);
//         delete [] imgfilename;
//       }
//         
//       delete [] filenamepath;
//     }
//   }

  //make FB directory
  is = mkdir (fb_path, S_IRWXU);
  if( is )
  {
    printf("ERROR: can not make %s\n", fa_path);
    exit(-1);
  }
  is = chdir(fb_path);
  int num_fb = feret.getFbSub();
  for(int i = 0; i < num_fb; i++)
  {
    CvSubject subject = feret.getFbSubject(i);
    int num_pic = subject.getnum();
    for(int j = 0; j < num_pic; j++)
    {
      string filename = subject.getname(j);
      char * filenamepath = new char[100];
      sprintf( filenamepath, "%s/%s",fb_path, filename.c_str());
      is = chdir( filenamepath );
      if (is != 0)
      {
        is = mkdir(filenamepath, S_IRWXU);
        if( is )
        {
          printf("ERROR: can not make %s\n", filenamepath);
          exit(-1);
        }
        chdir(filenamepath);
        char * imgfilename = new char[100];
        sprintf(imgfilename, "%s/%s", fbpathname, filename.c_str() );
        processGabors( imgfilename);
        delete [] imgfilename;
      }
        
      delete [] filenamepath;
    }
  }
  
  
  delete [] fa_path;
  delete [] fb_path;
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

void PrepareData::processGabors(const char * filename)
{
  IplImage *img = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR);
  
  if(img->nChannels == 3)
  {
    IplImage *grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvCvtColor( img, grayimg, CV_RGB2GRAY );
    cvReleaseImage(&img);
    img = grayimg;
  }
  
  for (int iNu = -1; iNu <= 3; iNu++)
  {
    for(int iMu = 0; iMu < 8; iMu++)
    {
      CvGabor gabor(iMu, iNu);
      IplImage *response = cvCreateImage( cvGetSize(img), IPL_DEPTH_32F, 1 );
      gabor.conv_img( img, response, CV_GABOR_MAG );
      
      char * xmlname = new char[100];
      sprintf(xmlname, "%d_%d.xml", iNu, iMu);
      cvSave( xmlname, response, NULL, NULL, cvAttrList(0,0));
      printf("Write %s %s\n", filename, xmlname);
      cvReleaseImage(&response);
      delete [] xmlname;
    }
  }

  cvReleaseImage(&img);
}

