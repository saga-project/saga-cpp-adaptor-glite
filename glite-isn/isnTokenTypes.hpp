#ifndef INC_isnTokenTypes_hpp_
#define INC_isnTokenTypes_hpp_

/* $ANTLR 2.7.7 (2006-11-01): "isn_grammar.g" -> "isnTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API isnTokenTypes {
#endif
	enum {
		EOF_ = 1,
		K_NULL = 4,
		K_IS = 5,
		K_IN = 6,
		K_LIKE = 7,
		K_ESCAPE = 8,
		K_NOT = 9,
		K_AND = 10,
		K_OR = 11,
		K_ALL = 12,
		K_ANY = 13,
		K_STRING_LIST = 14,
		SVC_CMP = 15,
		DATA_CMP = 16,
		S_CHAR_LITERAL = 17,
		LPAREN = 18,
		S_NUMBER = 19,
		COMMA = 20,
		RPAREN = 21,
		K_VO = 22,
		NE = 23,
		EQ = 24,
		LESS = 25,
		GT = 26,
		LE = 27,
		GE = 28,
		S_IDENTIFIER = 29,
		WS_ = 30,
		DIGIT = 31,
		INTEGER = 32,
		FLOAT = 33,
		LETTER = 34,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_isnTokenTypes_hpp_*/
