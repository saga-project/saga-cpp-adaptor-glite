/* $ANTLR 2.7.7 (2006-11-01): "sd_grammar.g" -> "sd_parser.cpp"$ */
#include "sd_parser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
#line 1 "sd_grammar.g"
#line 8 "sd_parser.cpp"
sd_parser::sd_parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

sd_parser::sd_parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,3)
{
}

sd_parser::sd_parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

sd_parser::sd_parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,3)
{
}

sd_parser::sd_parser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,3)
{
}

int  sd_parser::service_keyword() {
#line 76 "sd_grammar.g"
	int tok_type = 0;
#line 37 "sd_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST service_keyword_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k3 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k3_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k4 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k4_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k5 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k5_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k6 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k6_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k7 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k7_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k8 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k8_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k9 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k9_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k10 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k10_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k11 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k11_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case K_TYPE:
		{
			k1 = LT(1);
			k1_AST = astFactory->create(k1);
			match(K_TYPE);
#line 77 "sd_grammar.g"
			tok_type = k1->getType();
#line 73 "sd_parser.cpp"
			break;
		}
		case K_NAME:
		{
			k2 = LT(1);
			k2_AST = astFactory->create(k2);
			match(K_NAME);
#line 78 "sd_grammar.g"
			tok_type = k2->getType();
#line 83 "sd_parser.cpp"
			break;
		}
		case K_SITE:
		{
			k3 = LT(1);
			k3_AST = astFactory->create(k3);
			match(K_SITE);
#line 79 "sd_grammar.g"
			tok_type = k3->getType();
#line 93 "sd_parser.cpp"
			break;
		}
		case K_ENDPOINT:
		{
			k4 = LT(1);
			k4_AST = astFactory->create(k4);
			match(K_ENDPOINT);
#line 80 "sd_grammar.g"
			tok_type = k4->getType();
#line 103 "sd_parser.cpp"
			break;
		}
		case K_SERVICE:
		{
			k5 = LT(1);
			k5_AST = astFactory->create(k5);
			match(K_SERVICE);
#line 81 "sd_grammar.g"
			tok_type = k5->getType();
#line 113 "sd_parser.cpp"
			break;
		}
		case K_UID:
		{
			k6 = LT(1);
			k6_AST = astFactory->create(k6);
			match(K_UID);
#line 82 "sd_grammar.g"
			tok_type = k6->getType();
#line 123 "sd_parser.cpp"
			break;
		}
		case K_URI:
		{
			k7 = LT(1);
			k7_AST = astFactory->create(k7);
			match(K_URI);
#line 83 "sd_grammar.g"
			tok_type = k7->getType();
#line 133 "sd_parser.cpp"
			break;
		}
		case K_INTERFACE_VERSION:
		{
			k8 = LT(1);
			k8_AST = astFactory->create(k8);
			match(K_INTERFACE_VERSION);
#line 84 "sd_grammar.g"
			tok_type = k8->getType();
#line 143 "sd_parser.cpp"
			break;
		}
		case K_IMPLEMENTATION_VERSION:
		{
			k9 = LT(1);
			k9_AST = astFactory->create(k9);
			match(K_IMPLEMENTATION_VERSION);
#line 85 "sd_grammar.g"
			tok_type = k9->getType();
#line 153 "sd_parser.cpp"
			break;
		}
		case K_IMPLEMENTOR:
		{
			k10 = LT(1);
			k10_AST = astFactory->create(k10);
			match(K_IMPLEMENTOR);
#line 86 "sd_grammar.g"
			tok_type = k10->getType();
#line 163 "sd_parser.cpp"
			break;
		}
		case K_CAPABILITY:
		{
			k11 = LT(1);
			k11_AST = astFactory->create(k11);
			match(K_CAPABILITY);
#line 87 "sd_grammar.g"
			tok_type = k11->getType();
#line 173 "sd_parser.cpp"
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
		recover(ex,_tokenSet_0);
	}
	returnAST = service_keyword_AST;
	return tok_type;
}

int  sd_parser::authz_keyword() {
#line 90 "sd_grammar.g"
	int tok_type = 0;
#line 193 "sd_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST authz_keyword_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k3 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k3_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k4 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k4_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case K_VO:
		{
			k1 = LT(1);
			k1_AST = astFactory->create(k1);
			match(K_VO);
#line 91 "sd_grammar.g"
			tok_type = k1->getType();
#line 215 "sd_parser.cpp"
			break;
		}
		case K_VOMS:
		{
			k2 = LT(1);
			k2_AST = astFactory->create(k2);
			match(K_VOMS);
#line 92 "sd_grammar.g"
			tok_type = k2->getType();
#line 225 "sd_parser.cpp"
			break;
		}
		case K_FQAN:
		{
			k3 = LT(1);
			k3_AST = astFactory->create(k3);
			match(K_FQAN);
#line 93 "sd_grammar.g"
			tok_type = k3->getType();
#line 235 "sd_parser.cpp"
			break;
		}
		case K_DN:
		{
			k4 = LT(1);
			k4_AST = astFactory->create(k4);
			match(K_DN);
#line 94 "sd_grammar.g"
			tok_type = k4->getType();
#line 245 "sd_parser.cpp"
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
		recover(ex,_tokenSet_1);
	}
	returnAST = authz_keyword_AST;
	return tok_type;
}

