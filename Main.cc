/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Main.cc

    \par Last Author: Martin Loesch (<loesch@@ira.uka.de>)
    \par Date of last change: 29.11.09

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
#include "Containers.h"
#include "ARFFFileHandler.h"


const char c_CommandOption[] = "command";
const char c_CommandOptionShort[] = "command,c";

const char c_HelpOption[] = "help";
const char c_HelpOptionShort[] = "help,h";

const char c_LoadOption[] = "load";
const char c_LoadOptionShort[] = "load,l";

const char c_SaveOption[] = "save";
const char c_SaveOptionShort[] = "save,s";


OperationMode parseOperationMode(string mode)
{
  OperationMode res=INVALID;
  
  if ( mode=="copy" || mode=="cp"){
    res = COPY; }
  else if ( mode=="show" || mode=="sh"){
    res = SHOW; }
  else { res = INVALID; }

  return res;
}

void printUsageHint(string progname, bpo::options_description& options)
{
  cout << endl;
  cout << "Usage: " << progname.c_str() << " command [OPTIONS] " << endl << endl;
  cout << options << endl;
  cout << endl;
  cout << "The command controls what the manager does with the given files. Possible values are:" << endl;
  cout << " copy,cp   Copy the (first) loaded file to the save file(s)." << endl;
  cout << " show,sh   Show the (first) loaded file to standard output." << endl;
}

int performCopy()
{
  ArffFileHandling::ARFFFileHandler handler;

  handler.load(g_inputfilename);
  std::cerr<< "** Successfully loaded file :  " << g_inputfilename << std::endl;
  
  int res = handler.save(g_outputfilename);
  std::cerr << "** Successfully stored data to a file :  " << g_outputfilename << std::endl;

  return res;
}

int performShow()
{
  ArffFileHandling::ARFFFileHandler handler;
  handler.load(g_inputfilename);

  std::cerr<< "** Successfully loaded file :  " << g_inputfilename << std::endl;
  
  ArffFileHandling::TrainingsDataContainer* readData = handler.getData();
  readData->print(std::cout);

  std::cerr << "** Printed loaded data to standard output" << std::endl;

  return 0;
}

int processCopyParameters(bpo::variables_map& vm)
{
  if ( !vm.count(c_LoadOption) || !vm.count(c_SaveOption) ){
    return 1;
  }
  
  g_inputfilename = vm[c_LoadOption].as<string>();
  g_outputfilename = vm[c_SaveOption].as<string>();

  return 0;
}

int processShowParameters(bpo::variables_map& vm)
{
  if (!vm.count(c_LoadOption)){
    return 1;
  }
  
  g_inputfilename = vm[c_LoadOption].as<string>();

  return 0;
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
      (c_CommandOptionShort, bpo::value<string>(), "")
      ;

    bpo::positional_options_description pdesc;
    pdesc.add(c_CommandOption, 1);

    bpo::options_description optionsDescr("All options");
    optionsDescr.add(optDesc).add(reqDesc);
    
    // evaluate command line parameters
    bpo::variables_map vm;
    bpo::store(bpo::command_line_parser(argc, argv).options(optionsDescr).positional(pdesc).run(), vm);
    bpo::notify(vm);

    if ( vm.count(c_HelpOption) || !vm.count(c_CommandOption)){
      printUsageHint(argv[0], optionsDescr);
      exit(0);
    }

    g_mode = parseOperationMode(vm[c_CommandOption].as<string>());
    int parseRes=0;
    switch (g_mode){
    case COPY: parseRes = processCopyParameters(vm);
          break;
    case SHOW: parseRes = processShowParameters(vm);
	  break;
    case INVALID: printUsageHint(argv[0], optionsDescr);
	     exit(1);
    default: break;
    }

    if (parseRes){
      printUsageHint(argv[0], optionsDescr);
      exit(1);
    }
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
  
  std::cerr << "*** Using ARFF file handler library ***" << std::endl;

  int res=0;
  
  switch (g_mode){
  case COPY: res = performCopy();
        break;
  case SHOW: res = performShow();
	break;
  default: exit(1);
  }
  
  return res;
}


