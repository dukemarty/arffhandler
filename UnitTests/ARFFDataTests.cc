/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFDataTests.cc

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change: 09.08.10

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
#include <stdexcept>

/* my includes */
#include "ARFFDataTests.h"
#include <ARFFData.h>

using namespace ArffFileHandling;


CPPUNIT_TEST_SUITE_REGISTRATION(ARFFDataTests);


void ARFFDataTests::setUp(void)
{
}

void ARFFDataTests::tearDown(void)
{
}

void ARFFDataTests::ConstructorTests(void)
{
  ARFFData test1;
  CPPUNIT_ASSERT(!test1.isValid());
  CPPUNIT_ASSERT(test1.getNumberOfFeatures()==0);
  CPPUNIT_ASSERT(test1.getNumberOfClasses()==0);
  CPPUNIT_ASSERT(test1.getData()==NULL);
}

void ARFFDataTests::FeatureHandlingTests(void)
{
  ARFFData test1;
  CPPUNIT_ASSERT(test1.getNumberOfFeatures()==0);
  test1.addFeature(0, "Feature1");
  CPPUNIT_ASSERT(test1.getNumberOfFeatures()==1);
  test1.addFeature(1, "Feature2");
  CPPUNIT_ASSERT(test1.getNumberOfFeatures()==2);
  CPPUNIT_ASSERT(test1.getFeatureName(0) == "Feature1");
  CPPUNIT_ASSERT(test1.getFeatureName(1) == "Feature2");
  CPPUNIT_ASSERT_THROW(test1.getFeatureName(2), std::out_of_range);
}

void ARFFDataTests::ClassHandlingTests(void)
{
  ARFFData test1;
  CPPUNIT_ASSERT(test1.getNumberOfClasses()==0);
  test1.addClass(0, "Class1");
  CPPUNIT_ASSERT(test1.getNumberOfClasses()==1);
  test1.addClass(1, "Class2");
  CPPUNIT_ASSERT(test1.getNumberOfClasses()==2);
  test1.addClass(2, "Class3");
  CPPUNIT_ASSERT(test1.getNumberOfClasses()==3);
  CPPUNIT_ASSERT(test1.getClassIndex("Class1")==0);
  CPPUNIT_ASSERT(test1.getClassIndex("Class2")==1);
  CPPUNIT_ASSERT(test1.getClassIndex("Class3")==2);
  CPPUNIT_ASSERT(test1.getClassName(0)=="Class1");
  CPPUNIT_ASSERT(test1.getClassName(1)=="Class2");
  CPPUNIT_ASSERT(test1.getClassName(2)=="Class3");
}

void ARFFDataTests::InitializationTests(void)
{
  ARFFData test1, test2;

  CPPUNIT_ASSERT(!test1.isValid());
  test1.initData();
  CPPUNIT_ASSERT(test1.isValid());

  CPPUNIT_ASSERT(!test2.isValid());
  test2.addFeature(0, "Feature1");
  test2.addFeature(1, "Feature2");
  test2.addClass(0, "Class1");
  test2.addClass(1, "Class2");
  test2.addClass(2, "Class3");
  CPPUNIT_ASSERT(!test2.isValid());
  test2.initData();
  CPPUNIT_ASSERT(test2.isValid());
  CPPUNIT_ASSERT(test2.getNumberOfFeatures()==2);
  CPPUNIT_ASSERT(test2.getNumberOfClasses()==3);
}

void ARFFDataTests::InstanceHandlingTests(void)
{
  ARFFData test1;
  test1.addFeature(0, "Feature1");
  test1.addFeature(1, "Feature2");
  test1.addClass(0, "Class1");
  test1.addClass(1, "Class2");
  test1.addClass(2, "Class3");
  test1.initData();

  CPPUNIT_ASSERT(test1.getNumberOfInstances()==0);
}


#if ARFFDataTests_test
#include <stdio.h>
int main(int argc, char **argv)
{
  // This is a module-test block. You can put code here that tests
  // just the contents of this C file, and build it by saying
  //             make ARFFDataTests_test
  // Then, run the resulting executable (ARFFDataTests_test).
  // If it works as expected, the module is probably correct. ;-)

  fprintf(stderr, "Testing ARFFDataTests\n");

  return 0;
}
#endif /* ARFFDataTests_test */
