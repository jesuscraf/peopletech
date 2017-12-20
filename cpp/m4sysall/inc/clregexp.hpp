//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                clregexp.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                17-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//		This module defines a class to handle regular expressions
//		Based on usenet regexp.h
//
//==============================================================================


#ifndef CLREGEXPR_H
#define CLREGEXPR_H

#include "m4sysall_dll.hpp"
#include "m4types.hpp"


#define RE_NREGS	10  /* number of registers available */


/* bit definitions for syntax */
#define RE_NO_BK_PARENS		1    /* no quoting for parentheses */
#define RE_NO_BK_VBAR		2    /* no quoting for vertical bar */
#define RE_BK_PLUS_QM		4    /* quoting needed for + and ? */
#define RE_TIGHT_VBAR		8    /* | binds tighter than ^ and $ */
#define RE_NEWLINE_OR		16   /* treat newline as or */
#define RE_CONTEXT_INDEP_OPS	32   /* ^$?*+ are special in all contexts */
#define RE_ANSI_HEX		64   /* ansi sequences (\n etc) and \xhh */
#define RE_NO_GNU_EXTENSIONS   128   /* no gnu extensions */

/* definitions for some common regexp styles */
#define RE_SYNTAX_AWK	(RE_NO_BK_PARENS|RE_NO_BK_VBAR|RE_CONTEXT_INDEP_OPS)
#define RE_SYNTAX_EGREP	(RE_SYNTAX_AWK|RE_NEWLINE_OR)
#define RE_SYNTAX_GREP	(RE_BK_PLUS_QM|RE_NEWLINE_OR)
#define RE_SYNTAX_EMACS	0



class M4_DECL_M4SYSALL ClRegExp{
	
public:
	typedef struct re_pattern_buffer
	{
		m4char_t *buffer; 	 /* compiled pattern */
		m4int_t allocated;	 /* allocated size of compiled pattern */
		m4int_t used;		 /* actual length of compiled pattern */
		m4char_t *fastmap;	 /* fastmap[ch] is true if ch can start pattern */
		m4char_t *translate;	 /* translation to apply during compilation/matching */
		m4char_t fastmap_accurate; /* true if fastmap is valid */
		m4char_t can_be_null;	 /* true if can match empty string */
		m4char_t uses_registers;	 /* registers are used and need to be initialized */
		m4char_t anchor;		 /* anchor: 0=none 1=begline 2=begbuf */
	} *regexp_t;
	
	typedef struct re_registers
	{
		m4int_t start[RE_NREGS];  /* start offset of region */
		m4int_t end[RE_NREGS];    /* end offset of region */
	} *regexp_registers_t;
	
	
	ClRegExp (void);
	
	virtual ~ClRegExp (void);
	
	m4int_t SetSyntax(m4int_t syntax);
	/* This sets the syntax to use and returns the previous syntax.  The
	syntax is specified by a bit mask of the above defined bits. */
	
	m4char_t *CompilePattern(m4char_t *regex, m4int_t regex_size);
	/* This compiles the regexp (given in regex and length in regex_size).
	This returns NULL if the regexp compiled successfully, and an error
	message if an error was encountered.  The buffer field must be
	initialized to a memory area allocated by malloc (or to NULL) before
	use, and the allocated field must be set to its length (or 0 if buffer is
	NULL).  Also, the translate field must be set to point to a valid
	translation table, or NULL if it is not used. */
	
	m4int_t Match(m4char_t *string, m4int_t size, m4int_t pos);
	/* This tries to match the regexp against the string.  This returns the
	length of the matched portion, or -1 if the pattern could not be
	matched and -2 if an error (such as failure stack overflow) is
	encountered. */
	
	m4int_t Match2(m4char_t *string1, m4int_t size1, m4char_t *string2, m4int_t size2, m4int_t pos, m4int_t mstop);
	/* This tries to match the regexp to the concatenation of string1 and
	string2.  This returns the length of the matched portion, or -1 if the
	pattern could not be matched and -2 if an error (such as failure stack
	overflow) is encountered. */
	
	m4int_t Search(m4char_t *string, m4int_t size, m4int_t startpos,	m4int_t range);
	/* This rearches for a substring matching the regexp.  This returns the first
	index at which a match is found.  range specifies at how many positions to
	try matching; positive values indicate searching forwards, and negative
	values indicate searching backwards.  mstop specifies the offset beyond
	which a match must not go.  This returns -1 if no match is found, and
	-2 if an error (such as failure stack overflow) is encountered. */
	
	m4int_t Search2(m4char_t *string1, m4int_t size1, m4char_t *string2, m4int_t size2, m4int_t startpos, m4int_t range,m4int_t mstop);
	/* This is like re_search, but search from the concatenation of string1 and
	string2.  */
	
	void CompileFastmap(void);
	/* This computes the fastmap for the regexp.  For this to have any effect,
	the calling program must have initialized the fastmap field to point
	to an array of 256 characters. */

	void SetSensitive (m4bool_t IsSensitive);
	/*Set the sensitive mode on|off*/

	regexp_registers_t GetpRegisters(void){
		return m_poRegs;
	}
	/*Returns the registers to inspect the mathcing*/

