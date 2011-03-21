/*
 * Copyright (c) Members of the EGEE Collaboration. 2009-2010.
 * See http://www.eu-egee.org/partners/ for details on the copyright
 * holders.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
header
{
   #include "isn_leafnode.hpp"
   #include "isn_internalnode.hpp"
   #include <iostream>
   #include <string>
   #include <list>
   #include <math.h>
   #include <utility>
   #include <stdlib.h>
   using namespace std;
}

options
{
   language=Cpp;
}

class isn_parser extends Parser;

options
{
   k = 3;
   exportVocab = isn;
   buildAST = true;
}

tokens {
K_NULL  = "null";
K_IS  = "is";
K_IN  = "in";
K_LIKE = "like";
K_ESCAPE = "escape";
K_NOT  = "not" <AST=isn_internalnode>;
K_AND = "and" <AST=isn_internalnode>;
K_OR = "or" <AST=isn_internalnode>;
K_ALL ="all";
K_ANY ="any";
K_STRING_LIST;
SVC_CMP<AST=isn_leafnode>;
DATA_CMP<AST=isn_leafnode>;
}

like_clause returns [pair<string, char> s] :
   rhs:S_CHAR_LITERAL!
   {
      s.first = rhs->getText();
      s.second = '\0';
   }
   (K_ESCAPE esc:S_CHAR_LITERAL!
   {
      //Check there are at least three characters
      //to allow for the quotes
      if ( esc->getText().size() > 2 )
      {
         //Character [1] to allow for the quote
         s.second = esc->getText()[1];
      }

      else
      {
         s.second = '\0';
      }
   })?
   ;

//You don't need to quote numbers
in_clause returns [std::list<std::string> slist] :
   slist=bracketed_list
   ;

//This should match a bracketed list
//('string', num, num, 'string', etc.)
bracketed_list  returns [std::list<std::string> slist] :
   LPAREN

   (lit:S_CHAR_LITERAL! {slist.push_back(lit->getText());} |
    num:S_NUMBER! {slist.push_back(num->getText());} )

   (COMMA! (lit2:S_CHAR_LITERAL! {slist.push_back(lit2->getText());} |
            num2:S_NUMBER! {slist.push_back(num2->getText());}) )*
   RPAREN
   ;

string_literal_list!  : lit:S_CHAR_LITERAL!
//{
//s = lit->getText();
//myRef->addListElement(lit->getText());
//std::cout << lit->getText() << "  " << myRef->getListSize() <<endl;
//}
   ( COMMA! lit2:S_CHAR_LITERAL!
   {
      //s = s + "," + lit2->getText();
      //std::cout << myRef->getListSize() <<endl;
      //myRef->addListElement(lit2->getText());
      //std::cout << lit2->getText() <<endl;
   } )*
;

vo_filter : vo_filter_expression
;

vo_filter_expression : vo_and_expression (K_OR^ vo_and_expression )*
;

vo_and_expression : vo_expression ( K_AND^ vo_expression )*
;

vo_expression : (K_NOT^)? vo_comparison
//| K_NOT^ vo_comparison
| LPAREN! vo_filter_expression RPAREN!
| K_NOT^ LPAREN! vo_filter_expression RPAREN!
;

vo_comparison
{
   bool allVal = false;
   bool anyVal = false;
   int knot = 0;
   int op;
   string rhs;
   string comp;
   string s;
   Refisn_leafnode myRef;
   list<string> slist;
   std::pair<std::string, char> rhsLike;
} :
   //Check for ANY/ALL clause
   (K_ANY! {anyVal = true;} |
    K_ALL! {allVal = true;})?

   K_VO! (K_NOT! {knot=1;})? K_IN! slist=in_clause
   {
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refisn_leafnode)#vo_comparison;
      myRef->setLValue(K_VO);
      myRef->setOpType(K_IN);
      myRef->setRValType(K_STRING_LIST);

      while ( slist.size() > 0 )
      {
          s = slist.front();
          myRef->addListElement(s);
          slist.pop_front();
      }

      if ( knot )
      {
         myRef->setNotPrefix();
      }

      //Deal with ANY/ALL
      if ( anyVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ALL_VALUE);
      }
   } |

   K_VO! (K_NOT! {knot = 1;})? K_LIKE rhsLike=like_clause
   {
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refisn_leafnode)#vo_comparison;
      myRef->setLValue(K_VO);
      myRef->setOpType(K_LIKE);
      myRef->setRValue(rhsLike.first);

      //Set the escape character
      //for the 'LIKE' clause
      myRef->setEscapeChar(rhsLike.second);

      if ( knot )
      {
         myRef->setNotPrefix();
      }

      //Deal with ANY/ALL
      if ( anyVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ALL_VALUE);
      }
   } |

   K_VO! op=eqop rval:S_CHAR_LITERAL!
   {
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refisn_leafnode)#vo_comparison;
      myRef->setLValue(K_VO);
      myRef->setOpType(op);
      myRef->setRValue(rval->getText());

      //Deal with ANY/ALL
      if ( anyVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ALL_VALUE);
      }
   } |
   
   K_VO! K_IS (K_NOT! {knot = 1;})? K_NULL!
   {
      //Deal with (Non-)existance of VOs
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refisn_leafnode)#vo_comparison;
      myRef->setLValue(K_VO);
      myRef->setOpType(K_NULL);
      myRef->setRValue("*");

      //Reverse the NOT flag as it makes things easier
      //VO IS NULL <=> (! (VO = *) )
      //VO IS NOT NULL <=> (VO = *)
      if ( !knot )
      {
         myRef->setNotPrefix();
      }
   }
   ;

entity_filter : entity_filter_expression
;

entity_filter_expression : entity_and_expression ( K_OR^ entity_and_expression )*
;

entity_and_expression : entity_expression ( K_AND^ entity_expression )*
;

entity_expression : entity_comparison
| K_NOT^ entity_comparison
| LPAREN! entity_filter_expression RPAREN!
| K_NOT^ LPAREN! entity_filter_expression RPAREN!
;

eqop returns [int tok_type = 0] :
   o1:NE! { tok_type = o1->getType(); } |
   o2:EQ! { tok_type = o2->getType(); }
;

relop! returns [int tok_type = 0] :
   o1:EQ   { tok_type = o1->getType(); } |
   o2:LESS { tok_type = o2->getType(); } |
   o3:GT   { tok_type = o3->getType(); } |
   o4:NE   { tok_type = o4->getType(); } |
   o5:LE   { tok_type = o5->getType(); } |
   o6:GE   { tok_type = o6->getType(); }
;

entity_comparison
{
   bool allVal = false;
   bool anyVal = false;
   int op; int rhs1 = 0; int knot = 0;
   double dval;
   Refisn_leafnode myRef;
   std::list<std::string> slist;
   std::string s;
   std::pair<std::string, char> likePair;
   std::string likeString;
} :
   //Check for ANY/ALL clause
   (K_ANY! {anyVal = true;} |
    K_ALL! {allVal = true;})?

   lhs:S_IDENTIFIER! (K_NOT! {knot=1;})? K_IN! slist=in_clause
   {
      //Deal with the 'IN' oerator
      #entity_comparison = #([DATA_CMP]);
      myRef = (Refisn_leafnode)#entity_comparison;
      myRef->setLKey(lhs->getText());
      myRef->setOpType(K_IN);
      myRef->setRValType(K_STRING_LIST);

      //Add everything in the 'IN' list to this element
      while ( slist.size() > 0 )
      {
         s = slist.front();
         myRef->addListElement(s);
         slist.pop_front();
      }

      //Deal with the NOT condition
      if ( knot )
      {
         myRef->setNotPrefix();
      }

      //Deal with ANY/ALL
      if ( anyVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ALL_VALUE);
      }
   } |

   lhs0:S_IDENTIFIER (K_NOT! {knot = 1;})? K_LIKE likePair=like_clause
   {
      //Deal with the 'LIKE' oerator
      likeString = likePair.first;
      #entity_comparison = #([DATA_CMP]);
      myRef = (Refisn_leafnode)#entity_comparison;
      myRef->setLKey(lhs0->getText());
      myRef->setOpType(K_LIKE);
      myRef->setRValue(likeString);

      //Set the escape character
      //for the 'LIKE' clause
      myRef->setEscapeChar(likePair.second);

      //Deal with the NOT condition
      if ( knot )
      {
         myRef->setNotPrefix();
      }

      //Deal with ANY/ALL
      if ( anyVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ALL_VALUE);
      }
   } |

   lhs1:S_IDENTIFIER! op=relop (lit:S_CHAR_LITERAL! { rhs1 = 1; } | num:S_NUMBER!)
   {
      //Deal with the relational operators
      #entity_comparison = #([DATA_CMP]);
      myRef = (Refisn_leafnode)#entity_comparison;
      myRef->setLKey(lhs1->getText());
      myRef->setOpType(op);

      if ( rhs1 )
      {
         myRef->setRValType(S_CHAR_LITERAL);
         myRef->setRValue(lit->getText());
      }

      else
      {
         myRef->setRValType(S_NUMBER);
         dval = atof(num->getText().c_str());
         myRef->setRValNum(dval);
      }

      //Deal with ANY/ALL
      if ( anyVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ALL_VALUE);
      }
   } |

   lhs2:S_IDENTIFIER! op=eqop slist=bracketed_list
   {
      //Deal with the equality operators
      #entity_comparison = #([DATA_CMP]);
      myRef = (Refisn_leafnode)#entity_comparison;
      myRef->setLKey(lhs2->getText());
      myRef->setOpType(op);
      myRef->setRValType(K_STRING_LIST);

      //Add everything in the 'IN' list to this element
      while ( slist.size() > 0 )
      {
         s = slist.front();
         myRef->addListElement(s);
         slist.pop_front();
      }

      //Deal with ANY/ALL
      if ( anyVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(isn_leafnode::ALL_VALUE);
      }
   } |

   lhs3:S_IDENTIFIER! K_IS (K_NOT! {knot = 1;})? K_NULL!
   {
      //Deal with (Non-)existance of entity attributes
      #entity_comparison = #([DATA_CMP]);
      myRef = (Refisn_leafnode)#entity_comparison;
      myRef->setLKey(lhs3->getText());
      myRef->setOpType(K_NULL);
      myRef->setRValue("NULL");

      //Unlike the other filters we don't
      //reverse the NOT flag.
      if ( knot )
      {
         myRef->setNotPrefix();
      }

   }
;

class isn_lexer extends Lexer;

options
{
   k = 2;
   caseSensitive = false;
   exportVocab = isn;
   testLiterals = false;
   caseSensitiveLiterals = false;
}

WS_ options { paraphrase = "white space"; } : (' ' | '\t' | '\n' | '\r') { _ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP; } ;

LPAREN options { paraphrase = "left parenthesis"; } : '(' ;

RPAREN options { paraphrase = "right parenthesis"; } : ')' ;

COMMA options { paraphrase = "comma"; } : ',' ;

protected
DIGIT options { paraphrase = "digit"; } : '0' .. '9';

protected
INTEGER options { paraphrase = "integer"; } : (DIGIT)+ ;

protected
FLOAT options { paraphrase = "float"; } : INTEGER ('.' INTEGER)? | '.' INTEGER  ;

S_NUMBER options { paraphrase = "number"; } : FLOAT (('e') ('+' | '-')? FLOAT)? ;

S_CHAR_LITERAL options { paraphrase = "char literal"; } : '\'' ( ~'\'' )* '\'' ;

EQ options { paraphrase = "equals"; } : '=' ;

NE options { paraphrase = "not equals"; } :  '<' { _ttype = LESS; }
                                                 (   ( '>' { _ttype = NE; } )
                                                   | ( '=' { _ttype = LE; } ) )?
                                              | "!=" ;
GT options { paraphrase = "greater than"; } : '>' ( '=' { _ttype = GE; } )? ;

protected
LETTER options { paraphrase = "letter"; } : 'a' .. 'z';

S_IDENTIFIER options { testLiterals = true; paraphrase = "identifier"; } : LETTER (DIGIT | LETTER | '_')* ;
