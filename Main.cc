/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Main.cc

    \par Last Author: Martin Loesch (<professional@@martinloesch.net>)
    \par Date of last change: 22.05.13

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
#include <boost/filesystem.hpp>

/* my includes */
#include "Main.h"
#include "arffhandlerlib.h"


namespace bfs = boost::filesystem;


//@{
//! \name Constants for parameter names
const char c_CommandOption[] = "command";
const char c_CommandOptionShort[] = "command,c";

const char c_FeatureOption[] = "feature";
const char c_FeatureOptionShort[] = "feature,f";

const char c_HelpOption[] = "help";
const char c_HelpOptionShort[] = "help,h";

const char c_LoadOption[] = "load";
const char c_LoadOptionShort[] = "load,l";

const char c_SaveOption[] = "save";
const char c_SaveOptionShort[] = "save,s";
//@}


OperationMode parseOperationMode(string mode)
{
  OperationMode res=INVALID;
  
  if ( mode=="copy" || mode=="cp" ){
    res = COPY; }
  else if ( mode=="show" || mode=="sh" ){
    res = SHOW; }
  else if ( mode=="remove" || mode=="rm" ){
    res = REMFEATURE; }
  else if ( mode=="splitclasses" || mode=="sc" ){
    res = SPLITCLASSES; }
  else {
    res = INVALID; }

  return res;
}

void printUsageHint(string progname, bpo::options_description& options)
{
  cout << endl;
  cout << "Usage: " << progname.c_str() << " command [OPTIONS] " << endl << endl;
  cout << options << endl;
  cout << endl;
  cout << "The command controls what the manager does with the given files. Possible values are:" << endl;
  cout << " copy,cp     Copy the (first) loaded file to the save file(s)." << endl;
  cout << " remove,rm   Remove feature, which has to be given using 'feature' option." << endl;
  cout << " show,sh     Show the (first) loaded file to standard output." << endl;
  cout << " splitclasses, sc    Split data into single files for each class." << endl;
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

int performRemFeature()
{
  ArffFileHandling::ARFFFileHandler handler;
  handler.load(g_inputfilename);

  std::cerr << "** Successfully loaded file :  " << g_inputfilename << std::endl;

  int res=0;
  
  int success = handler.removeFeature(g_featurename);
  switch (success){
    case 0: std::cerr << "** Successfully removed feature >" << g_featurename << "< from data" << std::endl;
      break;
    case 1: std::cerr << "** Error: Feature >" << g_featurename << "< could not be removed for unknown reasons!" << std::endl;
      res = 1;
      break;
    case 2: std::cerr << "** Error: Feature name >" << g_featurename << "< is not unique, so the feature could not be removed!" << std::endl;
      res = 1;
      break;
    default: assert(false);
  }

  if (res==0){
    if ( g_outputfilename!= ""){
      res = handler.save(g_outputfilename);
      std::cerr << "** Successfully stored data to a file :  " << g_outputfilename << std::endl;
    } else {
      ArffFileHandling::ARFFData* readData = handler.getData();
      readData->printData(std::cout);
      
      std::cerr << "** Printed loaded data to standard output" << std::endl;
    }
  }
  
  return res;
}

int performSplitClasses()
{
  ArffFileHandling::ARFFFileHandler handler;
  handler.load(g_inputfilename);

  std::cerr << "** Successfully loaded file :  " << g_inputfilename << std::endl;

  ArffFileHandling::ARFFFileHandlerSet classData = handler.splitSingleClasses();

  bfs::path basename = bfs::path(g_outputfilename).parent_path();
  basename /= bfs::path(g_outputfilename).stem();
  bfs::path suffix = bfs::path(g_outputfilename).extension();
  for (std::map<string, ArffFileHandling::ARFFFileHandler* >::iterator it=classData.begin(); it!=classData.end(); ++it){
    string filename = basename.native() + "-" + it->first + suffix.native();
    it->second->save(filename);
  }

  return 0;
}

int performShow()
{
  ArffFileHandling::ARFFFileHandler handler;
  handler.load(g_inputfilename);

  std::cerr<< "** Successfully loaded file :  " << g_inputfilename << std::endl;
  
  ArffFileHandling::ARFFData* readData = handler.getData();
  readData->printData(std::cout);

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

int processRemFeatureParameters(bpo::variables_map& vm)
{
  if ( !vm.count(c_LoadOption) || !vm.count(c_FeatureOption) ){
    return 1;
  }

  g_inputfilename = vm[c_LoadOption].as<string>();
  g_featurename = vm[c_FeatureOption].as<string>();

  if ( vm.count(c_SaveOption) ){
    g_outputfilename = vm[c_SaveOption].as<string>();
  } else {
    g_outputfilename = "";
  }
  
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

int processSplitclassParameters(bpo::variables_map& vm)
{
  if ( !vm.count(c_LoadOption) || !vm.count(c_SaveOption) ){
    return 1;
  }

  g_inputfilename = vm[c_LoadOption].as<string>();
  g_outputfilename = vm[c_SaveOption].as<string>();

  return 0;
}

void processCommandlineParameters(int argc, char **argv)
{
  try {
    // define allowed command line options
    bpo::options_description optDesc("Allowed options");
    optDesc.add_options()
      (c_HelpOptionShort, "Produce help message.")
      (c_FeatureOptionShort, bpo::value<string>(), "Name of a feature the command is working on (see commands).")
      (c_SaveOptionShort, bpo::value<string>(), "ARFF file the training data container is written to.")
      ;

    bpo::options_description reqDesc("Necessary options");
    reqDesc.add_options()
      (c_LoadOptionShort, bpo::value<string>(), "ARFF file which is loaded into a training data container.")
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
      case REMFEATURE: parseRes = processRemFeatureParameters(vm);
	break;
      case SHOW: parseRes = processShowParameters(vm);
	break;
      case SPLITCLASSES: parseRes = processSplitclassParameters(vm);
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

//! \cond false
int main(int argc, char **argv)
{
  processCommandlineParameters(argc, argv);
  
  std::cerr << "*** Using ARFF file handler library ***" << std::endl;

  int res=0;
  
  switch (g_mode){
    case COPY: res = performCopy();
      break;
    case REMFEATURE: res = performRemFeature();
      break;
    case SHOW: res = performShow();
      break;
    case SPLITCLASSES: res = performSplitClasses();
      break;
    default: exit(1);
  }
  
  return res;
}
//! \endcond

