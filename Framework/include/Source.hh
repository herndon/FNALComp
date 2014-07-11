#ifndef Source_hh
#define Source_hh

namespace fc {
  class Event;

  class Source {
  public:
    Source() = default;
    Source(const Source&) = delete;
    virtual ~Source();

    virtual bool fillNextEvent( Event& ) = 0;

  };
}

#endif //Source_hh
