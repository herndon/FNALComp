#include "Services/include/Random.hh"
#include <iostream>

fc::Random::Random(int seed){
  engine.seed(seed);
  std::cout << "Random::Random constructor. Using seed: " << seed << std::endl; 
}

double fc::Random::getUniformDouble(double min, double max) {
  std::uniform_real_distribution<double> dist(min,max);
  return dist( getEngine());
}

double fc::Random::getNormalDouble (double mean, double dev) {
  std::normal_distribution<double> dist(mean,dev);
  return dist( getEngine());
}
