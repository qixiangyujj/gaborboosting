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
#include "cvfacedb.h"

CvFaceDB::CvFaceDB()
{
}


CvFaceDB::~CvFaceDB()
{
}




/*!
    \fn CvFaceDB::getName()
 */
char* CvFaceDB::getName()
{
  return (char*)name;
}


/*!
    \fn CvFaceDB::clone()
 */
CvFaceDB* CvFaceDB::clone()
{
    /// @todo implement me
}


/*!
    \fn CvFaceDB::is_xm2vts()
 */
bool CvFaceDB::is_xm2vts()
{
	bool result = false;
	char * dbname = getName();
	if( !strcmp(dbname, "XM2VTS")) 
		result = true;
	return result;
}


/*!
    \fn CvFaceDB::is_feret()
 */
bool CvFaceDB::is_feret()
{
	bool result = false;
	char * dbname = getName();
	if ( !strcmp(dbname, "FERET")) 
		result = true;
	return result;
}
