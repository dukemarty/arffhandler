/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandler.h
    \brief 

    Please put your documentation for this file here.

    Creator: Martin Lösch (<loesch@ira.uka.de>)
    Date of creation: 07.06.08

    Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    Date of last change: 29.11.09

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
#include <map>

/* my includes */
#include "Containers.h"

using namespace std;


namespace ArffFileHandling {

  typedef map<int, string> FeatureList;
  typedef map<int, string> ClassValuesByIndex;
  typedef map<string, int> ClassValuesByName;
  
  /*!
    \class ARFFFileHandler
    \brief Gives functionality to load and save ARFF files.
    
    Uses CHARMbase::FeatureContainerSequence to handle the data.
  */
  class ARFFFileHandler {
  private:
    bool _valid;
    string _filename;
    FeatureList _featList;

    TrainingsDataContainer* _data;
    ClassValuesByName _class2index;
    ClassValuesByIndex _index2class;

    void clearData();

    void printHeader(fstream& out, string filename) const;
    
  protected:
    
  public:
    ARFFFileHandler();
    ARFFFileHandler(string filename);
    ~ARFFFileHandler();
    
    
    bool load(string filename);
    bool save(string filename) const;

    TrainingsDataContainer* getData() const;
    int getNumberOfFeatures() const;
    
  };

};

#endif /* ARFFFILEHANDLER_H */
