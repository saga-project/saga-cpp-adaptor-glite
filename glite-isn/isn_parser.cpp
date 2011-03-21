/* $ANTLR 2.7.7 (2006-11-01): "isn_grammar.g" -> "isn_parser.cpp"$ */
#include "isn_parser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
#line 1 "isn_grammar.g"
#line 8 "isn_parser.cpp"
isn_parser::isn_parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

isn_parser::isn_parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,3)
{
}

isn_parser::isn_parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

isn_parser::isn_parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,3)
{
}

isn_parser::isn_parser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,3)
{
}

pair<string, char>  isn_parser::like_clause() {
#line 61 "isn_grammar.g"
	pair<string, char> s;
#line 37 "isn_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST like_clause_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  rhs = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST rhs_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  esc = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST esc_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		rhs = LT(1);
		rhs_AST = astFactory->create(rhs);
		match(S_CHAR_LITERAL);
#line 63 "isn_grammar.g"
		
		s.first = rhs->getText();
		s.second = '\0';
		
#line 55 "isn_parser.cpp"
		{
		switch ( LA(1)) {
		case K_ESCAPE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp1_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp1_AST);
			match(K_ESCAPE);
			esc = LT(1);
			esc_AST = astFactory->create(esc);
			match(S_CHAR_LITERAL);
#line 68 "isn_grammar.g"
			
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
			
#line 82 "isn_parser.cpp"
			break;
		}
		case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
		case K_AND:
		case K_OR:
		case RPAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		like_clause_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = like_clause_AST;
	return s;
}

std::list<std::string>  isn_parser::in_clause() {
#line 85 "isn_grammar.g"
	std::list<std::string> slist;
#line 111 "isn_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST in_clause_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		slist=bracketed_list();
		astFactory->addASTChild( currentAST, returnAST );
		in_clause_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = in_clause_AST;
	return slist;
}

std::list<std::string>  isn_parser::bracketed_list() {
#line 91 "isn_grammar.g"
	std::list<std::string> slist;
#line 132 "isn_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST bracketed_list_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lit = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lit_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  num = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST num_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lit2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lit2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  num2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST num2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp2_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp2_AST);
		match(LPAREN);
		{
		switch ( LA(1)) {
		case S_CHAR_LITERAL:
		{
			lit = LT(1);
			lit_AST = astFactory->create(lit);
			match(S_CHAR_LITERAL);
#line 94 "isn_grammar.g"
			slist.push_back(lit->getText());
#line 159 "isn_parser.cpp"
			break;
		}
		case S_NUMBER:
		{
			num = LT(1);
			num_AST = astFactory->create(num);
			match(S_NUMBER);
#line 95 "isn_grammar.g"
			slist.push_back(num->getText());
#line 169 "isn_parser.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				{
				switch ( LA(1)) {
				case S_CHAR_LITERAL:
				{
					lit2 = LT(1);
					lit2_AST = astFactory->create(lit2);
					match(S_CHAR_LITERAL);
#line 97 "isn_grammar.g"
					slist.push_back(lit2->getText());
#line 191 "isn_parser.cpp"
					break;
				}
				case S_NUMBER:
				{
					num2 = LT(1);
					num2_AST = astFactory->create(num2);
					match(S_NUMBER);
#line 98 "isn_grammar.g"
					slist.push_back(num2->getText());
#line 201 "isn_parser.cpp"
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
				}
				}
				}
			}
			else {
				goto _loop8;
			}
			
		}
		_loop8:;
		} // ( ... )*
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp4_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp4_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp4_AST);
		match(RPAREN);
		bracketed_list_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = bracketed_list_AST;
	return slist;
}

void isn_parser::string_literal_list() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST string_literal_list_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lit = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lit_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lit2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lit2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		lit = LT(1);
		lit_AST = astFactory->create(lit);
		match(S_CHAR_LITERAL);
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				lit2 = LT(1);
				lit2_AST = astFactory->create(lit2);
				match(S_CHAR_LITERAL);
#line 109 "isn_grammar.g"
				
				//s = s + "," + lit2->getText();
				//std::cout << myRef->getListSize() <<endl;
				//myRef->addListElement(lit2->getText());
				//std::cout << lit2->getText() <<endl;
				
