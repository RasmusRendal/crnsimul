%{
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.h"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type
  make_T_NUMBER (const std::string &s, const yy::parser::location_type& loc);
  yy::parser::symbol_type
  make_T_DECIMAL (const std::string &s, const yy::parser::location_type& loc);
%}

int             [0-9]+
decimal         [0-9]+\.[0-9]+
T_NAME          [a-zA-Z][a-zA-Z_0-9]*
T_RIGHTARROW    "->"
T_BIARROW       "<->"
T_PARENSTART    "("
T_PARENEND      ")"
T_SET           ":="
T_PLUS          "+"
T_END           ";"
T_NEWLINE       [\n]
blank           [ \t\r]

%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}
"#".* loc.step();
{blank}+    loc.step();
\n+         loc.lines(yyleng); loc.step();

{T_RIGHTARROW}  return yy::parser::make_T_RIGHTARROW  (loc);
{T_BIARROW}     return yy::parser::make_T_BIARROW     (loc);
{T_PARENSTART}  return yy::parser::make_T_PARENSTART  (loc);
{T_PARENEND}    return yy::parser::make_T_PARENEND    (loc);
{T_SET}         return yy::parser::make_T_SET         (loc);
{T_PLUS}        return yy::parser::make_T_PLUS        (loc);
{T_END}         return yy::parser::make_T_END         (loc);

{int}           return make_T_NUMBER(yytext, loc);
{decimal}       return make_T_DECIMAL(yytext, loc);
{T_NAME}        return yy::parser::make_T_NAME (yytext, loc);
.               {
                  throw yy::parser::syntax_error
                  (loc, "invalid character: " + std::string(yytext));
                }

<<EOF>>    return yy::parser::make_END (loc);
%%

yy::parser::symbol_type
make_T_NUMBER (const std::string &s, const yy::parser::location_type& loc)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_T_NUMBER ((int) n, loc);
}

yy::parser::symbol_type
make_T_DECIMAL (const std::string &s, const yy::parser::location_type& loc)
{
  errno = 0;
  double d = atof (s.c_str());
  return yy::parser::make_T_DECIMAL ( (double) d, loc);
}

void driver::scan_begin (const std::string &instream)
{
  yy_flex_debug = trace_scanning;
  yy_scan_string(instream.c_str());
}

void driver::scan_end () {}