int  sd_parser::authz_keyword_with_all() {
#line 97 "sd_grammar.g"
	int tok_type = 0;
#line 265 "sd_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST authz_keyword_with_all_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k3 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k3_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k4 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k4_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k5 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k5_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case K_VO:
		{
			k1 = LT(1);
			k1_AST = astFactory->create(k1);
			match(K_VO);
#line 98 "sd_grammar.g"
			tok_type = k1->getType();
#line 289 "sd_parser.cpp"
			break;
		}
		case K_VOMS:
		{
			k2 = LT(1);
			k2_AST = astFactory->create(k2);
			match(K_VOMS);
#line 99 "sd_grammar.g"
			tok_type = k2->getType();
#line 299 "sd_parser.cpp"
			break;
		}
		case K_FQAN:
		{
			k3 = LT(1);
			k3_AST = astFactory->create(k3);
			match(K_FQAN);
#line 100 "sd_grammar.g"
			tok_type = k3->getType();
#line 309 "sd_parser.cpp"
			break;
		}
		case K_DN:
		{
			k4 = LT(1);
			k4_AST = astFactory->create(k4);
			match(K_DN);
#line 101 "sd_grammar.g"
			tok_type = k4->getType();
#line 319 "sd_parser.cpp"
			break;
		}
		case K_ALL:
		{
			k5 = LT(1);
			k5_AST = astFactory->create(k5);
			match(K_ALL);
#line 102 "sd_grammar.g"
			tok_type = k5->getType();
#line 329 "sd_parser.cpp"
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
		recover(ex,_tokenSet_2);
	}
	returnAST = authz_keyword_with_all_AST;
	return tok_type;
}

string  sd_parser::data_keyword() {
#line 105 "sd_grammar.g"
	string tok = "";
#line 349 "sd_parser.cpp"
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST data_keyword_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k1_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k2_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k3 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k3_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k4 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k4_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k5 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k5_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k6 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k6_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k7 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k7_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k8 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k8_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k9 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k9_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k10 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k10_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k11 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k11_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k12 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k12_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k13 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k13_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k14 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k14_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k15 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k15_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k16 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k16_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  k17 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST k17_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case K_TYPE:
		{
			k1 = LT(1);
			k1_AST = astFactory->create(k1);
			match(K_TYPE);
#line 106 "sd_grammar.g"
			tok = k1->getText();
#line 397 "sd_parser.cpp"
			break;
		}
		case K_NAME:
		{
			k2 = LT(1);
			k2_AST = astFactory->create(k2);
			match(K_NAME);
#line 107 "sd_grammar.g"
			tok = k2->getText();
#line 407 "sd_parser.cpp"
			break;
		}
		case K_SITE:
		{
			k3 = LT(1);
			k3_AST = astFactory->create(k3);
			match(K_SITE);
#line 108 "sd_grammar.g"
			tok = k3->getText();
#line 417 "sd_parser.cpp"
			break;
		}
		case K_ENDPOINT:
		{
			k4 = LT(1);
			k4_AST = astFactory->create(k4);
			match(K_ENDPOINT);
#line 109 "sd_grammar.g"
			tok = k4->getText();
#line 427 "sd_parser.cpp"
			break;
		}
		case K_SERVICE:
		{
			k5 = LT(1);
			k5_AST = astFactory->create(k5);
			match(K_SERVICE);
#line 110 "sd_grammar.g"
			tok = k5->getText();
#line 437 "sd_parser.cpp"
			break;
		}
		case K_UID:
		{
			k6 = LT(1);
			k6_AST = astFactory->create(k6);
			match(K_UID);
#line 111 "sd_grammar.g"
			tok = k6->getText();
#line 447 "sd_parser.cpp"
			break;
		}
		case K_URI:
		{
			k7 = LT(1);
			k7_AST = astFactory->create(k7);
			match(K_URI);
#line 112 "sd_grammar.g"
			tok = k7->getText();
#line 457 "sd_parser.cpp"
			break;
		}
		case K_INTERFACE_VERSION:
		{
			k8 = LT(1);
			k8_AST = astFactory->create(k8);
			match(K_INTERFACE_VERSION);
#line 113 "sd_grammar.g"
			tok = k8->getText();
#line 467 "sd_parser.cpp"
			break;
		}
		case K_IMPLEMENTATION_VERSION:
		{
			k9 = LT(1);
			k9_AST = astFactory->create(k9);
			match(K_IMPLEMENTATION_VERSION);
#line 114 "sd_grammar.g"
			tok = k9->getText();
#line 477 "sd_parser.cpp"
			break;
		}
		case K_IMPLEMENTOR:
		{
			k10 = LT(1);
			k10_AST = astFactory->create(k10);
			match(K_IMPLEMENTOR);
#line 115 "sd_grammar.g"
			tok = k10->getText();
#line 487 "sd_parser.cpp"
			break;
		}
		case K_CAPABILITY:
		{
			k11 = LT(1);
			k11_AST = astFactory->create(k11);
			match(K_CAPABILITY);
#line 116 "sd_grammar.g"
			tok = k11->getText();
#line 497 "sd_parser.cpp"
			break;
		}
		case K_VO:
		{
			k12 = LT(1);
			k12_AST = astFactory->create(k12);
			match(K_VO);
#line 117 "sd_grammar.g"
			tok = k12->getText();
#line 507 "sd_parser.cpp"
			break;
		}
		case K_VOMS:
		{
			k13 = LT(1);
			k13_AST = astFactory->create(k13);
			match(K_VOMS);
#line 118 "sd_grammar.g"
			tok = k13->getText();
#line 517 "sd_parser.cpp"
			break;
		}
		case K_FQAN:
		{
			k14 = LT(1);
			k14_AST = astFactory->create(k14);
			match(K_FQAN);
#line 119 "sd_grammar.g"
			tok = k14->getText();
#line 527 "sd_parser.cpp"
			break;
		}
		case K_DN:
		{
			k15 = LT(1);
			k15_AST = astFactory->create(k15);
			match(K_DN);
#line 120 "sd_grammar.g"
			tok = k15->getText();
#line 537 "sd_parser.cpp"
			break;
		}
		case K_ALL:
		{
			k16 = LT(1);
			k16_AST = astFactory->create(k16);
			match(K_ALL);
#line 121 "sd_grammar.g"
			tok = k16->getText();
#line 547 "sd_parser.cpp"
			break;
		}
		case S_IDENTIFIER:
		{
			k17 = LT(1);
			k17_AST = astFactory->create(k17);
			match(S_IDENTIFIER);
#line 122 "sd_grammar.g"
			tok = k17->getText();
#line 557 "sd_parser.cpp"
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
		recover(ex,_tokenSet_3);
	}
	returnAST = data_keyword_AST;
	return tok;
}

