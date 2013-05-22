/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandler.h
    \brief 

    Please put your documentation for this file here.

    Creator: Martin Lösch (<loesch@ira.uka.de>)
    Date of creation: 07.06.08

    Last Author: Martin Loesch (<professional@@martinloesch.net>)
    Date of last change: 20.05.13

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
#include <string>
#include <iostream>
#include <map>

/* my includes */
#include "ARFFContainers.h"
#include "ARFFData.h"

using namespace std;


namespace ArffFileHandling {

  // forward declaration
  class ARFFFileHandler;

  class ARFFFileHandlerSet {
  public:
    std::map<string, ARFFFileHandler* > dataset;

    ARFFFileHandlerSet();
    ~ARFFFileHandlerSet();

    std::map<string, ARFFFileHandler* >::iterator begin(){ return dataset.begin(); }
    std::map<string, ARFFFileHandler* >::iterator end(){ return dataset.end(); }
  };

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
    void printHeader(ostream& out, string filename) const;

    /*!
      \brief Split arff data set into data of single classes.
      
      @return set of ARFFData objects, one for each class
    */
    ARFFDataSet splitClasses() const;
    
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
    /*!
      \brief Initializefile handler directly with arff content.

      ATTENTION: The content is not copied, but the ownership of the data pointer is taken over by the new file handler!
      
      @param content pointer to content data object.
    */
    ARFFFileHandler(ARFFData* content);
    ~ARFFFileHandler();
    //@}

    //@{
    //! \name Storage-related methods
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
    //@}

    //@{
    //! \name ARFF manipulation methods
    /*!
      \brief Remove feature from ARFF data.

      @param index index of the feature which shall be removed
      @return true if removal was successful, false else
    */
    bool removeFeature(int index);

    /*!
      \brief Remove feature from ARFF data.

      \attention If the name is used for more than one feature, it can not be removed! Use the features index instead!
      
      @param name name of the feature which shall be removed
      @return 0 if the removal was successful, 1 if it could not be removed (unknown reason), 2 if the feature name is used more than once
    */     
    int removeFeature(string name);
    //@}

    //@{
    ARFFFileHandlerSet splitSingleClasses() const;
    //@}
    
    /*!
      \brief Get the read data as an object.

      \attention The data remains under control of the ARFFFileHandler object, so be careful not to delete it as long as the data is used!

      @return point to the data member object
    */
    ARFFData* getData();
  };

};

#endif /* ARFFFILEHANDLER_H */
