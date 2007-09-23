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
#ifndef PREPAREDATAMAKINGFEATUREDATA_H
#define PREPAREDATAMAKINGFEATUREDATA_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cvgabor.h"
#include "cvpoolparams.h"
#include "cvgaborfeaturepool.h"
#include "cvtrainingdata.h"

namespace PrepareData {

/**
	@author Mian Zhou <M.Zhou@reading.ac.uk>
*/
class MakingFeatureData{
public:
    MakingFeatureData();

    ~MakingFeatureData();
    MakingFeatureData(CvPoolParams *params, const char* pathname, const char*depathname);
    MakingFeatureData(int height, int width, 
                      int minscale, int maxscale, 
                      int orients, 
                      const char* pathname, const char*depathname);
    MakingFeatureData(int height, int width, 
                      int minscale, int maxscale, 
                      int orients, 
                      const char* pathname, const char*depathname, 
                      bool reduce);
    void init(CvPoolParams *params);
    void setFlag(int flag);
    void preparedir();
    void savefeaturedata(CvTrainingData *data, const char *filename);
    void generatename(CvGaborFeature *feature, char *filename);
    void savefeaturedata(CvTrainingData *data, string filename);
    void run();
    void clear();
    

protected:
    CvGaborFeaturePool *pool;
    char *source;
    char *destiny;
    int flag;
    int width;
    int height;
    int minscale;
    int maxscale;
    int orients;
};

}

#endif
