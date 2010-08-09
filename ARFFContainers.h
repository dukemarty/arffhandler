/*! \if exist This is -*- C++ -*- from nbg \endif

  \file  ARFFContainers.h
  \brief This file assembles the different container classes of cHARm.

  Different types of special containers are used in the cHARm system to process data in the extraction, training and recognition of features for the classification of human activity.
    
  \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
  \par Date of last change: 09.08.10

  \author    Martin Loesch (<loesch@ira.uka.de>)
  \date      08.02.07
  \par Copyright:
  Martin Loesch, Chair Prof. Dillmann (IAIM)\n
  Institute for Computer Science and Engineering (CSE)\n
  University of Karlsruhe. All rights reserved\n
  http://wwwiaim.ira.uka.de
*/

#ifndef ARFFCONTAINERS_H
#define ARFFCONTAINERS_H

/* system includes */
#include <string>
#include <vector>
#include <iostream>

/* my includes */
#include <ARFFHandlerConfiguration.h>


using namespace std;


namespace ARFFHANDLERNAMESPACE {

  // **********************************************************************
  /*!
    \class FeatureContainer
    \brief This class is a container for extracted feature data.

    Besides the raw data (which could be represented by a simple array of doubles, additional information is saved, at the moment the size of the array. The following picture shows a diagram of the container.

    \image html featurecontainer.png "Diagram of the composition of a FeatureContainer object"
    \image latex featurecontainer.pdf "Diagram of the composition of a FeatureContainer object" width=10cm
  */
  class FeatureContainer {
  private:
    /*!
      \brief Check that object is valid.

      @return true if the object is a valid FeatureContainer, false if non-valid
    */      
    bool checkIsValid() const;
  
  protected:

    //! number of features = number of values in the container
    unsigned int m_numberOfFeatures;

    //! pointer to the memory holding the real data
    double* m_data;
  
  
  public:

    //@{
    //! \name Constructors & Destructors

    /*!
      \brief Standard constructor.
    */
    FeatureContainer();
    /*!
      \brief Initialize the container for a given size.

      @post m_data != NULL
      @post m_numberOfFeatures == numberOfFeatures
      
      @param numberOfFeatures number of values that will be saved
    */
    FeatureContainer(int numberOfFeatures);
    /*!
      \brief Copy constructor.

      @post m_data == orig.m_data
      @post m_numberOfFeatures == orig.m_numberOfFeatures
      @post \f$ \forall_i\f$ m_data[i] == orig.m_data[i]
    */
    FeatureContainer(const FeatureContainer& orig);
    virtual ~FeatureContainer();
    //@}

    //@{
    //! \name Getter & Setter
  
    unsigned int getNumberOfFeatures() const;
    /*!
      \brief Set number of elements in object.

      @post m_numberOfFeatures = newSize
      @post \f$ \forall_i \f$ m_data[i] == 0
    */
    void setNumberOfFeatures(const unsigned int newSize);
    double getData(const unsigned int index) const;
    void setData(const unsigned int index, const double newData);
    double& operator[](const unsigned int i){ return m_data[i]; };

    double* getDataPointer(){ return m_data; };
    //@}

    //@{
    //! \name Operators

    /*!
      \post \f$(\mbox{m\_\,numberOfFeatures}=0 \wedge \mbox{m\_data}=\mbox{NULL}) \vee (\mbox{m\_\,data}\not=\mbox{NULL}) )\f$
    */
    FeatureContainer& operator=(const FeatureContainer& right);
    //@}

    //@{
    //! \name Tester
    const bool isEmpty() const { return (m_data==NULL); };
    const bool isEqual(FeatureContainer* other) const;
    const bool operator==(FeatureContainer& rvalue) const { return isEqual(&rvalue); };
    //@}
    
  };


  
  // **********************************************************************
  /*!
    \class FeatureContainerSequence
    \brief This class is a container for sequences of extracted feature data.

    This class holds and manages a list of FeatureContainer objects. Only references of the stored FeatureContainer's stored, but no copies are made. The memory management of the containers (i.e. call of delete) remains by the user. The following picture shows a diagram of the container and how FeatureContainer objects are included.

    \image html featurecontainersequence.png "Diagram of the composition of a FeatureContainerSequence"
    \image latex featurecontainersequence.pdf "Diagram of the composition of a FeatureContainerSequence" width=10cm
  */
  class FeatureContainerSequence {
  private:
    //! pointer to vector that holds the pointers to the FeatureContainer's
    vector<FeatureContainer* > _list;
  
