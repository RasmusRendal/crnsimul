%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.4.2"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  # include <map>
  # include <vector>
  # include <exception>
  # include "reaction.h"
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "driver.h"
}

%define api.token.prefix {TOK_}
%token
    END  0          "end of file"
    T_RIGHTARROW    "->"
    T_BIARROW       "<->"
    T_PARENSTART    "("
    T_PARENEND      ")"
    T_SET           ":="
    T_PLUS          "+"
    T_END           ";"
;

%token <std::string>    T_NAME      "name"
%token <int>            T_NUMBER    "number"
%nterm <std::vector<Reaction>> reactions
%nterm <Reaction> reaction
%nterm <std::map<std::string, int>> species
%nterm <std::pair<std::string, int>> specie

// Bison enforces that all datatypes used in terms are printable
// with the << operator, or have a custom printer defined.
// We bypass this by doing nothing.
%printer {} <std::vector<Reaction>>
%printer {} <Reaction>
%printer {} <std::map<std::string, int>>
%printer {} <std::pair<std::string, int>>

%printer {yyo << $$;} <*>;

%%

%start program;

program         : %empty {}
                | concentrations {}
                | concentrations reactions { drv.reactions = $2; }
                ;

concentrations  : concentration {}
                | concentrations concentration {}
                ;

concentration   : "name" ":=" "number" ";" {drv.Concentration[$1] = $3;}
                ;

reactions       : reaction { auto r = std::vector<Reaction>(); r.push_back(std::move($1)); $$ = r; }
                | reactions reaction { auto r = $1; r.push_back($2); $$ = r; }
                ;

reaction        : species "->" species ";" { $$ = Reaction($1, $3, 1); }
                ;

species         : specie { auto map = std::map<std::string, int>(); map.insert(std::move($1)); $$ = map; }
                | species "+" specie { auto map = std::move($1); map.insert(std::move($3)); $$ = $1; }
                | "number" { if ($1 != 0) throw std::runtime_error("Parser error"); $$ = std::map<std::string, int>(); }
                ;

specie          : "number" "name" { $$ = std::make_pair<std::string,int>(std::move($2), std::move($1)); }
                | "name" { $$ = std::make_pair<std::string,int>(std::move($1), 1); }
                ;

%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  //std::cerr << l << ": " << m << '\n';
}
