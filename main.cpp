#include "Polynom.h"

//using namespace std;

typedef void (*commandArgless_t) ();
typedef void (*commandOneArg_t) (const string&);

using std::map;
using std::cin;

map<string, Polynom> environment;
map<string, commandArgless_t> arglessCommands;
map<string, commandOneArg_t> oneArgCommands;

void help ()
{
    cout << "#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#\n"
            "# THE FOLLOWING COMMANDS ARE AVAILABLE:                                 #\n"
            "#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#\n"
            "# @set name polynomial - save polynomial to the environment as 'name'   #\n"
            "# @do expression       - evaluate the expression and print it's result  #\n"
            "#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#\n"
            "# @show name  - print the 'name' polynomial from the environment        #\n"
            "# @list       - print all polynomials from the environment              #\n"
//            "#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#\n"
            "# @erase name - delete the 'name' polynomial from the environment       #\n"
            "# @clear      - delete all polynomials from the environment             #\n"
            "#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#\n"
            "# @syntax     - the guide on using '@set' & '@do'                       #\n"
            "# @help       - show this message again                                 #\n"
            "# @exit       - terminate the program                                   #\n"
            "#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#\n";
}

void syntax ()
{
    cout << "#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#\n"
            "# POLYNOMIAL SYNTAX:                                            #\n"
            "# A sequence of monomials, separated with + or -                #\n"
            "# Each monomials should be in one of the following formats:     #\n"
            "# kx^n | x^n | kx | k | x, where k - coefficient, n - power.    #\n"
            "# E.g. 'x^3 - 4x^2 + x - 12'                                    #\n"
            "# In @do command polynom must be enclosed in brackets.          #\n"
            "# E.g. '@do (2x-1) * 3.5'                                       #\n"
            "#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#\n"
            "# EXPRESSION SYNTAX:                                            #\n"
            "# 'left_operand' 'operator' 'right_operand'.                    #\n"
            "# Following operations are supported:                           #\n"
            "# Common: +, -, *, /, ^;                                        #\n"
            "# Specific: <- and -> - pass the operand from the arrow 'tail'  #\n"
            "#   to the operand from the arrow 'head' as X.                  #\n"
            "#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#\n";
}

bool exists (const string & name)
{
    return environment.find (name) != environment.end ();
}

void show (const string & name)
{
    if (exists (name))
    {
        environment[name].print ();
        cout << endl;
    }
    else
    {
        cout << "There is no polynomial with name '" << name << "'\n";
    }
}

void showAll ()
{
    if (environment.empty ())
    {
        cout << "The environment is empty\n";
    }
    else {
        for (const auto & item : environment)
        {
            cout << item.first << ": ";
            item.second.print ();
            cout << '\n';
        }
    }
}

void free (const string & name)
{
    if (exists (name))
    {
        cout << "The polynomial 'a': ";
        environment[name].print ();
        cout << " was successfully erased";
        environment.erase (name);
    }
    else
        cout << "There is no polynomial with name '" << name << "'\n";
}

void clear ()
{
    environment.clear ();
}

