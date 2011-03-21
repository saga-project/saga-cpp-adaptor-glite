#ifndef INC_sdTokenTypes_hpp_
#define INC_sdTokenTypes_hpp_

/* $ANTLR 2.7.7 (2006-11-01): "sd_grammar.g" -> "sdTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API sdTokenTypes {
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
		K_VO = 12,
		K_VOMS = 13,
		K_FQAN = 14,
		K_DN = 15,
		K_TYPE = 16,
		K_NAME = 17,
		K_UID = 18,
		K_URI = 19,
		K_ENDPOINT = 20,
		K_SITE = 21,
		K_SERVICE = 22,
		K_INTERFACE_VERSION = 23,
		K_IMPLEMENTATION_VERSION = 24,
		K_IMPLEMENTOR = 25,
		K_CAPABILITY = 26,
		K_ALL = 27,
		K_ANY = 28,
		K_STRING_LIST = 29,
		SVC_CMP = 30,
		DATA_CMP = 31,
		S_IDENTIFIER = 32,
		LPAREN = 33,
		RPAREN = 34,
		S_CHAR_LITERAL = 35,
		S_NUMBER = 36,
		COMMA = 37,
		NE = 38,
		EQ = 39,
		LESS = 40,
		GT = 41,
		LE = 42,
		GE = 43,
		WS_ = 44,
		DIGIT = 45,
		INTEGER = 46,
		FLOAT = 47,
		LETTER = 48,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_sdTokenTypes_hpp_*/
