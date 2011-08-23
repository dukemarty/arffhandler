/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ContainersTests.h
    \brief Test cases for the content of CHARMbase/Containers.h/.cc .

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 23.08.11

    \author    Martin Loesch (<loesch@ira.uka.de>)
    \date      25.05.08
    \par Copyright:
               Martin Loesch, Chair Prof. Dillmann (IAIM)
               Institute for Computer Science and Engineering (CSE)
	       University of Karlsruhe. All rights reserved
	       http://wwwiaim.ira.uka.de
*/

#ifndef ARFFCONTAINERSTESTS_H
#define ARFFCONTAINERSTESTS_H

/* system includes */
#include <string>

/* my includes */
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ARFFContainers.h>


using namespace ArffFileHandling;


/*!
  \class ContainersTests
  \brief


*/
class ARFFContainersTests : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE (ARFFContainersTests);
  CPPUNIT_TEST (FeatureContainerConstructionTest);
  CPPUNIT_TEST (FeatureContainerAccessTest);
  CPPUNIT_TEST (FeatureContainerSequenceTest);
  CPPUNIT_TEST (TrainingsDataContainerTest);
  CPPUNIT_TEST_SUITE_END ();

private:
  FeatureContainer *fcp1, *fcp2, *fcp3;
  FeatureContainerSequence *fcsp1, *fcsp2, *fcsp3;
  FeatureContainer *fcsfc1templ, *fcsfc2templ, *fcsfc3templ;
  
protected:
  void FeatureContainerConstructionTest(void);
  void FeatureContainerAccessTest(void);
  void FeatureContainerSequenceTest(void);
  void TrainingsDataContainerTest(void);
  
public:
  void setUp(void);
  void tearDown(void);
};

#endif /* ARFFCONTAINERSTESTS_H */
