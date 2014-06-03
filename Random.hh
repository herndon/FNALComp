#ifndef Random_hh
#define Random_hh
#include<random>

std::default_random_engine & globalEngine( );

void seedEngine( );

double getUniformDouble ( );

double getNormalDouble ( );
#endif // Random_hh
