#ifndef UniqueRootDirectory_hh
#define UniqueRootDirectory_hh
//============================================================================
// UniqueRootDirectory.hh
// header for UniqueRootDirectory class
/// Create a uniquely named root directory at the top level of the currently opened file.
//  This is a hack until a better solution is created.
//
//============================================================================
#include <string>

class TDirectory;
class TNamed;

namespace fc {

///
/// Class UniqueRootDirectory
/// Create a uniquely named root directory at the top level of the currently opened file.
///

class UniqueRootDirectory {

 
public:

  UniqueRootDirectory(std::string const& dirName);
  ~UniqueRootDirectory();

  TDirectory* directory() const { return _tdir; }

  // For objects such as TGraphs that are not automagically written to the output file,
  // add this object to the list of thing to be written.
  void addToFile ( TNamed* obj, std::string const& name, std::string const& title);

private:

  // Save the current root directory at c'tor time.  Restore it at d'tor time.
  TDirectory* _savedDirectory;

  // Name of the directory to create.
  std::string _name;

  // The current TDirectory after it is created.
  TDirectory* _tdir;

  // Helper function to add decorations to the input name to ensure its uniqueness.
  std::string ensureUnique( std::string const& name);

};
} // end namespace fc

#endif // UniqueRootDirectory_hh
