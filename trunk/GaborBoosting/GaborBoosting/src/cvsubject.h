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
#ifndef CVSUBJECT_H
#define CVSUBJECT_H
#include <vector>
#include <iostream>
#include <string>

using namespace std;
/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvSubject{
public:
    CvSubject();

    ~CvSubject();
    void setname(const char* filename);
    void setname(string filename);
    int getnum();
    void setID(int id);
     CvSubject(int id);
    void clear();
    int getId();
    string getname(int no);

protected:
    string ID;
    int id;
    int num;
    vector<string> nametable;
};

#endif
