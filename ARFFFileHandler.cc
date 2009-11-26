/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandler.cc

    Creator: Martin Lösch (<loesch@ira.uka.de>)
    Date of creation: 07.06.08

    Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    Date of last change: 26.11.09

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

/* my includes */
#include "ARFFFileHandler.h"
#include "Enums.h"

using namespace std;


ArffFileHandling::TrainingsDataContainer* ArffFileHandling::ARFFFileHandler::load(string filename)
{
  //Open given File
  ifstream arffFile;                          
  arffFile.open(filename.c_str(), ios_base::in);
  
  //Successfully opened?
  if (!arffFile){    
    BGDBG (3, "Throwing exception FileError\n");
    throw FileError(__FILE__, __LINE__, filename, GenericFileError, "Open .arff file for loading feature sequence");
  }

  list = new InputFeatureList();
	
  /////////////////// Read attributes ///////////////////////////
  //Add attributes with FeatureNumber unequal to fnNoFeatureNumber, to InputFeatureList
  char buf[3072];	//TODO Improve: zeilenweises lesen umstellen auf blockweise
  bg::string attrName;
  int attributeCountAll = 0;
  while (arffFile.getline(buf, sizeof(buf))){
    //Check only lines starting with "@attribute"
    if(bg::string(buf).startsWith("@attribute")) {
      attributeCountAll++;
      //Check FeatureNumber
      bg::strlist tokens = bg::string(buf).split(" ");
      attrName = tokens[1];
      if(makeFeatureNumber(attrName) != fnNoFeatureNumber) {
	list->addFeature(makeFeatureNumber(attrName));
      } else {
      }
    } else if(bg::string(buf).startsWith("@data")) {
      //Break if entering the data part
      break;
    }
  }
  
  // Reset get pointer position to stream beginning
  arffFile.seekg( 0, ios_base::beg );
  
	
  /////////////////// Build translation table ///////////////////
  //Build a translation table for AttributeIndex (in the ARFF-File)  => index in the InputeFeatureList
  int translationTable[attributeCountAll];
  int attributeIndex = 0;	//index in the ARFF-File
  while (arffFile.getline(buf, sizeof(buf))){
    //Check only lines starting with "@attribute"
    if(bg::string(buf).startsWith("@attribute")) {
      bg::strlist tokens = bg::string(buf).split(" ");
      attrName = tokens[1].c_str();
      translationTable[attributeIndex] = list->findFeature(makeFeatureNumber(attrName));
      attributeIndex++;
    } else if(bg::string(buf).startsWith("@data")) {
      //Break if entering the data part
      break;
    }
  }
  
//   cout<<"Translation table:"<<endl;
//   for(int i = 0; i < attributeCountAll; i++){
//     cout << "[" << i << "]=" << translationTable[i] << " , ";
//     if(i%20 == 0){ cout<<endl; }
//   }
//   cout<<endl;
	
	
	
  /////////////////// Read data /////////////////////////////////
  //Reading data part line by line 
  FeatureContainerSequence* data = new FeatureContainerSequence();
  FeatureContainer* instance;
  FLOAT_T newData;	//stores one attribute in one line
  int instanceCounter=0;
  int listLen = list->getNumber();
  while (arffFile.getline(buf, sizeof(buf))){
    //ignore comments, and if no "," is in the line, ignore it
    if(((bg::string(buf))[0] == '%') || ((bg::string(buf))[0] == '\n') || (bg::string(buf).find(",") == bg::string::npos) ){
      continue;
    }

    //One instance per line
    instance = new FeatureContainer(listLen);
    attributeIndex = 0;	//index of the attributes in the current line/instance
    instanceCounter++;

    //Split the line at the seperators
    bg::strlist tokens = bg::string(buf).split(",");
    for (unsigned int i=0; i<tokens.size(); i++){
      //save only attributes which are in the InputFeatureList (which means they translate to unequal -1)
      if(translationTable[attributeIndex] != -1){
	newData = tokens[i].toFloat();
	instance->setData(translationTable[attributeIndex], newData);
      } // else { cout << "Invalides Attribut gefunden [" << attributeIndex<<";"<<translationTable[attributeIndex]<<"] "<<endl; }
      attributeIndex++;
    }
    
    data->append(instance);
  }
	
  arffFile.close();
  
  return data;

}

// CHARMinputmodules::ARFFFileHandler::ARFFFileHandler()
// {
// }

// CHARMinputmodules::ARFFFileHandler::~ARFFFileHandler()
// {
// }

// void CHARMinputmodules::ARFFFileHandler::printLongARFFHeader(fstream s, bg::string filename, bg::string activityname, int numberOfInstances, InputFeatureList* features)
// {
//   // 1. Commentary above the true header of the ARFF file, provides additional information about the content of the file.
//   s << "% 1.  Title:" << endl << "% " << filename.lastPathComponent().c_str() << endl << "%" << endl;
//   s << "% 2.  Created by:" << endl << "% CHARM Offline Trainer" << endl << "%" << endl;
//   s << "% 3.  Creation date:" << endl << "% " << bg::time::now().toString(true).c_str() << endl << "%" << endl;
//   s << "% 4.  Creation time:" << endl << "% " << bg::time::now().toString(true).c_str() << endl << "%" << endl;
//   s << "% 5.  Number of Instances:" << endl << "% " << numberOfInstances << endl << "%" << endl;
//   s << "% 6.  Number Of Attributes:" << endl << "% " << features->getNumber() << endl << "%" << endl;
//   s << "% 7.  Attribute Information:" << endl << "%" << endl;
//   s << "% 8.  Class Distribution" << endl;

