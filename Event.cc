#include <sstream>
#include "Event.hh"
#include "Exception.hh"

void fc::Event::throwNotFoundException(ProductID const& id) {
  std::ostringstream os;
  os<<"Could not find product type:"<<id.typeName()<<" label:"<<id.label();
  throw Exception(os.str());
}
