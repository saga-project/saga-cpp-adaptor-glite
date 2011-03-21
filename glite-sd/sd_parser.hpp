#ifndef INC_sd_parser_hpp_
#define INC_sd_parser_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.7 (2006-11-01): "sd_grammar.g" -> "sd_parser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "sdTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

#line 19 "sd_grammar.g"

   #include "sd_leafnode.hpp"
   #include "sd_internalnode.hpp"
   #include <iostream>
   #include <string>
   #include <list>
   #include <math.h>
   #include <utility>
   #include <stdlib.h>
   using namespace std;

#line 24 "sd_parser.hpp"
class CUSTOM_API sd_parser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public sdTokenTypes
{
#line 1 "sd_grammar.g"
#line 28 "sd_parser.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	sd_parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	sd_parser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	sd_parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	sd_parser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	sd_parser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return sd_parser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return sd_parser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return sd_parser::tokenNames;
	}
	public: int  service_keyword();
	public: int  authz_keyword();
	public: int  authz_keyword_with_all();
	public: string  data_keyword();
	public: void service_filter();
	public: void service_filter_expression();
	public: void service_and_expression();
	public: void service_expression();
	public: void service_comparison();
	public: std::list<std::string>  in_clause();
	public: pair<string, char>  like_clause();
	public: int  eqop();
	public: std::list<std::string>  bracketed_list();
	public: void string_literal_list();
	public: void vo_filter();
	public: void vo_filter_expression();
	public: void vo_and_expression();
	public: void vo_expression();
	public: void vo_comparison();
	public: void data_filter();
	public: void data_filter_expression();
	public: void data_and_expression();
	public: void data_expression();
	public: void data_comparison();
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
	static const int NUM_TOKENS = 49;
#else
	enum {
		NUM_TOKENS = 49
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
	static const unsigned long _tokenSet_8_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_8;
	static const unsigned long _tokenSet_9_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_9;
	static const unsigned long _tokenSet_10_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_10;
	static const unsigned long _tokenSet_11_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_11;
	static const unsigned long _tokenSet_12_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_12;
	static const unsigned long _tokenSet_13_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_13;
	static const unsigned long _tokenSet_14_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_14;
	static const unsigned long _tokenSet_15_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_15;
	static const unsigned long _tokenSet_16_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_16;
};

#endif /*INC_sd_parser_hpp_*/
