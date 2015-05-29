#ifdef STAT

#include "solver/interface.hpp"
#include "main/Options.hpp"

Result* solvePoint (LTA::Lta*, Options*, istream&,  ostream&);

Result* solveSmall (LTA::Lta*, Options*, istream&,  ostream&);

Result* solveLive (LTA::Lta*, Options*, istream&,  ostream&);

#endif
