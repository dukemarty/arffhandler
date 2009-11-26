/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandler.h
    \brief 

    Please put your documentation for this file here.

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

#ifndef ARFFFILEHANDLER_H
#define ARFFFILEHANDLER_H

/* system includes */
#include <bgtools.h>
#include <iostream>

/* my includes */
#include "Containers.h"
// #include "FileError.h"
// #include "FeatureDescriptors.h"
// #include "FalseRequestError.h"

// using namespace CHARMbase;
// using namespace CHARMerrors;
using namespace std;


namespace ArffFileHandling {

  /*!
    \class ARFFFileHandler
    \brief Gives functionality to load and save ARFF files.
    
    Uses CHARMbase::FeatureContainerSequence to handle the data.
  */
  class ARFFFileHandler {
  private:
//     /*!
//       \brief called by save() and append() to prevent redundancy

//       @param sq
//       @param filename
//       @param append
//       @param features
//       @return
//     */
//     static bool writeSequence(FeatureContainerSequence* sq, bg::string filename,  bool append, InputFeatureList* features=NULL);
    
  protected:
    
  public:
    
    static TrainingsDataContainer* load(string filename) throw (FileError);
//     ARFFFileHandler();
//     ~ARFFFileHandler();


//     /*!
//       \brief

//       @param s
//       @param filename
//       @param activityname
//       @param numberOfInstances
//       @param features
//     */
//     static void printLongARFFHeader(fstream s, bg::string filename, bg::string activityname, int numberOfInstances, InputFeatureList* features);

//     /*!
//       \brief

//       @param s
//       @param filename
//     */
//     static void printShortARFFHeader(fstream s, bg::string filename);
    
//     /*!
//       \brief Loads the data from the given file into a FeatureContainerSequence. 

//       @param filename Filename of the ARFF file which should be loaded. Absolute path or relative to executing directory.
//       @param list Reference to an InputFeatureList which will be set to fit the loaded FeatureContainer. 
//       @return FeatureContainerSequence object(-reference) which is initialized with the data from the given filename.
//     */
//     static FeatureContainerSequence* load(bg::string filename, InputFeatureList* &list) throw (FileError);

//     /*!
//       \brief Saves the given FeatureContainerSequence into the given filename (overrides existing file).

//       @param sequence Data which should be saved.
//       @param filename Filename of the ARFF file into which data should be saved. Absolute path or relative to executing directory. If file already exists, it will be overridden!
//       @return true if saving was successful else false.
//     */
//     static bool save(FeatureContainerSequence* sequence, bg::string filename);
    
//     /*!
//       \brief Saves the given FeatureContainerSequence by appending it to the given filename.

//       @param sequence Data which should be appended.
//       @param filename Filename of the ARFF file to which data should be appended. Absolute path or relative to executing directory. File has to exist (head part of ARFF won't be written)!
//       @return true if appending was successful else false.
//      */
//     static bool append(FeatureContainerSequence* sequence, bg::string filename) throw (FalseRequestError);
    
//     /*!
//       \brief Saves the given FeatureContainer by appending it to the given filename.

//       @param dataPoint Data which should be appended.
//       @param filename Filename of the ARFF file to which data should be appended. Absolute path or relative to executing directory. File has to exist (head part of ARFF won't be written)!
//       @return true if appending was successful else false.
//      */
//     static bool append(FeatureContainer* dataPoint, bg::string filename) throw (FalseRequestError);
  };

};

#endif /* ARFFFILEHANDLER_H */
