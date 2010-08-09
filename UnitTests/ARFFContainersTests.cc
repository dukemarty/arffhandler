/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFContainersTests.cc

    Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    Date of last change: 09.08.10

    \author    Martin Lösch (<loesch@ira.uka.de>)
    \date      25.05.08
    \par Copyright:
               Martin Loesch, Chair Prof. Dillmann (IAIM)
               Institute for Computer Science and Engineering (CSE)
	       University of Karlsruhe. All rights reserved
	       http://wwwiaim.ira.uka.de
*/

/* system includes */
#include <assert.h>

/* my includes */
#include "ARFFContainersTests.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ARFFContainersTests);


void ARFFContainersTests::setUp(void)
{
  // set up for FeatureContainer tests
  fcp1 = new FeatureContainer;
  fcp2 = new FeatureContainer(7);
  fcp3 = new FeatureContainer(*fcp2);

  // set up for FeatureContainerSequence tests
  fcsp1 = new FeatureContainerSequence;
  fcsp2 = new FeatureContainerSequence;
  fcsp3 = new FeatureContainerSequence;

  fcsfc1templ = new FeatureContainer(2);
  fcsfc2templ = new FeatureContainer(2);
  fcsfc3templ = new FeatureContainer(2);

  (*fcsfc1templ)[0] = 1.1;  (*fcsfc1templ)[1] = 1.2;
  (*fcsfc2templ)[0] = 2.1;  (*fcsfc2templ)[1] = 2.2;
  (*fcsfc3templ)[0] = 3.1;  (*fcsfc3templ)[1] = 3.2;
}

void ARFFContainersTests::tearDown(void)
{
  // tear down from FeatureContainer tests
  delete fcp1;
  delete fcp2;
  delete fcp3;

  // tear down from FeatureContainerSequence tests
  delete fcsp1;
  delete fcsp2;
  delete fcsp3;
  delete fcsfc1templ;
  delete fcsfc2templ;
  delete fcsfc3templ;
}

void ARFFContainersTests::FeatureContainerConstructionTest(void)
{
  // empty constructor
  CPPUNIT_ASSERT(fcp1->getNumberOfFeatures()==0);
  CPPUNIT_ASSERT(fcp1->getDataPointer()==NULL);

  // constructor with number
  CPPUNIT_ASSERT(fcp2->getNumberOfFeatures()==7);
  CPPUNIT_ASSERT(fcp2->getDataPointer()!=NULL);

  for (unsigned int i=0; i<7; i++){
    fcp2->setData(i, i+2.13);
    CPPUNIT_ASSERT_EQUAL(fcp2->getData(i), i+2.13);
  }
  
  // constructor with another object
  CPPUNIT_ASSERT(fcp3->getNumberOfFeatures()==7);
  CPPUNIT_ASSERT(fcp3->getDataPointer()!=NULL);
  
  for (unsigned int i=0; i<7; i++){
    fcp3->setData(i, i+2.13);
    CPPUNIT_ASSERT_EQUAL(fcp3->getData(i), i+2.13);
  }
}

void ARFFContainersTests::FeatureContainerAccessTest(void)
{
  // getNumberOfFeatures()/setNumberOfFeatures()
  for (unsigned int i=0; i<7; i++){
    double t1=fcp2->getData(i);
    fcp2->setData(i, (i+1.2)*3.4);
    CPPUNIT_ASSERT_EQUAL(fcp2->getData(i), (i+1.2)*3.4);
    fcp2->setData(i, t1);
    CPPUNIT_ASSERT_EQUAL(fcp2->getData(i), t1);
  }

  // operatoren [] und =
  for (unsigned int i=0; i<7; i++){
    double t1=(*fcp2)[i];
    (*fcp2)[i] = (i + 1.2) * 3.4;
    CPPUNIT_ASSERT_EQUAL((*fcp2)[i], (i+1.2)*3.4);
    (*fcp2)[i] = t1;
    CPPUNIT_ASSERT_EQUAL((*fcp2)[i], t1);
  }

  // isEmpty()...";
  CPPUNIT_ASSERT_EQUAL(fcp1->isEmpty(), true);
  CPPUNIT_ASSERT_EQUAL(fcp2->isEmpty(), false);
  CPPUNIT_ASSERT_EQUAL(fcp3->isEmpty(), false);
}

void ARFFContainersTests::FeatureContainerSequenceTest(void)
{
  FeatureContainer *fcsfc1, *fcsfc2, *fcsfc3;
  fcsfc1 = new FeatureContainer(*fcsfc1templ);
  fcsfc2 = new FeatureContainer(*fcsfc2templ);
  fcsfc3 = new FeatureContainer(*fcsfc3templ);
  
  fcsp2->append(fcsfc1);
  fcsp3->append(fcsfc2);
  fcsp3->append(fcsfc3);
  fcsp3->clear();

  // getSeqLength()
  CPPUNIT_ASSERT_EQUAL(fcsp1->getSeqLength(), 0);
  CPPUNIT_ASSERT(fcsp2->getSeqLength()!=0);
  CPPUNIT_ASSERT_EQUAL(fcsp3->getSeqLength(), 0);

  fcsfc2 = new FeatureContainer(*fcsfc2templ);
  fcsfc3 = new FeatureContainer(*fcsfc3templ);
  fcsp3->append(fcsfc2); fcsp3->append(fcsfc3);
  // append()
  CPPUNIT_ASSERT_EQUAL((*(fcsp2->getContainer()))[0], (*fcsfc1)[0]);
  CPPUNIT_ASSERT_EQUAL((*(fcsp2->getContainer()))[1], (*fcsfc1)[1]);
  CPPUNIT_ASSERT(fcsp3->getContainer(0)!=NULL);
  CPPUNIT_ASSERT(fcsp3->getContainer(1));

  // getContainer()... ";
  CPPUNIT_ASSERT_EQUAL((*(fcsp2->getContainer()))[0], (*fcsfc1)[0]);
  CPPUNIT_ASSERT_EQUAL((*(fcsp2->getContainer()))[1], (*fcsfc1)[1]);
  CPPUNIT_ASSERT_EQUAL((*(fcsp3->getContainer(0)))[0], (*fcsfc2)[0]);
  CPPUNIT_ASSERT_EQUAL((*(fcsp3->getContainer(0)))[1], (*fcsfc2)[1]);
  CPPUNIT_ASSERT_EQUAL((*(fcsp3->getContainer(1)))[0], (*fcsfc3)[0]);
  CPPUNIT_ASSERT_EQUAL((*(fcsp3->getContainer(1)))[0], (*fcsfc3)[0]);

  // clear()... ";
  CPPUNIT_ASSERT(fcsp2->getSeqLength()!=0);
  CPPUNIT_ASSERT(fcsp3->getSeqLength());
  fcsp2->clear();
  fcsp3->clear();
  CPPUNIT_ASSERT_EQUAL(fcsp2->getSeqLength(), 0);
  CPPUNIT_ASSERT_EQUAL(fcsp3->getSeqLength(), 0);
}

void ARFFContainersTests::TrainingsDataContainerTest(void)
{
}


#if ARFFContainersTests_test
#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make ARFFContainersTests_test
  // Then, run the resulting executable (ARFFContainersTests_test).
  // If it works as expected, the module is probably correct. ;-)

  fprintf(stderr, "Testing ARFFContainersTests\n");

  return 0;
}
#endif /* ARFFContainersTests_test */