//   // 2. Declaration of attributes and other informations
//   cout << "@relation " << filename.lastPathComponent().c_str() << "-" << activityname.c_str() << endl << endl;
//   for (unsigned int f=0; f<features->getNumber(); f++){
//     cout << "@attribute " << getFeatureName(static_cast<CHARMbase::FeatureNumber>(features->getFeature(f))).c_str() << "\tNUMERIC" << endl;
//   }
//   cout << "@attribute class\t{" << activityname.c_str() << "," << "Not}" << endl << endl << endl;

//   cout << "@data" << endl;  
  
//   return;
// }

// void CHARMinputmodules::ARFFFileHandler::printShortARFFHeader(fstream s, bg::string filename)
// {
  
// }

// FeatureContainerSequence* CHARMinputmodules::ARFFFileHandler::load(bg::string filename, InputFeatureList*& list) throw (FileError) {
//   //Open given File
//   ifstream arffFile;                          
//   arffFile.open(filename.c_str(), ios_base::in);
  
//   //Successfully opened?
//   if (!arffFile){    
//     BGDBG (3, "Throwing exception FileError\n");
//     throw FileError(__FILE__, __LINE__, filename, GenericFileError, "Open .arff file for loading feature sequence");
//   }

//   list = new InputFeatureList();
	
//   /////////////////// Read attributes ///////////////////////////
//   //Add attributes with FeatureNumber unequal to fnNoFeatureNumber, to InputFeatureList
//   char buf[3072];	//TODO Improve: zeilenweises lesen umstellen auf blockweise
//   bg::string attrName;
//   int attributeCountAll = 0;
//   while (arffFile.getline(buf, sizeof(buf))){
//     //Check only lines starting with "@attribute"
//     if(bg::string(buf).startsWith("@attribute")) {
//       attributeCountAll++;
//       //Check FeatureNumber
//       bg::strlist tokens = bg::string(buf).split(" ");
//       attrName = tokens[1];
//       if(makeFeatureNumber(attrName) != fnNoFeatureNumber) {
// 	list->addFeature(makeFeatureNumber(attrName));
//       } else {
//       }
//     } else if(bg::string(buf).startsWith("@data")) {
//       //Break if entering the data part
//       break;
//     }
//   }
  
//   // Reset get pointer position to stream beginning
//   arffFile.seekg( 0, ios_base::beg );
  
	
//   /////////////////// Build translation table ///////////////////
//   //Build a translation table for AttributeIndex (in the ARFF-File)  => index in the InputeFeatureList
//   int translationTable[attributeCountAll];
//   int attributeIndex = 0;	//index in the ARFF-File
//   while (arffFile.getline(buf, sizeof(buf))){
//     //Check only lines starting with "@attribute"
//     if(bg::string(buf).startsWith("@attribute")) {
//       bg::strlist tokens = bg::string(buf).split(" ");
//       attrName = tokens[1].c_str();
//       translationTable[attributeIndex] = list->findFeature(makeFeatureNumber(attrName));
//       attributeIndex++;
//     } else if(bg::string(buf).startsWith("@data")) {
//       //Break if entering the data part
//       break;
//     }
//   }
  
// //   cout<<"Translation table:"<<endl;
// //   for(int i = 0; i < attributeCountAll; i++){
// //     cout << "[" << i << "]=" << translationTable[i] << " , ";
// //     if(i%20 == 0){ cout<<endl; }
// //   }
// //   cout<<endl;
	
	
	
//   /////////////////// Read data /////////////////////////////////
//   //Reading data part line by line 
//   FeatureContainerSequence* data = new FeatureContainerSequence();
//   FeatureContainer* instance;
//   FLOAT_T newData;	//stores one attribute in one line
//   int instanceCounter=0;
//   int listLen = list->getNumber();
//   while (arffFile.getline(buf, sizeof(buf))){
//     //ignore comments, and if no "," is in the line, ignore it
//     if(((bg::string(buf))[0] == '%') || ((bg::string(buf))[0] == '\n') || (bg::string(buf).find(",") == bg::string::npos) ){
//       continue;
//     }

//     //One instance per line
//     instance = new FeatureContainer(listLen);
//     attributeIndex = 0;	//index of the attributes in the current line/instance
//     instanceCounter++;

//     //Split the line at the seperators
//     bg::strlist tokens = bg::string(buf).split(",");
//     for (unsigned int i=0; i<tokens.size(); i++){
//       //save only attributes which are in the InputFeatureList (which means they translate to unequal -1)
//       if(translationTable[attributeIndex] != -1){
// 	newData = tokens[i].toFloat();
// 	instance->setData(translationTable[attributeIndex], newData);
//       } // else { cout << "Invalides Attribut gefunden [" << attributeIndex<<";"<<translationTable[attributeIndex]<<"] "<<endl; }
//       attributeIndex++;
//     }
    
//     data->append(instance);
//   }
	
//   arffFile.close();
  
//   return data;
// }

// bool CHARMinputmodules::ARFFFileHandler::save(FeatureContainerSequence* sequence, bg::string filename)
// {
//   return writeSequence(sequence, filename, false);
// }

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
