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

    std::default_random_engine & getEngine() {
        return engine;
    }

    double getUniformDouble (double min, double max);

    double getNormalDouble (double mean, double dev);

    double getExponentialDouble (double exponent);

private:

    std::default_random_engine engine;

};
}// end namespace fc

#endif // Services_Random_hh
