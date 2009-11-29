/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Main.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    Date of last change: 29.11.09

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


//! enum for valid operation modes 
typedef enum {
  INVALID,
  COPY,
  SHOW
} OperationMode;


//! global operation mode
static OperationMode g_mode;
//! global name of input file
static std::string g_inputfilename;
//! global name of output file
static std::string g_outputfilename;


/*!
  \brief Parse operation mode from a string (= command line parameter).

  @param mode mode as a string
  @return related enum to the input parameter
*/
OperationMode parseOperationMode(string mode);

/*!
  \brief Print usage of the stand-alone activity recognizer to standard out.

  @param progname name of the program that was called
  @param options object containing the allowed options together with their description
*/
void printUsageHint(string progname, bpo::options_description& options);

/*!
  \brief Perform a copy operation.

  \pre The parameters \em load and \em save must be given. The loaded file is written to the save file.

  @return 0 if everything went right, 1 else
*/
int performCopy();
/*!
  \brief Perform a show operation.

  \pre The \em load parameter must be given. The loaded files data is written to the standard output.

  @return 0 if everything went right, 1 else
*/
int performShow();

/*!
  \brief Parse parameters for a copy operation.

  The parameters which are necessary for a copy are a load and at least one save parameter.
  
  @param vm structure containing all parameters
  @return 0 if parsing was successful, 1 if parameters were missing
*/
int processCopyParameters(bpo::variables_map& vm);
/*!
  \brief Parse parameters for a show operation.

  The parameter which is necessary for the show is at least one load parameter.
  
  @param vm structure containing all parameters
  @return 0 if parsing was successful, 1 if parameters were missing
*/
int processShowParameters(bpo::variables_map& vm);

/*!
  \brief Process command line parameters and make settings accordingly.

  @param argc same name and function as argc in main() routine
  @param argv same name and function as argv in main() routine
*/
void processCommandlineParameters(int argc, char **argv);
  

#endif /* MAIN_H */
