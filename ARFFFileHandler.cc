/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandler.cc

    Creator: Martin Lösch (<loesch@ira.uka.de>)
    Date of creation: 07.06.08

    Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    Date of last change: 27.11.09

    Revision: 0.1

    Copyright: Martin Lösch, Chair Prof. Dillmann (IAIM)
               Institute for Computer Science and Engineering (CSE)
	       University of Karlsruhe. All rights reserved
	       http://wwwiaim.ira.uka.de
    Author:    Martin Lösch (<loesch@ira.uka.de>)
    Date:      07.06.08
*/

/* system includes */
#include <assert.h>
#include <fstream>
#include <stdexcept>

/* my includes */
#include "ARFFFileHandler.h"
// #include "Enums.h"


ArffFileHandling::ARFFFileHandler::ARFFFileHandler()
{
  _valid = false;
  _filename = "";
  _data = NULL;
}

ArffFileHandling::ARFFFileHandler::ARFFFileHandler(string filename)
{
  _filename = filename;
  _valid = true;
  _data = NULL;

  try {
    load(filename);
  } catch (ios_base::failure& e){
    _filename = "";
    _featList.clear();
    _valid = false;
    
    delete _data;
    _data = NULL;
  }
}

ArffFileHandling::ARFFFileHandler::~ARFFFileHandler()
{
  delete _data;
}

void ArffFileHandling::ARFFFileHandler::clearData()
{
  delete _data;
  _data = NULL;

  _featList.clear();
  _class2index.clear();
  _index2class.clear();

  _valid = false;
  
  assert (_valid == false);
}

void ArffFileHandling::ARFFFileHandler::printHeader(fstream& out, string filename) const
{
  // write meta header
  out << "% 1.  Title:" << endl << "% " << filename.c_str() << endl << "%" << endl;
  out << "% 2.  Created by:" << endl << "% ARFF Handler Library" << endl << "%" << endl;
  out << "% 3.  Creation date:" << endl << "% " << bg::time::now().toString(true).c_str() << endl << "%" << endl;
  out << "% 4.  Creation time:" << endl << "% " << bg::time::now().toString(true).c_str() << endl << "%" << endl;
  out << "% 5.  Number of Instances:" << endl << "% " << _data->getTotalNumberOfContainers() << endl << "%" << endl;
  out << "% 6.  Number Of Attributes:" << endl << "% " << getNumberOfFeatures() << endl << "%" << endl;
  out << "% 7.  Attribute Information:" << endl << "%" << endl;
  out << "% 8.  Class Distribution" << endl << endl;

  out << "@relation '" << filename.c_str() << "-" << _class2index.size() << "-classes'" << endl << endl;
	
  // write attributes
  for(unsigned int f=1; f<_featList.size(); f++){
    if (_featList.find(f)->second == "class"){
      out << "@attribute class {";
      out << _index2class.find(0)->second;
      for (unsigned int c=1; c<_index2class.size(); c++){
	out << "," << _index2class.find(c)->second;
      }
      out << "}" << endl << endl;
    } else {
      out << "@attribute " << _featList.find(f)->second << " numeric" << endl;
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
      bg::strlist tokens = bg::string(buf).split(" ");
      attrName = tokens[1];
      if (attrName == "class"){
	string classline(buf);
	// parsing valid class values
	int openPos = classline.find("{");
	int closePos = classline.find("}");
	bg::string values = classline.substr(openPos + 1, closePos - openPos - 1);
	bg::strlist classValTokens = values.split(",");
	for (unsigned int i=0; i<classValTokens.size(); i++){
	  _class2index[classValTokens[i]] = i;
	  _index2class[i] = classValTokens[i];
	}
      } else {
	if (tokens[2] != "numeric"){
	  BGDBG (3, "Throwing exception domain_error\n");
	  throw domain_error("Attribute is not numeric");
	}
      }
      _featList[attributeCountAll] = attrName;
      attributeCountAll++;
    } else if(bg::string(buf).startsWith("@data")) {
      //Break if entering the data part
      break;
    }
  }

  /////////////////// Read data /////////////////////////////////
  _data = new TrainingsDataContainer(_class2index.size());

  _valid = true;

  //Reading data part line by line 
  FeatureContainerSequence* currentSequence=NULL;
  FeatureContainer* instance;
  double newData;	//stores one attribute in one line
  int currentClass=_class2index.size();
  
  while (arffFile.getline(buf, sizeof(buf))){
    //ignore comments, and if no "," is in the line, ignore it
    if(((bg::string(buf))[0] == '%') || ((bg::string(buf))[0] == '\n') || (bg::string(buf).find(",") == bg::string::npos) ){
      continue;
    }

    //One instance per line
    instance = new FeatureContainer(getNumberOfFeatures());

    
    //Split the line at the separators
    bg::strlist tokens = bg::string(buf).split(",");
    for (unsigned int attrCount=0; attrCount<tokens.size(); attrCount++){
      //save only attributes which are in the InputFeatureList (which means they translate to unequal -1)
      if( _featList[attrCount] != "class"){
	newData = tokens[attrCount].toFloat();
	instance->setData(attrCount, newData);
      } else {
	int instanceClass = _class2index[tokens[attrCount]];
	if (instanceClass != currentClass){ // have to start a new feature container sequence
	  if (currentSequence){
	    _data->addData(currentClass, currentSequence);
	  }
	  currentSequence = new FeatureContainerSequence();
	  currentClass = instanceClass;
	}
      }
    }
    
    currentSequence->append(instance);
  }

  _data->addData(currentClass, currentSequence);
	
  arffFile.close();
  
  return _valid;
}