void sd_parser::service_filter() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST service_filter_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		service_filter_expression();
		astFactory->addASTChild( currentAST, returnAST );
		service_filter_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_4);
	}
	returnAST = service_filter_AST;
}

void sd_parser::service_filter_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST service_filter_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		service_and_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_OR)) {
				Refsd_internalnode tmp1_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp1_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp1_AST));
				match(K_OR);
				service_and_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop8;
			}
			
		}
		_loop8:;
		} // ( ... )*
		service_filter_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = service_filter_expression_AST;
}

void sd_parser::service_and_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST service_and_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		service_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_AND)) {
				Refsd_internalnode tmp2_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp2_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp2_AST));
				match(K_AND);
				service_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop11;
			}
			
		}
		_loop11:;
		} // ( ... )*
		service_and_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_6);
	}
	returnAST = service_and_expression_AST;
}

void sd_parser::service_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST service_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case K_TYPE:
		case K_NAME:
		case K_UID:
		case K_URI:
		case K_ENDPOINT:
		case K_SITE:
		case K_SERVICE:
		case K_INTERFACE_VERSION:
		case K_IMPLEMENTATION_VERSION:
		case K_IMPLEMENTOR:
		case K_CAPABILITY:
		{
			service_comparison();
			astFactory->addASTChild( currentAST, returnAST );
			service_expression_AST = currentAST.root;
			break;
		}
		case LPAREN:
		{
			match(LPAREN);
			service_filter_expression();
			astFactory->addASTChild( currentAST, returnAST );
			match(RPAREN);
			service_expression_AST = currentAST.root;
			break;
		}
		default:
			if ((LA(1) == K_NOT) && ((LA(2) >= K_TYPE && LA(2) <= K_CAPABILITY))) {
				Refsd_internalnode tmp5_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp5_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp5_AST));
				match(K_NOT);
				service_comparison();
				astFactory->addASTChild( currentAST, returnAST );
				service_expression_AST = currentAST.root;
			}
			else if ((LA(1) == K_NOT) && (LA(2) == LPAREN)) {
				Refsd_internalnode tmp6_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp6_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp6_AST));
				match(K_NOT);
				match(LPAREN);
				service_filter_expression();
				astFactory->addASTChild( currentAST, returnAST );
				match(RPAREN);
				service_expression_AST = currentAST.root;
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_7);
	}
	returnAST = service_expression_AST;
}

void sd_parser::service_comparison() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  rval = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST rval_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
#line 137 "sd_grammar.g"
	int knot = 0; int lhs; int op;
	std::pair<std::string, char> rhs;
	std::list<std::string> rhsEq;
	std::list<std::string> slist;
	std::string s;
	Refsd_leafnode myRef;
	
#line 739 "sd_parser.cpp"
	
	try {      // for error handling
		if (((LA(1) >= K_TYPE && LA(1) <= K_CAPABILITY)) && (LA(2) == K_IN || LA(2) == K_NOT) && (LA(3) == K_IN || LA(3) == LPAREN)) {
			lhs=service_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 145 "sd_grammar.g"
				knot=1;
#line 752 "sd_parser.cpp"
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
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 146 "sd_grammar.g"
			
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)service_comparison_AST;
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
			
#line 794 "sd_parser.cpp"
			currentAST.root = service_comparison_AST;
			if ( service_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				service_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = service_comparison_AST->getFirstChild();
			else
				currentAST.child = service_comparison_AST;
			currentAST.advanceChildToEnd();
			service_comparison_AST = currentAST.root;
		}
		else if (((LA(1) >= K_TYPE && LA(1) <= K_CAPABILITY)) && (LA(2) == K_LIKE || LA(2) == K_NOT) && (LA(3) == K_LIKE || LA(3) == S_CHAR_LITERAL)) {
			lhs=service_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 171 "sd_grammar.g"
				knot = 1;
#line 814 "sd_parser.cpp"
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
			match(K_LIKE);
			rhs=like_clause();
			astFactory->addASTChild( currentAST, returnAST );
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 172 "sd_grammar.g"
			
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)service_comparison_AST;
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
			
#line 848 "sd_parser.cpp"
			currentAST.root = service_comparison_AST;
			if ( service_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				service_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = service_comparison_AST->getFirstChild();
			else
				currentAST.child = service_comparison_AST;
			currentAST.advanceChildToEnd();
			service_comparison_AST = currentAST.root;
		}
		else if (((LA(1) >= K_TYPE && LA(1) <= K_CAPABILITY)) && (LA(2) == NE || LA(2) == EQ) && (LA(3) == S_CHAR_LITERAL)) {
			lhs=service_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			op=eqop();
			astFactory->addASTChild( currentAST, returnAST );
			rval = LT(1);
			rval_AST = astFactory->create(rval);
			match(S_CHAR_LITERAL);
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 190 "sd_grammar.g"
			
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)service_comparison_AST;
			myRef->setLValue(lhs);
			//std::cout << "keyword type " << lhs << endl;
			myRef->setOpType(op);
			//std::cout << "op type " << op << endl;
			myRef->setRValue(rval->getText());
			//std::cout << "literal " << rval->getText() << endl;
			
#line 878 "sd_parser.cpp"
			currentAST.root = service_comparison_AST;
			if ( service_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				service_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = service_comparison_AST->getFirstChild();
			else
				currentAST.child = service_comparison_AST;
			currentAST.advanceChildToEnd();
			service_comparison_AST = currentAST.root;
		}
		else if (((LA(1) >= K_TYPE && LA(1) <= K_CAPABILITY)) && (LA(2) == NE || LA(2) == EQ) && (LA(3) == LPAREN)) {
			lhs=service_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			op=eqop();
			astFactory->addASTChild( currentAST, returnAST );
			rhsEq=bracketed_list();
			astFactory->addASTChild( currentAST, returnAST );
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 202 "sd_grammar.g"
			
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)service_comparison_AST;
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
			
#line 914 "sd_parser.cpp"
			currentAST.root = service_comparison_AST;
			if ( service_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				service_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = service_comparison_AST->getFirstChild();
			else
				currentAST.child = service_comparison_AST;
			currentAST.advanceChildToEnd();
			service_comparison_AST = currentAST.root;
		}
		else if (((LA(1) >= K_TYPE && LA(1) <= K_CAPABILITY)) && (LA(2) == K_IS)) {
			lhs=service_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp13_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp13_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp13_AST);
			match(K_IS);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 220 "sd_grammar.g"
				knot = 1;
#line 938 "sd_parser.cpp"
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
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 221 "sd_grammar.g"
			
			//Deal with (Non-)existance of service attributes
			service_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)service_comparison_AST;
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
			