#line 259 "isn_parser.cpp"
			}
			else {
				goto _loop11;
			}
			
		}
		_loop11:;
		} // ( ... )*
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_1);
	}
	returnAST = string_literal_list_AST;
}

void isn_parser::vo_filter() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_filter_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		vo_filter_expression();
		astFactory->addASTChild( currentAST, returnAST );
		vo_filter_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_1);
	}
	returnAST = vo_filter_AST;
}

void isn_parser::vo_filter_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_filter_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		vo_and_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_OR)) {
				Refisn_internalnode tmp6_AST = Refisn_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp6_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp6_AST));
				match(K_OR);
				vo_and_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop15;
			}
			
		}
		_loop15:;
		} // ( ... )*
		vo_filter_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = vo_filter_expression_AST;
}

void isn_parser::vo_and_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_and_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		vo_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_AND)) {
				Refisn_internalnode tmp7_AST = Refisn_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp7_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp7_AST));
				match(K_AND);
				vo_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop18;
			}
			
		}
		_loop18:;
		} // ( ... )*
		vo_and_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_3);
	}
	returnAST = vo_and_expression_AST;
}

void isn_parser::vo_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		if ((_tokenSet_4.member(LA(1))) && (_tokenSet_5.member(LA(2)))) {
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				Refisn_internalnode tmp8_AST = Refisn_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp8_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp8_AST));
				match(K_NOT);
				break;
			}
			case K_ALL:
			case K_ANY:
			case K_VO:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			vo_comparison();
			astFactory->addASTChild( currentAST, returnAST );
			vo_expression_AST = currentAST.root;
		}
		else if ((LA(1) == LPAREN)) {
			match(LPAREN);
			vo_filter_expression();
			astFactory->addASTChild( currentAST, returnAST );
			match(RPAREN);
			vo_expression_AST = currentAST.root;
		}
		else if ((LA(1) == K_NOT) && (LA(2) == LPAREN)) {
			Refisn_internalnode tmp11_AST = Refisn_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp11_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp11_AST));
			match(K_NOT);
			match(LPAREN);
			vo_filter_expression();
			astFactory->addASTChild( currentAST, returnAST );
			match(RPAREN);
			vo_expression_AST = currentAST.root;
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = vo_expression_AST;
}

void isn_parser::vo_comparison() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  rval = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST rval_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
#line 132 "isn_grammar.g"
	
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
	
#line 443 "isn_parser.cpp"
	
	try {      // for error handling
		if ((LA(1) == K_ALL || LA(1) == K_ANY || LA(1) == K_VO) && (LA(2) == K_IN || LA(2) == K_NOT || LA(2) == K_VO) && (LA(3) == K_IN || LA(3) == K_NOT || LA(3) == LPAREN)) {
			{
			switch ( LA(1)) {
			case K_ANY:
			{
				match(K_ANY);
#line 146 "isn_grammar.g"
				anyVal = true;
#line 454 "isn_parser.cpp"
				break;
			}
			case K_ALL:
			{
				match(K_ALL);
#line 147 "isn_grammar.g"
				allVal = true;
#line 462 "isn_parser.cpp"
				break;
			}
			case K_VO:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(K_VO);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 149 "isn_grammar.g"
				knot=1;
#line 483 "isn_parser.cpp"
				break;
			}
			case K_IN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(K_IN);
			slist=in_clause();
			astFactory->addASTChild( currentAST, returnAST );
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 150 "isn_grammar.g"
			
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refisn_leafnode)vo_comparison_AST;
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
			
#line 531 "isn_parser.cpp"
			currentAST.root = vo_comparison_AST;
			if ( vo_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				vo_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = vo_comparison_AST->getFirstChild();
			else
				currentAST.child = vo_comparison_AST;
			currentAST.advanceChildToEnd();
			vo_comparison_AST = currentAST.root;
		}
		else if ((LA(1) == K_VO) && (LA(2) == K_LIKE || LA(2) == K_NOT) && (LA(3) == K_LIKE || LA(3) == S_CHAR_LITERAL)) {
			match(K_VO);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 181 "isn_grammar.g"
				knot = 1;
#line 550 "isn_parser.cpp"
				break;
			}
			case K_LIKE:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp21_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp21_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp21_AST);
			match(K_LIKE);
			rhsLike=like_clause();
			astFactory->addASTChild( currentAST, returnAST );
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 182 "isn_grammar.g"
			
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refisn_leafnode)vo_comparison_AST;
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
			
