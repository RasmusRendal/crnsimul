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
using SpeciesPair = std::pair<std::string, int>;
using SpeciesList = std::map<std::string, int>;

void InsertToSpecieMap(SpeciesList &list, SpeciesPair &toInsert) {
	if (list.find(toInsert.first) == list.end()) {
		list.insert(toInsert);
	} else {
		list[toInsert.first] += toInsert.second;
	}
  }
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
                | concentrations reactions { drv.network.reactionList = $2; }
                ;

concentrations  : concentration {}
                | concentrations concentration {}
                ;

concentration   : "name" ":=" "number" ";" {drv.network.initNetworkState[$1] = $3;}
                ;

reactions       : reaction { auto r = std::vector<Reaction>(); r.push_back(std::move($1)); $$ = r; }
                | reactions reaction { auto r = $1; r.push_back($2); $$ = r; }
                ;

reaction        : species "->" species ";" { $$ = Reaction($1, $3, 1); }
                | species "->" "(" "number" ")" species ";" { $$ = Reaction($1, $6, $4); }
                ;

species         : specie { auto map = std::map<std::string, int>(); InsertToSpecieMap(map, $1); $$ = map; }
                | species "+" specie { auto map = std::move($1); InsertToSpecieMap(map, $3); $$ = map; }
                | "number" { if ($1 != 0) throw std::runtime_error("Parser error"); $$ = std::map<std::string, int>(); }
                ;

specie          : "number" "name" { $$ = std::make_pair<std::string,int>(std::move($2), std::move($1)); }
                | "name" { $$ = std::make_pair<std::string,int>(std::move($1), 1); }
                ;

%%

void yy::parser::error (const location_type&, const std::string&)
{
  //std::cerr << l << ": " << m << '\n';
}
