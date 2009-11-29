/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFData.cc

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change: 29.11.09

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

/* my includes */
#include "ARFFData.h"

ArffFileHandling::ARFFData::ARFFData()
{
  _data = NULL;
  _valid = false;
}

ArffFileHandling::ARFFData::~ARFFData()
{
  delete _data;
}

bool ArffFileHandling::ARFFData::isValid() const
{
  return _valid;
}

void ArffFileHandling::ARFFData::clear()
{
  _featList.clear();
  _class2index.clear();
  _index2class.clear();

  delete _data;
  _data = NULL;

  _valid = false;

  assert (_valid == false);
}

unsigned int ArffFileHandling::ARFFData::getNumberOfFeatures() const
{
  return _featList.size();
}

string ArffFileHandling::ARFFData::getFeatureName(unsigned int index) const
{
  return _featList.find(index)->second;
}

void ArffFileHandling::ARFFData::addFeature(unsigned int index, string name)
{
  _featList[index] = name;
}

unsigned int ArffFileHandling::ARFFData::getNumberOfClasses() const
{
  return _class2index.size();
}

string ArffFileHandling::ARFFData::getClassName(unsigned int index) const
{
  return _index2class.find(index)->second;
}

unsigned int ArffFileHandling::ARFFData::getClassIndex(string name) const
{
  return _class2index.find(name)->second;
}

void ArffFileHandling::ARFFData::addClass(unsigned int index, string name)
{
  _class2index[name] = index;
  _index2class[index] = name;
}

ArffFileHandling::TrainingsDataContainer* ArffFileHandling::ARFFData::getData() const
{
  return _data;
}

int ArffFileHandling::ARFFData::getNumberOfInstances() const
{
  assert (_valid==true);
  
  return _data->getTotalNumberOfContainers();
}

void ArffFileHandling::ARFFData::initData()
{
  delete _data;
  _data = NULL;
  _valid = false;

  _data = new TrainingsDataContainer(getNumberOfClasses());
  _valid = true;

  assert (_valid==true);
}

void ArffFileHandling::ARFFData::addDataSequence(unsigned int classindex, FeatureContainerSequence* data)
{
  assert (_valid==true);
  
  _data->addData(classindex, data);
}

void ArffFileHandling::ARFFData::printData(ostream& outstream) const
{
  assert (_valid==true);
  
  // prepare class names
  bg::strlist classNames;
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
