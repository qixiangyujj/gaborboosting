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
#ifndef ADAGABORCV2BINS_H
#define ADAGABORCV2BINS_H
#include <cvbin.h>

namespace AdaGabor {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class Cv2Bins{
public:
    Cv2Bins();

    ~Cv2Bins();
    Cv2Bins(CvBin* bin1, CvBin* bin2);
    bool check(double val1, double val2);
    int getNum();
     Cv2Bins(const Cv2Bins& b);
    AdaGabor::Cv2Bins operator = (const Cv2Bins& b);
    void clear();

protected:
    CvBin* bin1;
    CvBin* bin2;
};

}

#endif