#line 971 "sd_parser.cpp"
			currentAST.root = service_comparison_AST;
			if ( service_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				service_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = service_comparison_AST->getFirstChild();
			else
				currentAST.child = service_comparison_AST;
			currentAST.advanceChildToEnd();
			service_comparison_AST = currentAST.root;
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_7);
	}
	returnAST = service_comparison_AST;
}

std::list<std::string>  sd_parser::in_clause() {
#line 264 "sd_grammar.g"
	std::list<std::string> slist;
#line 996 "sd_parser.cpp"
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
		recover(ex,_tokenSet_7);
	}
	returnAST = in_clause_AST;
	return slist;
}

pair<string, char>  sd_parser::like_clause() {
#line 240 "sd_grammar.g"
	pair<string, char> s;
#line 1017 "sd_parser.cpp"
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
#line 242 "sd_grammar.g"
		
		s.first = rhs->getText();
		s.second = '\0';
		
#line 1035 "sd_parser.cpp"
		{
		switch ( LA(1)) {
		case K_ESCAPE:
		{
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp16_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp16_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp16_AST);
			match(K_ESCAPE);
			esc = LT(1);
			esc_AST = astFactory->create(esc);
			match(S_CHAR_LITERAL);
#line 247 "sd_grammar.g"
			
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
			
#line 1062 "sd_parser.cpp"
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
		recover(ex,_tokenSet_7);
	}
	returnAST = like_clause_AST;
	return s;
}

int  sd_parser::eqop() {
#line 464 "sd_grammar.g"
	int tok_type = 0;
#line 1091 "sd_parser.cpp"
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
#line 465 "sd_grammar.g"
			tok_type = o1->getType();
#line 1109 "sd_parser.cpp"
			eqop_AST = currentAST.root;
			break;
		}
		case EQ:
		{
			o2 = LT(1);
			o2_AST = astFactory->create(o2);
			match(EQ);
#line 466 "sd_grammar.g"
			tok_type = o2->getType();
#line 1120 "sd_parser.cpp"
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
		recover(ex,_tokenSet_8);
	}
	returnAST = eqop_AST;
	return tok_type;
}

std::list<std::string>  sd_parser::bracketed_list() {
#line 270 "sd_grammar.g"
	std::list<std::string> slist;
#line 1141 "sd_parser.cpp"
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
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp17_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp17_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp17_AST);
		match(LPAREN);
		{
		switch ( LA(1)) {
		case S_CHAR_LITERAL:
		{
			lit = LT(1);
			lit_AST = astFactory->create(lit);
			match(S_CHAR_LITERAL);
#line 273 "sd_grammar.g"
			slist.push_back(lit->getText());
#line 1168 "sd_parser.cpp"
			break;
		}
		case S_NUMBER:
		{
			num = LT(1);
			num_AST = astFactory->create(num);
			match(S_NUMBER);
#line 274 "sd_grammar.g"
			slist.push_back(num->getText());
#line 1178 "sd_parser.cpp"
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
#line 276 "sd_grammar.g"
					slist.push_back(lit2->getText());
#line 1200 "sd_parser.cpp"
					break;
				}
				case S_NUMBER:
				{
					num2 = LT(1);
					num2_AST = astFactory->create(num2);
					match(S_NUMBER);
#line 277 "sd_grammar.g"
					slist.push_back(num2->getText());
#line 1210 "sd_parser.cpp"
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
				goto _loop24;
			}
			
		}
		_loop24:;
		} // ( ... )*
		ANTLR_USE_NAMESPACE(antlr)RefAST tmp19_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
		tmp19_AST = astFactory->create(LT(1));
		astFactory->addASTChild(currentAST, tmp19_AST);
		match(RPAREN);
		bracketed_list_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_7);
	}
	returnAST = bracketed_list_AST;
	return slist;
}

