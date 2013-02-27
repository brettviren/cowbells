#include "Cowbells/PrimaryGeneratorUtil.h"
#include "Cowbells/strutil.h"
#include <Randomize.hh>
#include <stdexcept>
#include <iostream>

Cowbells::Timerator::Timerator(std::string kindesc)
    : m_gen(&Cowbells::Timerator::gen_zero), m_period(1.0), m_last_time(0.0)
{
    this->set_uri(kindesc);
}
Cowbells::Timerator::Timerator(double period, std::string distribution, double starting)
    : m_gen(&Cowbells::Timerator::gen_zero), m_period(1.0), m_last_time(0.0)
{
    set_period(period);
    set_distribution(distribution);
    set_starting(starting);
}

void Cowbells::Timerator::set_uri(std::string kindesc)
{
    const std::string delim = "&";
    std::string name = "";
    name = get_startswith_rest(kindesc,"timedist=",delim);
    if (name != "") {
        this->set_distribution(name);
    }
    this->set_period(uri_double(kindesc,"period",1.0));
    this->set_starting(uri_double(kindesc,"starting",0.0));
}

void Cowbells::Timerator::set_period(double period)
{
    m_period = period;
}
void Cowbells::Timerator::set_distribution(std::string distribution)
{
    distribution = lower(distribution);
    std::string which = 
        Cowbells::get_startswith("exponential,fixed,null,zero", distribution);
    if (which == "exponential") {
        std::cerr << "Using exponential time distribution" << std::endl;
        m_gen = &Cowbells::Timerator::gen_expon;
        return;
    }

    if (which == "fixed") {
        std::cerr << "Using fixed period time distribution" << std::endl;
        m_gen = &Cowbells::Timerator::gen_fixed;
        return;
    }

    if (which == "null" || which == "zero") {
        std::cerr << "Using no time distribution" << std::endl;
        m_gen = &Cowbells::Timerator::gen_zero;
        return;
    }

    std::cerr << "Unknown timing distribution: \"" << distribution << "\" using \""<<which<<"\"" << std::endl;
    throw std::invalid_argument("Unknown timing distribution requested");
}
void Cowbells::Timerator::set_starting(double starting)
{
    m_last_time = starting;
}
                               

double Cowbells::Timerator::operator()()
{
    return this->gen();
}
double Cowbells::Timerator::gen()
{
    return (this->*m_gen)();
}

double Cowbells::Timerator::gen_expon()
{
    double u = G4UniformRand();
    double dt = ((-1.0 * log(u)) * m_period);
    m_last_time += dt;
    return m_last_time;
}

double Cowbells::Timerator::gen_fixed()
{
    m_last_time += m_period;
    return m_last_time;
}

double Cowbells::Timerator::gen_zero()
{
    return 0.0;
}
