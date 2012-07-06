/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFDataTests.cc

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 08.11.11

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


CPPUNIT_TEST_SUITE_REGISTRATION(ARFFDataTests);


void ARFFDataTests::setUp(void)
{
}

void ARFFDataTests::tearDown(void)
{
}

void ARFFDataTests::initDataForManipulationTests(ARFFData& testdata)
{
  testdata.addFeature(0, "Feature0");
  testdata.addFeature(1, "Feature1");
  testdata.addFeature(2, "Feature2");
  testdata.addFeature(3, "Feature3");
  testdata.addFeature(4, "class");

  testdata.addClass(0, "Class1");
  testdata.addClass(1, "Class2");
  
  testdata.initData();

  FeatureContainerSequence* seq=NULL;
  FeatureContainer* inst=NULL;

  seq = new FeatureContainerSequence();
  inst = new FeatureContainer(testdata.getNumberOfFeatures() - 1);
  for (int i=0; i<4; ++i){
    inst->setData(i, 13 + i*i);
  }
  seq->append(inst);

  delete inst;
  inst = new FeatureContainer(testdata.getNumberOfFeatures() - 1);
  for (int i=0; i<4; ++i){
    inst->setData(i, 5 + i*i);
  }
  seq->append(inst);
  testdata.addDataSequence(0, seq);
  
  seq = new FeatureContainerSequence();
  delete inst;
  inst = new FeatureContainer(testdata.getNumberOfFeatures() - 1);
  for (int i=0; i<4; ++i){
    inst->setData(i, 17 + i*i);
  }
  seq->append(inst);
  testdata.addDataSequence(1, seq);

  delete inst;
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

  CPPUNIT_ASSERT(test1.getFeatureIndex("Feature1")==0);
  CPPUNIT_ASSERT(test1.getFeatureIndex("Feature2")==1);
  CPPUNIT_ASSERT(test1.getFeatureIndex("Feature3")==-1);
  test1.addFeature(2, "Feature1");
  CPPUNIT_ASSERT(test1.getFeatureIndex("Feature1")==0);
  
  CPPUNIT_ASSERT(test1.isFeatureUnique("Feature1")==false);
  CPPUNIT_ASSERT(test1.isFeatureUnique("Feature2")==true);
  CPPUNIT_ASSERT(test1.isFeatureUnique("Feature3")==false);
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

void ARFFDataTests::DataManipulationTests(void)
{
  ARFFData test1;
  initDataForManipulationTests(test1);

  CPPUNIT_ASSERT(test1.isValid()==true);
  CPPUNIT_ASSERT(test1.getNumberOfFeatures()==5);
  CPPUNIT_ASSERT(test1.getNumberOfClasses()==2);
  TrainingsDataContainer* data1 = test1.getData();
  FeatureContainer* inst = data1->getFrame(0, 0, 0);
  for (int i=0; i<4; ++i){
    CPPUNIT_ASSERT(inst->getData(i)==13+i*i);
  }
  inst = data1->getFrame(0, 0, 1);
  for (int i=0; i<4; ++i){
    CPPUNIT_ASSERT(inst->getData(i)==5+i*i);
  }
  inst = data1->getFrame(1, 0, 0);
  for (int i=0; i<4; ++i){
    CPPUNIT_ASSERT(inst->getData(i)==17+i*i);
  }

  bool remRes = test1.removeFeature("Feature1");
  CPPUNIT_ASSERT(remRes==true);
  CPPUNIT_ASSERT(test1.isValid()==true);
  CPPUNIT_ASSERT(test1.getNumberOfFeatures()==4);
  CPPUNIT_ASSERT(test1.getNumberOfClasses()==2);
  data1 = test1.getData();
  inst = data1->getFrame(0, 0, 0);
  CPPUNIT_ASSERT(inst->getData(0)==13);
  for (int i=2; i<4; ++i){
    CPPUNIT_ASSERT(inst->getData(i-1)==13+i*i);
  }
  inst = data1->getFrame(0, 0, 1);
  CPPUNIT_ASSERT(inst->getData(0)==5);
  for (int i=2; i<4; ++i){
    CPPUNIT_ASSERT(inst->getData(i-1)==5+i*i);
  }
  inst = data1->getFrame(1, 0, 0);
  CPPUNIT_ASSERT(inst->getData(0)==17);
  for (int i=2; i<4; ++i){
    CPPUNIT_ASSERT(inst->getData(i-1)==17+i*i);
  }
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