void sd_parser::string_literal_list() {
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
#line 288 "sd_grammar.g"
				
				//s = s + "," + lit2->getText();
				//std::cout << myRef->getListSize() <<endl;
				//myRef->addListElement(lit2->getText());
				//std::cout << lit2->getText() <<endl;
				
#line 1268 "sd_parser.cpp"
			}
			else {
				goto _loop27;
			}
			
		}
		_loop27:;
		} // ( ... )*
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_4);
	}
	returnAST = string_literal_list_AST;
}

void sd_parser::vo_filter() {
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
		recover(ex,_tokenSet_4);
	}
	returnAST = vo_filter_AST;
}

void sd_parser::vo_filter_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_filter_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		vo_and_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_OR)) {
				Refsd_internalnode tmp21_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp21_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp21_AST));
				match(K_OR);
				vo_and_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop31;
			}
			
		}
		_loop31:;
		} // ( ... )*
		vo_filter_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = vo_filter_expression_AST;
}

void sd_parser::vo_and_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_and_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		vo_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_AND)) {
				Refsd_internalnode tmp22_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp22_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp22_AST));
				match(K_AND);
				vo_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop34;
			}
			
		}
		_loop34:;
		} // ( ... )*
		vo_and_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_6);
	}
	returnAST = vo_and_expression_AST;
}

void sd_parser::vo_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		if ((_tokenSet_9.member(LA(1))) && (_tokenSet_10.member(LA(2)))) {
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				Refsd_internalnode tmp23_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp23_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp23_AST));
				match(K_NOT);
				break;
			}
			case K_VO:
			case K_VOMS:
			case K_FQAN:
			case K_DN:
			case K_ALL:
			case K_ANY:
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
			Refsd_internalnode tmp26_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
			tmp26_AST = astFactory->create(LT(1));
			astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp26_AST));
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
		recover(ex,_tokenSet_7);
	}
	returnAST = vo_expression_AST;
}

void sd_parser::vo_comparison() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  rval = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST rval_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
#line 311 "sd_grammar.g"
	
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
	
#line 1457 "sd_parser.cpp"
	
	try {      // for error handling
		if ((_tokenSet_11.member(LA(1))) && (_tokenSet_12.member(LA(2))) && (LA(3) == K_IN || LA(3) == K_NOT || LA(3) == LPAREN)) {
			{
			switch ( LA(1)) {
			case K_ANY:
			{
				match(K_ANY);
#line 327 "sd_grammar.g"
				anyVal = true;
#line 1468 "sd_parser.cpp"
				break;
			}
			case K_ALL:
			{
				match(K_ALL);
#line 328 "sd_grammar.g"
				allVal = true;
#line 1476 "sd_parser.cpp"
				break;
			}
			case K_VO:
			case K_VOMS:
			case K_FQAN:
			case K_DN:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			lhs=authz_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 330 "sd_grammar.g"
				knot=1;
#line 1501 "sd_parser.cpp"
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
#line 331 "sd_grammar.g"
			
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)vo_comparison_AST;
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
			
#line 1549 "sd_parser.cpp"
			currentAST.root = vo_comparison_AST;
			if ( vo_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				vo_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = vo_comparison_AST->getFirstChild();
			else
				currentAST.child = vo_comparison_AST;
			currentAST.advanceChildToEnd();
			vo_comparison_AST = currentAST.root;
		}
		else if (((LA(1) >= K_VO && LA(1) <= K_DN)) && (LA(2) == K_LIKE || LA(2) == K_NOT) && (LA(3) == K_LIKE || LA(3) == S_CHAR_LITERAL)) {
			lhs=authz_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 362 "sd_grammar.g"
				knot = 1;
#line 1569 "sd_parser.cpp"
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp34_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp34_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp34_AST);
			match(K_LIKE);
			rhsLike=like_clause();
			astFactory->addASTChild( currentAST, returnAST );
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 363 "sd_grammar.g"
			
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)vo_comparison_AST;
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
			
#line 1617 "sd_parser.cpp"
			currentAST.root = vo_comparison_AST;
			if ( vo_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				vo_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = vo_comparison_AST->getFirstChild();
			else
				currentAST.child = vo_comparison_AST;
			currentAST.advanceChildToEnd();
			vo_comparison_AST = currentAST.root;
		}
		else if (((LA(1) >= K_VO && LA(1) <= K_DN)) && (LA(2) == NE || LA(2) == EQ) && (LA(3) == S_CHAR_LITERAL)) {
			lhs=authz_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			op=eqop();
			astFactory->addASTChild( currentAST, returnAST );
			rval = LT(1);
			rval_AST = astFactory->create(rval);
			match(S_CHAR_LITERAL);
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 392 "sd_grammar.g"
			
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)vo_comparison_AST;
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
			
#line 1655 "sd_parser.cpp"
			currentAST.root = vo_comparison_AST;
			if ( vo_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				vo_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = vo_comparison_AST->getFirstChild();
			else
				currentAST.child = vo_comparison_AST;
			currentAST.advanceChildToEnd();
			vo_comparison_AST = currentAST.root;
		}
		else if (((LA(1) >= K_VO && LA(1) <= K_DN)) && (LA(2) == NE || LA(2) == EQ) && (LA(3) == LPAREN)) {
			lhs=authz_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			op=eqop();
			astFactory->addASTChild( currentAST, returnAST );
			rhsEq=bracketed_list();
			astFactory->addASTChild( currentAST, returnAST );
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 412 "sd_grammar.g"
			
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)vo_comparison_AST;
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
			
#line 1691 "sd_parser.cpp"
			currentAST.root = vo_comparison_AST;
			if ( vo_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				vo_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = vo_comparison_AST->getFirstChild();
			else
				currentAST.child = vo_comparison_AST;
			currentAST.advanceChildToEnd();
			vo_comparison_AST = currentAST.root;
		}
		else if ((_tokenSet_13.member(LA(1))) && (LA(2) == K_IS)) {
			lhs=authz_keyword_with_all();
			astFactory->addASTChild( currentAST, returnAST );
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp35_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp35_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp35_AST);
			match(K_IS);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 430 "sd_grammar.g"
				knot = 1;
#line 1715 "sd_parser.cpp"
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
#line 431 "sd_grammar.g"
			
			//Deal with (Non-)existance of VOs
			vo_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(SVC_CMP))));
			myRef = (Refsd_leafnode)vo_comparison_AST;
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
			
