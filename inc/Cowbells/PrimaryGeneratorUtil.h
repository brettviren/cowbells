/**
 * \class PrimaryGeneratorUtil
 *
 * \brief Classes to assist in writing primary generator classes
 *
 * bv@bnl.gov Wed Feb 27 09:03:04 2013
 *
 */



#ifndef PRIMARYGENERATORUTIL_H
#define PRIMARYGENERATORUTIL_H

#include <string>

namespace Cowbells {




class Timerator {
    typedef double (Timerator::*Generator) ();
    Generator m_gen;
    double m_period;
    double m_last_time;
public:
    /// Create a time generator of given distribution and
    /// characteristic time period.  Distributions may be
    /// "exponential" or "fixed" periods.  All times given or returned
    /// are assumed to be in units of seconds.
    Timerator(double period_seconds = 1.0,
              std::string distribution="null", 
              double starting = 0.0);

    /// Parse kindesc for period/distribution/starting
    Timerator(std::string kindesc);

    void set_uri(std::string kindesc);
    void set_period(double period);
    void set_distribution(std::string distribution);
    void set_starting(double starting);

    /// Return the next time.
    double operator()();
    double gen();

    double gen_expon();
    double gen_fixed();
    double gen_zero();

};
}

#endif  // PRIMARYGENERATORUTIL_H
