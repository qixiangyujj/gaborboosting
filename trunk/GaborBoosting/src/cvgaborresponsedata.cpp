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
#include "cvgaborresponsedata.h"

namespace PrepareData {

CvGaborResponseData::CvGaborResponseData()
{
}


CvGaborResponseData::~CvGaborResponseData()
{
	clear();
}


}


/*!
    \fn PrepareData::CvGaborResponseData::CvGaborResponseData(CvFaceDB *db, CvPoolParams *param)
 */
 PrepareData::CvGaborResponseData::CvGaborResponseData(CvFaceDB *db, CvPoolParams *param)
{
	setDB( db );
	setParam( param );
	
	nRespones = nImages * nScales * nOrientations;
}


/*!
    \fn PrepareData::CvGaborResponseData::setDB(CvFaceDB *db)
 */
void PrepareData::CvGaborResponseData::setDB(CvFaceDB *db)
{
	database = db->clone();
	char * dbname = database->getName();
	if( !strcmp(dbname, "XM2VTS")) 
		dbtype = XM2VTS;
	else if ( !strcmp(dbname, "FERET")) 
		dbtype = FERET;
	printf("...............a %s database copied!\n\n", dbname);
	 
	if(dbtype == XM2VTS)
	{
		int nClients = ((CvXm2vts*)database)->get_num_sub();
		int imgsperClient = ((CvXm2vts*)database)->get_num_pic();
		nImages = nClients * imgsperClient;
		
	}
	else if(dbtype == FERET)
	{
		/// @todo implement me
	}
}


/*!
    \fn PrepareData::CvGaborResponseData::setParam(CvPoolParams *param)
 */
void PrepareData::CvGaborResponseData::setParam(CvPoolParams *param)
{
	Orients = cvCloneMat( param->getOrients() );
	Scales = cvCloneMat( param->getScales() );
	
	CvSize size_o;
	size_o = cvGetSize( Orients );
	nOrientations = size_o.width * size_o.height;
	
	CvSize size_s;
	size_s = cvGetSize( Scales );
	nScales = size_s.width * size_s.height;
	
}



/*!
    \fn PrepareData::CvGaborResponseData::clear()
 */
void PrepareData::CvGaborResponseData::clear()
{
	delete database;
	cvReleaseMat( &Orients );
	cvReleaseMat( &Scales );
	for (int i = 0; i < nRespones ; i++)
	{
		cvReleaseImage( &(responses[i]) );
	}
	
	
}


/*!
    \fn PrepareData::CvGaborResponseData::generate()
 */
void PrepareData::CvGaborResponseData::generate()
{
	printf("Generating the response data, please waiting ....\n");
	int n = 0;
	responses = (IplImage **)cvAlloc(nRespones*sizeof(IplImage *));
	if(dbtype == XM2VTS)
	{
		
		char * xm2vts_path = ((CvXm2vts*)database)->getPath();
		int nClients = ((CvXm2vts*)database)->get_num_sub();
		int imgsperClient = ((CvXm2vts*)database)->get_num_pic();
		char filename[200];
		for( int i = 1; i <= nClients; i++ )
		{
			for(int j = 1; j <= imgsperClient; j++ )
			{
				sprintf(filename, "%s/%d_%d.jpg", xm2vts_path, i, j);			
				IplImage *img = cvLoadImage( filename,  CV_LOAD_IMAGE_GRAYSCALE );
				for(int k = 0; k < nScales; k++)
				{
					int scale = (int)cvGetReal1D( Scales, k );
					for(int l = 0; l < nOrientations; l++)
					{
						int orientation = (int)cvGetReal1D( Orients, l);
						CvGabor gaborfilter(orientation, scale);
						responses[n] = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_32F, 1);
						gaborfilter.conv_img((IplImage*)img, (IplImage*)responses[n], CV_GABOR_MAG);
						n++;
					}
				}	
				cvReleaseImage(&img);
			}
			//if( fmod(i,2.0) == 0.0)
			//	printf("%d\%\n",i/2);
			std::cout << i<< "\r" << std::flush;
		}
		printf("\n");
	}
	else if(dbtype == FERET)
	{
		/// @todo implement me
	}
	printf("\n%d response images are generated.\n", n);
}







