#ifndef Source_hh
#define Source_hh
#include <memory>

namespace fc {
  class Event;

  class Source {
  public:
    Source() = default;
    Source(const Source&) = delete;
    virtual ~Source();

    //Returns an empty unique_ptr when out of events
    virtual std::unique_ptr<Event> getNextEvent() = 0;

  };
}

#endif //Source_hh
