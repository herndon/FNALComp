#include "Exception.hh"
#include "UniqueRootDirectory.hh"

#include "TNamed.h"
#include "TDirectory.h"
#include "TFile.h"

#include <sstream>

// Helper functions.
namespace {

  TFile * getTFileOrThrow( std::string const& name ){

    if ( !gFile ){
      throw fc::Exception("UniqueRootDirectory::getTFileOrThrow: for directory named "
			  + name
			  + " gFile is not set. Is there an open TFile?");
    }
    return gFile;

  }

  // Form filenames of the form name_n, where name and n are passed in as arguments.
  std::string decoratedName ( std::string const& baseName, size_t n){

    if ( n == 0 ) return baseName;

    std::ostringstream out;
    out << "_" << n;
    std::string name(baseName) ;
    name += out.str();
    return name;
  }

  // Form a unqiue name by appeneding _n onto the base name
  std::string ensureUnique( std::string const& baseName){

    // A wild guess.
    static const size_t maxAttempts(10);

    TFile* file = getTFileOrThrow(baseName);

    for ( size_t i=0; i<maxAttempts; ++i ){
      std::string const& name = decoratedName( baseName, i);
      TDirectory* sdir = file->GetDirectory( name.c_str() );
      if ( !sdir ) return name;
    }

    throw fc::Exception("UniqueRootDirectory::ensureUnqiue: for directory named "
			+ baseName
			+ " Could not form a unique name .... quitting");

  }


}

fc::UniqueRootDirectory::UniqueRootDirectory ( std::string const& name ):
  _savedDirectory(gDirectory),
  _name(name),
  _tdir(nullptr){

  std::string uniqueName = ensureUnique( _name );
  _tdir=gDirectory->mkdir( uniqueName.c_str() );
  _tdir->cd();

}


// Restore initial working directory.
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
