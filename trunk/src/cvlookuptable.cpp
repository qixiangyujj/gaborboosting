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
#include "cvlookuptable.h"

namespace AdaGabor {

CvLookUpTable::CvLookUpTable()
{
}


CvLookUpTable::~CvLookUpTable()
{
  this->clear();
}


}



/*!
    \fn AdaGabor::CvLookUpTable::clear()
 */
void AdaGabor::CvLookUpTable::clear()
{
  for (int i = 0; i < binlist.size(); i++)
  {
    CvBin *bin = binlist.at(i);
    delete bin;
  }
  for (int j = 0; j < twinbinlist.size(); j++)
  {
    Cv2Bins *bins = twinbinlist.at(j);
    delete bins;
  }
  binlist.clear();
  twinbinlist.clear();
}


/*!
    \fn AdaGabor::CvLookUpTable::setNumBins(int num)
 */
void AdaGabor::CvLookUpTable::setNumBins(int num)
{
  if (num_data == 1)
  {
    this->num_bins = num;
    for (int i = 0; i < this->num_bins; i++)
    {
      CvBin * bin = new CvBin((1.0/this->num_bins)*(double)i ,(1.0/this->num_bins)*(double)(i+1));
      binlist.push_back(bin);
    }
  }
  else if (num_data == 2)
  {
    this->num_bintwins = num*num;
    for (int i = 0; i < num; i++)
    {
      for (int j = 0; j < num; j++)
      {
        CvBin * bin1 = new CvBin((1.0/num)*(double)i, (1.0/num)*(double)(i+1));
        CvBin * bin2 = new CvBin((1.0/num)*(double)j ,(1.0/num)*(double)(j+1));
        Cv2Bins *bins = new Cv2Bins(bin1, bin2);
        this->twinbinlist.push_back(bins);
        delete bin1;
        delete bin2;
      }
    }
  }
  else
  {
    perror("CvLookUpTable::setNumBins ");
    exit(-1);
  }
}



/*!
    \fn AdaGabor::CvLookUpTable::build()
 */
void AdaGabor::CvLookUpTable::build()
{
  if (num_data == 1)
  {
    this->num = data->getnumsample();
    CvMat *mat = data->getdata();
    cvNormalize(mat, mat, 0.0, 1.0, CV_MINMAX);
    double v; 
    for (int i = 0; i < data->getnumsample(); i++)
    {
      v = cvGetReal1D(mat, i);
      for (int j = 0 ; j < binlist.size(); j++)
      {
        CvBin *bin = (binlist.at(j));
        if(bin->check(v)) break;
      }
    }
    cvReleaseMat(&mat);
  }
  else if (num_data == 2)
  {
    if (data1->getnumsample()!=data2->getnumsample())
    {
      perror("CvLookUpTable\n");
      printf("the sample numbers are not equal!\n");
      exit(-1);
    }
    this->num = data1->getnumsample();
    CvMat *mat1 = data1->getdata();
    CvMat *mat2 = data2->getdata();
    cvNormalize(mat1, mat1, 0.0, 1.0, CV_MINMAX);
    cvNormalize(mat2, mat2, 0.0, 1.0, CV_MINMAX);
    double v1, v2;
    for (int i = 0; i < this->num; i++)
    {
      v1 = cvGetReal1D(mat1, i);
      v2 = cvGetReal1D(mat2, i);
      for(int j = 0; j < twinbinlist.size(); j++)
      {
        Cv2Bins *bins = (twinbinlist.at(j));
        if (bins->check(v1, v2)) break;
      }
    }
    cvReleaseMat(&mat1);
    cvReleaseMat(&mat2);
  }
  else
  {
    perror("CvLookUpTable::build\n");
    exit(-1);
  }
  
}




/*!
    \fn AdaGabor::CvLookUpTable::numsample()
 */
int AdaGabor::CvLookUpTable::numsample()
{
  return num;
}


/*!
    \fn AdaGabor::CvLookUpTable::retrieve(int binindex)
 */
int AdaGabor::CvLookUpTable::retrieve(int binindex)
{
  if(num_data != 1)
  {
    perror("CvLookUpTable::retrieve\n");
    exit(-1);
  }
  if((binindex > this->num_bins)||(binindex < 0))
  {
    printf("The bin index is out of range!\n");
    exit(-1);
  }
  CvBin *bin = (binlist.at(binindex));
  return bin->getNum();
}


/*!
    \fn AdaGabor::CvLookUpTable::retrieve(int binindex1, int binindex2)
 */
int AdaGabor::CvLookUpTable::retrieve(int binindex1, int binindex2)
{
  if(num_data != 2)
  {
    perror("CvLookUpTable::retrieve\n");
    exit(-1);
  }
  int step = (int)sqrt((double)this->num_bintwins);
  int index = binindex1*step + binindex2;
  if ((index > this->num_bintwins)||(index < 0))
  {
    printf("The bin index is out of range!\n");
    exit(-1);
  }
  Cv2Bins *bins = (twinbinlist.at(index));
  return bins->getNum();
}


/*!
    \fn AdaGabor::CvLookUpTable::CvLookUpTable(CvTrainingData *data)
 */
 AdaGabor::CvLookUpTable::CvLookUpTable(CvTrainingData *data)
{
  this->data = data;
  this->num_data = 1;
}


/*!
    \fn AdaGabor::CvLookUpTable::CvLookUpTable(CvTrainingData *data1, CvTrainingData *data2)
 */
 AdaGabor::CvLookUpTable::CvLookUpTable(CvTrainingData *data1, CvTrainingData *data2)
{
  this->data1 = data1;
  this->data2 = data2;
  this->num_data = 2;
}
