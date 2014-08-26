#include "Services/include/Random.hh"
#include <iostream>

fc::Random::Random(int seed) {
    _engine.seed(seed);
}

double fc::Random::uniformDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min,max);
    return dist( engine());
}

double fc::Random::normalDouble (double mean, double dev) {
    std::normal_distribution<double> dist(mean,dev);
    return dist( engine());
}

double fc::Random::exponentialDouble (double exponent) {
    std::exponential_distribution<double> dist(exponent);
    return dist( engine());
}
