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
#include "cvgaborfeaturepairselect.h"

namespace AdaGabor {

CvGaborFeaturePairSelect::CvGaborFeaturePairSelect()
{
}


CvGaborFeaturePairSelect::~CvGaborFeaturePairSelect()
{
  clear();
}


}


/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::CvGaborFeaturePairSelect(CvPoolParams *param, CvFaceDB *facedb)
 */
 AdaGabor::CvGaborFeaturePairSelect::CvGaborFeaturePairSelect(CvPoolParams *param, CvFaceDB *facedb)
{
  setPool(param);
  setDB(facedb);
  setNumBins(DEFAULT_NO_BINS);
  setMutFile( "mut_table.txt" );
  build();
}


/*!
\fn AdaGabor::CvGaborFeaturePairSelect::CvGaborFeaturePairSelect::CvGaborFeaturePairSelect(CvPoolParams *param, CvFaceDB *facedb, int nobins)
 */
AdaGabor::CvGaborFeaturePairSelect::CvGaborFeaturePairSelect::CvGaborFeaturePairSelect(CvPoolParams *param, CvFaceDB *facedb, int nobins)
{
  setPool(param);
  setDB(facedb);
  setNumBins(nobins);
  setMutFile( "mut_table.txt" );
  build();
}


/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::setPool(CvPoolParams *param)
 */
void AdaGabor::CvGaborFeaturePairSelect::setPool(CvPoolParams *param)
{
  old_pool = new AdaGabor::CvGaborFeaturePairPool( param );
  printf(".................pool built!\n");
}


/*!
\fn AdaGabor::CvGaborFeaturePairSelect::setPool(CvPoolParams *param, CvGaborFeature *start_feature, CvGaborFeature *end_feature)
 */
void AdaGabor::CvGaborFeaturePairSelect::setPool(CvPoolParams *param, CvGaborFeature *start_feature, CvGaborFeature *end_feature)
{
  old_pool = new AdaGabor::CvGaborFeaturePairPool( param, start_feature, end_feature );
  printf(".................pool built!\n");
}


/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::setDB(CvFaceDB *facedb)
 */
void AdaGabor::CvGaborFeaturePairSelect::setDB(CvFaceDB *facedb)
{
  database = facedb->clone();
  printf("............... Database copied!\n\n");
}


/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::setNumBins(int b)
 */
void AdaGabor::CvGaborFeaturePairSelect::setNumBins(int b)
{
  this->numbins = b;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::build()
 */
void AdaGabor::CvGaborFeaturePairSelect::build()
{
  time_t start,end;
  double dif;
  time (&start);
  for(long i = 0; i < old_pool->size(); i++)
  {
    
    //time (&start);
    CvGaborFeaturePair *pair = old_pool->getPair(i);
    // Display the pair information
    //printf("Get a new pair\n");
    //pair->describe();
    
    CvMutInf *mut = new CvMutInf(pair, this->database,numbins);
    double v = mut->get();
    //printf("The mutural information of this pair is %f\n\n", v);
    
    if(fmod((long double)i,1000.0) == 0)
    {
      time (&end);
      dif = difftime (end,start);
      printf("It took %.2lf seconds for 1000 pairsddddd.\n", dif);
      printf("\n\n");
    }
    
    pair->setMutInf( v );
    //pair->write( mutfile );
    {
      FILE * file;
      file = fopen(mutfile, "a");
      assert( file );
      fprintf (file, "%f\n", v);
      //fprintf (file, "%d %f\n", i, v);
      fclose( file );
    }
    
    
    delete mut;
    int n = 100*i/(old_pool->size());
    //std::cout << "Mutural Information calculation: " << n << "%\r" << std::flush;
    //time (&end);
    //dif = difftime (end,start);
    // printf("It took %.2lf seconds.\n", dif);
  }
  cout << endl;
 
  {
    FILE * file;
    file = fopen(mutfile, "a");
    assert( file );
    fprintf (file, "%s\n", "finsihed!");
    fclose( file );
  }
}


/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::clear()
 */
void AdaGabor::CvGaborFeaturePairSelect::clear()
{
  if(old_pool != NULL) delete old_pool;
  //if(new_pool != NULL) delete new_pool;
  if(database != NULL) delete database;
  if( mutfile != NULL ) delete [] mutfile;
}


/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::resume()
 */
void AdaGabor::CvGaborFeaturePairSelect::resume()
{
  printf("%s\n", mutfile);
  FILE * file;
    //file = fopen (filename,"r");
  printf("hello\n");
  if ((file=fopen( mutfile, "r" )) == NULL)
  {
    perror(mutfile);
    exit(1);
  }
  
  printf("Loading pairs from the log file %s", mutfile);
  int x1,y1,Mu1,Nu1,x2,y2,Mu2,Nu2;
  float mutinf;
  
  long n = 0;
  long i;
  while (!feof(file))
  {
    /*
    if (fscanf(file, "(%d,", &x1) == EOF) break;
    if (fscanf(file, " %d,", &y1) == EOF) break;
    if (fscanf(file, " %d,", &Mu1) == EOF) break;
    if (fscanf(file, " %d)", &Nu1) == EOF) break;
    CvGaborFeature ffeature1( x1, y1, Mu1, Nu1 );
    if (fscanf(file, " (%d,", &x2) == EOF) break;
    if (fscanf(file, " %d,", &y2) == EOF) break;
    if (fscanf(file, " %d,", &Mu2) == EOF) break;
    if (fscanf(file, " %d)", &Nu2) == EOF) break;
    if (fscanf(file, " %f\n", &mutinf) == EOF) break;
    CvGaborFeature ffeature2( x2, y2, Mu2, Nu2 ); 
    */
    if (fscanf(file, "%f\n", &mutinf) == EOF) break;
    CvGaborFeaturePair *pair = old_pool->getPair( n );
    CvGaborFeature *mfeature1 = pair->getFeature( 1 );
    CvGaborFeature *mfeature2 = pair->getFeature( 2 );
    
    //if((ffeature1==*mfeature1)&&(ffeature2==*mfeature2))
    {
      pair->setMutInf( mutinf );
      n++;
    }
    //else 
    //  break;
  }
  printf("    ... done!\n");
  fclose(file);
  resume( n );
}


/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::resume(int n)
 */
void AdaGabor::CvGaborFeaturePairSelect::resume(int n)
{
  for(long i = n; i < old_pool->size(); i++)
  {

    CvGaborFeaturePair *pair = old_pool->getPair(i);

    //pair->describe();
    
    CvMutInf *mut = new CvMutInf(pair, this->database,numbins);
    double v = mut->get();

    pair->setMutInf( v );
    //pair->write( mutfile );
    {
      FILE * file;
      file = fopen(mutfile, "a");
      assert( file );
      fprintf (file, "%f\n", v);
      fclose( file );
    }
    delete mut;
    int p = 100*i/(old_pool->size());
    std::cout << "Mutural Information calculation: " << p << "%\r" << std::flush;
  }
  {
    FILE * file;
    file = fopen(mutfile, "a");
    assert( file );
    fprintf (file, "%s\n", "finsihed!");
    fclose( file );
  }
}





/*!
    \fn AdaGabor::CvGaborFeaturePairSelect::setMutFile(const char* filename)
 */
void AdaGabor::CvGaborFeaturePairSelect::setMutFile(const char* filename)
{
  //if(mutfile != NULL) delete [] mutfile;
  mutfile = new char[30];
  strcpy( mutfile, filename);
  
}
