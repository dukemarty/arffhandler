/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFDataTests.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 25.08.11

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2010-08-09
    \par Copyright:
               Martin Loesch, Chair Prof. Dillmann (IAIM)\n
               Institute for Computer Science and Engineering (CSE)\n
	       University of Karlsruhe. All rights reserved\n
	       http://wwwiaim.ira.uka.de
*/

#ifndef ARFFDATATESTS_H
#define ARFFDATATESTS_H

/* system includes */
#include <string>

/* my includes */
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <ARFFData.h>


using namespace ArffFileHandling;


/*!
  \class ARFFDataTests
  \brief


*/
class ARFFDataTests : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE (ARFFDataTests);
  CPPUNIT_TEST (ConstructorTests);
  CPPUNIT_TEST (FeatureHandlingTests);
  CPPUNIT_TEST (ClassHandlingTests);
  CPPUNIT_TEST (InitializationTests);
  CPPUNIT_TEST (InstanceHandlingTests);
  CPPUNIT_TEST (DataManipulationTests);
  CPPUNIT_TEST_SUITE_END ();

private:
  void initDataForManipulationTests(ARFFData& testdata);
  
protected:
  void ConstructorTests(void);
  void FeatureHandlingTests(void);
  void ClassHandlingTests(void);
  void InitializationTests(void);
  void InstanceHandlingTests(void);
  void DataManipulationTests(void);
  
public:
  void setUp(void);
  void tearDown(void);
};

#endif /* ARFFDATATESTS_H */
