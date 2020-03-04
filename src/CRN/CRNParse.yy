%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.4.2"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
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

%printer {yyo << $$;} <*>;

%%

%start program;

program         : %empty {}
                | concentrations {}
                | concentrations reactions {}
                ;

concentrations  : concentration {}
                | concentrations concentration {}
                ;

concentration   : "name" ":=" "number" ";" {drv.Concentration[$1] = $3;}
                ;

reactions       : reaction {}
                | reactions reaction {}
                ;

reaction        : species "->" species ";" {}
                | species "<->" species ";" {}
                ;

species         : specie {}
                | species "+" specie {}
                ;

specie          : "number" "name" {}
                | "name" {}
                ;

%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
