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
#include "cvgaborfeaturepair.h"


namespace AdaGabor {

CvGaborFeaturePair::CvGaborFeaturePair()
{
  this->num = 0;
}


CvGaborFeaturePair::~CvGaborFeaturePair()
{
}



}


/*!
    \fn AdaGabor::CvGaborFeaturePair::CvGaborFeaturePair(CvGaborFeature* feature1, CvGaborFeature* feature2)
 */
 AdaGabor::CvGaborFeaturePair::CvGaborFeaturePair(CvGaborFeature* feature1, CvGaborFeature* feature2)
{
  this->num = 0;
  if ((feature1 == NULL) || (feature2 == NULL)) 
  {
    printf("The features must be initilaized.\n");
    exit(-1);
  }
  //this->feature1 = new CvGaborFeature(feature1->getx(), feature1->gety(), feature1->getMu(), feature1->getNu());
  this->feature1 = feature1;
  this->num++;
  //this->feature2 = new CvGaborFeature(feature2->getx(), feature2->gety(), feature2->getMu(), feature2->getNu());
  this->feature2 = feature2;
  this->num++;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::addFeature(CvGaborFeature* feature)
 */
bool AdaGabor::CvGaborFeaturePair::addFeature(CvGaborFeature* feature)
{
  if ((this->feature1 == NULL) || (this->feature2 == NULL))
  {
    if(this->feature1 == NULL) //this->feature1 = new CvGaborFeature(feature->getx(), feature->gety(), feature->getMu(), feature->getNu());
      this->feature1 = feature;
    else //this->feature2 = new CvGaborFeature(feature->getx(), feature->gety(), feature->getMu(), feature->getNu());
      this->feature2 = feature;
    num++;
    return true;
  }
  else 
  {
    printf("The pair is full.\n");
    return false;
  }
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::clone()
 */
AdaGabor::CvGaborFeaturePair* AdaGabor::CvGaborFeaturePair::clone()
{
  CvGaborFeaturePair *newpair = new CvGaborFeaturePair(this->feature1, this->feature2);
  newpair->setMutInf( this->MutInf);
  return newpair;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::getFeature1()
 */
CvGaborFeature* AdaGabor::CvGaborFeaturePair::getFeature1()
{
  return this->feature1;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::getFeature2()
 */
CvGaborFeature* AdaGabor::CvGaborFeaturePair::getFeature2()
{
  return this->feature2;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::CvGaborFeaturePair(const CvGaborFeaturePair& p)
 */
 AdaGabor::CvGaborFeaturePair::CvGaborFeaturePair(const CvGaborFeaturePair& p)
{
  num = p.num;
  MutInf = p.MutInf;
  /*feature1 = new CvGaborFeature(p.feature1->getx(), 
                                p.feature1->gety(),
                                p.feature1->getMu(),
                                p.feature1->getNu());
  feature2 = new CvGaborFeature(p.feature2->getx(), 
                                p.feature2->gety(),
                                p.feature2->getMu(),
                                p.feature2->getNu());
  */
  this->feature1 = p.feature1;
  this->feature2 = p.feature2;
}




/*!
    \fn AdaGabor::CvGaborFeaturePair::operator=(const CvGaborFeaturePair& a)
 */
AdaGabor::CvGaborFeaturePair& AdaGabor::CvGaborFeaturePair::operator=(const AdaGabor::CvGaborFeaturePair& a)
{
  if (this == &a)      // Same object?
    return *this;
  delete this->feature1;
  delete this->feature2;
  this->num = a.num;
  this->MutInf = a.MutInf;
  this->feature1 = new CvGaborFeature(a.feature1->getx(), 
                                      a.feature1->gety(),
                                      a.feature1->getMu(),
                                      a.feature1->getNu());
  this->feature2 = new CvGaborFeature(a.feature2->getx(), 
                                      a.feature2->gety(),
                                      a.feature2->getMu(),
                                      a.feature2->getNu());
  return *this;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::setMutInf(double inf)
 */
void AdaGabor::CvGaborFeaturePair::setMutInf(double inf)
{
  this->MutInf = inf;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::getMutInf()
 */
double AdaGabor::CvGaborFeaturePair::getMutInf()
{
  return this->MutInf;
}


/*!
\fn AdaGabor::CvGaborFeaturePair::describe()
 */
void AdaGabor::CvGaborFeaturePair::describe()
{
  printf("(x: %d, y: %d, Mu: %d, Nu: %d) v.s. (X: %d, y: %d, Mu: %d, Nu: %d)\n", 
         feature1->getx(),
         feature1->gety(),
         feature1->getMu(),
         feature1->getNu(),
         feature2->getx(),
         feature2->gety(),
         feature2->getMu(),
         feature2->getNu());
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::getData(CvFaceDB *database, int n)
 */
CvTrainingData* AdaGabor::CvGaborFeaturePair::getData(CvFaceDB *database, int n)
{
  if (n == 1)
  {
    //printf("Get the data for the 1st feature from database.......\n");
    CvTrainingData *data = feature1->_XM2VTSMulti_F((CvXm2vts*)database);
    return data;
  }
  else if (n == 2)
  {
    //printf("Get the data for the 2nd feature from database.......\n");
    CvTrainingData *data = feature2->_XM2VTSMulti_F((CvXm2vts*)database);
    return data;
  }
  else
  {
    perror("CvGaborFeaturePair::getData ");
    printf("Can not select after 2nd feature!\n");
    exit(-1);
  }
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::operator < (const CvGaborFeaturePair& a)
 */
bool AdaGabor::CvGaborFeaturePair::operator < (const CvGaborFeaturePair& a)
{
  if(this->MutInf < a.MutInf) return true;
  else return false;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::operator > (const CvGaborFeaturePair& a)
 */
bool AdaGabor::CvGaborFeaturePair::operator > (const CvGaborFeaturePair& a)
{
  if(this->MutInf > a.MutInf) return true;
  else return false;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::getFeature(int n)
 */
CvGaborFeature* AdaGabor::CvGaborFeaturePair::getFeature(int n)
{
  if (n == 1)
  {
    return this->feature1;
  }
  else if (n == 2)
  {
    return this->feature2;
  }
  else
  {
    perror("CvGaborFeaturePair::getFeature ");
    printf("Can not select after 2nd feature!\n");
    exit(-1);
  }
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::clear()
 */
void AdaGabor::CvGaborFeaturePair::clear()
{  
  //if (feature1 != NULL) delete feature1;
  //if (feature2 != NULL) delete feature2;
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::write(const char* filename)
 */
void AdaGabor::CvGaborFeaturePair::write(const char* filename)
{
  string str(filename);
  size_t found;
  found = str.find_last_of(".");
  string ext = str.substr(found+1);
  
  if ((ext.compare("xml")==0)||(ext.compare("XML")==0)) 
  {
   writeXML( filename );
  }
  else if (ext.compare("txt")==0) writeTXT( filename );
  else writeTXT( filename );
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::writeXML(const char* filename)
 */
void AdaGabor::CvGaborFeaturePair::writeXML(const char* filename)
{
  CvMemStorage* storage = cvCreateMemStorage( 0 );
  CvFileStorage* fs = cvOpenFileStorage( filename, storage, CV_STORAGE_WRITE);
  
  cvStartWriteStruct( fs, "CvGaborFeaturePair",
                      CV_NODE_MAP, NULL,
                      cvAttrList(0,0));
  for(int j = 1; j <= 2; j++)
  {
    CvGaborFeature *feature = getFeature(j);
    cvStartWriteStruct( fs, "CvGaborFeature",
                        CV_NODE_MAP, NULL,
                        cvAttrList(0,0));
    cvWriteInt(fs, "x", feature->getx());
    cvWriteInt(fs, "y", feature->gety());
    cvWriteInt(fs, "Mu", feature->getMu());
    cvWriteInt(fs, "Nu", feature->getNu());
    cvEndWriteStruct(fs); 
  }
  cvWriteReal(fs, "MutInf", getMutInf());
  cvEndWriteStruct( fs );

  cvReleaseFileStorage(&fs);
  cvReleaseMemStorage(&storage);
}


/*!
    \fn AdaGabor::CvGaborFeaturePair::writeTXT(const char *filename)
 */
void AdaGabor::CvGaborFeaturePair::writeTXT(const char *filename)
{
  FILE * file;
  file = fopen (filename,"a");
  
  int x = feature1->getx();
  int y = feature1->gety();
  int Mu = feature1->getMu();
  int Nu = feature1->getNu();
  
  fprintf (file, "(%d, %d, %d, %d) ",x, y, Mu, Nu);
  x = feature2->getx();
  y = feature2->gety();
  Mu = feature2->getMu();
  Nu = feature2->getNu();
  fprintf (file, "(%d, %d, %d, %d) ",x, y, Mu, Nu);
  
  double mut = getMutInf();
  fprintf (file, "%f\n", mut);
  fclose(file);
}
