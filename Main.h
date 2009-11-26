/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Main.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    Date of last change: 26.11.09

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2009-11-26
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (IAIM)\n
              Institute for Computer Science and Engineering (CSE)\n
	      University of Karlsruhe. All rights reserved\n
	      http://wwwiaim.ira.uka.de
*/

#ifndef MAIN_H
#define MAIN_H

/* system includes */
#include <boost/program_options.hpp>

/* my includes */
/* (none) */

namespace bpo = boost::program_options;
using namespace std;


//! global name of input file
static std::string g_inputfilename;
//! global name of output file
static std::string g_outputfilename;


/*!
  \brief Print usage of the stand-alone activity recognizer to standard out.

  @param progname name of the program that was called
  @param options object containing the allowed options together with their description
*/
void printUsageHint(string progname, bpo::options_description& options);

/*!
  \brief Process command line parameters and make settings accordingly.

  @param argc same name and function as argc in main() routine
  @param argv same name and function as argv in main() routine
*/
void processCommandlineParameters(int argc, char **argv);
  

#endif /* MAIN_H */
