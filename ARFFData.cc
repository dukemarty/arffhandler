/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFData.cc

    \par Last Author: Martin Loesch (<professional@@martinloesch.net>)
    \par Date of last change: 31.05.13

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
#include <algorithm>
#include <stdexcept>

/* my includes */
#include "ARFFData.h"


ArffFileHandling::ARFFDataSet::ARFFDataSet()
{
}

ArffFileHandling::ARFFDataSet::~ARFFDataSet()
{
  for (map<string, ARFFData* >::iterator it=dataset.begin(); it!=dataset.end(); ++it) {
    delete it->second;
  }
}


ArffFileHandling::ARFFData::ARFFData()
{
  _data = NULL;
  _valid = false;
}

ArffFileHandling::ARFFData::~ARFFData()
{
  delete _data;
}

void ArffFileHandling::ARFFData::copyMetaData(const ARFFData& rvalue)
{
  _featList = rvalue._featList;
  _class2index = rvalue._class2index;
  _index2class = rvalue._index2class;
}

void ArffFileHandling::ARFFData::replaceAndOwn(ARFFData* content)
{
  delete _data;
  _data = content->_data;

  _valid = (_data != NULL);

  copyMetaData(*content);

  content->_data = NULL;
}

bool ArffFileHandling::ARFFData::checkFeatureListValidity() const
{
  assert (_data!=NULL);
  
  bool validness=true;

  if (_data->getNumberOfActivities()==0 || _data->getNumberOfSequences(0)==0){
    return true;
  }
  
  if (_featList.size()!=(_data->getFrame(0, 0, 0)->getNumberOfFeatures()+1)){
    validness = false;
  }

  for (unsigned int i=0; i<_featList.size(); i++){
    if (_featList.count(static_cast<int>(i))!=1){
      validness = false;
      break;
    }
  }

  return validness;
}

bool ArffFileHandling::ARFFData::checkClassListValidity() const
{
  bool validness=true;
  
  if (_data==NULL || _class2index.size()!=_data->getNumberOfActivities()){
    return false;
  }

  if (_class2index.size()!=_index2class.size()){
    validness = false;
  } else {
    for (ClassValuesByIndex::const_iterator it=_index2class.begin(); it!=_index2class.end(); ++it){
      if (_class2index.find(it->second)==_class2index.end() || it->first!=_class2index.find(it->second)->second){
	validness = false;
      }
    }
    for (ClassValuesByName::const_iterator it=_class2index.begin(); it!=_class2index.end(); ++it){
      if (_index2class.find(it->second)==_index2class.end() || it->first!=_index2class.find(it->second)->second){
	validness = false;
      }
    }
  }
  
  return validness;
}

bool ArffFileHandling::ARFFData::checkValidity() const
{
  bool validness=true;

  if (_data==NULL){
    validness = false;
  } else {
    
    if (!checkFeatureListValidity()){
      validness = false;
    }
    
    if (!checkClassListValidity()){
      validness = false;
    }

  }

  return validness;
}

vector<int> ArffFileHandling::ARFFData::findIndizesForFeature(string name) const
{
  vector<int> results;
  
  for (FeatureList::const_iterator it=_featList.begin(); it!=_featList.end(); ++it){
    if (it->second==name){
      results.push_back(it->first);
    }
  }

  return  results;
}

bool ArffFileHandling::ARFFData::removeFeatureConsistently(unsigned int index)
{
  assert (checkFeatureListValidity());
  
  if (index>=_featList.size()){ return false; }
  
  for (unsigned int i=index; i<_featList.size()-1; ++i){
    _featList[i] = _featList[i+1];
  }
  _featList.erase(_featList.size()-1);
  
  _valid = checkValidity();

  return true;
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
  if (index >= _featList.size()){
    throw std::out_of_range("Requested feature does not exists.");
  }
  
  return _featList.find(index)->second;
}

int ArffFileHandling::ARFFData::getFeatureIndex(string name) const
{
  vector<int> indizes = findIndizesForFeature(name);
  
  int res=-1;
  if (indizes.size()>0){
    sort(indizes.begin(), indizes.end());
    res = indizes[0];
  }

  return  res;
}

void ArffFileHandling::ARFFData::addFeature(unsigned int index, string name)
{
  _featList[index] = name;

  _valid = checkValidity();
}

bool ArffFileHandling::ARFFData::isFeatureUnique(string name) const
{
  vector<int> indizes = findIndizesForFeature(name);

  return (indizes.size()==1);
}

unsigned int ArffFileHandling::ARFFData::getNumberOfClasses() const
{
  return _class2index.size();
}

string ArffFileHandling::ARFFData::getClassName(unsigned int index) const
{
  string res="";
  ClassValuesByIndex::const_iterator it = _index2class.find(index);

  if (it!=_index2class.end()){ res = it->second; }
  
  return res;
}

int ArffFileHandling::ARFFData::getClassIndex(string name) const
{
  int res=-1;
  ClassValuesByName::const_iterator it = _class2index.find(name);

  if (it!=_class2index.end()){ res = it->second; }

  return res;
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

  _valid = checkValidity();
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
  vector<string> classNames;
  for (unsigned int i=0; i<getNumberOfClasses(); i++){
    classNames.push_back(getClassName(i));
  }
  
  _data->print(outstream, &classNames);
}

bool ArffFileHandling::ARFFData::removeFeature(unsigned int index)
{
  assert (_valid==true);
  
  if (index>_featList.size()){ return false; }

  bool res=true;
  
  //! \todo Remove feature from list
  res = removeFeatureConsistently(index);

  if (res){
    res = _data->removeFeatureDataCompletely(index);
  }

  _valid = checkValidity();

  return res;
}

bool ArffFileHandling::ARFFData::removeFeature(string name)
{
  int index = getFeatureIndex(name);
  bool res=false;

  if (index==-1){
    res = false;
  } else {
    res = removeFeature(index);
  }

  return res;
}

ArffFileHandling::ARFFData* ArffFileHandling::ARFFData::filterClassData(vector<string> classnames) const
{
  if (classnames.size()==0){ return NULL; }

  ARFFData* res = new ARFFData();
  map<int, int> mapOld2New;

  int i=0;
  for (vector<string>::iterator nameIt=classnames.begin(); nameIt!=classnames.end(); ++nameIt){
    ClassValuesByName::const_iterator it = _class2index.find(*nameIt);
    if (it!=_class2index.end()){
      res->addClass(i, *nameIt);
      mapOld2New[it->second] = i;
      ++i;
    }
  }

  res->_featList = _featList;
  res->initData();
  TrainingsDataContainer* resData = res->getData();

  for (unsigned int actI=0; actI<_data->getNumberOfActivities(); ++actI){
    if (mapOld2New.count(actI)==0) continue;
    for (int seqI=0; seqI<_data->getNumberOfSequences(actI); ++seqI){
      resData->addData(mapOld2New[actI], new FeatureContainerSequence(*(_data->getSequence(actI, seqI))));
    }
  }

  return res;
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
