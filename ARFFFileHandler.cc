/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandler.cc

    Creator: Martin L�sch (<loesch@ira.uka.de>)
    Date of creation: 07.06.08

    Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    Date of last change: 17.12.09

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
#include <stdexcept>

/* my includes */
#include "ARFFFileHandler.h"


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
  out << "% 3.  Creation date:" << endl << "% " << bg::time::now().toString(true).c_str() << endl << "%" << endl;
  out << "% 4.  Creation time:" << endl << "% " << bg::time::now().toString(true).c_str() << endl << "%" << endl;
  out << "% 5.  Number of Instances:" << endl << "% " << _arffcontent.getNumberOfInstances() << endl << "%" << endl;
  out << "% 6.  Number Of Attributes:" << endl << "% " << _arffcontent.getNumberOfFeatures() << endl << "%" << endl;
  out << "% 7.  Attribute Information:" << endl << "%" << endl;
  out << "% 8.  Class Distribution" << endl << endl;

  out << "@relation '" << filename.c_str() << "-" << _arffcontent.getNumberOfClasses() << "-classes'" << endl << endl;
	
  // write attributes
  for(unsigned int f=1; f<_arffcontent.getNumberOfFeatures(); f++){
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

bool ArffFileHandling::ARFFFileHandler::load(string filename)
{
  clearData();
  
  //Open given File
  ifstream arffFile;                          
  arffFile.open(filename.c_str(), ios_base::in);
  
  //Successfully opened?
  if (!arffFile){    
    BGDBG (3, "Throwing exception ios_base::failure\n");
    throw ios_base::failure("Open .arff file for loading feature sequence");
  }

  /////////////////// Read attributes ///////////////////////////
  char buf[3072];
  bg::string attrName;
  int attributeCountAll = 0;
  while (arffFile.getline(buf, sizeof(buf))){
    //Check only lines starting with "@attribute"
    if(bg::string(buf).startsWith("@attribute")) {
      //Check FeatureNumber
      bg::strlist tokensAfterSpaceSplit = bg::string(buf).split(" ");
      bg::strlist tokens;
      for (bg::strlist::iterator itSpace=tokensAfterSpaceSplit.begin(); itSpace!=tokensAfterSpaceSplit.end(); itSpace++){
	bg::strlist tokensAfterTabSplit = itSpace->split("\t");
	for (bg::strlist::iterator itTab=tokensAfterTabSplit.begin(); itTab!=tokensAfterTabSplit.end(); itTab++){
	  tokens.push_back(*itTab);
	}
      }
      
      attrName = tokens[1];
      if (attrName == "class"){
	string classline(buf);
	// parsing valid class values
	int openPos = classline.find("{");
	int closePos = classline.find("}");
	bg::string values = classline.substr(openPos + 1, closePos - openPos - 1);
	bg::strlist classValTokens = values.split(",");
	for (unsigned int i=0; i<classValTokens.size(); i++){
	  _arffcontent.addClass(i, classValTokens[i]);
	}
      } else {
	if (tokens[2] != "numeric"){
	  BGDBG (3, "Throwing exception domain_error\n");
	  throw domain_error("Attribute is not numeric");
	}
      }
      _arffcontent.addFeature(attributeCountAll, attrName);
      attributeCountAll++;
    } else if(bg::string(buf).startsWith("@data")) {
      //Break if entering the data part
      break;
    }
  }

  /////////////////// Read data /////////////////////////////////
  _arffcontent.initData();

  //Reading data part line by line 
  FeatureContainerSequence* currentSequence=NULL;
  FeatureContainer* instance;
  double newData;	//stores one attribute in one line
  int currentClass=_arffcontent.getNumberOfClasses();
  
  while (arffFile.getline(buf, sizeof(buf))){
    //ignore comments, and if no "," is in the line, ignore it
    if(((bg::string(buf))[0] == '%') || ((bg::string(buf))[0] == '\n') || (bg::string(buf).find(",") == bg::string::npos) ){
      continue;
    }

    //One instance per line
    instance = new FeatureContainer(_arffcontent.getNumberOfFeatures());

    
    //Split the line at the separators
    bg::strlist tokens = bg::string(buf).split(",");
    for (unsigned int attrCount=0; attrCount<tokens.size(); attrCount++){
      if( _arffcontent.getFeatureName(attrCount) != "class"){
	newData = tokens[attrCount].toFloat();
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

  _arffcontent.addDataSequence(currentClass, currentSequence);
	
  arffFile.close();
  
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
