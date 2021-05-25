//
// Created by NebelFox on 16.04.2021.
//

#ifndef INC_3_POLYNOMIALS_POLYNOM_H
#define INC_3_POLYNOMIALS_POLYNOM_H

#include <iostream>
#include <list>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>

using std::list;
using std::string;
using std::cout;
using std::endl;
using std::regex;
using std::smatch;
using std::regex_match;
using std::sregex_iterator;

class Polynom
{
public:
    using power_t = int;
    struct Monom {
        double coef;
        power_t power;
    };

    Polynom ();
//    explicit Polynom (const string & pattern, bool _);
    explicit Polynom (string && pattern);
    explicit Polynom (const list<Monom> & instance);
    explicit Polynom (const Monom && instance);

    Polynom (const Polynom & instance);

    ~Polynom ();

    static bool compare (const Monom & left, const Monom & right);
    static void groupUp (list<Monom> & instance);
    static void deepcopy (const list<Monom> & from, list<Monom> & to);
    static void purify (list<Monom> & instance);
    static bool match (const string & instance);
    static const regex& polynomPattern ();
    static const regex& additionPattern ();

    Polynom& operator= (const Polynom & instance);

    friend Polynom operator+ (const Polynom & left, const Polynom & right);
    Polynom& operator+= (const Polynom & other);

    Polynom operator- () const;

    friend Polynom operator- (const Polynom & left, const Polynom & right);
    Polynom& operator-= (const Polynom & other);

    friend Polynom operator* (const Polynom & base, int multiplier);
    friend Polynom operator* (const int multiplier, const Polynom & base);
    Polynom& operator*= (int multiplier);

    friend Polynom operator* (const Polynom & base, double multiplier);
    friend Polynom operator* (double multiplier, const Polynom & base);
    Polynom& operator*= (double multiplier);

    friend Polynom operator* (const Polynom & base, const Monom & multiplier);
    friend Polynom operator* (const Monom & multiplier, const Polynom & base);
    Polynom& operator*= (const Monom & multiplier);

    friend Polynom operator* (const Polynom & left, const Polynom & right);
    Polynom& operator*= (const Polynom & other);

    friend Polynom operator/ (const Polynom & base, double divider);
    Polynom& operator/= (double divider);

    friend Polynom operator/ (const Polynom & base, const Monom & divider);
    Polynom& operator/= (const Monom & divider);

    friend Polynom operator/ (const Polynom & base, const Polynom & divider);
    Polynom& operator/= (const Polynom & divider);

    friend bool operator== (const Polynom & left, const Polynom & right);
    friend bool operator!= (const Polynom & left, const Polynom & right);

    Polynom pow (unsigned power) const;
    double super (double x) const;
    Polynom super (const Polynom & x) const;

    bool empty () const;
    bool zero () const;
    const Monom& head () const;
    void clear ();
    unsigned length () const;
    power_t power () const;

    void print () const;

private:
    list<Monom> monoms;
};


#endif //INC_3_POLYNOMIALS_POLYNOM_H
