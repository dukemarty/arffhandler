/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFData.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 25.08.11

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
#include <string>
#include <map>

/* my includes */
#include "ARFFContainers.h"

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

    //! number list of all features of the instances 
    FeatureList _featList;

    //! mapping of all classes to their position in the ordered list of classes
    ClassValuesByName _class2index;
    //! mapping of all valid class indizes to their name
    ClassValuesByIndex _index2class;

    //! pointer to the actual data (instances)
    TrainingsDataContainer* _data;

    /*!
      \brief Check validity of feature list.

      The feature list is valid if it holds the same number of features as the TrainingsDataContainer and if the feature (numbers) are a full list without holes, starting from 0.
      If there is no data in the TrainingsDataContainer yet, the result is true.

      @pre _data != NULL

      @return true if the list of features is valid, false else
    */
    bool checkFeatureListValidity() const;

    /*!
      \brief Check validity of class list.

      The feature list is valid if it holds the same number of classes as the TrainingsDataContainer and if the classes (numbers) are a full list without holes, starting from 0.

      @pre _data

      @return true if the list of classes is valid, false else
    */
    bool checkClassListValidity() const;
    
    /*!
      \brief Check object for its validity.

      For an object to be valid, it must contain data (even if it's empty) and the number of
      classes and features must match those of the TrainingsDataContainer.

      @return true if the object is valid, false else
    */
    bool checkValidity() const;

    /*!
      \brief Find all feature indizes for a given feature name.

      @param name of feature
      @return list of feature indizes which have the given name
    */
    vector<int> findIndizesForFeature(string name) const;
    
    /*!
      \brief Remove feature consistently from list and data.

      'Consistently' in this context means: The object has to be valid before; afterwards, it is not valid, because the feature number in the feature list and in the data are inconsistent. But the feature is removed from the feature list, and the features which had a higher feature number the the removed one are re-numbered.

      @pre isValid() == true

      @param index index of the feature to remove
      @return true if the feature was removed, false else (e.g. because the index does not exist)
    */
    bool removeFeatureConsistently(unsigned int index);

  protected:
    
  public:
    //@{
    //! \name Constructors & Destructor
    /*!
      \brief Initialize an empty (non-valid) ARFF data object.

      @post isValid()==false
    */
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
    /*!
      \brief Get number of features in the arff data.

      @return number of features.
    */
    unsigned int getNumberOfFeatures() const;
    /*!
      \brief Get name of a feature.

      @param index number of the feature whose name is queried
      @return name of the feature
    */
    string getFeatureName(unsigned int index) const;
    /*!
      \brief Get index of a feature given its name.

      \attention If there is more than one feature with the given name, the index of the first of these features (the one with the lowest index) is returned!
      
      @param name name of the feature
      @return index of the feature if it exists, -1 else
    */
    int getFeatureIndex(string name) const;;
    /*!
      \brief Add a feature to the arff data.

      \attention If a feature with the same number already exists, it is overwritten!
      
      @param index number of the new feature
      @param name name of the new feature
    */
    void addFeature(unsigned int index, string name);
    /*!
      \brief Check whether a feature name is unique

      If the name does not denote any feature, the result will also be false!
      
      @param name name of the feature which is tested
      @return true if the name is used for more than one feature, false else
    */
    bool isFeatureUnique(string name) const;
    //@}

    //@{
    //! \name Handling class information
    /*!
      \brief Get the number of classes

      @return number of classes
    */
    unsigned int getNumberOfClasses() const;
    /*!
      \brief Get the name of a class in the data.

      @param index number of the class (according to the order)
      @return name of the class, "" if the class (index) does not exist
    */
    string getClassName(unsigned int index) const;
    /*!
      \brief Get the number of a class in the data.

      @param name name of the class
      @return index of the class, -1 if the class (name) does not exist
    */
    int getClassIndex(string name) const;
    /*!
      \brief Add new class to set of classes of the arff data.

      @param index index of the new class
      @param name name of the class
    */
    void addClass(unsigned int index, string name);
    //@}

    //@{
    //! \name Handling instance data
    /*!
      \brief Get a pointer to the instance data.

      @return pointer to the internal TrainingDataContainer object, NULL if isValid()==false holds
    */
    TrainingsDataContainer* getData() const;
    /*!
      \brief Get the complete number of instances, summarized over all classes.

      @pre isValid()==true
      
      @return number of all instances in the data set
    */
    int getNumberOfInstances() const;
    /*!
      \brief Initialize data container.

      \attention For this method to be used correctly, the number of classes must be known, i.e. all classes must have been added via the addClass() method.
      
      @post isValid()==true
    */
    void initData();
    /*!
      \brief Add data sequence to the instance data set.

      \attention The ownership of the data sequence (i.e. memory management) is taken over!
      
      @pre isValid()==true

      @param classindex number of class for which an instance sequence is added
      @param data point to data sequence
    */
    void addDataSequence(unsigned int classindex, FeatureContainerSequence* data);
    /*!
      \brief Print content of the instance data to an output stream

      @pre isValid()==true

      @param outstream stream to which the data ist printed
    */
    void printData(ostream& outstream) const;
    //@}

    //@{
    //! \name Manipulate data
    /*!
      \brief Remove a feature with a given index.

      @pre isValid()==true
      
      @param index index of the feature
      @return true if the feature was removed, false else (e.g. because index does not exist)
    */
    bool removeFeature(unsigned int index);
    /*!
      \brief Remove a feature with a given name.

      \attention If there is more than one feature with the name, the first feature (i.e. that with the lowest index) is removed.
      
      @pre isValid()==true

      @param name name of the feature
      @return true if a feature was removed, false else (e.g. because the name does not exist)
    */
    bool removeFeature(string name);
    //@}
  };

};

#endif /* ARFFDATA_H */