#line 598 "isn_parser.cpp"
			currentAST.root = vo_comparison_AST;
			if ( vo_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				vo_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = vo_comparison_AST->getFirstChild();
			else
				currentAST.child = vo_comparison_AST;
			currentAST.advanceChildToEnd();
			vo_comparison_AST = currentAST.root;
		}
		else if ((LA(1) == K_VO) && (LA(2) == NE || LA(2) == EQ)) {
			match(K_VO);
			op=eqop();
			astFactory->addASTChild( currentAST, returnAST );
			rval = LT(1);
			rval_AST = astFactory->create(rval);
			match(S_CHAR_LITERAL);
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 211 "isn_grammar.g"
			
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refisn_leafnode)vo_comparison_AST;
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
			
#line 635 "isn_parser.cpp"
			currentAST.root = vo_comparison_AST;
			if ( vo_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				vo_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = vo_comparison_AST->getFirstChild();
			else
				currentAST.child = vo_comparison_AST;
			currentAST.advanceChildToEnd();
			vo_comparison_AST = currentAST.root;
		}
		else if ((LA(1) == K_VO) && (LA(2) == K_IS)) {
			match(K_VO);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp24_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp24_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp24_AST);
			match(K_IS);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 230 "isn_grammar.g"
				knot = 1;
#line 658 "isn_parser.cpp"
				break;
			}
			case K_NULL:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(K_NULL);
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 231 "isn_grammar.g"
			
			//Deal with (Non-)existance of VOs
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refisn_leafnode)vo_comparison_AST;
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
			
#line 690 "isn_parser.cpp"
			currentAST.root = vo_comparison_AST;
			if ( vo_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				vo_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = vo_comparison_AST->getFirstChild();
			else
				currentAST.child = vo_comparison_AST;
			currentAST.advanceChildToEnd();
			vo_comparison_AST = currentAST.root;
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = vo_comparison_AST;
}

int  isn_parser::eqop() {
#line 264 "isn_grammar.g"
	int tok_type = 0;
#line 715 "isn_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST eqop_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  o1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST o1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  o2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST o2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case NE:
		{
			o1 = LT(1);
			o1_AST = astFactory->create(o1);
			match(NE);
#line 265 "isn_grammar.g"
			tok_type = o1->getType();
#line 733 "isn_parser.cpp"
			eqop_AST = currentAST.root;
			break;
		}
		case EQ:
		{
			o2 = LT(1);
			o2_AST = astFactory->create(o2);
			match(EQ);
#line 266 "isn_grammar.g"
			tok_type = o2->getType();
#line 744 "isn_parser.cpp"
			eqop_AST = currentAST.root;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_6);
	}
	returnAST = eqop_AST;
	return tok_type;
}

void isn_parser::entity_filter() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST entity_filter_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		entity_filter_expression();
		astFactory->addASTChild( currentAST, returnAST );
		entity_filter_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_1);
	}
	returnAST = entity_filter_AST;
}

void isn_parser::entity_filter_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST entity_filter_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		entity_and_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_OR)) {
				Refisn_internalnode tmp27_AST = Refisn_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp27_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp27_AST));
				match(K_OR);
				entity_and_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop29;
			}
			
		}
		_loop29:;
		} // ( ... )*
		entity_filter_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_2);
	}
	returnAST = entity_filter_expression_AST;
}

void isn_parser::entity_and_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST entity_and_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		entity_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_AND)) {
				Refisn_internalnode tmp28_AST = Refisn_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp28_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp28_AST));
				match(K_AND);
				entity_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop32;
			}
			
		}
		_loop32:;
		} // ( ... )*
		entity_and_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_3);
	}
	returnAST = entity_and_expression_AST;
}

