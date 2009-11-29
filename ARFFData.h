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


namespace ArffFileHandling {
  
  typedef map<int, string> FeatureList;
  typedef map<int, string> ClassValuesByIndex;
  typedef map<string, int> ClassValuesByName;
  
  /*!
    \class ARFFData
    \brief Representation for the data which is contained in an ARFF file.
    
    
  */
  class ARFFData {
  private:
    bool _valid;

    FeatureList _featList;

    ClassValuesByName _class2index;
    ClassValuesByIndex _index2class;

    TrainingsDataContainer* _data;
    
  protected:
    
  public:
    ARFFData();
    ~ARFFData();

    bool isValid() const;
    
    unsigned int getNumberOfFeatures() const;
    string getFeatureName(unsigned int index) const;
    void addFeature(unsigned int index, string name);

    unsigned int getNumberOfClasses() const;
    string getClassName(unsigned int index) const;
    unsigned int getClassIndex(string name) const;
    void addClass(unsigned int index, string name); 

    TrainingsDataContainer* getData() const;
    int getNumberOfInstances() const;
    void initData();
    void addDataSequence(unsigned int classindex, FeatureContainerSequence* data);
    void printData(ostream& outstream) const;
    
    void clear();
  };

};

#endif /* ARFFDATA_H */
