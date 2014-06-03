#include "Random.hh"

std::default_random_engine & globalEngine( ){
  static std::default_random_engine engine{};
  return engine;
}

void seedEngine( ) {
  globalEngine().seed(1);
}

double getUniformDouble ( ) {
  static std::uniform_real_distribution<double> dist(0.0,1.0);
  return dist( globalEngine());
}

double getNormalDouble ( ) {
  static std::normal_distribution<double> dist(0.0,1.0);
  return dist( globalEngine());
}
