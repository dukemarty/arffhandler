/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFContainers.cc

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change: 09.08.10

    \author    Martin Loesch (<loesch@ira.uka.de>)
    \date      08.02.07
    \par Copyright:
               Martin Loesch, Chair Prof. Dillmann (IAIM)\n
               Institute for Computer Science and Engineering (CSE)\n
	       University of Karlsruhe. All rights reserved\n
	       http://wwwiaim.ira.uka.de
*/

/* system includes */
#include <fstream>
#include <assert.h>
#include <cmath>

/* my includes */
#include "ARFFHandlerConfiguration.h"
#include "ARFFContainers.h"



// **********************************************************************
// ***** FeatureContainer stuff *****************************************

ARFFHANDLERNAMESPACE::FeatureContainer::FeatureContainer()
{
  m_numberOfFeatures = 0;

  m_data = NULL;
}

ARFFHANDLERNAMESPACE::FeatureContainer::FeatureContainer(const int numberOfFeatures)
{
  assert (numberOfFeatures>0);
  
  m_numberOfFeatures = numberOfFeatures;

  try {
    m_data = new double[m_numberOfFeatures];
  } catch (std::bad_alloc& e){
    cerr << "Error: Memory for new FeatureContainer object could not be allocated!\n";
    throw;
  }

  assert (m_data!=NULL);
}

ARFFHANDLERNAMESPACE::FeatureContainer::FeatureContainer(const ARFFHANDLERNAMESPACE::FeatureContainer& orig)
{
  m_numberOfFeatures = orig.m_numberOfFeatures;

  try {
    m_data = new double[m_numberOfFeatures];
  } catch (std::bad_alloc& e){
    cerr << "Error: Memory for new FeatureContainer object could not be allocated!\n";
    throw;
  }

  memcpy(m_data, orig.m_data, m_numberOfFeatures*sizeof(double));

  assert (m_data!=NULL);
}

ARFFHANDLERNAMESPACE::FeatureContainer::~FeatureContainer()
{
  if (m_data) delete[] m_data;
}

bool ARFFHANDLERNAMESPACE::FeatureContainer::checkIsValid() const
{
  return (m_data!=NULL);
}

unsigned int ARFFHANDLERNAMESPACE::FeatureContainer::getNumberOfFeatures() const
{
  return m_numberOfFeatures;
}

void ARFFHANDLERNAMESPACE::FeatureContainer::setNumberOfFeatures(const unsigned int newSize)
{
  assert (newSize>0);

  m_numberOfFeatures = newSize;

  if (m_data) delete[] m_data;

  try {
    m_data = new double[m_numberOfFeatures];
  } catch (std::bad_alloc& e){
    cerr << "Error: Memory for new FeatureContainer object could not be allocated!\n";
    throw;
  }

  for (unsigned int i=0; i<m_numberOfFeatures; i++){ m_data[i] = 0; }

  assert (m_data!=NULL);
}

double ARFFHANDLERNAMESPACE::FeatureContainer::getData(const unsigned int index) const
{
  assert (checkIsValid());
  assert (index<m_numberOfFeatures);
  
  return m_data[index];
}

void ARFFHANDLERNAMESPACE::FeatureContainer::setData(const unsigned int index, const double newData)
{
  assert (checkIsValid());
  assert (index<m_numberOfFeatures);
  
  m_data[index] = newData;

  assert (checkIsValid());
  assert (getData(index)==newData);
}

ARFFHANDLERNAMESPACE::FeatureContainer& ARFFHANDLERNAMESPACE::FeatureContainer::operator=(const ARFFHANDLERNAMESPACE::FeatureContainer& right)
{
  m_numberOfFeatures = right.m_numberOfFeatures;

  if (m_data) delete[] m_data;

  if (m_numberOfFeatures>0){
    try {
      m_data = new double[m_numberOfFeatures];
    } catch (std::bad_alloc& e){
      cerr << "Error: Memory for new FeatureContainer object could not be allocated!\n";
      throw;
    }
    
    memcpy(m_data, right.m_data, m_numberOfFeatures*sizeof(double));
      
  } else {
    m_data = NULL;
  }
    
  assert ( (m_numberOfFeatures==0 && m_data==NULL) || (m_data!=NULL) );
  return *this;
}

