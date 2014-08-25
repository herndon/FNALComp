#ifndef Services_Random_hh
#define Services_Random_hh
#include<random>

namespace fc {

///
/// Class Random
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-05
/// Wrapper for std library random functions
///

class Random {


public:

    explicit Random(int);

    std::default_random_engine & engine() {
        return _engine;
    }

    double uniformDouble (double min, double max);

    double normalDouble (double mean, double dev);

    double exponentialDouble (double exponent);

private:

    std::default_random_engine _engine;

};
}// end namespace fc

#endif // Services_Random_hh
