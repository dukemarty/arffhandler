/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandler.cc

    Creator: Martin L�sch (<loesch@ira.uka.de>)
    Date of creation: 07.06.08

    Last Author: Martin Loesch (<professional@@martinloesch.net>)
    Date of last change: 22.05.13

    Revision: 0.1

    Copyright: Martin L�sch, Chair Prof. Dillmann (IAIM)
               Institute for Computer Science and Engineering (CSE)
	       University of Karlsruhe. All rights reserved
	       http://wwwiaim.ira.uka.de
    Author:    Martin L�sch (<loesch@ira.uka.de>)
    Date:      07.06.08
*/

/* system includes */
#include <assert.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

/* my includes */
#include "ARFFFileHandler.h"

namespace bpt = boost::posix_time;
namespace ba = boost::algorithm;



ArffFileHandling::ARFFFileHandlerSet::ARFFFileHandlerSet()
{
}

ArffFileHandling::ARFFFileHandlerSet::~ARFFFileHandlerSet()
{
  for (map<string, ARFFFileHandler* >::iterator it=dataset.begin(); it!=dataset.end(); ++it) {
    delete it->second;
  }
}


ArffFileHandling::ARFFFileHandler::ARFFFileHandler()
{
  _filename = "";
}

ArffFileHandling::ARFFFileHandler::ARFFFileHandler(string filename)
{
  _filename = filename;

  try {
    load(filename);
  } catch (ios_base::failure& e){
    _filename = "";
    _arffcontent.clear();
  }
}

ArffFileHandling::ARFFFileHandler::ARFFFileHandler(ARFFData* content)
{
  _arffcontent.replaceAndOwn(content);
}

ArffFileHandling::ARFFFileHandler::~ARFFFileHandler()
{
}

void ArffFileHandling::ARFFFileHandler::clearData()
{
  _arffcontent.clear();
}

void ArffFileHandling::ARFFFileHandler::printHeader(ostream& out, string filename) const
{
  // write meta header
  out << "% 1.  Title:" << endl << "% " << filename.c_str() << endl << "%" << endl;
  out << "% 2.  Created by:" << endl << "% ARFF Handler Library" << endl << "%" << endl;
  out << "% 3.  Creation date:" << endl << "% " << bpt::to_iso_string(bpt::microsec_clock::local_time()) << endl << "%" << endl;
  out << "% 4.  Creation time:" << endl << "% " << bpt::to_iso_string(bpt::microsec_clock::local_time()) << endl << "%" << endl;
  out << "% 5.  Number of Instances:" << endl << "% " << _arffcontent.getNumberOfInstances() << endl << "%" << endl;
  out << "% 6.  Number Of Attributes:" << endl << "% " << _arffcontent.getNumberOfFeatures() << endl << "%" << endl;
  out << "% 7.  Attribute Information:" << endl << "%" << endl;
  out << "% 8.  Class Distribution" << endl << endl;

  out << "@relation '" << filename.c_str() << "-" << _arffcontent.getNumberOfClasses() << "-classes'" << endl << endl;
	
  // write attributes
  for(unsigned int f=0; f<_arffcontent.getNumberOfFeatures(); f++){
    if (_arffcontent.getFeatureName(f) == "class"){
      out << "@attribute class {";
      out << _arffcontent.getClassName(0);
      for (unsigned int c=1; c<_arffcontent.getNumberOfClasses(); c++){
	out << "," << _arffcontent.getClassName(c);
      }
      out << "}" << endl << endl;
    } else {
      out << "@attribute " << _arffcontent.getFeatureName(f) << " numeric" << endl;
    }
  }
  out << endl;
    
  out << "@data" << endl;
}

ArffFileHandling::ARFFDataSet ArffFileHandling::ARFFFileHandler::splitClasses() const
{
  ARFFDataSet res;
  vector<string> names;
  
  for (unsigned int i=0; i<_arffcontent.getNumberOfClasses(); ++i){
    names.clear();
    names.push_back(_arffcontent.getClassName(i));
    res.dataset[_arffcontent.getClassName(i)] = _arffcontent.filterClassData(names);
  }
  
  return res;
}

