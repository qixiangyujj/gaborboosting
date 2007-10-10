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
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "cvgabadafeatureselect.h"


int main(int argc, char *argv[])
{
  char * signfilename = new char[50];
  char * weaksfilename = new char[50];

  strcpy( signfilename, "significant.txt" );
  strcpy( weaksfilename, "weaks.xml" );
  
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-input" ) )
    {
      strcpy(signfilename, argv[++i]);
    }
    else if( !strcmp( argv[i], "-output" ) )
    {
      strcpy(weaksfilename, argv[++i]);
    }
  }
  CvGabAdaFeatureSelect *featureSelec = new CvGabAdaFeatureSelect;
  featureSelec->sign2weaks( signfilename, weaksfilename );
  
  delete featureSelec;

  delete [] signfilename;
  delete [] weaksfilename;
  
  return EXIT_SUCCESS;
}
