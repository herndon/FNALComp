#include <fstream>
#include <iostream>
#include <string>
#include "Exception.hh"
#include "Config.hh"

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
      } else {
	throw Exception("Config::_initConfig: Unrecongnized parameter in configfile.txt");
      }
    }

  } // endl while loop through configfile

}


 
void fc::Config::printConfig( void ) const {

  std::cout << "Program configuration information" << std::endl;

  if (_genData) {
    std::cout << "genData configuration" << std::endl;
    std::cout << "Number Events: " << _numberEvents << std::endl;
    std::cout << "Number Tracks " << _numberTracks << std::endl;
    std::cout << "Random number seed " << _seed << std::endl;
  }

}