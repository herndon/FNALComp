#include <fstream>
#include <iostream>
#include <string>
#include "Services/include/Exception.hh"
#include "Services/include/Config.hh"

fc::Config::Config(std::ifstream & configfile,int genData):
  _genData(genData),
  _numberEvents(128),
  _numberTracks(10),
  _seed(1) {

  _initConfig(configfile);
  printConfig();

}

void fc::Config::_initConfig(std::ifstream & configfile) {


  if (!configfile){
    throw Exception("Config::_initConfig: can't open configfile.txt file");
  }

  std::string configString;
 

  while(configfile){
    configfile >> configString; 

    if (_genData){
      if (configString == "NumberEvents") {
	configfile >> _numberEvents;
      } else if (configString == "NumberTracks") { 
	configfile >> _numberTracks;
      } else if (configString == "Seed") {
	configfile >> _seed;
      } else if (configString == "RootFileName") {
	configfile >> _rootFileName;
      } else {
	throw Exception("Config::_initConfig: Unrecognized parameter in configfile.txt");
      }
    } else {
      if (configString == "RootFileName") {
	configfile >> _rootFileName;
      } else if (configString == "minCandPTCut") {
	configfile >> _minCandPTCut;
      } else if (configString == "minPTCut") {
	configfile >> _minPTCut;
      } else if (configString == "maxChi2NDofCut") {
	configfile >> _maxChi2NDofCut;
      } else {
	throw Exception("Config::_initConfig: Unrecognized parameter in configfile.txt");
      }

    }

  } // endl while loop through configfile

}


 
void fc::Config::printConfig( void ) const {

  std::cout << "Program configuration information" << std::endl;

  if (_genData) {
    std::cout << "gen Data configuration" << std::endl;
    std::cout << "Number Events: " << _numberEvents << std::endl;
    std::cout << "Number Tracks " << _numberTracks << std::endl;
    std::cout << "Random number seed " << _seed << std::endl;
    std::cout << "Root file name " << _rootFileName << std::endl;
  } else {
    std::cout << "reco Data configuration" << std::endl;
    std::cout << "Root file name " << _rootFileName << std::endl;
  }

}
