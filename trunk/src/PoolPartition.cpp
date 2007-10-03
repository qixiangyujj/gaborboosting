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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <iostream>
#include <cstdlib>
#include "cvferet.h"

int main(int argc, char *argv[])
{
  int N = 30240;
  int numParts = 120;
  
  int total = N*(N-1)/2;
  int size_part = total/numParts;
  int nums[numParts];
  
  int n = 0;
  int sum = 0;
  for(int i = 0; i < N; i++)
  {
    sum = sum + (N-i-1);
    if(sum > size_part)  
    {
      i--;
      nums[n] = i;
      
      //printf("%d    %d\n", i, n);
      n++;
      sum = 0;
    }
  }
  
  CvFeret feret;
  feret.openPath("/local/FaceDB/FERET/fa/JPG/", "/local/FaceDB/FERET/fb/JPG/");
  
  
  
  
  return EXIT_SUCCESS;  
}