void isn_parser::entity_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST entity_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case K_ALL:
		case K_ANY:
		case S_IDENTIFIER:
		{
			entity_comparison();
			astFactory->addASTChild( currentAST, returnAST );
			entity_expression_AST = currentAST.root;
			break;
		}
		case LPAREN:
		{
			match(LPAREN);
			entity_filter_expression();
			astFactory->addASTChild( currentAST, returnAST );
			match(RPAREN);
			entity_expression_AST = currentAST.root;
			break;
		}
		default:
			if ((LA(1) == K_NOT) && (LA(2) == K_ALL || LA(2) == K_ANY || LA(2) == S_IDENTIFIER)) {
				Refisn_internalnode tmp31_AST = Refisn_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp31_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp31_AST));
				match(K_NOT);
				entity_comparison();
				astFactory->addASTChild( currentAST, returnAST );
				entity_expression_AST = currentAST.root;
			}
			else if ((LA(1) == K_NOT) && (LA(2) == LPAREN)) {
				Refisn_internalnode tmp32_AST = Refisn_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp32_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp32_AST));
				match(K_NOT);
				match(LPAREN);
				entity_filter_expression();
				astFactory->addASTChild( currentAST, returnAST );
				match(RPAREN);
				entity_expression_AST = currentAST.root;
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = entity_expression_AST;
}

void isn_parser::entity_comparison() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lhs = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lhs_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lhs0 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lhs0_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lhs1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lhs1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lit = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lit_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  num = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST num_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lhs2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lhs2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lhs3 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lhs3_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
#line 278 "isn_grammar.g"
	
	bool allVal = false;
	bool anyVal = false;
	int op; int rhs1 = 0; int knot = 0;
	double dval;
	Refisn_leafnode myRef;
	std::list<std::string> slist;
	std::string s;
	std::pair<std::string, char> likePair;
	std::string likeString;
	
#line 935 "isn_parser.cpp"
	
	try {      // for error handling
		if ((LA(1) == K_ALL || LA(1) == K_ANY || LA(1) == S_IDENTIFIER) && (LA(2) == K_IN || LA(2) == K_NOT || LA(2) == S_IDENTIFIER) && (LA(3) == K_IN || LA(3) == K_NOT || LA(3) == LPAREN)) {
			{
			switch ( LA(1)) {
			case K_ANY:
			{
				match(K_ANY);
#line 291 "isn_grammar.g"
				anyVal = true;
#line 946 "isn_parser.cpp"
				break;
			}
			case K_ALL:
			{
				match(K_ALL);
#line 292 "isn_grammar.g"
				allVal = true;
#line 954 "isn_parser.cpp"
				break;
			}
			case S_IDENTIFIER:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			lhs = LT(1);
			lhs_AST = astFactory->create(lhs);
			match(S_IDENTIFIER);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 294 "isn_grammar.g"
				knot=1;
#line 977 "isn_parser.cpp"
				break;
			}
			case K_IN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(K_IN);
			slist=in_clause();
			astFactory->addASTChild( currentAST, returnAST );
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 295 "isn_grammar.g"
			
			//Deal with the 'IN' oerator
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refisn_leafnode)entity_comparison_AST;
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
			
#line 1028 "isn_parser.cpp"
			currentAST.root = entity_comparison_AST;
			if ( entity_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				entity_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = entity_comparison_AST->getFirstChild();
			else
				currentAST.child = entity_comparison_AST;
			currentAST.advanceChildToEnd();
			entity_comparison_AST = currentAST.root;
		}
		else if ((LA(1) == S_IDENTIFIER) && (LA(2) == K_LIKE || LA(2) == K_NOT) && (LA(3) == K_LIKE || LA(3) == S_CHAR_LITERAL)) {
			lhs0 = LT(1);
			lhs0_AST = astFactory->create(lhs0);
			astFactory->addASTChild(currentAST, lhs0_AST);
			match(S_IDENTIFIER);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 329 "isn_grammar.g"
				knot = 1;
#line 1050 "isn_parser.cpp"
				break;
			}
			case K_LIKE:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp40_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp40_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp40_AST);
			match(K_LIKE);
			likePair=like_clause();
			astFactory->addASTChild( currentAST, returnAST );
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 330 "isn_grammar.g"
			
			//Deal with the 'LIKE' oerator
			likeString = likePair.first;
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refisn_leafnode)entity_comparison_AST;
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
			