const bool ARFFHANDLERNAMESPACE::FeatureContainer::isEqual(ARFFHANDLERNAMESPACE::FeatureContainer* other) const
{
  assert (checkIsValid());

  //unequal number of features => containers aren't equal 
  if(other->getNumberOfFeatures() != this->m_numberOfFeatures){ return false; }
	
  //test every feature
  for(unsigned int i = 0; i<m_numberOfFeatures; i++){
    if(fabs(other->getData(i) - m_data[i]) < 0.000001){ return false; }
  }
  
  //passed every test, seems equal
  return true;
}

  
// **********************************************************************
// ***** FeatureContainerSequence stuff *********************************

ARFFHANDLERNAMESPACE::FeatureContainerSequence::FeatureContainerSequence()
{
}

ARFFHANDLERNAMESPACE::FeatureContainerSequence::~FeatureContainerSequence()
{
  for (vector<FeatureContainer* >::iterator it=_list.begin(); it!=_list.end(); it++){
    delete *it;
  }
}

void ARFFHANDLERNAMESPACE::FeatureContainerSequence::clear()
{
  for (vector<FeatureContainer* >::iterator it=_list.begin(); it!=_list.end(); it++){
    delete *it;
  }
  
  _list.clear();
}

bool ARFFHANDLERNAMESPACE::FeatureContainerSequence::append(ARFFHANDLERNAMESPACE::FeatureContainer* newFC)
{
  assert (newFC!=NULL);
  
  try {
    _list.push_back(new FeatureContainer(*newFC));
  } catch (std::bad_alloc& e){
    cerr << "Not enough memory: FeatureContainer (" << newFC << ") could not be appended to FeatureContainerSequence!\n";
    return false;
  }

  return true;
}



// **********************************************************************
// ***** TrainingsDataContainer stuff ***********************************

ARFFHANDLERNAMESPACE::TrainingsDataContainer::TrainingsDataContainer(unsigned int numberOfActivities)
{
  _numberOfActivities = numberOfActivities;
  _data.resize(_numberOfActivities);
}

ARFFHANDLERNAMESPACE::TrainingsDataContainer::~TrainingsDataContainer()
{
  for (unsigned int i=0; i<_numberOfActivities; i++){
    for (vector<FeatureContainerSequence* >::iterator it=_data[i].begin(); it!=_data[i].end(); it++){
      delete *it;
    }
  }
}

int ARFFHANDLERNAMESPACE::TrainingsDataContainer::getTotalNumberOfContainers() const
{
  int result=0;
  
  for (unsigned int i=0; i<_numberOfActivities; i++){
    for (unsigned int j=0; j<_data[i].size(); j++){
      result+= _data[i][j]->getSeqLength();
    }
  }
  
  assert (result>=0);
  return result;
}

void ARFFHANDLERNAMESPACE::TrainingsDataContainer::addData(const unsigned int activity, ARFFHANDLERNAMESPACE::FeatureContainerSequence* newData)
{
  assert (activity < _numberOfActivities);
  assert (newData!=NULL);
  
  _data[activity].push_back(newData);
}

void ARFFHANDLERNAMESPACE::TrainingsDataContainer::print(ostream& o, vector<string>* activityNames) const
{
  if (activityNames!=NULL && activityNames->size()>=_numberOfActivities){
    printDataWithActivities(o, activityNames);
  } else {
    printDataOnly(o);
  }
}