int main ()
{
    const regex commandArglessPattern ("^(list|clear|syntax|help)\\s*$");
    arglessCommands["list"] = showAll;
    arglessCommands["clear"] = clear;
    arglessCommands["syntax"] = syntax;
    arglessCommands["help"] = help;

    const regex commandOneArgPattern ("^(show|erase)\\s+([a-zA-Z_]+)\\s*$");
    oneArgCommands["show"] = show;
    oneArgCommands["free"] = free;

    const regex commandExitPattern ("^exit\\s*$");
    const regex commandSetPattern ("^set\\s+([a-zA-Z]+)\\s+([+0-9x^\\- ]+)\\s*$");
    const regex commandDoPattern ("^do\\s+"
                                  "(?:([a-zA-Z_]+)|([+\\-]?[0-9]+(?:\\.[0-9]+)?)|(?:\\(([+0-9x^\\- ]+)\\)))"
                                  "\\s+"
                                  "(\\+|-|\\*|/|\\^|<-|->)"
                                  "\\s+"
                                  "(?:([a-zA-Z_]+)|([+\\-]?[0-9]+(?:\\.[0-9]+)?)|(?:\\(([+0-9x^\\- ]+)\\)))"
                                  );
    Polynom r = 2.0f * Polynom ("+10x");


    help (); cout << endl;
    bool running = true;
    string input;
    smatch match;

    // are used in @do command
    // extracted to here to avoid declaring new variables each command call
    Polynom result;
    bool doPrint = true;

    while (running){
        cout << "@";
        getline (cin, input);
        if (regex_match (input, commandExitPattern))
        {
            running = false;
        }
        else
        {
            if (regex_match (input, match, commandArglessPattern))
            {
                arglessCommands[match[1].str ()] ();
            }
            else if (regex_match (input, match, commandOneArgPattern))
            {
                oneArgCommands[match[1].str ()] (match[2].str ());
            }
            else if (regex_match (input, match, commandSetPattern))
            {
                if (Polynom::match (match[2].str ()))
                    environment[match[1].str ()] = Polynom(match[2].str ());
                else
                    cout << "Polynomial syntax error\n";
            }
            else if (regex_match (input, match, commandDoPattern))
            {
                /*cout << "Items: ";
                for (const auto & item : match)
                {
                    cout << '\'' << item.str () << "' ";
                }
                cout << endl;*/

                Polynom left, right;
                // left operand
                if (!match[1].str ().empty ())
                {
                    if (!exists (match[1].str ()))
                    {
                        cout << "The polynomial with name '" << match[1].str () << "' doesn't exist\n";
                        continue;
                    }
                    else
                        left = environment[match[1].str ()];
                }
                else
                {
                    left = Polynom (match[2].str () + match[3].str ());
                }
                // right operand
                if (!match[5].str ().empty ())
                {
                    if (!exists (match[5].str ()))
                    {
                        cout << "The polynomial with name '" << match[5].str () << "' doesn't exist\n";
                        continue;
                    }
                    else
                        right = environment[match[5].str ()];
                }
                else
                {
                    right = Polynom (match[6].str () + match[7].str ());
                }
                string oper = match[4].str ();
                if (oper == "+")
                    result = left + right;
                else if (oper == "-")
                    result = left - right;
                else if (oper == "*")
                    result = left * right;
                else if (oper == "/")
                {
                    result = left / right;
                    doPrint = false;
                    cout << "The result: (";
                    right.print ();
                    cout << ")(";
                    result.print ();
                    cout << ')';
                    Polynom rest = left - (result * right);
                    if (!rest.zero ())
                    {
                        cout << ' ';
                        if (rest.head ().coef > 0.0f) cout << '+';
                        rest.print ();
                    }
                    cout << endl;
                }
                else if (oper == "^")
                    result = left.pow ((unsigned) right.head ().coef);
                else if (oper == "->")
//                    result = right.super (left);
                {
                    if (!match[2].str ().empty ())
                    {
                        result = Polynom (Polynom::Monom {.coef = right.super (left.head ().coef)});
                    }
                    else
                        result = right.super (left);
                }
                else if (oper == "<-")
//                    result = left.super (right);
                    if (!match[5].str ().empty ())
                    {
                        result = Polynom (Polynom::Monom {.coef = left.super (right.head ().coef)});
                    }
                    else
                        result = left.super (right);
                else
                {
                    cout << "Unsupported operation: '" << oper << "'\n";
                    continue;
                }

                if (doPrint)
                {
                    cout << "The result: ";
                    result.print ();
                    cout << endl;
                }
                doPrint = true;
            }
            else
            {
                cout << "Unsupported command\n";
            }

        }
        cout << endl;
    }
    return 0;
}
