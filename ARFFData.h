/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFData.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change: 29.11.09

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2009-11-29
    \par Copyright:
               Martin Loesch, Chair Prof. Dillmann (IAIM)\n
               Institute for Computer Science and Engineering (CSE)\n
	       University of Karlsruhe. All rights reserved\n
	       http://wwwiaim.ira.uka.de
*/

#ifndef ARFFDATA_H
#define ARFFDATA_H

/* system includes */
#include <bgtools.h>

/* my includes */
#include "Containers.h"

using namespace std;


/*!
  \namespace ArffFileHandling
  \brief Namespace for all classes and types of the ARFF Handler Library.
*/
namespace ArffFileHandling {

  //! type for numbered feature lists
  typedef map<int, string> FeatureList;
  //! type for mapping from a unique class name to an id number
  typedef map<int, string> ClassValuesByIndex;
  //! type for mapping from a unique id number to a class name
  typedef map<string, int> ClassValuesByName;
  
  /*!
    \class ARFFData
    \brief Representation for the data which is contained in an ARFF file.
    
  */
  class ARFFData {
  private:
    //! flag for the validity of the object, basically: is a data container initialized
    bool _valid;

    //! 
    FeatureList _featList;

    //!
    ClassValuesByName _class2index;
    //!
    ClassValuesByIndex _index2class;

    //! pointer to the actual data (instances)
    TrainingsDataContainer* _data;
    
  protected:
    
  public:
    //@{
    //! \name Constructors & Destructor
    ARFFData();
    ~ARFFData();
    //@}

    /*!
      \brief Check whether the object is valid for use.

      Valid in this sense means, that instance data can be added and queried. Features and classes can always be added and queried, independently of the validness of the object.

      @return true if the object is valid, false else
    */
    bool isValid() const;

    /*!
      \brief Clear the object: Remove all information about features, classes and instances.

      @post isValid()==false
    */
    void clear();

    //@{
    //! \name Handling feature information
    unsigned int getNumberOfFeatures() const;
    string getFeatureName(unsigned int index) const;
    void addFeature(unsigned int index, string name);
    //@}

    //@{
    //! \name Handling class information
    unsigned int getNumberOfClasses() const;
    string getClassName(unsigned int index) const;
    unsigned int getClassIndex(string name) const;
    void addClass(unsigned int index, string name);
    //@}

    //@{
    //! \name Handling instance data
    /*!
      \brief

      @return
    */
    TrainingsDataContainer* getData() const;
    /*!
      /brief

      @pre isValid()==true
      
      @return
    */
    int getNumberOfInstances() const;
    /*!
      \brief

      @post isValid()==true
    */
    void initData();
    /*!
      \brief

      @pre isValid()==true

      @param classindex
      @param data
    */
    void addDataSequence(unsigned int classindex, FeatureContainerSequence* data);
    /*!
      \brief

      @pre isValid()==true

      @param outstream
    */
    void printData(ostream& outstream) const;
    //@}
    
  };

};

#endif /* ARFFDATA_H */
