/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFData.cc

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change: 09.08.10

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2009-11-29
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (IAIM)\n
              Institute for Computer Science and Engineering (CSE)\n
	      University of Karlsruhe. All rights reserved\n
	      http://wwwiaim.ira.uka.de
*/

/* system includes */
#include <assert.h>
#include <vector>
#include <stdexcept>

/* my includes */
#include "ARFFData.h"

ARFFHANDLERNAMESPACE::ARFFData::ARFFData()
{
  _data = NULL;
  _valid = false;
}

ARFFHANDLERNAMESPACE::ARFFData::~ARFFData()
{
  delete _data;
}

bool ARFFHANDLERNAMESPACE::ARFFData::isValid() const
{
  return _valid;
}

void ARFFHANDLERNAMESPACE::ARFFData::clear()
{
  _featList.clear();
  _class2index.clear();
  _index2class.clear();

  delete _data;
  _data = NULL;

  _valid = false;

  assert (_valid == false);
}

unsigned int ARFFHANDLERNAMESPACE::ARFFData::getNumberOfFeatures() const
{
  return _featList.size();
}

string ARFFHANDLERNAMESPACE::ARFFData::getFeatureName(unsigned int index) const
{
  if (index >= _featList.size()){
    throw std::out_of_range("Requested feature does not exists.");
  }
  
  return _featList.find(index)->second;
}

void ARFFHANDLERNAMESPACE::ARFFData::addFeature(unsigned int index, string name)
{
  _featList[index] = name;
}

unsigned int ARFFHANDLERNAMESPACE::ARFFData::getNumberOfClasses() const
{
  return _class2index.size();
}

string ARFFHANDLERNAMESPACE::ARFFData::getClassName(unsigned int index) const
{
  string res="";
  ClassValuesByIndex::const_iterator it = _index2class.find(index);

  if (it!=_index2class.end()){ res = it->second; }
  
  return res;
}

int ARFFHANDLERNAMESPACE::ARFFData::getClassIndex(string name) const
{
  int res=-1;
  ClassValuesByName::const_iterator it = _class2index.find(name);

  if (it!=_class2index.end()){ res = it->second; }

  return res;
}

void ARFFHANDLERNAMESPACE::ARFFData::addClass(unsigned int index, string name)
{
  _class2index[name] = index;
  _index2class[index] = name;
}

ARFFHANDLERNAMESPACE::TrainingsDataContainer* ARFFHANDLERNAMESPACE::ARFFData::getData() const
{
  return _data;
}

int ARFFHANDLERNAMESPACE::ARFFData::getNumberOfInstances() const
{
  assert (_valid==true);
  
  return _data->getTotalNumberOfContainers();
}

void ARFFHANDLERNAMESPACE::ARFFData::initData()
{
  delete _data;
  _data = NULL;
  _valid = false;
  
  _data = new TrainingsDataContainer(getNumberOfClasses());
  _valid = true;

  assert (_valid==true);
}

void ARFFHANDLERNAMESPACE::ARFFData::addDataSequence(unsigned int classindex, FeatureContainerSequence* data)
{
  assert (_valid==true);
  
  _data->addData(classindex, data);
}

void ARFFHANDLERNAMESPACE::ARFFData::printData(ostream& outstream) const
{
  assert (_valid==true);
  
  // prepare class names
  vector<string> classNames;
  for (unsigned int i=0; i<getNumberOfClasses(); i++){
    classNames.push_back(getClassName(i));
  }
  
  _data->print(outstream, &classNames);
}


#if ARFFData_test
#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make ARFFData_test
  // Then, run the resulting executable (ARFFData_test).
  // If it works as expected, the module is probably correct. ;-)

  fprintf(stderr, "Testing ARFFData\n");

  return 0;
}
#endif /* ARFFData_test */
