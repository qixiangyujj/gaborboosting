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
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <ml.h>
#include <cvaux.h>

#include <CommonCVFunction.h>


#include <cvadaboost.h>
#include <cvbinarytrainingdata.h>
#include <cvbingabadafeatureselect.h>
#include <cvfacedb.h>
#include <cvferet.h>
#include <cvgabadafeatureselect.h>
#include <cvgabor.h>
#include <cvgaborfeature.h>
#include <cvgaborfeaturepool.h>
#include <cvmultitrainingdata.h>
#include <cvpcafacereg.h>
#include <cvpoolparams.h>
#include <cvsubject.h>
#include <cvtestingdata.h>
#include <cvtrainingdata.h>
#include <cvweaklearner.h>
#include <cvxm2vts.h>

//using namespace AdaGabor;
#include <cv2bins.h>
#include <cvbin.h>
#include <cventropy.h>
#include <cvgaborfeaturepair.h>
#include <cvgaborfeaturepairpool.h>
#include <cvgaborfeaturepairselect.h>
#include <cvlookuptable.h>
#include <cvmutinf.h>

//using namespace MatFunctions;
#include <MatUsage.h>
#include <cvknnweaklearner.h>
#include <cvmultigabadafeatureselect.h>
#include <cvmultigabadafsm1.h>
#include <cvmweaklearner.h>


//using namespace PrepareData;
#include <PrepareData.h>
#include "cvgaborresponsedata.h"
#include <makingfeaturedata.h>

#include "Segmentation/cvfacesegment.h"
#include "cvgabordifferencedatamaker.h"

