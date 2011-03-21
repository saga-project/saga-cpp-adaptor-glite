#ifndef INC_isn_lexer_hpp_
#define INC_isn_lexer_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.7 (2006-11-01): "isn_grammar.g" -> "isn_lexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "isnTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
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

#line 24 "isn_lexer.hpp"
class CUSTOM_API isn_lexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public isnTokenTypes
{
#line 1 "isn_grammar.g"
#line 28 "isn_lexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return false;
	}
public:
	isn_lexer(ANTLR_USE_NAMESPACE(std)istream& in);
	isn_lexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	isn_lexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mWS_(bool _createToken);
	public: void mLPAREN(bool _createToken);
	public: void mRPAREN(bool _createToken);
	public: void mCOMMA(bool _createToken);
	protected: void mDIGIT(bool _createToken);
	protected: void mINTEGER(bool _createToken);
	protected: void mFLOAT(bool _createToken);
	public: void mS_NUMBER(bool _createToken);
	public: void mS_CHAR_LITERAL(bool _createToken);
	public: void mEQ(bool _createToken);
	public: void mNE(bool _createToken);
	public: void mGT(bool _createToken);
	protected: void mLETTER(bool _createToken);
	public: void mS_IDENTIFIER(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
};

#endif /*INC_isn_lexer_hpp_*/
