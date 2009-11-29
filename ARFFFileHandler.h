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
    //! actual content of the file
    ARFFData _arffcontent;

    //! name of the (loaded) file
    string _filename;

    /*!
      \brief Clear the object from all data.
    */
    void clearData();

    /*!
      \brief Print an arff header for the current data.

      @param out stream to which is written
      @param filename name of the read file (used in the header text)
    */
    void printHeader(fstream& out, string filename) const;
    
  protected:
    
  public:
    //@{
    //! \name Constructors & Destructor
    /*!
      \brief Initialize an empty file handler.
    */
    ARFFFileHandler();
    /*!
      \brief Initialize file handler directly with an arff file.

      @param filename name of an arff file which is loaded directly
    */
    ARFFFileHandler(string filename);
    ~ARFFFileHandler();
    //@}
    
    /*!
      \brief Load data from an arff file.

      @param filename full name of an arff file
      @return true if the load was successfull, false else
    */
    bool load(string filename);
    /*!
      \brief Save the current data to an arff file.

      @param filename name of the file the data will be stored in
      @return true if the save was successfull, false else
    */
    bool save(string filename) const;

    /*!
      \brief Get the read data as an object.

      \attention The data remains under control of the ARFFFileHandler object, so be careful not to delete it as long as the data is used!

      @return point to the data member object
    */
    ARFFData* getData();
  };

};

#endif /* ARFFFILEHANDLER_H */