void ARFFHANDLERNAMESPACE::TrainingsDataContainer::printDataOnly(ostream& ostr) const
{
  int featuresPerRow = getFrame(0,0,0)->getNumberOfFeatures();
  FeatureContainer* f=NULL;
  
  for (unsigned int dataClass=0; dataClass<_numberOfActivities; dataClass++){
    for (unsigned int seqIt=0; seqIt<_data[dataClass].size(); seqIt++){
      for (int s=0; s<_data[dataClass][seqIt]->getSeqLength(); s++){
	f = getFrame(dataClass,seqIt,s);
	ostr << (*f)[0];
	for (int i=1; i<featuresPerRow; i++){
	  ostr << "," << (*f)[i];
	}
	ostr << "," << dataClass << endl;
      }
    }
  }
}

void ARFFHANDLERNAMESPACE::TrainingsDataContainer::printDataWithActivities(ostream& ostr, vector<string>* activityNames) const
{
  int featuresPerRow = getFrame(0,0,0)->getNumberOfFeatures();
  FeatureContainer* f=NULL;
  
  for (unsigned int dataClass=0; dataClass<_numberOfActivities; dataClass++){
    for (unsigned int seqIt=0; seqIt<_data[dataClass].size(); seqIt++){
      for (int s=0; s<_data[dataClass][seqIt]->getSeqLength(); s++){
	f = getFrame(dataClass,seqIt,s);
	ostr << (*f)[0];
	for (int i=1; i<featuresPerRow; i++){
	  ostr << "," << (*f)[i];
	}
	ostr << "," << (*activityNames)[dataClass].c_str() << endl;
      }
    }
  }
}


#if ARFFContainers_test

using namespace ARFFHANDLERNAMESPACE;

#define RESULT { cerr << "correct" << endl; } else { cerr << "error" << endl; }

void testFeatureContainer()
{
  cerr << "==== Start testing FeatureContainer ====" << endl;
  FeatureContainer fc1, fc2(7);
  FeatureContainer fc3(fc2);
  cerr << "  Built 3 local object" << endl;
  
  FeatureContainer *fcm1, *fcm2, *fcm3;
  fcm1 = new FeatureContainer();
  fcm2 = new FeatureContainer(7);
  fcm3 = new FeatureContainer(*fcm2);
  cerr << "  Built 3 objects with new" << endl;

  cerr << "  Empty constructor... ";
  if (fc1.getNumberOfFeatures()==0 && fc1.getDataPointer()==NULL) RESULT;

  cerr << "  Constructor with number... ";
  if (fc2.getNumberOfFeatures()==7 && fc2.getDataPointer()!=NULL){
    bool rwerror=false;
    for (unsigned int i=0; i<7; i++){
      fc2.getData(i);
      fc2.setData(i, i+2.13);
      if (fc2.getData(i)!=i+2.13){
	cerr << "error setting/reading index " << i << endl;
	rwerror = true;
      }
    }
    if (!rwerror) RESULT;
  } else { cerr << "error" << endl; }

  cerr << "  Constructor with another object... ";
  if (fc3.getNumberOfFeatures()==7 && fc3.getDataPointer()!=NULL){
    bool rwerror=false;
    for (unsigned int i=0; i<7; i++){
      fc3.getData(i);
      fc3.setData(i, i+2.13);
      if (fc3.getData(i)!=i+2.13){
	cerr << "error setting/reading index " << i << endl;
	rwerror = true;
      }
    }
    if (!rwerror) RESULT;
  } else { cerr << "error" << endl; }

  delete fcm1;
  delete fcm2;
  delete fcm3;
  cerr << "  Deleted the three objects built using new" << endl;
  
  cerr << "  getNumberOfFeatures()/setNumberOfFeatures()... ";
  bool gserror=false;
  for (unsigned int i=0; i<7; i++){
    double t1=fc2.getData(i);
    fc2.setData(i, (i+1.2)*3.4);
    if (fc2.getData(i)!=(i+1.2)*3.4){
      cerr << "error setting/getting at index " << i << endl;
      gserror = true;
    }
    fc2.setData(i, t1);
    if(fc2.getData(i)!=t1){
      cerr << "error setting/getting at index " << i << endl;
      gserror = true;
    }
  }
  if (!gserror) RESULT;

  cerr << "  Operatoren [] und = ... ";
  gserror = false;
  for (unsigned int i=0; i<7; i++){
    double t1=fc2[i];
    fc2[i] = (i + 1.2) * 3.4;
    if (fc2[i]!=(i+1.2)*3.4){
      cerr << "error setting/getting at index " << i << endl;
      gserror = true;
    }
    fc2[i] = t1;
    if(fc2[i]!=t1){
      cerr << "error setting/getting at index " << i << endl;
      gserror = true;
    }
  }
  if (!gserror) RESULT;

  cerr << "  isEmpty()...";
  if (fc1.isEmpty() && !(fc2.isEmpty()) && !(fc3.isEmpty())) RESULT;
  
  cerr << endl;
}