bool ArffFileHandling::ARFFFileHandler::load(string filename)
{
  clearData();
  
  //Open given File
  ifstream arffFile;                          
  arffFile.open(filename.c_str(), ios_base::in);
  
  //Successfully opened?
  if (!arffFile){    
    cerr << "Throwing exception ios_base::failure\n";
    throw ios_base::failure("Open .arff file for loading feature sequence");
  }

  /////////////////// Read attributes ///////////////////////////
  char buf[3072];
  string attrName;
  int attributeCountAll = 0;
  while (arffFile.getline(buf, sizeof(buf))){
    string bufString(buf);
    //Check only lines starting with "@attribute"
    if (ba::starts_with(ba::to_lower_copy(bufString), "@attribute")) {
      //Check FeatureNumber
      vector<string> tokensAfterSpaceSplit;
      ba::split(tokensAfterSpaceSplit, bufString, ba::is_any_of(" "));
      vector<string> tokens;
      for (vector<string>::iterator itSpace=tokensAfterSpaceSplit.begin(); itSpace!=tokensAfterSpaceSplit.end(); itSpace++){
	vector<string> tokensAfterTabSplit;
	ba::split(tokensAfterTabSplit, *itSpace, ba::is_any_of("\t "));
	for (vector<string>::iterator itTab=tokensAfterTabSplit.begin(); itTab!=tokensAfterTabSplit.end(); itTab++){
	  if (*itTab != ""){
	    tokens.push_back(*itTab);
	  }
	}
      }

      attrName = tokens[1];
      if (ba::to_lower_copy(attrName) == "class"){
	string classline(buf);
	// parsing valid class values
	int openPos = classline.find("{");
	int closePos = classline.find("}");
	string values = classline.substr(openPos + 1, closePos - openPos - 1);
	vector<string> classValTokens;
	ba::split(classValTokens, values, ba::is_any_of(","));
	for (unsigned int i=0; i<classValTokens.size(); i++){
	  _arffcontent.addClass(i, classValTokens[i]);
	}
      } else {
	if ( (ba::to_lower_copy(tokens[2]) != "numeric") && (ba::to_lower_copy(tokens[2]) != "real") ){
	  cerr << "Throwing exception domain_error\n";
	  throw domain_error("Attribute is not numeric, but : <" + tokens[2] + ">");
	}
      }
      _arffcontent.addFeature(attributeCountAll, attrName);
      attributeCountAll++;
    } else if (ba::starts_with(ba::to_lower_copy(bufString), "@data")) {
      //Break if entering the data part
      break;
    }
  }



  
  /////////////////// Read data /////////////////////////////////
  _arffcontent.initData();

  //Reading data part line by line 
  FeatureContainerSequence* currentSequence=NULL;
  FeatureContainer* instance=NULL;
  double newData;	//stores one attribute in one line
  int currentClass=-1;
  
  while (arffFile.getline(buf, sizeof(buf))){
    string bufString(buf);
    //ignore comments, and if no "," is in the line, ignore it
    if((bufString[0] == '%') || (bufString[0] == '\n') || (bufString.find(",") == string::npos) ){
      continue;
    }

    //One instance per line
    delete instance;
    instance = new FeatureContainer(_arffcontent.getNumberOfFeatures() - 1);

    
    //Split the line at the separators
    vector<string> tokens;
    ba::split(tokens, bufString, ba::is_any_of(","));
    for (unsigned int attrCount=0; attrCount<tokens.size(); attrCount++){
      if( _arffcontent.getFeatureName(attrCount) != "class"){
	istringstream iss(tokens[attrCount]);
	iss >> newData;
	instance->setData(attrCount, newData);
      } else {
	int instanceClass = _arffcontent.getClassIndex(tokens[attrCount]);
	if (instanceClass != currentClass){ // have to start a new feature container sequence
	  if (currentSequence){
	    _arffcontent.addDataSequence(currentClass, currentSequence);
	  }
	  currentSequence = new FeatureContainerSequence();
	  currentClass = instanceClass;
	}
      }
    }
    
    currentSequence->append(instance);
  }
  delete instance;
  instance = NULL;

  _arffcontent.addDataSequence(currentClass, currentSequence);
	
  arffFile.close();

  cerr << endl << "--------------------------------------" << endl;
  cerr << "  ArffFileHandling::load()" << endl;
  TrainingsDataContainer* temp = _arffcontent.getData();
  cerr << "Address of TrainingsDataContainer :  " << temp << endl;
  cerr << "Number of activities loaded :  " << temp->getNumberOfActivities() << endl;
  for (unsigned int i=0; i<temp->getNumberOfActivities(); ++i){
    cerr << "Number of sequences loaded for activity=" << i << " :  " << temp->getNumberOfSequences(i) << endl;
    for (int j=0; j<temp->getNumberOfSequences(i); ++j){
      cerr << "    Length of sequence " << j << " :  " << temp->getSequenceLength(i, j) << endl;
    }
  }
  cerr << "--------------------------------------" << endl;
  
  return _arffcontent.isValid();
}

bool ArffFileHandling::ARFFFileHandler::save(string filename) const
{
  ofstream outfile(filename.c_str(), fstream::out);

  // write header
  printHeader(outfile, filename);

  // write actual data
  _arffcontent.printData(outfile);
  
  outfile.close();
  
  return _arffcontent.isValid();
}

bool ArffFileHandling::ARFFFileHandler::removeFeature(int index)
{
  return _arffcontent.removeFeature(index);
}

int ArffFileHandling::ARFFFileHandler::removeFeature(string name)
{
  int res=0;
  
  if (_arffcontent.isFeatureUnique(name)){
    if (_arffcontent.removeFeature(name)){
      res = 0;
    } else {
      res = 1;
    }
  } else {
    res = 2;
  }

  return res;
}

ArffFileHandling::ARFFFileHandlerSet ArffFileHandling::ARFFFileHandler::splitSingleClasses() const
{
  ARFFDataSet dataset = splitClasses();
  ARFFFileHandlerSet res;

  for (map<string, ARFFData* >::iterator it=dataset.begin(); it!=dataset.end(); ++it){
    res.dataset[it->first] = new ARFFFileHandler(it->second);
  }

  return res;
}

ArffFileHandling::ARFFData* ArffFileHandling::ARFFFileHandler::getData()
{
  return &_arffcontent;
}



#if ARFFFileHandler_test

using namespace std;

#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make ARFFFileHandler_test
  // Then, run the resulting executable (ARFFFileHandler_test).
  // If it works as expected, the module is probably correct. ;-)

  fprintf(stderr, "Testing ARFFFileHandler\n");

  return 0;
}
#endif /* ARFFFileHandler_test */
