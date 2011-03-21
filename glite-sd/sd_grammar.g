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
   #include "sd_leafnode.hpp"
   #include "sd_internalnode.hpp"
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

class sd_parser extends Parser;

options
{
   k = 3;
   exportVocab = sd;
   buildAST = true;
}

tokens {
K_NULL  = "null";
K_IS  = "is";
K_IN  = "in";
K_LIKE = "like";
K_ESCAPE = "escape";
K_NOT  = "not" <AST=sd_internalnode>;
K_AND = "and" <AST=sd_internalnode>;
K_OR = "or" <AST=sd_internalnode>;
K_VO = "vo";
K_VOMS = "voms";
K_FQAN = "fqan";
K_DN = "dn";
K_TYPE = "type";
K_NAME = "name";
K_UID = "uid";
K_URI = "url";
K_ENDPOINT = "endpoint";
K_SITE = "site";
K_SERVICE = "RelatedServices";
K_INTERFACE_VERSION = "InterfaceVersion";
K_IMPLEMENTATION_VERSION = "ImplementationVersion";
K_IMPLEMENTOR = "Implementor";
K_CAPABILITY = "capabilities";
K_ALL ="all";
K_ANY ="any";
K_STRING_LIST;
SVC_CMP<AST=sd_leafnode>;
DATA_CMP<AST=sd_leafnode>;
}

service_keyword! returns [int tok_type = 0] :
   k1:K_TYPE! { tok_type = k1->getType(); } |
   k2:K_NAME! { tok_type = k2->getType(); } |
   k3:K_SITE! { tok_type = k3->getType(); } |
   k4:K_ENDPOINT! { tok_type = k4->getType(); } |
   k5:K_SERVICE! { tok_type = k5->getType(); } |
   k6:K_UID! { tok_type = k6->getType(); } |
   k7:K_URI! { tok_type = k7->getType(); } |
   k8:K_INTERFACE_VERSION! { tok_type = k8->getType(); }|
   k9:K_IMPLEMENTATION_VERSION! { tok_type = k9->getType(); }|
   k10:K_IMPLEMENTOR! { tok_type = k10->getType(); }|
   k11:K_CAPABILITY! { tok_type = k11->getType(); }
   ;

authz_keyword! returns [int tok_type = 0] :
   k1:K_VO! { tok_type = k1->getType(); }|
   k2:K_VOMS! { tok_type = k2->getType(); }|
   k3:K_FQAN! { tok_type = k3->getType(); }|
   k4:K_DN! { tok_type = k4->getType(); }
   ;

authz_keyword_with_all! returns [int tok_type = 0] :
   k1:K_VO! { tok_type = k1->getType(); }|
   k2:K_VOMS! { tok_type = k2->getType(); }|
   k3:K_FQAN! { tok_type = k3->getType(); }|
   k4:K_DN! { tok_type = k4->getType(); }|
   k5:K_ALL! { tok_type = k5->getType(); }
   ;

data_keyword! returns [string tok = ""] :
   k1:K_TYPE! { tok = k1->getText(); } |
   k2:K_NAME! { tok = k2->getText(); } |
   k3:K_SITE! { tok = k3->getText(); } |
   k4:K_ENDPOINT! { tok = k4->getText(); } |
   k5:K_SERVICE! { tok = k5->getText(); } |
   k6:K_UID! { tok = k6->getText(); } |
   k7:K_URI! { tok = k7->getText(); } |
   k8:K_INTERFACE_VERSION! { tok = k8->getText(); }|
   k9:K_IMPLEMENTATION_VERSION! { tok = k9->getText(); }|
   k10:K_IMPLEMENTOR! { tok = k10->getText(); }|
   k11:K_CAPABILITY! { tok = k11->getText(); }|
   k12:K_VO! { tok = k12->getText(); }|
   k13:K_VOMS! { tok = k13->getText(); }|
   k14:K_FQAN! { tok = k14->getText(); }|
   k15:K_DN! { tok = k15->getText(); }|
   k16:K_ALL! { tok = k16->getText(); }|
   k17:S_IDENTIFIER! { tok = k17->getText(); }
   ;

service_filter : service_filter_expression ;

service_filter_expression : service_and_expression ( K_OR^ service_and_expression )*  ;

service_and_expression : service_expression ( K_AND^ service_expression)* ;

service_expression : service_comparison
| K_NOT^ service_comparison
| LPAREN! service_filter_expression RPAREN!
| K_NOT^ LPAREN! service_filter_expression RPAREN!
;