#line 1747 "sd_parser.cpp"
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
		recover(ex,_tokenSet_7);
	}
	returnAST = vo_comparison_AST;
}

void sd_parser::data_filter() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST data_filter_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		data_filter_expression();
		astFactory->addASTChild( currentAST, returnAST );
		data_filter_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_4);
	}
	returnAST = data_filter_AST;
}

void sd_parser::data_filter_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST data_filter_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		data_and_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_OR)) {
				Refsd_internalnode tmp38_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp38_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp38_AST));
				match(K_OR);
				data_and_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop45;
			}
			
		}
		_loop45:;
		} // ( ... )*
		data_filter_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_5);
	}
	returnAST = data_filter_expression_AST;
}

void sd_parser::data_and_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST data_and_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		data_expression();
		astFactory->addASTChild( currentAST, returnAST );
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == K_AND)) {
				Refsd_internalnode tmp39_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp39_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp39_AST));
				match(K_AND);
				data_expression();
				astFactory->addASTChild( currentAST, returnAST );
			}
			else {
				goto _loop48;
			}
			
		}
		_loop48:;
		} // ( ... )*
		data_and_expression_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_6);
	}
	returnAST = data_and_expression_AST;
}

void sd_parser::data_expression() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST data_expression_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	
	try {      // for error handling
		switch ( LA(1)) {
		case K_VO:
		case K_VOMS:
		case K_FQAN:
		case K_DN:
		case K_TYPE:
		case K_NAME:
		case K_UID:
		case K_URI:
		case K_ENDPOINT:
		case K_SITE:
		case K_SERVICE:
		case K_INTERFACE_VERSION:
		case K_IMPLEMENTATION_VERSION:
		case K_IMPLEMENTOR:
		case K_CAPABILITY:
		case K_ALL:
		case K_ANY:
		case S_IDENTIFIER:
		{
			data_comparison();
			astFactory->addASTChild( currentAST, returnAST );
			data_expression_AST = currentAST.root;
			break;
		}
		case LPAREN:
		{
			match(LPAREN);
			data_filter_expression();
			astFactory->addASTChild( currentAST, returnAST );
			match(RPAREN);
			data_expression_AST = currentAST.root;
			break;
		}
		default:
			if ((LA(1) == K_NOT) && (_tokenSet_14.member(LA(2)))) {
				Refsd_internalnode tmp42_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp42_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp42_AST));
				match(K_NOT);
				data_comparison();
				astFactory->addASTChild( currentAST, returnAST );
				data_expression_AST = currentAST.root;
			}
			else if ((LA(1) == K_NOT) && (LA(2) == LPAREN)) {
				Refsd_internalnode tmp43_AST = Refsd_internalnode(ANTLR_USE_NAMESPACE(antlr)nullAST);
				tmp43_AST = astFactory->create(LT(1));
				astFactory->makeASTRoot(currentAST, ANTLR_USE_NAMESPACE(antlr)RefAST(tmp43_AST));
				match(K_NOT);
				match(LPAREN);
				data_filter_expression();
				astFactory->addASTChild( currentAST, returnAST );
				match(RPAREN);
				data_expression_AST = currentAST.root;
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_7);
	}
	returnAST = data_expression_AST;
}

void sd_parser::data_comparison() {
	returnAST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)ASTPair currentAST;
	ANTLR_USE_NAMESPACE(antlr)RefAST data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  lit = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST lit_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
	ANTLR_USE_NAMESPACE(antlr)RefToken  num = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefAST num_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
#line 478 "sd_grammar.g"
	
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
	