bool ArffFileHandling::ARFFFileHandler::save(string filename) const
{
  fstream outfile(filename.c_str(), fstream::out);

  // write header
  printHeader(outfile, filename);

  // prepare class names
  bg::strlist classNames;
  for (unsigned int i=0; i<_index2class.size(); i++){
    classNames.push_back(_index2class.find(i)->second);
  }
  
  // write actual data
  _data->print(outfile, &classNames);
  
  outfile.close();
  
  return _valid;
}

ArffFileHandling::TrainingsDataContainer* ArffFileHandling::ARFFFileHandler::getData() const
{
  return _data;
}

int ArffFileHandling::ARFFFileHandler::getNumberOfFeatures() const
{
  return _featList.size();
}


// bool CHARMinputmodules::ARFFFileHandler::append(FeatureContainerSequence* sequence, bg::string filename) throw (FalseRequestError)
// {
//   return writeSequence(sequence, filename, true);
// }

// bool CHARMinputmodules::ARFFFileHandler::append(FeatureContainer* dataPoint, bg::string filename) throw (FalseRequestError)
// {
//   FeatureContainerSequence* fcs = new FeatureContainerSequence();
//   fcs->append(dataPoint);

//   return ARFFFileHandler::append(fcs, filename);
// }

// bool CHARMinputmodules::ARFFFileHandler::writeSequence(FeatureContainerSequence* sq, bg::string filename, bool append, InputFeatureList* features)
// {
//   //open given file as a filestream in given mode
//   fstream f(filename.c_str(), (append ? fstream::out|fstream::app : fstream::out));
	
//   //Write header only if not appending
//   if(!append) {
//     //Write @relation
//     f << "@relation 'Written by ARRFFileHandler'" << endl << endl;
	
//     //Write attributes
//     FeatureContainer* firstFC = sq->getContainer(0);
//     for(int i=1; i <= firstFC->getNumberOfFeatures(); ++i){
//       f << "@attribute " << getFeatureName(static_cast<FeatureNumber>(i)) << " numeric" << endl;
//     }
//     f << endl;
    
//     //Write datapart
//     f << "@data" << endl;
//   }
  
//   //Write Ddata from the given sequence
//   FeatureContainer* curFC;
//   for(int i=0; i<sq->getSeqLength(); ++i){
//     curFC = sq->getContainer(i);
//     //Append each value separated by , (except the last one)
//     for(int j=0; j<curFC->getNumberOfFeatures() - 1; ++j){
//       f << curFC->getData(j) << ",";
//     }
//     //no comma after last value, but start a new line
//     f << curFC->getData(curFC->getNumberOfFeatures() - 1) << endl;
//   }
	
//   //finished
// //   return f.close();
// //   return f.good();
//   return true;
// }


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

  cerr << "Test ARFFload: " << endl;

  bg::string path="/Users/martinloesch/Source/projects/ActivityRecognition/NoveltyDetector/TestDaten/";
bg::string file = path + "2D-Testdata1a.arff";
//	"readbook_selected_truncated.arff";
//	"readbook_complete_truncated10.arff";
	
  InputFeatureList* ifList = new InputFeatureList();
  FeatureContainerSequence* sq=NULL;

  try {
    sq = CHARMinputmodules::ARFFFileHandler::load(file, ifList);
  } catch (CHARMerrors::FileError& e){
    cerr << "Catched the following error: " << endl;
    cerr << "   " << e.toString().c_str() << endl;
  }
  
  if(sq==NULL){
    cerr<<"Gelesene Sequenz ist NULL"<<endl;
    return false;
  }
  
  FeatureContainer* firstline=sq->getContainer(0);
  cerr << "FeatureContainer firstline=" << firstline << endl;
  for (unsigned int i=0; i<firstline->getNumberOfFeatures(); i++){
    cerr << "   " << (*firstline)[i];
  }
  cerr << endl;
  
  return 0;
}
#endif /* ARFFFileHandler_test */