	re_pattern_buffer GetCompiled(void){
		return m_oCompiled;
	}
	const m4char_t* GetPatternString(void){
		return m_pszRegex;
	}
	
	void SetUseRegs(m4bool_t bUseRegs); /*chosse to use regs or not*/

protected:

	m4bool_t				m_bIsCompiled;
	m4bool_t				m_bIsSensitive;
	regexp_registers_t	m_poRegs;
	re_pattern_buffer	m_oCompiled;
	m4bool_t				m_bCompileInitialized;
	m4int_t					m_iRegexpSyntax;

	m4int_t					m_regexp_context_indep_ops;
	m4int_t					m_regexp_ansi_sequences;

public:
	//-----------------------------------
	//Enumeraciones
	//-----------------------------------

	enum regexp_compiled_ops /* opcodes for compiled regexp */
	{
		Cend,			/* end of pattern reached */
		Cbol,			/* beginning of line */
		Ceol,			/* end of line */
		Cset,			/* character set.  Followed by 32 bytes of set. */
		Cexact,		/* followed by a byte to match */
		Canychar,		/* matches any character except newline */
		Cstart_memory,	/* set register start addr (followed by reg number) */
		Cend_memory,		/* set register end addr (followed by reg number) */
		Cmatch_memory,	/* match a duplicate of reg contents (regnum follows)*/
		Cjump,		/* followed by two bytes (lsb,msb) of displacement. */
		Cstar_jump,		/* will change to jump/update_failure_jump at runtime */
		Cfailure_jump,	/* jump to addr on failure */
		Cupdate_failure_jump,	/* update topmost failure point and jump */
		Cdummy_failure_jump,	/* push a dummy failure point and jump */
		Cbegbuf,		/* match at beginning of buffer */
		Cendbuf,		/* match at end of buffer */
		Cwordbeg,		/* match at beginning of word */
		Cwordend,		/* match at end of word */
		Cwordbound,		/* match if at word boundary */
		Cnotwordbound,	/* match if not at word boundary */
#ifdef emacs
		Cemacs_at_dot,	/* emacs only: matches at dot */
#endif /* emacs */
		Csyntaxspec,		/* matches syntax code (1 byte follows) */
		Cnotsyntaxspec	/* matches if syntax code does not match (1 byte foll)*/
	};
	
	enum regexp_syntax_op	/* syntax codes for plain and quoted characters */
	{
		Rend,			/* special code for end of regexp */
		Rnormal,		/* normal character */
		Ranychar,		/* any character except newline */
		Rquote,		/* the quote character */
		Rbol,			/* match beginning of line */
		Reol,			/* match end of line */
		Roptional,		/* match preceding expression optionally */
		Rstar,		/* match preceding expr zero or more times */
		Rplus,		/* match preceding expr one or more times */
		Ror,			/* match either of alternatives */
		Ropenpar,		/* opening parenthesis */
		Rclosepar,		/* closing parenthesis */
		Rmemory,		/* match memory register */
		Rextended_memory,	/* \vnn to match registers 10-99 */
		Ropenset,		/* open set.  Internal syntax hard-coded below. */
		/* the following are gnu extensions to "normal" regexp syntax */
		Rbegbuf,		/* beginning of buffer */
		Rendbuf,		/* end of buffer */
		Rwordchar,		/* word character */
		Rnotwordchar,		/* not word character */
		Rwordbeg,		/* beginning of word */
		Rwordend,		/* end of word */
		Rwordbound,		/* word bound */
		Rnotwordbound,	/* not word bound */
#ifdef emacs
		Remacs_at_dot,	/* emacs: at dot */
		Remacs_syntaxspec,	/* syntaxspec */
		Remacs_notsyntaxspec,	/* notsyntaxspec */
#endif /* emacs */
		Rnum_ops
	};
private:
	//------------------------
	//tablas
	//------------------------
	m4uchar_t m_regexp_plain_ops[256];
	m4uchar_t m_regexp_quoted_ops[256];
	m4uchar_t m_regexp_precedences[Rnum_ops];
	m4char_t	m_acFastmap[256];
#ifdef SYNTAX_TABLE
	m4char_t *m_syntax_table;
#else
	m4char_t m_syntax_table[256];
	m4int_t m_iSyntaxTableInited;
#endif /* SYNTAX_TABLE */

	//-------------------
	//funciones internas
	//-------------------
	void CompileInitialize(void);
	m4int_t HexCharToDecimal(m4int_t ch);
	void CompileFastmapAux(m4char_t *code, m4int_t pos, m4char_t *visited, m4char_t *can_be_null, m4char_t *fastmap);
	m4int_t DoCompileFastmap(m4char_t *buffer, m4int_t used, m4int_t pos, m4char_t *can_be_null, m4char_t *fastmap);
	m4int_t _Match2(m4char_t *string1, m4int_t size1, m4char_t *string2, m4int_t size2, m4int_t pos, m4int_t mstop);
	m4int_t _Search2(m4char_t *string1, m4int_t size1, m4char_t *string2, m4int_t size2, m4int_t startpos, m4int_t range,m4int_t mstop);

	void RegexUpr (char *psxRegex);
	//to make insensitive searches possible.


	//almacenes de cadenas
	m4char_t *m_pszRegex;
	m4int_t m_iRegexSize;
};

#endif	// CLREGEXPR_H
