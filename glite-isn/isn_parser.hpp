#ifndef INC_isn_parser_hpp_
#define INC_isn_parser_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.7 (2006-11-01): "isn_grammar.g" -> "isn_parser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "isnTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

#line 19 "isn_grammar.g"

   #include "isn_leafnode.hpp"
   #include "isn_internalnode.hpp"
   #include <iostream>
   #include <string>
   #include <list>
   #include <math.h>
   #include <utility>
   #include <stdlib.h>
   using namespace std;

#line 24 "isn_parser.hpp"
class CUSTOM_API isn_parser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public isnTokenTypes
{
#line 1 "isn_grammar.g"
#line 28 "isn_parser.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	isn_parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	isn_parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	isn_parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	isn_parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	isn_parser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return isn_parser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return isn_parser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return isn_parser::tokenNames;
	}
	public: pair<string, char>  like_clause();
	public: std::list<std::string>  in_clause();
	public: std::list<std::string>  bracketed_list();
	public: void string_literal_list();
	public: void vo_filter();
	public: void vo_filter_expression();
	public: void vo_and_expression();
	public: void vo_expression();
	public: void vo_comparison();
	public: int  eqop();
	public: void entity_filter();
	public: void entity_filter_expression();
	public: void entity_and_expression();
	public: void entity_expression();
	public: void entity_comparison();
	public: int  relop();
public:
	ANTLR_USE_NAMESPACE(antlr)RefAST getAST()
	{
		return returnAST;
	}
	
protected:
	ANTLR_USE_NAMESPACE(antlr)RefAST returnAST;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 35;
#else
	enum {
		NUM_TOKENS = 35
	};
#endif
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
	static const unsigned long _tokenSet_5_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_5;
	static const unsigned long _tokenSet_6_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_6;
	static const unsigned long _tokenSet_7_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_7;
};

#endif /*INC_isn_parser_hpp_*/