service_comparison
{ int knot = 0; int lhs; int op;
  std::pair<std::string, char> rhs;
  std::list<std::string> rhsEq;
  std::list<std::string> slist;
  std::string s;
  Refsd_leafnode myRef;
} :
   lhs=service_keyword (K_NOT! {knot=1;})? K_IN! slist=in_clause
   {
      #service_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#service_comparison;
      myRef->setLValue(lhs);
      myRef->setOpType(K_IN);
      myRef->setRValType(K_STRING_LIST);

      while (slist.size() > 0)
      {
         s = slist.front();
         myRef->addListElement(s);
         slist.pop_front();
      }

      if ( knot )
      {
         myRef->setNotPrefix();
      }
      //std::cout << "front list element : "
      //          << myRef->frontListElement() << endl;
      //std::cout << "list size : "
      //          << myRef->getListSize() << endl;
      //std::cout << "in clause parsed" << endl;
   } |

   lhs=service_keyword (K_NOT! {knot = 1;})? K_LIKE! rhs=like_clause
   {
      #service_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#service_comparison;
      myRef->setLValue(lhs);
      myRef->setOpType(K_LIKE);
      myRef->setRValue(rhs.first);

      //Set the escape character
      //for the 'LIKE' clause
      myRef->setEscapeChar(rhs.second);

      if ( knot )
      {
         myRef->setNotPrefix();
      }
   } |

   lhs=service_keyword op=eqop rval:S_CHAR_LITERAL!
   {
      #service_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#service_comparison;
      myRef->setLValue(lhs);
      //std::cout << "keyword type " << lhs << endl;
      myRef->setOpType(op);
      //std::cout << "op type " << op << endl;
      myRef->setRValue(rval->getText());
      //std::cout << "literal " << rval->getText() << endl;
   } |

   lhs=service_keyword op=eqop rhsEq=bracketed_list
   {
      #service_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#service_comparison;
      myRef->setLValue(lhs);
      //std::cout << "keyword type " << lhs << endl;
      myRef->setOpType(op);
      //std::cout << "op type " << op << endl;
      myRef->setRValType(K_STRING_LIST);

      //Add everything in the 'eq/ne' list to this element
      while ( rhsEq.size() > 0 )
      {
         s = rhsEq.front();
         myRef->addListElement(s);
         rhsEq.pop_front();
      }
   } |

   lhs=service_keyword K_IS (K_NOT! {knot = 1;})? K_NULL!
   {
      //Deal with (Non-)existance of service attributes
      #service_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#service_comparison;
      myRef->setLValue(lhs);
      myRef->setOpType(K_NULL);
      myRef->setRValue("*");

      //Reverse the NOT flag as it makes things easier
      //ATTR IS NULL <=> (! (ATTR = *) )
      //ATTR IS NOT NULL <=> (ATTR = *)

      if ( knot )
      {
         myRef->setNotPrefix();
      }
   }
   ;

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
   int lhs;
   string rhs;
   string comp;
   string s;
   Refsd_leafnode myRef;
   list<string> slist;
   list<string> rhsEq;
   std::pair<std::string, char> rhsLike;
} :
   //Check for ANY/ALL clause
   (K_ANY! {anyVal = true;} |
    K_ALL! {allVal = true;})?

   lhs=authz_keyword (K_NOT! {knot=1;})? K_IN! slist=in_clause
   {
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#vo_comparison;
      myRef->setLValue(lhs);
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
         myRef->setAnyAllValue(sd_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(sd_leafnode::ALL_VALUE);
      }
   } |

   lhs=authz_keyword (K_NOT! {knot = 1;})? K_LIKE rhsLike=like_clause
   {
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#vo_comparison;
      myRef->setLValue(lhs);
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
         myRef->setAnyAllValue(sd_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(sd_leafnode::ALL_VALUE);
      }
   } |

   lhs=authz_keyword op=eqop rval:S_CHAR_LITERAL!
   {
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#vo_comparison;
      myRef->setLValue(lhs);
      myRef->setOpType(op);
      myRef->setRValue(rval->getText());

      //Deal with ANY/ALL
      if ( anyVal )
      {
         myRef->setAnyAllValue(sd_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(sd_leafnode::ALL_VALUE);
      }
   } |
   
   lhs=authz_keyword op=eqop rhsEq=bracketed_list
   {
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#vo_comparison;
      myRef->setLValue(lhs);
      //std::cout << "keyword type " << lhs << endl;
      myRef->setOpType(op);
      //std::cout << "op type " << op << endl;
      myRef->setRValType(K_STRING_LIST);

      //Add everything in the 'eq/ne' list to this element
      while ( rhsEq.size() > 0 )
      {
         s = rhsEq.front();
         myRef->addListElement(s);
         rhsEq.pop_front();
      }
   } |

   lhs=authz_keyword_with_all K_IS (K_NOT! {knot = 1;})? K_NULL!
   {
      //Deal with (Non-)existance of VOs
      #vo_comparison = #([SVC_CMP]);
      myRef = (Refsd_leafnode)#vo_comparison;
      myRef->setLValue(lhs);
      myRef->setOpType(K_NULL);
      myRef->setRValue("*");

      //Reverse the NOT flag as it makes things easier
      //VO IS NULL <=> (! (VO = *) )
      //VO IS NOT NULL <=> (VO = *)
      if ( knot )
      {
         myRef->setNotPrefix();
      }
   }
   ;

data_filter : data_filter_expression
;

data_filter_expression : data_and_expression ( K_OR^ data_and_expression )*
;

data_and_expression : data_expression ( K_AND^ data_expression )*
;

data_expression : data_comparison
| K_NOT^ data_comparison
| LPAREN! data_filter_expression RPAREN!
| K_NOT^ LPAREN! data_filter_expression RPAREN!
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

data_comparison
{
   bool allVal = false;
   bool anyVal = false;
   int op; int rhs1 = 0; int knot = 0;
   double dval;
   Refsd_leafnode myRef;
   std::list<std::string> slist;
   std::string tok;
   std::string s;
   std::pair<std::string, char> likePair;
   std::string likeString;
} :
   //Check for ANY/ALL clause
   (K_ANY! {anyVal = true;} |
    K_ALL! {allVal = true;})?

   (tok=data_keyword (K_NOT! {knot=1;})? K_IN! slist=in_clause
   {
      //Deal with the 'IN' oerator
      #data_comparison = #([DATA_CMP]);
      myRef = (Refsd_leafnode)#data_comparison;
      myRef->setLKey(tok);
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
         myRef->setAnyAllValue(sd_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(sd_leafnode::ALL_VALUE);
      }
   } |

   tok=data_keyword (K_NOT! {knot = 1;})? K_LIKE likePair=like_clause
   {
      //Deal with the 'LIKE' oerator
      likeString = likePair.first;
      #data_comparison = #([DATA_CMP]);
      myRef = (Refsd_leafnode)#data_comparison;
      myRef->setLKey(tok);
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
         myRef->setAnyAllValue(sd_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(sd_leafnode::ALL_VALUE);
      }
   } |

   tok=data_keyword op=relop (lit:S_CHAR_LITERAL! { rhs1 = 1; } | num:S_NUMBER!)
   {
      //Deal with the relational operators
      #data_comparison = #([DATA_CMP]);
      myRef = (Refsd_leafnode)#data_comparison;
      myRef->setLKey(tok);
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
         myRef->setAnyAllValue(sd_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(sd_leafnode::ALL_VALUE);
      }
   } |

   tok=data_keyword op=eqop slist=bracketed_list
   {
      //Deal with the equality operators
      #data_comparison = #([DATA_CMP]);
      myRef = (Refsd_leafnode)#data_comparison;
      myRef->setLKey(tok);
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
         myRef->setAnyAllValue(sd_leafnode::ANY_VALUE);
      }

      if ( allVal )
      {
         myRef->setAnyAllValue(sd_leafnode::ALL_VALUE);
      }
   } |

   tok=data_keyword K_IS (K_NOT! {knot = 1;})? K_NULL!
   {
      //Deal with (Non-)existance of data attributes
      #data_comparison = #([DATA_CMP]);
      myRef = (Refsd_leafnode)#data_comparison;
      myRef->setLKey(tok);
      myRef->setOpType(K_NULL);
      myRef->setRValue("NULL");

      //Unlike the other filters we don't
      //reverse the NOT flag.
      if ( knot )
      {
         myRef->setNotPrefix();
      }

   })
;

class sd_lexer extends Lexer;

options
{
   k = 2;
   caseSensitive = false;
   exportVocab = sd;
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

S_CHAR_LITERAL options { paraphrase = "string literal"; } : '\'' ( ~'\'' )* '\'' ;

EQ options { paraphrase = "equals"; } : '=' ;

NE options { paraphrase = "not equals"; } :  '<' { _ttype = LESS; }
                                                 (   ( '>' { _ttype = NE; } )
                                                   | ( '=' { _ttype = LE; } ) )?
                                              | "!=" ;
GT options { paraphrase = "greater than"; } : '>' ( '=' { _ttype = GE; } )? ;

protected
LETTER options { paraphrase = "letter"; } : 'a' .. 'z';

S_IDENTIFIER options { testLiterals = true; paraphrase = "identifier"; } : LETTER (DIGIT | LETTER | '_' | '-')* ;
