//
// Created by NebelFox on 16.04.2021.
//

#include "Polynom.h"

// utils
double powf (const double base, unsigned power)
{
    double result = base;
    while (power > 1)
    {
        result *= base;
        --power;
    }
    return result;
}

// constructors
Polynom::Polynom () = default;

Polynom::Polynom (string && pattern)
{
    Monom temp {};
    pattern.erase (remove (pattern.begin (), pattern.end (), ' '), pattern.end ());
    if (pattern[0] != '-' && pattern[0] != '+') pattern.insert (pattern.begin (), '+');
    std::vector<size_t> edges;
    for (size_t i=0; i<pattern.size (); ++i)
    {
        if (pattern[i] == '+' || pattern[i] == '-')
        {
            edges.push_back (i);
        }
    }

    size_t i, j=0;
    for (const size_t edge : edges)
    {
        temp.power = 0;
        temp.coef = (pattern[edge] == '-') ? -1.0f : 1.0f;
        i = edge + 1;
        while (pattern[i+j] == '.' || (pattern[i+j] >= '0' && pattern[i+j] <= '9')) ++j;
        if (j > 0) temp.coef *= std::stof (pattern.substr (i, j));
        i += j;
        if (pattern[i] == 'x')
        {
            temp.power = 1;
            if (pattern[++i] == '^')
            {
                j = 0;
                ++i;
                while (pattern[i+j] >= '0' && pattern[i+j] <= '9') ++j;
                if (j > 0) temp.power = std::stoi (pattern.substr (i, j));
            }
        }
        this->monoms.push_back (
            Monom {
                .coef = temp.coef,
                .power = temp.power
            }
        );
        j=0;
    }
    this->monoms.sort (compare);
    groupUp (this->monoms);

}
Polynom::Polynom (const Monom && instance)
{
    this->monoms.push_back (
        Monom {
            .coef = instance.coef,
            .power = instance.power
        }
    );
}
Polynom::Polynom (const list<Monom> & instance)
{
    deepcopy (instance, this->monoms);
}

// destructors
Polynom::~Polynom () = default;

// copying
Polynom::Polynom (const Polynom &instance)
{
    deepcopy (instance.monoms, this->monoms);
}
Polynom& Polynom::operator= (const Polynom & instance)
{
    this->monoms.clear ();
    for (const auto & monom : instance.monoms)
    {
        this->monoms.push_back (
            Monom {
                .coef = monom.coef,
                .power = monom.power
            }
        );
    }
    return *this;
}

// static methods
bool Polynom::compare (const Monom & left, const Monom & right)
{
    return (left.power > right.power);
}
void Polynom::groupUp (list<Monom> & instance)
{

    if (!instance.empty ())
    {
        auto prev = instance.begin ();
        for (auto current = ++instance.begin (); current != instance.end ();)
        {
            if (prev->power == current->power)
            {
                current->coef += prev->coef;
                current = instance.erase (prev);
            }
            prev = current++;
        }
    }
    purify (instance);
}
void Polynom::deepcopy (const list<Monom> & from, list<Monom> & to)
{
    for (const auto & item : from)
    {
        to.push_back (
            Monom {
                .coef = item.coef,
                .power = item.power
            }
        );
    }
}
void Polynom::purify (list<Monom> & instance)
{
    for (auto item = instance.cbegin (); item != instance.cend ();)
    {
        if (item->coef == 0.0f)
            item = instance.erase (item);
        else
            ++item;
    }
}
bool Polynom::match (const string & instance)
{
    return regex_match (instance, polynomPattern ());
}

// constants
const regex& Polynom::polynomPattern ()
{
    static const regex pattern  ("(\\s*(\\+|-)?\\s*((([0-9.]+)(x(\\^[0-9]+)?)?)|([0-9.]+)?x(\\^[0-9]+)?))+");
//    pattern = regex ("^\\s*");
    return pattern;
}

// addition
Polynom operator+ (const Polynom & left, const Polynom & right)
{
    list<Polynom::Monom> leftList;
    Polynom::deepcopy (left.monoms, leftList);
    list<Polynom::Monom> rightList;
    Polynom::deepcopy (right.monoms, rightList);
    leftList.merge (rightList, Polynom::compare);
    Polynom::groupUp (leftList);
    return Polynom (leftList);
}
Polynom& Polynom::operator+= (const Polynom & other)
{
    list<Monom> others;
    deepcopy (other.monoms, others);
    this->monoms.merge (others, compare);
    groupUp (this->monoms);
    return *this;
}