#line 1101 "isn_parser.cpp"
			currentAST.root = entity_comparison_AST;
			if ( entity_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				entity_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = entity_comparison_AST->getFirstChild();
			else
				currentAST.child = entity_comparison_AST;
			currentAST.advanceChildToEnd();
			entity_comparison_AST = currentAST.root;
		}
		else if ((LA(1) == S_IDENTIFIER) && ((LA(2) >= NE && LA(2) <= GE)) && (LA(3) == S_CHAR_LITERAL || LA(3) == S_NUMBER)) {
			lhs1 = LT(1);
			lhs1_AST = astFactory->create(lhs1);
			match(S_IDENTIFIER);
			op=relop();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case S_CHAR_LITERAL:
			{
				lit = LT(1);
				lit_AST = astFactory->create(lit);
				match(S_CHAR_LITERAL);
#line 361 "isn_grammar.g"
				rhs1 = 1;
#line 1126 "isn_parser.cpp"
				break;
			}
			case S_NUMBER:
			{
				num = LT(1);
				num_AST = astFactory->create(num);
				match(S_NUMBER);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 362 "isn_grammar.g"
			
			//Deal with the relational operators
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refisn_leafnode)entity_comparison_AST;
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
			
#line 1175 "isn_parser.cpp"
			currentAST.root = entity_comparison_AST;
			if ( entity_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				entity_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = entity_comparison_AST->getFirstChild();
			else
				currentAST.child = entity_comparison_AST;
			currentAST.advanceChildToEnd();
			entity_comparison_AST = currentAST.root;
		}
		else if ((LA(1) == S_IDENTIFIER) && (LA(2) == NE || LA(2) == EQ) && (LA(3) == LPAREN)) {
			lhs2 = LT(1);
			lhs2_AST = astFactory->create(lhs2);
			match(S_IDENTIFIER);
			op=eqop();
			astFactory->addASTChild( currentAST, returnAST );
			slist=bracketed_list();
			astFactory->addASTChild( currentAST, returnAST );
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 395 "isn_grammar.g"
			
			//Deal with the equality operators
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refisn_leafnode)entity_comparison_AST;
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
			
#line 1222 "isn_parser.cpp"
			currentAST.root = entity_comparison_AST;
			if ( entity_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				entity_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = entity_comparison_AST->getFirstChild();
			else
				currentAST.child = entity_comparison_AST;
			currentAST.advanceChildToEnd();
			entity_comparison_AST = currentAST.root;
		}
		else if ((LA(1) == S_IDENTIFIER) && (LA(2) == K_IS)) {
			lhs3 = LT(1);
			lhs3_AST = astFactory->create(lhs3);
			match(S_IDENTIFIER);
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp41_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp41_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp41_AST);
			match(K_IS);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 423 "isn_grammar.g"
				knot = 1;
#line 1247 "isn_parser.cpp"
				break;
			}
			case K_NULL:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			match(K_NULL);
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 424 "isn_grammar.g"
			
			//Deal with (Non-)existance of entity attributes
			entity_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refisn_leafnode)entity_comparison_AST;
			myRef->setLKey(lhs3->getText());
			myRef->setOpType(K_NULL);
			myRef->setRValue("NULL");
			
			//Unlike the other filters we don't
			//reverse the NOT flag.
			if ( knot )
			{
			myRef->setNotPrefix();
			}
			
			
#line 1279 "isn_parser.cpp"
			currentAST.root = entity_comparison_AST;
			if ( entity_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				entity_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = entity_comparison_AST->getFirstChild();
			else
				currentAST.child = entity_comparison_AST;
			currentAST.advanceChildToEnd();
			entity_comparison_AST = currentAST.root;
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_0);
	}
	returnAST = entity_comparison_AST;
}

