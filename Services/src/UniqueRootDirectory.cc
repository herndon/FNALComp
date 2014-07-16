#include "UniqueRootDirectory.hh"

#include "TNamed.h"
#include "TDirectory.h"

fc::UniqueRootDirectory::UniqueRootDirectory ( std::string const& name ):
  _savedDirectory(gDirectory),
  _name(name),
  _tdir(nullptr){

  std::string uniqueName = ensureUnique( _name );
  _tdir=gDirectory->mkdir( uniqueName.c_str() );
  _tdir->cd();
  //_tdir->ls();
}

fc::UniqueRootDirectory::~UniqueRootDirectory (){
  _savedDirectory->cd();
}

void fc::UniqueRootDirectory::addToFile ( TNamed* obj,
					  std::string const& name,
					  std::string const& title){
  obj->SetName( name.c_str() );
  obj->SetTitle( title.c_str() );
  _tdir->Append(obj);
}

// Do nothing for now; fill in a real implementation.
std::string fc::UniqueRootDirectory::ensureUnique( std::string const& name){
  return name;
}
