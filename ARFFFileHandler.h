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
#include "ARFFData.h"

using namespace std;


namespace ArffFileHandling {

  /*!
    \class ARFFFileHandler
    \brief Gives functionality to load and save ARFF files.
    
  */
  class ARFFFileHandler {
  private:
    ARFFData _arffcontent;

    string _filename;

    void clearData();

    void printHeader(fstream& out, string filename) const;
    
  protected:
    
  public:
    ARFFFileHandler();
    ARFFFileHandler(string filename);
    ~ARFFFileHandler();
    
    
    bool load(string filename);
    bool save(string filename) const;

    ARFFData* getData();
  };

};

#endif /* ARFFFILEHANDLER_H */
