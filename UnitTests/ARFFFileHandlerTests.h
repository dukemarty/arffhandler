/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  ARFFFileHandlerTests.h
    \brief 

    Please put your documentation for this file here.

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

#ifndef ARFFFILEHANDLERTESTS_H
#define ARFFFILEHANDLERTESTS_H

/* system includes */
#include <string>

/* my includes */
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


/*!
  \class ARFFFileHandlerTests
  \brief


*/
class ARFFFileHandlerTests : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE (ARFFFileHandlerTests);
  CPPUNIT_TEST (LoadTest);
  CPPUNIT_TEST (SaveTest);
  CPPUNIT_TEST_SUITE_END ();

 private:

protected:
  void LoadTest(void);
  void SaveTest(void);
  
public:
  void setUp(void);
  void tearDown(void);
};

#endif /* ARFFFILEHANDLERTESTS_H */
