/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Main.cc
    
    This is the main module of your program.

    
    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change:  2010-08-09

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2010-08-09
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (IAIM)\n
              Institute for Computer Science and Engineering (CSE)\n
	      University of Karlsruhe. All rights reserved\n
	      http://his.anthropomatik.kit.edu
*/


/* system includes */
#include <string>

/* my includes */
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>


/*
  \brief The program's main function.


*/
int main(int argc, char **argv)
{
  // inform test-listener about test results
  CPPUNIT_NS::TestResult testresult;

  // register a listener to collect test results
  CPPUNIT_NS::TestResultCollector collectedresults;
  testresult.addListener(&collectedresults);

  // listener to print the results of the tests
  CPPUNIT_NS::BriefTestProgressListener progress;
  testresult.addListener(&progress);

  // insert test-suite into test-runner via the registry
  CPPUNIT_NS::TestRunner testrunner;
  testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  testrunner.run(testresult);

  // print results in compiler-format
  CPPUNIT_NS::CompilerOutputter  compileroutputter(&collectedresults, std::cerr);
  compileroutputter.write();

  // feedback whether tests were successful
  return collectedresults.wasSuccessful () ? 0 : 1;
}