void testFeatureContainerSequence()
{
  cerr << endl << "==== Start testing FeatureContainerSequence ====" << endl;
  FeatureContainer fcsfc1(2), fcsfc2(2), fcsfc3(2);
  fcsfc1[0] = 1.1; fcsfc1[1] = 1.2;
  fcsfc2[0] = 2.1; fcsfc2[1] = 2.2;
  fcsfc3[0] = 3.1; fcsfc3[1] = 3.2;
  FeatureContainer *fcsfc1t, *fcsfc2t, *fcsfc3t;
  fcsfc1t = new FeatureContainer(fcsfc1);
  fcsfc2t = new FeatureContainer(fcsfc2);
  fcsfc3t = new FeatureContainer(fcsfc3);
  
  FeatureContainerSequence fcs1, fcs2, fcs3;
  fcs2.append(fcsfc1t);
  fcs3.append(fcsfc2t);
  fcs3.append(fcsfc3t);
  fcs3.clear();

  cerr << "  getSeqLength()... ";
  if (fcs1.getSeqLength()==0 && fcs2.getSeqLength()!=0 && fcs3.getSeqLength()==0) RESULT;

  fcsfc2t = new FeatureContainer(fcsfc2);
  fcsfc3t = new FeatureContainer(fcsfc3);
  fcs3.append(fcsfc2t); fcs3.append(fcsfc3t);
  cerr << "  append()... ";
  bool aperror=false;
  if (!((*fcs2.getContainer())[0]==fcsfc1[0]) && !((*fcs2.getContainer())[1]==fcsfc1[1])){ aperror = true; }
  if (!fcs3.getContainer(0) && !fcs3.getContainer(1)){ aperror = true; }
  if (!aperror) RESULT;

  cerr << "  getContainer()... ";
  bool gcerror=false;
  if (!((*fcs2.getContainer())[0]==fcsfc1[0]) && !((*fcs2.getContainer())[1]==fcsfc1[1])){ gcerror = true; }
  if (!((*fcs3.getContainer(0))[0]==fcsfc2[0]) && !((*fcs3.getContainer(0))[1]==fcsfc2[1])){ gcerror = true; }
  if (!((*fcs3.getContainer(1))[0]==fcsfc3[0]) && !((*fcs3.getContainer(1))[0]==fcsfc3[0])){ gcerror = true; }
  if (!gcerror) RESULT;
  
  cerr << "  clear()... ";
  bool clerror=false;
  if (fcs2.getSeqLength() && fcs3.getSeqLength()){
    fcs2.clear();
    fcs3.clear();
    if (!fcs2.getSeqLength() && !fcs3.getSeqLength()) RESULT;
  } else {
    cerr << "error" << endl;
  }

  cerr << endl;
}

#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make ARFFContainers_test
  // Then, run the resulting executable (ARFFContainers_test).
  // If it works as expected, the module is probably correct. ;-)

  cerr << endl << "===== ===== Testing ARFFContainers ===== =====" << endl << endl;

  testFeatureContainer();

  testFeatureContainerSequence();

  cerr << endl << "==== Start testing TrainingsDataContainer ====" << endl;
  cerr << "  not implemented yet" << endl << endl;
  
  return 0;
}
#endif /* Containers_test */