int  isn_parser::relop() {
#line 269 "isn_grammar.g"
	int tok_type = 0;
#line 1304 "isn_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST relop_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  o1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST o1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  o2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST o2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  o3 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST o3_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  o4 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST o4_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  o5 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST o5_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  o6 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST o6_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case EQ:
		{
			o1 = LT(1);
			o1_AST = astFactory->create(o1);
			match(EQ);
#line 270 "isn_grammar.g"
			tok_type = o1->getType();
#line 1330 "isn_parser.cpp"
			break;
		}
		case LESS:
		{
			o2 = LT(1);
			o2_AST = astFactory->create(o2);
			match(LESS);
#line 271 "isn_grammar.g"
			tok_type = o2->getType();
#line 1340 "isn_parser.cpp"
			break;
		}
		case GT:
		{
			o3 = LT(1);
			o3_AST = astFactory->create(o3);
			match(GT);
#line 272 "isn_grammar.g"
			tok_type = o3->getType();
#line 1350 "isn_parser.cpp"
			break;
		}
		case NE:
		{
			o4 = LT(1);
			o4_AST = astFactory->create(o4);
			match(NE);
#line 273 "isn_grammar.g"
			tok_type = o4->getType();
#line 1360 "isn_parser.cpp"
			break;
		}
		case LE:
		{
			o5 = LT(1);
			o5_AST = astFactory->create(o5);
			match(LE);
#line 274 "isn_grammar.g"
			tok_type = o5->getType();
#line 1370 "isn_parser.cpp"
			break;
		}
		case GE:
		{
			o6 = LT(1);
			o6_AST = astFactory->create(o6);
			match(GE);
#line 275 "isn_grammar.g"
			tok_type = o6->getType();
#line 1380 "isn_parser.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_7);
	}
	returnAST = relop_AST;
	return tok_type;
}

void isn_parser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
	factory.registerFactory(9, "isn_internalnode", isn_internalnode::factory);
	factory.registerFactory(10, "isn_internalnode", isn_internalnode::factory);
	factory.registerFactory(11, "isn_internalnode", isn_internalnode::factory);
	factory.registerFactory(15, "isn_leafnode", isn_leafnode::factory);
	factory.registerFactory(16, "isn_leafnode", isn_leafnode::factory);
	factory.setMaxNodeType(34);
}
const char* isn_parser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"\"null\"",
	"\"is\"",
	"\"in\"",
	"\"like\"",
	"\"escape\"",
	"\"not\"",
	"\"and\"",
	"\"or\"",
	"\"all\"",
	"\"any\"",
	"K_STRING_LIST",
	"SVC_CMP",
	"DATA_CMP",
	"char literal",
	"left parenthesis",
	"number",
	"comma",
	"right parenthesis",
	"K_VO",
	"not equals",
	"equals",
	"LESS",
	"greater than",
	"LE",
	"GE",
	"identifier",
	"white space",
	"digit",
	"integer",
	"float",
	"letter",
	0
};

const unsigned long isn_parser::_tokenSet_0_data_[] = { 2100226UL, 0UL, 0UL, 0UL };
// EOF "and" "or" RPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet isn_parser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long isn_parser::_tokenSet_1_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet isn_parser::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long isn_parser::_tokenSet_2_data_[] = { 2097154UL, 0UL, 0UL, 0UL };
// EOF RPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet isn_parser::_tokenSet_2(_tokenSet_2_data_,4);
const unsigned long isn_parser::_tokenSet_3_data_[] = { 2099202UL, 0UL, 0UL, 0UL };
// EOF "or" RPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet isn_parser::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long isn_parser::_tokenSet_4_data_[] = { 4207104UL, 0UL, 0UL, 0UL };
// "not" "all" "any" K_VO 
const ANTLR_USE_NAMESPACE(antlr)BitSet isn_parser::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long isn_parser::_tokenSet_5_data_[] = { 29373152UL, 0UL, 0UL, 0UL };
// "is" "in" "like" "not" "all" "any" K_VO NE EQ 
const ANTLR_USE_NAMESPACE(antlr)BitSet isn_parser::_tokenSet_5(_tokenSet_5_data_,4);
const unsigned long isn_parser::_tokenSet_6_data_[] = { 393216UL, 0UL, 0UL, 0UL };
// S_CHAR_LITERAL LPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet isn_parser::_tokenSet_6(_tokenSet_6_data_,4);
const unsigned long isn_parser::_tokenSet_7_data_[] = { 655360UL, 0UL, 0UL, 0UL };
// S_CHAR_LITERAL S_NUMBER 
const ANTLR_USE_NAMESPACE(antlr)BitSet isn_parser::_tokenSet_7(_tokenSet_7_data_,4);