  protected:

  
  public:
    //@{
    //! \name Constructors & Destructors
    /*!
      \brief Standard constructor.
    */
    FeatureContainerSequence();
    ~FeatureContainerSequence();
    //@}

    //@{
    //! \name Getter & Setter
    int getSeqLength() const { return _list.size(); };

    /*!
      \brief Append a FeatureContainer.

      @param newFC object to append
      @return true if the container has been appended, false else
    */
    bool append(FeatureContainer* newFC);
    /*!
      \brief Get the pointer to an element of the sequence.
    
      @param index describes which element shall be accessed, 0 means the first inserted
      @return pointer to the (index)-th FeatureContainer
    */
    FeatureContainer* getContainer(const int index=0) const { return _list[index]; };
    //@}

    /*!
      \brief Clear the sequence of all feature containers.

      For every FeatureContainer in the sequence, delete is called to free the memory.
    */
    void clear();
  
  };



  // **********************************************************************
  /*!
    \class TrainingsDataContainer
    \brief This class hold sequences of features which are intended to be used for training a new classifier.

    This class holds two lists of object of the class FeatureContainerSequence: One list for positive examples of the activity, and one for negative examples of the activity. The following picture shows a diagram of the container.
    
    \image html trainingsdatacontainer.png "Diagram of the composition of a TrainingsDataContainer object"
    \image latex trainingsdatacontainer.pdf "Diagram of the composition of a TrainingsDataContainer object" width=8cm

    The idea behind the distinction between feature and feature container is to have different sequences which were recorded to train a new activity. Each sequence itself contains a number of single frames/pictures/whatever, and from each frame a number of features can be extracted (which a stored in a FeatureContainer object).
  */
  class TrainingsDataContainer {
  private:
    //! number of activities to be trained (fixed to 2, only for compatibility with old code base)
    unsigned int _numberOfActivities;

    //! list of training sequences, one for positive and one for negative examples
    vector< vector<FeatureContainerSequence* > > _data;

    /*!
      \brief Output extracted training data plain to output.

      The format the data is outputted is simple: The features of one frame are separated by commas, and after the last feature the activity class is added (which is - normally - 0 for positive and 1 for negative examples).

      @param o stream the output shall be written to
    */
    void printDataOnly(ostream& o) const;

    /*!
      \brief Output extracted training data with activity names to output.

      The format the data is outputted is the same as for printDataOnly() with a little difference: Instead of the activity class, the name of the current activity is written at the end of each line (but negative examples are - normally - named as "Not" for each activity).
      
      @param outstream stream the output shall be written to
      @param activityNames list of activity names
    */
    void printDataWithActivities(ostream& outstream, vector<string>* activityNames) const;
  
  protected:


  public:
    //@{
    //! \name Constructors & Destructors
    TrainingsDataContainer(unsigned int numberOfActivities=2);
    ~TrainingsDataContainer();
    //@}

    //@{
    //! \name Getter & Setter
    unsigned int getNumberOfActivities() const { return _numberOfActivities; };
    void addData(const unsigned int activity, FeatureContainerSequence* newData);
    int getNumberOfSequences(const unsigned int activity) const { return _data[activity].size(); };
    int getSequenceLength(const int unsigned activity, const int sequence) const { return _data[activity][sequence]->getSeqLength(); };
    FeatureContainer* getFrame(const int unsigned activity, const int sequence, const int frame) const { return _data[activity][sequence]->getContainer(frame); };
    int getTotalNumberOfContainers() const;
    //@}

    /*!
      \brief Print complete data to a stream.

      If the second parameter is not null, the names of the activities are printed together with the data instances. Otherwise, the class of the data points (the activities) are not printed.

      @param outstream stream the output is written to
      @param activityNames list of activity names
    */
    void print(ostream& outstream, vector<string>* activityNames=NULL) const;
  };

}
  
#endif /* ARFFCONTAINERS_H */
