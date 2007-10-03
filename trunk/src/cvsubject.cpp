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
#include "cvsubject.h"

CvSubject::CvSubject()
{
}


CvSubject::~CvSubject()
{
}




/*!
    \fn CvSubject::setname(const char* filename)
 */
void CvSubject::setname(const char* filename)
{
  string s( filename );
  nametable.push_back( s );
  num = nametable.size();
}


/*!
    \fn CvSubject::setname(string filename)
 */
void CvSubject::setname(string filename)
{
  nametable.push_back(filename);
  num = nametable.size();
}


/*!
    \fn CvSubject::getnum()
 */
int CvSubject::getnum()
{
  return nametable.size();
}


/*!
    \fn CvSubject::setID(int id)
 */
void CvSubject::setID(int id)
{
  this->id = id;
  
}


/*!
    \fn CvSubject::CvSubject(int id)
 */
 CvSubject::CvSubject(int id)
{
  setID( id );
}


/*!
    \fn CvSubject::clear()
 */
void CvSubject::clear()
{
  nametable.clear();
  ID.clear();
}


/*!
    \fn CvSubject::getId()
 */
int CvSubject::getId()
{
  return id;
}


/*!
    \fn CvSubject::getname(int no)
 */
string CvSubject::getname(int no)
{
  if(no >= nametable.size())
  {
    perror("the index out of range!\n");
    exit( 1 );
  }
  
  string name = nametable[no];
  return name;
}
