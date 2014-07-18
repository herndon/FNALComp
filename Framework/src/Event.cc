#include <sstream>
#include "Framework/include/Event.hh"
#include "Services/include/Exception.hh"

void fc::Event::throwNotFoundException(ProductID const& id) const {
  std::ostringstream os;
  os<<"Could not find product type:"<<id.typeName()<<" label:"<<id.label();
  throw Exception(os.str());
}
