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
#include "cvmultitrainingdata.h"

CvMultiTrainingData::CvMultiTrainingData()
 : CvTrainingData()
{
}


CvMultiTrainingData::~CvMultiTrainingData()
{
}




/*!
    \fn CvMultiTrainingData::Binary(int clsidx1, int clsidx2)
 */
CvBinaryTrainingData* CvMultiTrainingData::Binary(int clsidx1, int clsidx2)
{
     if ((clsidx1 <= 0) || (clsidx1 > ncls)|| (clsidx2 <= 0) || (clsidx2 > ncls))
     {
       perror("Class index exceeds the number of class or negative \n");
       return NULL;
     }
     else
     {
       CvTrainingData *data1 = new CvTrainingData;
       data1 = split(clsidx1);
       
       CvTrainingData *data2 = new CvTrainingData;
       data2 = split(clsidx2);

       CvBinaryTrainingData *bindata = new CvBinaryTrainingData;
       bindata = (CvBinaryTrainingData*)(data1->merge(data2));
       
       delete data1;
       delete data2;
       return bindata;
     }
}