#line 1948 "sd_parser.cpp"
	
	try {      // for error handling
		{
		if ((LA(1) == K_ANY)) {
			match(K_ANY);
#line 492 "sd_grammar.g"
			anyVal = true;
#line 1956 "sd_parser.cpp"
		}
		else if ((LA(1) == K_ALL) && (_tokenSet_15.member(LA(2)))) {
			match(K_ALL);
#line 493 "sd_grammar.g"
			allVal = true;
#line 1962 "sd_parser.cpp"
		}
		else if ((_tokenSet_15.member(LA(1))) && (_tokenSet_3.member(LA(2)))) {
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		{
		if ((_tokenSet_15.member(LA(1))) && (LA(2) == K_IN || LA(2) == K_NOT) && (LA(3) == K_IN || LA(3) == LPAREN)) {
			tok=data_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 495 "sd_grammar.g"
				knot=1;
#line 1982 "sd_parser.cpp"
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
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 496 "sd_grammar.g"
			
			//Deal with the 'IN' oerator
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refsd_leafnode)data_comparison_AST;
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
			
#line 2033 "sd_parser.cpp"
			currentAST.root = data_comparison_AST;
			if ( data_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				data_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = data_comparison_AST->getFirstChild();
			else
				currentAST.child = data_comparison_AST;
			currentAST.advanceChildToEnd();
		}
		else if ((_tokenSet_15.member(LA(1))) && (LA(2) == K_LIKE || LA(2) == K_NOT) && (LA(3) == K_LIKE || LA(3) == S_CHAR_LITERAL)) {
			tok=data_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 530 "sd_grammar.g"
				knot = 1;
#line 2052 "sd_parser.cpp"
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
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp51_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp51_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp51_AST);
			match(K_LIKE);
			likePair=like_clause();
			astFactory->addASTChild( currentAST, returnAST );
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 531 "sd_grammar.g"
			
			//Deal with the 'LIKE' oerator
			likeString = likePair.first;
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refsd_leafnode)data_comparison_AST;
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
			
#line 2103 "sd_parser.cpp"
			currentAST.root = data_comparison_AST;
			if ( data_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				data_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = data_comparison_AST->getFirstChild();
			else
				currentAST.child = data_comparison_AST;
			currentAST.advanceChildToEnd();
		}
		else if ((_tokenSet_15.member(LA(1))) && ((LA(2) >= NE && LA(2) <= GE)) && (LA(3) == S_CHAR_LITERAL || LA(3) == S_NUMBER)) {
			tok=data_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			op=relop();
			astFactory->addASTChild( currentAST, returnAST );
			{
			switch ( LA(1)) {
			case S_CHAR_LITERAL:
			{
				lit = LT(1);
				lit_AST = astFactory->create(lit);
				match(S_CHAR_LITERAL);
#line 562 "sd_grammar.g"
				rhs1 = 1;
#line 2126 "sd_parser.cpp"
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
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 563 "sd_grammar.g"
			
			//Deal with the relational operators
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refsd_leafnode)data_comparison_AST;
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
			
#line 2175 "sd_parser.cpp"
			currentAST.root = data_comparison_AST;
			if ( data_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				data_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = data_comparison_AST->getFirstChild();
			else
				currentAST.child = data_comparison_AST;
			currentAST.advanceChildToEnd();
		}
		else if ((_tokenSet_15.member(LA(1))) && (LA(2) == NE || LA(2) == EQ) && (LA(3) == LPAREN)) {
			tok=data_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			op=eqop();
			astFactory->addASTChild( currentAST, returnAST );
			slist=bracketed_list();
			astFactory->addASTChild( currentAST, returnAST );
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 596 "sd_grammar.g"
			
			//Deal with the equality operators
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refsd_leafnode)data_comparison_AST;
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
			
#line 2220 "sd_parser.cpp"
			currentAST.root = data_comparison_AST;
			if ( data_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				data_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = data_comparison_AST->getFirstChild();
			else
				currentAST.child = data_comparison_AST;
			currentAST.advanceChildToEnd();
		}
		else if ((_tokenSet_15.member(LA(1))) && (LA(2) == K_IS)) {
			tok=data_keyword();
			astFactory->addASTChild( currentAST, returnAST );
			ANTLR_USE_NAMESPACE(antlr)RefAST tmp52_AST = ANTLR_USE_NAMESPACE(antlr)nullAST;
			tmp52_AST = astFactory->create(LT(1));
			astFactory->addASTChild(currentAST, tmp52_AST);
			match(K_IS);
			{
			switch ( LA(1)) {
			case K_NOT:
			{
				match(K_NOT);
#line 624 "sd_grammar.g"
				knot = 1;
#line 2243 "sd_parser.cpp"
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
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(currentAST.root);
#line 625 "sd_grammar.g"
			
			//Deal with (Non-)existance of data attributes
			data_comparison_AST = ANTLR_USE_NAMESPACE(antlr)RefAST(astFactory->make((new ANTLR_USE_NAMESPACE(antlr)ASTArray(1))->add(astFactory->create(DATA_CMP))));
			myRef = (Refsd_leafnode)data_comparison_AST;
			myRef->setLKey(tok);
			myRef->setOpType(K_NULL);
			myRef->setRValue("NULL");
			
			//Unlike the other filters we don't
			//reverse the NOT flag.
			if ( knot )
			{
			myRef->setNotPrefix();
			}
			
			
#line 2275 "sd_parser.cpp"
			currentAST.root = data_comparison_AST;
			if ( data_comparison_AST!=ANTLR_USE_NAMESPACE(antlr)nullAST &&
				data_comparison_AST->getFirstChild() != ANTLR_USE_NAMESPACE(antlr)nullAST )
				  currentAST.child = data_comparison_AST->getFirstChild();
			else
				currentAST.child = data_comparison_AST;
			currentAST.advanceChildToEnd();
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		data_comparison_AST = currentAST.root;
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		recover(ex,_tokenSet_7);
	}
	returnAST = data_comparison_AST;
}

int  sd_parser::relop() {
#line 469 "sd_grammar.g"
	int tok_type = 0;
#line 2301 "sd_parser.cpp"
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
#line 470 "sd_grammar.g"
			tok_type = o1->getType();
#line 2327 "sd_parser.cpp"
			break;
		}
		case LESS:
		{
			o2 = LT(1);
			o2_AST = astFactory->create(o2);
			match(LESS);
#line 471 "sd_grammar.g"
			tok_type = o2->getType();
#line 2337 "sd_parser.cpp"
			break;
		}
		case GT:
		{
			o3 = LT(1);
			o3_AST = astFactory->create(o3);
			match(GT);
#line 472 "sd_grammar.g"
			tok_type = o3->getType();
#line 2347 "sd_parser.cpp"
			break;
		}
		case NE:
		{
			o4 = LT(1);
			o4_AST = astFactory->create(o4);
			match(NE);
#line 473 "sd_grammar.g"
			tok_type = o4->getType();
#line 2357 "sd_parser.cpp"
			break;
		}
		case LE:
		{
			o5 = LT(1);
			o5_AST = astFactory->create(o5);
			match(LE);
#line 474 "sd_grammar.g"
			tok_type = o5->getType();
#line 2367 "sd_parser.cpp"
			break;
		}
		case GE:
		{
			o6 = LT(1);
			o6_AST = astFactory->create(o6);
			match(GE);
#line 475 "sd_grammar.g"
			tok_type = o6->getType();
#line 2377 "sd_parser.cpp"
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
		recover(ex,_tokenSet_16);
	}
	returnAST = relop_AST;
	return tok_type;
}

void sd_parser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory )
{
	factory.registerFactory(9, "sd_internalnode", sd_internalnode::factory);
	factory.registerFactory(10, "sd_internalnode", sd_internalnode::factory);
	factory.registerFactory(11, "sd_internalnode", sd_internalnode::factory);
	factory.registerFactory(30, "sd_leafnode", sd_leafnode::factory);
	factory.registerFactory(31, "sd_leafnode", sd_leafnode::factory);
	factory.setMaxNodeType(48);
}
const char* sd_parser::tokenNames[] = {
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
	"\"vo\"",
	"\"voms\"",
	"\"fqan\"",
	"\"dn\"",
	"\"type\"",
	"\"name\"",
	"\"uid\"",
	"\"url\"",
	"\"endpoint\"",
	"\"site\"",
	"\"RelatedServices\"",
	"\"InterfaceVersion\"",
	"\"ImplementationVersion\"",
	"\"Implementor\"",
	"\"capabilities\"",
	"\"all\"",
	"\"any\"",
	"K_STRING_LIST",
	"SVC_CMP",
	"DATA_CMP",
	"identifier",
	"left parenthesis",
	"right parenthesis",
	"string literal",
	"number",
	"comma",
	"not equals",
	"equals",
	"LESS",
	"greater than",
	"LE",
	"GE",
	"white space",
	"digit",
	"integer",
	"float",
	"letter",
	0
};

const unsigned long sd_parser::_tokenSet_0_data_[] = { 736UL, 192UL, 0UL, 0UL };
// "is" "in" "like" "not" NE EQ 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long sd_parser::_tokenSet_1_data_[] = { 704UL, 192UL, 0UL, 0UL };
// "in" "like" "not" NE EQ 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long sd_parser::_tokenSet_2_data_[] = { 32UL, 0UL, 0UL, 0UL };
// "is" 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_2(_tokenSet_2_data_,4);
const unsigned long sd_parser::_tokenSet_3_data_[] = { 736UL, 4032UL, 0UL, 0UL };
// "is" "in" "like" "not" NE EQ LESS GT LE GE 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long sd_parser::_tokenSet_4_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long sd_parser::_tokenSet_5_data_[] = { 2UL, 4UL, 0UL, 0UL };
// EOF RPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_5(_tokenSet_5_data_,4);
const unsigned long sd_parser::_tokenSet_6_data_[] = { 2050UL, 4UL, 0UL, 0UL };
// EOF "or" RPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_6(_tokenSet_6_data_,4);
const unsigned long sd_parser::_tokenSet_7_data_[] = { 3074UL, 4UL, 0UL, 0UL };
// EOF "and" "or" RPAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_7(_tokenSet_7_data_,4);
const unsigned long sd_parser::_tokenSet_8_data_[] = { 0UL, 10UL, 0UL, 0UL };
// LPAREN S_CHAR_LITERAL 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_8(_tokenSet_8_data_,4);
const unsigned long sd_parser::_tokenSet_9_data_[] = { 402715136UL, 0UL, 0UL, 0UL };
// "not" "vo" "voms" "fqan" "dn" "all" "any" 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_9(_tokenSet_9_data_,4);
const unsigned long sd_parser::_tokenSet_10_data_[] = { 402715360UL, 192UL, 0UL, 0UL };
// "is" "in" "like" "not" "vo" "voms" "fqan" "dn" "all" "any" NE EQ 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_10(_tokenSet_10_data_,4);
const unsigned long sd_parser::_tokenSet_11_data_[] = { 402714624UL, 0UL, 0UL, 0UL };
// "vo" "voms" "fqan" "dn" "all" "any" 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_11(_tokenSet_11_data_,4);
const unsigned long sd_parser::_tokenSet_12_data_[] = { 62016UL, 0UL, 0UL, 0UL };
// "in" "not" "vo" "voms" "fqan" "dn" 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_12(_tokenSet_12_data_,4);
const unsigned long sd_parser::_tokenSet_13_data_[] = { 134279168UL, 0UL, 0UL, 0UL };
// "vo" "voms" "fqan" "dn" "all" 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_13(_tokenSet_13_data_,4);
const unsigned long sd_parser::_tokenSet_14_data_[] = { 536866816UL, 1UL, 0UL, 0UL };
// "vo" "voms" "fqan" "dn" "type" "name" "uid" "url" "endpoint" "site" 
// "RelatedServices" "InterfaceVersion" "ImplementationVersion" "Implementor" 
// "capabilities" "all" "any" S_IDENTIFIER 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_14(_tokenSet_14_data_,4);
const unsigned long sd_parser::_tokenSet_15_data_[] = { 268431360UL, 1UL, 0UL, 0UL };
// "vo" "voms" "fqan" "dn" "type" "name" "uid" "url" "endpoint" "site" 
// "RelatedServices" "InterfaceVersion" "ImplementationVersion" "Implementor" 
// "capabilities" "all" S_IDENTIFIER 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_15(_tokenSet_15_data_,4);
const unsigned long sd_parser::_tokenSet_16_data_[] = { 0UL, 24UL, 0UL, 0UL };
// S_CHAR_LITERAL S_NUMBER 
const ANTLR_USE_NAMESPACE(antlr)BitSet sd_parser::_tokenSet_16(_tokenSet_16_data_,4);


