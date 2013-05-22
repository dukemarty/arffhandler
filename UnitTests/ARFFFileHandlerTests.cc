/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandlerTests.cc

    \par Last Author: Martin Loesch (<professional@@martinloesch.net>)
    \par Date of last change: 20.05.13

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2010-08-09
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (IAIM)\n
              Institute for Computer Science and Engineering (CSE)\n
	      University of Karlsruhe. All rights reserved\n
	      http://wwwiaim.ira.uka.de
*/

/* system includes */
#include <assert.h>

/* my includes */
#include "ARFFFileHandlerTests.h"
#include <ARFFFileHandler.h>
#include <ARFFData.h>

using namespace ArffFileHandling;


CPPUNIT_TEST_SUITE_REGISTRATION(ARFFFileHandlerTests);


void ARFFFileHandlerTests::setUp(void)
{
}

void ARFFFileHandlerTests::tearDown(void)
{
}

void ARFFFileHandlerTests::LoadTest(void)
{
  ARFFFileHandler test1;
  test1.load("iris.arff");

  ARFFData* data1 = test1.getData();

  CPPUNIT_ASSERT(data1->getNumberOfClasses()==3);
  CPPUNIT_ASSERT(data1->getNumberOfFeatures()==5);
  CPPUNIT_ASSERT(data1->getNumberOfInstances()==150);
}

void ARFFFileHandlerTests::SaveTest(void)
{
}

void ARFFFileHandlerTests::SplitClassesTest(void)
{
  ARFFFileHandler test1;
  test.load("iris.arff");

  
}


#if ARFFFileHandlerTests_test
#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make ARFFFileHandlerTests_test
  // Then, run the resulting executable (ARFFFileHandlerTests_test).
  // If it works as expected, the module is probably correct. ;-)

  fprintf(stderr, "Testing ARFFFileHandlerTests\n");

  return 0;
}
#endif /* ARFFFileHandlerTests_test */