// subtraction
Polynom Polynom::operator- () const
{
    list<Monom> instance;
    deepcopy (this->monoms, instance);
    for (auto & item : instance)
    {
        item.coef = -item.coef;
    }
    return Polynom (instance);
}
Polynom operator- (const Polynom & left, const Polynom & right)
{
    return left + (-right);
}
Polynom& Polynom::operator-= (const Polynom & other)
{
    Polynom temp = -other;
    this->monoms.merge (temp.monoms, compare);
    groupUp (this->monoms);
    return *this;
}

// multiplication
Polynom operator* (const Polynom & base, const int multiplier)
{
    list<Polynom::Monom> instance;
    Polynom::deepcopy (base.monoms, instance);
    for (auto & item : instance)
    {
        item.coef *= multiplier;
    }
    return Polynom (instance);
}
Polynom& Polynom::operator*= (const int multiplier)
{
    for (auto & item : this->monoms)
    {
        item.coef *= multiplier;
    }
    return *this;
}
Polynom operator* (const int multiplier, const Polynom & base)
{
    return base * multiplier;
}
Polynom operator* (const Polynom & base, const double multiplier)
{
    list<Polynom::Monom> instance;
    Polynom::deepcopy (base.monoms, instance);
    for (auto & item : instance)
    {
        item.coef *= multiplier;
    }
    return Polynom (instance);
}
Polynom operator* (const double multiplier, const Polynom & base)
{
    return base * multiplier;
}

Polynom& Polynom::operator*= (const double multiplier)
{
    for (auto & item : this->monoms)
    {
        item.coef *= multiplier;
    }
    return *this;
}
Polynom operator* (const Polynom & base, const Polynom::Monom & multiplier)
{
    list<Polynom::Monom> instance;
    for (const auto & item : base.monoms)
    {
        instance.push_back (
            Polynom::Monom {
                .coef = item.coef * multiplier.coef,
                .power = item.power + multiplier.power
            }
        );
    }
    return Polynom (instance);
}
Polynom operator* (const Polynom::Monom & multiplier, const Polynom & base)
{
    return base * multiplier;
}
Polynom& Polynom::operator*= (const Monom & multiplier)
{
    for (auto & item : this->monoms)
    {
        item.coef *= multiplier.coef;
        item.power += multiplier.power;
    }
    return *this;
}
Polynom operator* (const Polynom & left, const Polynom & right)
{
    list<Polynom::Monom> instance;
    for (const auto & item : right.monoms)
    {
        Polynom temp = left * item;
        instance.insert (instance.cend (), temp.monoms.cbegin (), temp.monoms.cend ());
    }
    instance.sort (Polynom::compare);
    Polynom::groupUp (instance);
    return Polynom (instance);
}
Polynom& Polynom::operator*= (const Polynom & other)
{
    for (auto item = this->monoms.begin (); item != this->monoms.end (); ++item)
    {
        Polynom temp = (*this) * (*item);
        this->monoms.erase (item);
        for (const auto & monom : temp.monoms)
        {
            this->monoms.push_back (monom);
        }
    }
    this->monoms.sort (compare);
    groupUp (this->monoms);
    return *this;
}

// division
Polynom operator/ (const Polynom & base, const double divider)
{
    Polynom instance (base);
    for (auto & item : instance.monoms)
    {
        item.coef /= divider;
    }
    return instance;
}
Polynom& Polynom::operator/= (const double divider)
{
    for (auto & item : this->monoms)
    {
        item.coef /= divider;
    }
    return *this;
}
Polynom operator/ (const Polynom & base, const Polynom::Monom & divider)
{
    Polynom instance (base);
    for (auto & item : instance.monoms)
    {
        item.coef /= divider.coef;
        item.power -= divider.power;
    }
    return instance;
}
Polynom& Polynom::operator/= (const Monom & divider)
{
    for (auto & item : this->monoms)
    {
        item.coef /= divider.coef;
        item.power -= divider.power;
    }
    return *this;
}
Polynom operator/ (const Polynom & base, const Polynom & divider)
{
    if (divider.empty () || divider.zero ())
    {
        return Polynom {};
    }
    if (divider.power () > base.power ())
    {
        return Polynom (Polynom::Monom {0, 0});
    }
    list<Polynom::Monom> instance;
    Polynom temp (base);
    Polynom::Monom multiplier {};
    while (temp.power () >= divider.power ())
    {
        multiplier.coef = temp.monoms.cbegin ()->coef / divider.monoms.cbegin ()->coef;
        multiplier.power = temp.monoms.cbegin ()->power - divider.monoms.cbegin ()->power;
        temp -= (divider * multiplier);
        instance.push_back (
            Polynom::Monom {
                .coef = multiplier.coef,
                .power = multiplier.power
            }
        );
    }
//    cout << "Division complete\n";
    return Polynom (instance);
}
Polynom& Polynom::operator/= (const Polynom & divider)
{
    if (divider.empty () || divider.zero ())
    {

    }
    if (divider.power () > this->power ())
    {
        this->clear ();
        this->monoms.push_back (
            Monom {
                .coef = 0.0f,
                .power = 0
            }
        );
    }

    Monom multiplier {};
    while (this->power () >= divider.power ())
    {
        multiplier.coef = this->monoms.cbegin ()->coef / divider.monoms.cbegin ()->coef;
        multiplier.power = this->monoms.cbegin ()->power - divider.monoms.cbegin ()->power;
        *this -= (divider * multiplier);
    }

    return *this;
}

