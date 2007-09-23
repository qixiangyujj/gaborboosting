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

#include "MatUsage.h"
bool MatFunctions::isInMat(CvMat *mat, double val)
{
  CvSize size = cvGetSize( mat );
  int row = size.height;
  int col = size.width;
  
  bool in = false;
  for(int i = 0; i < row*col; i++)
  {
    double value = cvGetReal1D(mat, i);
    if(value == val) 
    {
      in = true;
      break;
    }
  }
  return in;
}

bool MatFunctions::checkMatSize(CvMat *mat, CvSize size)
{
  CvSize s = cvGetSize( mat );
  if((s.width == size.width)&&(s.height == size.height)) return true;
  else return false;
}

void MatFunctions::findRange(int N, int numParts, int job, int *istart, int *iend)
{
  double remain = fmod( (double)N, (double)numParts );
  int size_part;
  if( remain == 0.0 )
  {
    size_part = N/(numParts);
  }
  else
  {
    size_part = N/(numParts-1);
  }
  
  int *nums = new int[numParts];
  
  for(int i = 0; i < numParts; i++)
  {
    nums[i] = i*size_part;
  }
  
  if (job == numParts)
  {
    *istart = nums[job];
    *iend = N-1;
  }
  else
  {
    *istart = nums[job];
    *iend = nums[job+1]-1;
  }
  delete [] nums;
}

