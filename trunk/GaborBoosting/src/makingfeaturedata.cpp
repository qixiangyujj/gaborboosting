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
#include "makingfeaturedata.h"

namespace PrepareData {

MakingFeatureData::MakingFeatureData()
{
}


MakingFeatureData::~MakingFeatureData()
{
  clear();
}


}

/*!
\fn PrepareData::MakingFeatureData::MakingFeatureData(CvPoolParams *params, const char* pathname, const char*depathname)
 */
PrepareData::MakingFeatureData::MakingFeatureData(CvPoolParams *params, const char* pathname, const char*depathname)
{
  source = new char[50];
  destiny = new char[50];
  strcpy(source, pathname);
  strcpy(destiny, depathname);
  CvSize size = params->getImgSize();
  this->width = size.width;
  this->height = size.height;
  CvMat* matOrient = params->getOrients();
  CvMat* matScales = params->getScales();
  // To do 
  this->maxscale = 3;
  this->minscale = -1;
  this->orients = 8;
  init(params);
}


/*!
\fn PrepareData::MakingFeatureData::MakingFeatureData(int height, int width, int minscale, int maxscale, int orients, const char* pathname, const char*depathname, bool reduce)
 */
PrepareData::MakingFeatureData::MakingFeatureData(int height, int width, int minscale, int maxscale, int orients, const char* pathname, const char*depathname, bool reduce)
{
  source = new char[50];
  destiny = new char[50];
  strcpy(source, pathname);
  strcpy(destiny, depathname);
  this->height = height;
  this->width = width;
  this->minscale = minscale;
  this->maxscale = maxscale;
  this->orients = orients;
  
  CvPoolParams params(height, width, minscale, maxscale, orients, 0, 0, reduce);
  init(&params);
}



/*!
\fn PrepareData::MakingFeatureData::MakingFeatureData(int height, int width, int minscale, int maxscale, int orients, const char* pathname, const char*depathname)
 */
PrepareData::MakingFeatureData::MakingFeatureData(int height, int width, 
  int minscale, int maxscale, 
  int orients, 
  const char* pathname, const char*depathname)
{
  source = new char[50];
  destiny = new char[50];
  strcpy(source, pathname);
  strcpy(destiny, depathname);
  this->height = height;
  this->width = width;
  this->minscale = minscale;
  this->maxscale = maxscale;
  this->orients = orients;
  CvPoolParams params(height, width, minscale, maxscale, orients, 0, 0);
  init(&params);
}





/*!
\fn PrepareData::MakingFeatureData::init(CvPoolParams *params)
 */
void PrepareData::MakingFeatureData::init(CvPoolParams *params)
{
  pool = new CvGaborFeaturePool;
  pool->Init( params );
  //pool->write( "/local/gaborfeaturelist.txt" );
  printf("Number of features: %d\n", pool->getSize());
  preparedir();
}




/*!
    \fn PrepareData::MakingFeatureData::preparedir()
 */
void PrepareData::MakingFeatureData::preparedir()
{
  
  //if(flag == 0)
  {
    char *scalepath = new char[100];
    char *orientpath = new char[100];
    char *ch_scale = new char[5];
    char *ch_orient = new char[5];
    int is;
  
    for (int nscale = minscale; nscale <= maxscale; nscale++)
    {
      strcpy (scalepath, destiny);
      sprintf(ch_scale, "%d", nscale);
      strcat(scalepath, ch_scale);
      strcat(scalepath, "/");
      is = chdir(scalepath);
      if (is != 0)
      {
        is = mkdir (scalepath, S_IRWXU);
        if (is != 0) 
        {
          printf("%s write failed. \n", scalepath);
        }
        else
        {
          printf("%s has been created.\n", scalepath);
        }
        for (int norients = 0; norients < orients; norients++)
        {   

          strcpy(orientpath, destiny);
          strcat(orientpath, ch_scale);
          strcat(orientpath, "/");
          sprintf(ch_orient, "%d", norients);
          strcat(orientpath, ch_orient);
          strcat(orientpath, "/");
          printf("%s\n", orientpath);
          is = chdir(orientpath);
          if ( is != 0)
          {
            is = mkdir (orientpath, S_IRWXU);
            if (is != 0)
            { 
              printf("%s write failed.\n", orientpath);
            }
            else
            {
              printf("%s has been created.\n", orientpath);
            }
          }
        }
      }
    }
    delete [] scalepath;
    delete [] orientpath;
    delete [] ch_orient;
    delete [] ch_scale;
  }
  
}


/*!
    \fn PrepareData::MakingFeatureData::setFlag(int flag)
 */
void PrepareData::MakingFeatureData::setFlag(int flag)
{
  this->flag = flag;
}


/*!
    \fn PrepareData::MakingFeatureData::savefeaturedata(CvTrainingData *data, const char *filename)
 */
void PrepareData::MakingFeatureData::savefeaturedata(CvTrainingData *data, const char *filename)
{
  CvMat* mat = data->getdata();
  cvSave( filename, mat,
          "GaborFeatureData",
          NULL,
          cvAttrList());
  cvReleaseMat(&mat);
}


/*!
\fn PrepareData::MakingFeatureData::savefeaturedata(CvTrainingData *data, string filename)
 */
void PrepareData::MakingFeatureData::savefeaturedata(CvTrainingData *data, string filename)
{
  CvMat* mat = data->getdata();
  cvSave( filename.c_str(), mat,
          "GaborFeatureData",
          NULL,
          cvAttrList());
  cvReleaseMat(&mat);
}




/*!
    \fn PrepareData::MakingFeatureData::generatename(CvGaborFeature *feature)
 */
void PrepareData::MakingFeatureData::generatename(CvGaborFeature *feature, char *filename)
{
  int scale = feature->getNu();
  int orient = feature->getMu();
  int x = feature->getx();
  int y = feature->gety();

  char *ch_scale = new char[5];
  char *ch_orient = new char[5];
  char *ch_name = new char[10];
  
  
  strcpy( filename, destiny );
  
  sprintf( ch_scale, "%d", scale );
  strcat(filename, ch_scale);
  strcat(filename, "/");
  
  sprintf( ch_orient, "%d", orient );
  strcat( filename, ch_orient );
  strcat(filename, "/");
  
  sprintf(ch_name, "%d_%d.xml", x, y);
  strcat(filename, ch_name);
  
  delete [] ch_scale;
  delete [] ch_orient;
  delete [] ch_name;
}




/*!
    \fn PrepareData::MakingFeatureData::run()
 */
void PrepareData::MakingFeatureData::run()
{
  bool reduce = pool->isReduced();
  int numfeatures = pool->getSize();
  
  
  time_t start,end;
  double dif;
  for(int i=0; i < numfeatures; i++)
  {
    time (&start);
    CvGaborFeature *feature;
    feature = pool->getfeature(i);
    printf("Get a feature: Scale %d and Orientation %d at (%d,%d)\n", 
           feature->getNu(), feature->getMu(), feature->getx(), feature->gety());
    CvTrainingData *data;
    data = feature->_XM2VTSBin( source, reduce );
    char *filename = new char[100];
    generatename( feature, filename );
    savefeaturedata( data, filename );
    printf("Saved as %s\n", filename);
    time (&end);
    dif = difftime (end,start);
    printf("It took %.2lf seconds.\n", dif);
    delete data;
    delete [] filename;
  }
}


/*!
    \fn PrepareData::MakingFeatureData::clear()
 */
void PrepareData::MakingFeatureData::clear()
{
  delete [] source;
  delete [] destiny;
  if (pool != NULL) delete pool;
}