/*!
    \fn PrepareData::CvGaborResponseData::loadData(const char* datapath)
 */
void PrepareData::CvGaborResponseData::loadData(const char* datapath)
{
	printf("Loading the response data, please waiting ....\n");
	
	int n = 0;
	responses = (IplImage **)cvAlloc(nRespones*sizeof(IplImage *));
	if(dbtype == XM2VTS)
	{

		char * xm2vts_path = ((CvXm2vts*)database)->getPath();
		int nClients = ((CvXm2vts*)database)->get_num_sub();
		int imgsperClient = ((CvXm2vts*)database)->get_num_pic();
		char filename[200];
		for( int i = 1; i <= nClients; i++ )
		{
			for(int j = 1; j <= imgsperClient; j++ )
			{
				for(int k = 0; k < nScales; k++)
				{
					int scale = (int)cvGetReal1D( Scales, k );
					for(int l = 0; l < nOrientations; l++)
					{
						int orientation = (int)cvGetReal1D( Orients, l);
						sprintf(filename, "%s/%d/%d/%d_%d.xml", datapath, i, j, scale, orientation);
						responses[n] = (IplImage *)cvLoad(filename,NULL,NULL,NULL);
						n++;
					}
				}
			}
			std::cout << i<< "\r" << std::flush;
		}
	}
	else if(dbtype == FERET)
	{
		/// @todo implement me
	}
	printf("\n");

	printf("\n%d response images are loaded.\n", n);
}







/*!
    \fn PrepareData::CvGaborResponseData::getfeaturefrominstance(CvGaborFeature *feature, int client_index, int picture_index)
 */
double PrepareData::CvGaborResponseData::getfeaturefrominstance(CvGaborFeature *feature, int client_index, int picture_index)
{
	int scale_index = feature->getNu();
	int orientation_index = feature->getMu();
	int x = feature->getx();
	int y = feature->gety();
	int min_scale = (int)cvGetReal1D(Scales, 0);
	int min_orientation = (int)cvGetReal1D(Orients, 0);
	int nGaborResponse = nScales * nOrientations;
	
	int response_index;
	double feature_value = NAN;
	if(dbtype == XM2VTS)
	{
		int nPictureperClient = ((CvXm2vts *)database)->get_num_pic();
		response_index = (picture_index-1)*nGaborResponse + (client_index-1)*nPictureperClient + (scale_index-min_scale)*nOrientations + (orientation_index - min_orientation);
		IplImage *reponse_img = responses[response_index];
		feature_value = cvGetReal2D( reponse_img, y-1, x-1 );
	}
	else if(dbtype == FERET)
	{
	}
	
	return feature_value;
}


/*!
    \fn PrepareData::CvGaborResponseData::getfeaturefromall(CvGaborFeature *feature)
 */
CvMat* PrepareData::CvGaborResponseData::getfeaturefromall(CvGaborFeature *feature)
{
	CvMat * Vvector = cvCreateMat(1, nImages, CV_32FC1);
	int n = 0;
	if(dbtype == XM2VTS)
	{
		int nClient = ((CvXm2vts*)database)->get_num_sub();
		int nPicturePerClient = ((CvXm2vts*)database)->get_num_pic();
		for(int i = 1; i <= nClient; i++)
		{
			for(int j = 1; j <= nPicturePerClient; j++)
			{
				double value = getfeaturefrominstance( feature, i, j );
				cvSetReal1D( Vvector, n, value );
				n++;
			} 
		}
	}
	else if(dbtype == FERET)
	{
	}
	
	return Vvector;
}