// equality
bool operator== (const Polynom & left, const Polynom & right)
{
    if (left.monoms.size() != right.monoms.size ())
        return false;

    for (auto leftIt = left.monoms.cbegin (), rightIt = right.monoms.cbegin (); leftIt != left.monoms.cend (); ++leftIt, ++rightIt)
    {
        if (leftIt->power != rightIt->power || leftIt->coef != rightIt->coef)
            return false;
    }
    return true;
}
bool operator!= (const Polynom & left, const Polynom & right)
{
    return !(left == right);
}

// extra operators
Polynom Polynom::pow (unsigned power) const
{
    if (power == 0)
        return Polynom (Monom {1.0f, 0});
    else
    {
        Polynom instance (*this);
        while (power > 1)
        {
            instance = instance * (*this);
            --power;
        }
        return instance;
    }
}
double Polynom::super (const double X) const
{
    /*double sum = 0.0f;
    unsigned power = 0;
    double x = 1.0f;
    for (auto iterator = this->monoms.crbegin (); iterator != this->monoms.crend (); ++iterator)
    {
        while (iterator->power > power)
        {
            x *= __x;
            ++power;
        }
        sum += x * iterator->coef;
    }
    return sum;*/
    auto iterator = this->monoms.cbegin ();
    double sum = iterator->coef;
    unsigned prev_power = iterator->power;
    ++iterator;
    for (; iterator != this->monoms.cend (); ++iterator)
    {
        while (prev_power > iterator->power)
        {
            sum *= X;
            --prev_power;
        }
        sum += iterator->coef;
    }
    return sum;
}
Polynom Polynom::super (const Polynom & X) const
{
    Polynom instance;
    unsigned power = 0;
    Polynom x ("1");

    for (auto iterator = this->monoms.crbegin (); iterator != this->monoms.crend (); ++iterator)
    {
        while (iterator->power > power)
        {
            x = x * X;
            ++power;
        }
        instance += iterator->coef * x;
    }

    return instance;
}



// information
bool Polynom::empty () const
{
    return this->monoms.empty ();
}
bool Polynom::zero () const
{
    for (const auto & item : this->monoms)
    {
        if (item.coef != 0.0f)
        {
            return false;
        }
    }
    return true;
}
unsigned Polynom::length () const
{
    return this->monoms.size ();
}
Polynom::power_t Polynom::power () const
{
    return (this->monoms.cbegin ()->power);
}

const Polynom::Monom& Polynom::head () const
{
    return *(this->monoms.cbegin ());
}

// space manipulation
void Polynom::clear ()
{
    this->monoms.clear ();
}

// interface
void Polynom::print () const
{
    if (this->empty () || this->zero ())
    {
        cout << '0';
    }
    else
    {
        auto iterator = this->monoms.cbegin ();

        if (iterator->coef < 0.0f)
        {
            cout << '-';
        }
        if (iterator->power == 0 || (iterator->coef != 1.0 && iterator->coef != -1.0f))
            cout << ((iterator->coef < 0.0f) ? -iterator->coef : iterator->coef);

        if (iterator->power != 0)
        {
            cout << 'x';
            if (iterator->power != 1)
            {
                cout << '^';
                cout << iterator->power;
            }
        }
        ++iterator;
        for (; iterator != this->monoms.cend (); ++iterator)
        {
            cout << ((iterator->coef < 0.0f) ? " - " : " + ");
            if (iterator->coef != 1.0f && iterator->coef != -1.0f)
            {
                cout << ((iterator->coef < 0.0f) ? -iterator->coef : iterator->coef);
            }
            if (iterator->power != 0)
            {
                cout << 'x';
                if (iterator->power != 1)
                {
                    cout << '^' << iterator->power;
                }
            }
        }
    }
}