/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Main.cc

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change: 26.11.09

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2009-11-26
    \par Copyright:
              Martin Loesch, Chair Prof. Dillmann (IAIM)\n
              Institute for Computer Science and Engineering (CSE)\n
	      University of Karlsruhe. All rights reserved\n
	      http://wwwiaim.ira.uka.de
*/

/* system includes */
#include <assert.h>
#include <iostream>

/* my includes */
#include "Main.h"


const char c_HelpOption[] = "help";
const char c_HelpOptionShort[] = "help,h";

const char c_LoadOption[] = "load";
const char c_LoadOptionShort[] = "load,l";

const char c_SaveOption[] = "save";
const char c_SaveOptionShort[] = "save,s";


void printUsageHint(string progname, bpo::options_description& options)
{
  cout << endl;
  cout << "Usage: " << progname.c_str() << " [OPTIONS] " << endl << endl;
  cout << options << endl;
  cout << endl;
  cout << "A more detailled explanation of the program will follow." << endl;

}

void processCommandlineParameters(int argc, char **argv)
{
  try {
    // define allowed command line options
    bpo::options_description optDesc("Allowed options");
    optDesc.add_options()
      (c_HelpOptionShort, "produce help message")
      ;

    bpo::options_description reqDesc("Necessary options");
    reqDesc.add_options()
      (c_LoadOptionShort, bpo::value<string>(), "ARFF file which is loaded into a training data container.")
      (c_SaveOptionShort, bpo::value<string>(), "ARFF file the training data container is written to.")
      ;

    bpo::options_description optionsDescr("All options");
    optionsDescr.add(optDesc).add(reqDesc);

    // evaluate command line parameters
    bpo::variables_map vm;
    bpo::store(bpo::command_line_parser(argc, argv).options(optionsDescr).run(), vm);
    bpo::notify(vm);

    if (vm.count(c_HelpOption) || !vm.count(c_LoadOption) || !vm.count(c_SaveOption)){
      printUsageHint(argv[0], optionsDescr);
      exit(0);
    }

    g_inputfilename = vm[c_LoadOption].as<string>();
    g_outputfilename = vm[c_SaveOption].as<string>();
    
  } catch (exception& e){
    cerr << "Error: " << e.what() << endl;
    exit(1);
  } catch (...){
    cerr << "Exception of unknown type in the program." << endl;
    exit(2);
  }
}

int main(int argc, char **argv)
{
  processCommandlineParameters(argc, argv);
  
  std::cerr << "Using ARFF file handler library" << std::endl;

  TrainingsDataContainer* readData = ARFFFileHandler::load(g_inputfilename);

  bool res = ARFFFileHandler::save(readData, g_outputfilename);

  return res;
}


