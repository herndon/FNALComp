//============================================================================
// dataGen.cc
// Main program for driving data gen Modules
// Calls DataGenModule
// 
// Planned development: adapt to framework when ready
//
// Author Matt Herndon, University of Wisconsin,
//                      Fermi National Accelerator Laboratory
// 2014-05-22
//============================================================================
#include "DataGenModule.hh"
#include "Random.hh"

int main ()
{

  // Initialie random number generator
  seedEngine();

  // Instantiate Module classes

  DataGenModule myDataGenModule;

  // Run Module classes

  myDataGenModule.run();

  return 0; 

}
