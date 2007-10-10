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
#ifndef ADAGABORCVBIN_H
#define ADAGABORCVBIN_H
#include <vector>
using namespace std;

namespace AdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class CvBin{
public:
    CvBin();

    ~CvBin();
    void setBounds(double lower, double upper);
    double getUpper();
    double getLower();
    CvBin(double lower, double upper);
    bool check(double val);
    void add(double val);
    int getNum();
     CvBin(const CvBin& b);
    AdaGabor::CvBin& operator = (const CvBin& b);
    AdaGabor::CvBin* clone();
    void clear();
  
    
protected:
    double lowerBound;
    double upperBound;
    vector<double> elements;

};

}

#endif
