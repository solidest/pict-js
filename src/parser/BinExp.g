/**
 * Example:
 *    
 *    syntax-cli -m slr1 -g ./api/parser/BinExp.g --tokenize -p "3 and _var0-9*(3.9<=4) >= -汉字 or 变量>_var1/3==9"
 *    syntax-cli -m slr1 -g ./api/parser/BinExp.g --validate
 *    syntax-cli -m slr1 -g ./api/parser/BinExp.g -o ./api/parser/BinParser.js
 *    *eslint-disable*
 */
{
  "lex": {
    "rules": [
      ["\\s+", "/*skip whitespace*/"],
      ["and", "return 'AND'"],
      ["or", "return 'OR'"],
      ["!=", "return 'NOT_EQUAL'"],      
      ["!", "return 'NOT'"],
      ["==", "return 'BE_EQUAL'"],
      ["~=", "return 'NOT_EQUAL'"],
      ["~", "return 'NOT'"],
      ["^[\u4e00-\u9fa5a-zA-Z\\$_][\u4e00-\u9fa5a-zA-Z\\d_]*", "return 'VAR'"],
      ["[0-9]+(?:\\.[0-9]+)?\\b", "return 'NUMBER'"],
      ["\\+", "return '+'"],
      ["\\*", "return '*'"],
      ["-", "return '-'"],
      ["\\/", "return '/'"],
      ["\\(", "return '('"],
      ["\\)", "return ')'"],
      [">=", "return '>='"], 
      [">", "return '>'"], 
      ["<=", "return '<='"], 
      ["<", "return '<'"],
    ]
  },

  "operators": [
    ["left", "AND", "OR"],
    ["left", ">", "<", ">=", "<=", "BE_EQUAL", "NOT_EQUAL"],
    ["left", "+", "-"],
    ["left", "*", "/"],
    ["left", "UMINUS", "NOT"],
  ],

  "bnf": {
    "e": [
      ["e AND e", "$$ = new ExpAnd($1, $3)"],
      ["e OR e", "$$ = new ExpOr($1, $3)"],
      ["NOT e", "$$ = new ExpNot($2)"],
      ["e NOT_EQUAL e", "$$ = new ExpNotEqual($1, $3)"],
      ["e BE_EQUAL e", "$$ = new ExpBeEqual($1, $3)"],
      ["e >= e", "$$ = new ExpGreaterEqual($1, $3)"],
      ["e <= e", "$$ = new ExpLessEqual($1, $3)"],
      ["e > e", "$$ = new ExpGreater($1, $3)"],
      ["e < e", "$$ = new ExpLess($1, $3)"],
      ["e + e", "$$ = new ExpPlus($1, $3)"],
      ["e - e", "$$ = new ExpMinus($1, $3)"],
      ["e * e", "$$ = new ExpTimes($1, $3)"],
      ["e / e", "$$ = new ExpDivide($1, $3)"],
      ["- e", "$$ = new ExpMinus(new ExpNumber(0), $2)", {
        "prec": "UMINUS"
      }],
      ["( e )", "$$ = $2"],
      ["NUMBER", "$$ = new ExpNumber(yytext)"],
      ["VAR", "$$ = new ExpVar(yytext)"],
    ],
  },

  "moduleInclude": `
        const { ExpNotEqual, ExpBeEqual, ExpGreater, ExpGreaterEqual, ExpNot, ExpAnd, ExpOr, ExpLess, ExpLessEqual,
          ExpNumber, ExpPlus, ExpMinus, ExpTimes, ExpDivide, ExpVar} = require('./Exp');
    `,

}