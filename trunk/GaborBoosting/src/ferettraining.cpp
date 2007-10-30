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
#include "cvpoolparams.h"
#include "cvferet.h"
#include "cvbingabadafeatureselect.h"

int main(int argc, char *argv[])
{
  int id = 29;
  
  int width = 64;
  int height = 64;
  int bound = 0;
  int interval = 0;
  int norientations = 8;
  int minscale = -1;
  int maxscale = 3;
  bool reduced = true;
  
  
  
  CvPoolParams param(height, width, minscale, maxscale, norientations, interval, bound, reduced);
  
  CvFeret feret("/windows/D/Data/feret/", "/local/FaceDB/FERET/fa/PPMS/", "/local/FaceDB/FERET/fb/PPMS/");
  
  CvBinGabAdaFeatureSelect FeatureSelec(&feret, &param, id);
  FeatureSelec.train(200);
  return 0;
}



