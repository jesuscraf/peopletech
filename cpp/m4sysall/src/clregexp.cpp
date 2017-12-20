//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                clregexp.cpp   
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
//		Based on usenet regexp.c
//
//==============================================================================

/*

  based on usenet regexp.c
*/
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include "clregexp.hpp"
#include "m4stl.hpp"
#include "m4sysalloc.hpp"
#include "m4unicode.hpp"

#define MACRO_BEGIN do {
#define MACRO_END } while (0)


//--------------------------
//Constructor
//--------------------------
ClRegExp::ClRegExp(){
	m_bCompileInitialized=M4_FALSE;
	m_bIsCompiled=M4_FALSE;
	m_bIsSensitive=M4_TRUE;
	m_iRegexpSyntax=0;
	
	m_poRegs=0;
	
	m_oCompiled.allocated=0;
	m_oCompiled.buffer=0;
	m_oCompiled.translate=0;
	m_oCompiled.fastmap=m_acFastmap;

	m_pszRegex=0;
	m_iRegexSize=0;

#if  !defined(SYNTAX_TABLE)
	m_iSyntaxTableInited=0;
#endif
};

ClRegExp::~ClRegExp (void){
	if ( m_poRegs ) delete  m_poRegs;
	if (m_pszRegex) delete	[]m_pszRegex;

}

void ClRegExp::SetUseRegs(m4bool_t bUseRegs){
	
	if (bUseRegs && !m_poRegs){
		m_poRegs=new re_registers;
	}
	else if (!bUseRegs && m_poRegs){
		delete m_poRegs;
		m_poRegs=0;
	}
}

void ClRegExp::SetSensitive(m4bool_t IsSensitive){
	if (m_bIsSensitive != IsSensitive){
		m_bIsSensitive = IsSensitive;
		m_bIsCompiled=M4_FALSE;
	}
}

#define NUM_LEVELS  5    /* number of precedence levels in use */
#define MAX_NESTING 100  /* max nesting level of operators */


#define SYNTAX(ch) m_syntax_table[(m4uchar_t)(ch)]
#define Sword 1



void ClRegExp::CompileInitialize(void)
{
	m4int_t a;
	
#if  !defined(SYNTAX_TABLE)
	
	if (!m_iSyntaxTableInited)
    {
		m_iSyntaxTableInited = 1;

		/* Bug 0126182
		La inicialización es todo a 1 menos el 0
		Se echa para atrás por el impacto
		m_syntax_table[0] = 0;
		memset(m_syntax_table + 1, Sword, 255);
		*/
		memset(m_syntax_table, 0, 256);
		for (a = 'a'; a <= 'z'; a++)
			m_syntax_table[a] = Sword;
		for (a = 'A'; a <= 'Z'; a++)
			m_syntax_table[a] = Sword;
		for (a = '0'; a <= '9'; a++)
			m_syntax_table[a] = Sword;
    }
#endif /*  !SYNTAX_TABLE */
	m_bCompileInitialized = M4_TRUE;
	for (a = 0; a < 256; a++)
    {
		m_regexp_plain_ops[a] = Rnormal;
		m_regexp_quoted_ops[a] = Rnormal;
    }
	for (a = '0'; a <= '9'; a++)
		m_regexp_quoted_ops[a] = Rmemory;
	m_regexp_plain_ops['\134'] = Rquote;
	if (m_iRegexpSyntax & RE_NO_BK_PARENS)
    {
		m_regexp_plain_ops['('] = Ropenpar;
		m_regexp_plain_ops[')'] = Rclosepar;
    }
	else
    {
		m_regexp_quoted_ops['('] = Ropenpar;
		m_regexp_quoted_ops[')'] = Rclosepar;
    }
	if (m_iRegexpSyntax & RE_NO_BK_VBAR)
		m_regexp_plain_ops['\174'] = Ror;
	else
		m_regexp_quoted_ops['\174'] = Ror;
	m_regexp_plain_ops['*'] = Rstar;
	if (m_iRegexpSyntax & RE_BK_PLUS_QM)
    {
		m_regexp_quoted_ops['+'] = Rplus;
		m_regexp_quoted_ops['?'] = Roptional;
    }
	else
    {
		m_regexp_plain_ops['+'] = Rplus;
		m_regexp_plain_ops['?'] = Roptional;
    }
	if (m_iRegexpSyntax & RE_NEWLINE_OR)
		m_regexp_plain_ops['\n'] = Ror;
	m_regexp_plain_ops['\133'] = Ropenset;
	m_regexp_plain_ops['\136'] = Rbol;
	m_regexp_plain_ops['$'] = Reol;
	m_regexp_plain_ops['.'] = Ranychar;
	if (!(m_iRegexpSyntax & RE_NO_GNU_EXTENSIONS))
    {
		m_regexp_quoted_ops['w'] = Rwordchar;
		m_regexp_quoted_ops['W'] = Rnotwordchar;
		m_regexp_quoted_ops['<'] = Rwordbeg;
		m_regexp_quoted_ops['>'] = Rwordend;
		m_regexp_quoted_ops['b'] = Rwordbound;
		m_regexp_quoted_ops['B'] = Rnotwordbound;
		m_regexp_quoted_ops['`'] = Rbegbuf;
		m_regexp_quoted_ops['\''] = Rendbuf;
    }
	if (m_iRegexpSyntax & RE_ANSI_HEX)
		m_regexp_quoted_ops['v'] = Rextended_memory;
	for (a = 0; a < Rnum_ops; a++)
		m_regexp_precedences[a] = 4;
	if (m_iRegexpSyntax & RE_TIGHT_VBAR)
    {
		m_regexp_precedences[Ror] = 3;
		m_regexp_precedences[Rbol] = 2;
		m_regexp_precedences[Reol] = 2;
    }
	else
    {
		m_regexp_precedences[Ror] = 2;
		m_regexp_precedences[Rbol] = 3;
		m_regexp_precedences[Reol] = 3;
    }
	m_regexp_precedences[Rclosepar] = 1;
	m_regexp_precedences[Rend] = 0;
	m_regexp_context_indep_ops = (m_iRegexpSyntax & RE_CONTEXT_INDEP_OPS) != 0;
	m_regexp_ansi_sequences = (m_iRegexpSyntax & RE_ANSI_HEX) != 0;
}

//----------------------------
//SetSyntax
//----------------------------
m4int_t ClRegExp::SetSyntax(m4int_t syntax)
{
	m4int_t ret;
	
	ret = m_iRegexpSyntax;
	m_iRegexpSyntax = syntax;
	CompileInitialize();
	m_bIsCompiled=M4_FALSE;
	return ret;
}

//-------------------------------------
//HexCharToDecimal
//-------------------------------------
m4int_t ClRegExp::HexCharToDecimal(m4int_t ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	return 16;
}

//-------------------------------------
//CompilePattern
//-------------------------------------

m4char_t *ClRegExp::CompilePattern(m4char_t *regex, m4int_t size)
{
	m4int_t a, pos, op, current_level, level, opcode;
	m4int_t pattern_offset, alloc;
	m4int_t starts[NUM_LEVELS * MAX_NESTING], starts_base;
	m4int_t future_jumps[MAX_NESTING], num_jumps;
	m4uchar_t ch;
	m4char_t *pattern, *translate;
	m4int_t next_register, paren_depth, num_open_registers, open_registers[RE_NREGS];
	m4int_t beginning_context;
	
/* #define NEXTCHAR(var)			\
	MACRO_BEGIN				\
    if (pos >= size)			\
	goto ends_prematurely;		\
    (var) = m_pszRegex[pos];			\
    pos++;				\
	MACRO_END
*/
#define NEXTCHAR(var)			MACRO_BEGIN		 if (pos >= size)	goto ends_prematurely;    (var) = m_pszRegex[pos];	    pos++;	MACRO_END	

/* #define ALLOC(amount)				\
	MACRO_BEGIN					\
    if (pattern_offset+(amount) > alloc)	\
	{						\
	alloc += 256 + (amount);		\
	pattern = (m4char_t*)realloc(pattern, alloc);	\
	if (!pattern)				\
	goto out_of_memory;			\
	}						\
	MACRO_END
*/
	
#define ALLOC(amount)		MACRO_BEGIN			    if (pattern_offset+(amount) > alloc)	{						alloc += 256 + (amount);		pattern = (m4char_t*)realloc(pattern, alloc);	if (!pattern)				goto out_of_memory;			}						MACRO_END

#define STORE(ch) pattern[pattern_offset++] = (ch)
	
#define CURRENT_LEVEL_START (starts[starts_base + current_level])
	
#define SET_LEVEL_START starts[starts_base + current_level] = pattern_offset
	
/* #define PUSH_LEVEL_STARTS if (starts_base < (MAX_NESTING-1)*NUM_LEVELS) \
	starts_base += NUM_LEVELS;			\
	else						\
	goto too_complex
*/
	
#define PUSH_LEVEL_STARTS if (starts_base < (MAX_NESTING-1)*NUM_LEVELS) 	starts_base += NUM_LEVELS;				else							goto too_complex

#define POP_LEVEL_STARTS starts_base -= NUM_LEVELS
	
/* #define PUT_ADDR(offset,addr)				\
	MACRO_BEGIN						\
    m4int_t disp = (addr) - (offset) - 2;			\
    pattern[(offset)] = disp & 0xff;			\
    pattern[(offset)+1] = (disp>>8) & 0xff;		\
	MACRO_END
*/
	#define PUT_ADDR(offset,addr)		MACRO_BEGIN	    m4int_t disp = (addr) - (offset) - 2;	    pattern[(offset)] = disp & 0xff;	    pattern[(offset)+1] = (disp>>8) & 0xff;		MACRO_END

/* #define INSERT_JUMP(pos,type,addr)			\
	MACRO_BEGIN						\
    m4int_t a, p = (pos), t = (type), ad = (addr);		\
    for (a = pattern_offset - 1; a >= p; a--)		\
	pattern[a + 3] = pattern[a];			\
    pattern[p] = t;					\
    PUT_ADDR(p+1,ad);					\
    pattern_offset += 3;				\
	MACRO_END
*/
	
	#define INSERT_JUMP(pos,type,addr)			MACRO_BEGIN		    m4int_t a, p = (pos), t = (type), ad = (addr);	    for (a = pattern_offset - 1; a >= p; a--)		pattern[a + 3] = pattern[a];		    pattern[p] = t;				    PUT_ADDR(p+1,ad);				    pattern_offset += 3;				MACRO_END

#define SETBIT(buf,offset,bit) (buf)[(offset)+(bit)/8] |= (1<<((bit) & 7))
	
/* #define SET_FIELDS				\
	MACRO_BEGIN					\
    m_oCompiled.allocated = alloc;			\
    m_oCompiled.buffer = pattern;			\
    m_oCompiled.used = pattern_offset;		\
	MACRO_END
   */
	
#define SET_FIELDS		MACRO_BEGIN				    m_oCompiled.allocated = alloc;			    m_oCompiled.buffer = pattern;			    m_oCompiled.used = pattern_offset;			MACRO_END
   
/* #define GETHEX(var)						\
	MACRO_BEGIN							\
    m4char_t gethex_ch, gethex_value;				\
    NEXTCHAR(gethex_ch);					\
    gethex_value = HexCharToDecimal(gethex_ch);		\
    if (gethex_value == 16)					\
	goto hex_error;						\
    NEXTCHAR(gethex_ch);					\
    gethex_ch = HexCharToDecimal(gethex_ch);			\
    if (gethex_ch == 16)					\
	goto hex_error;						\
    (var) = gethex_value * 16 + gethex_ch;			\
	MACRO_END
*/
	
#define GETHEX(var)			MACRO_BEGIN	    m4char_t gethex_ch, gethex_value;				    NEXTCHAR(gethex_ch);					    gethex_value = HexCharToDecimal(gethex_ch);		    if (gethex_value == 16)		goto hex_error;	    NEXTCHAR(gethex_ch);					    gethex_ch = HexCharToDecimal(gethex_ch);			    if (gethex_ch == 16)			goto hex_error;						    (var) = gethex_value * 16 + gethex_ch;				MACRO_END

/* #define ANSI_TRANSLATE(ch)				\
	MACRO_BEGIN						\
    switch (ch)						\
	{							\
      case 'a':						\
      case 'A':						\
	  ch = 7;  audible bell 			\
	  break;						\
      case 'b':						\
      case 'B':						\
	  ch = 8;  backspace 				\
	  break;						\
      case 'f':						\
      case 'F':						\
	  ch = 12; form feed 			\
	  break;						\
      case 'n':						\
      case 'N':						\
	  ch = 10;  line feed 			\
	  break;						\
      case 'r':						\
      case 'R':						\
	  ch = 13;  carriage return 			\
	  break;						\
      case 't':						\
      case 'T':						\
	  ch = 9;  tab 				\
	  break;						\
      case 'v':						\
      case 'V':						\
	  ch = 11;  vertical tab 			\
	  break;						\
      case 'x':  hex code 				\
      case 'X':						\
	  GETHEX(ch);					\
	  break;						\
      default:						\
	   other characters passed through 		\
	  if (translate)					\
	  ch = translate[(m4uchar_t)ch];		\
	  break;						\
      }							\
MACRO_END
*/

#define ANSI_TRANSLATE(ch)		MACRO_BEGIN			    switch (ch)				{	      case 'a':			      case 'A':				  ch = 7; /* audible bell */	  break;		      case 'b':		      case 'B':			  ch = 8; /* backspace */		  break;			      case 'f':	      case 'F':			  ch = 12; /* form feed */	  break;		      case 'n':			      case 'N':				  ch = 10; /* line feed */	  break;			      case 'r':		      case 'R':				  ch = 13; /* carriage return */		  break;	      case 't':	      case 'T':				  ch = 9; /* tab */		  break;		      case 'v':		      case 'V':				  ch = 11; /* vertical tab */		  break;		      case 'x': /* hex code */	      case 'X':			  GETHEX(ch);			  break;			      default:				  /* other characters passed through */	  if (translate)					  ch = translate[(m4uchar_t)ch];		  break;						      }							MACRO_END




if (m_bIsCompiled && m_bIsSensitive && m_iRegexSize==size && (strcmp(m_pszRegex,regex)==0) ){
	return NULL; //intentan que compilemos lo mismo->ya está así que salimos
}

if (m_bIsCompiled && !m_bIsSensitive && m_iRegexSize==size){

	m4char_t *pszTemp=new m4char_t [size+1];

	strcpy(pszTemp,regex); 
	RegexUpr(pszTemp);
	if (strcmp(m_pszRegex,pszTemp)==0){
		delete [] pszTemp;
		return NULL; //intentan que compilemos lo mismo->ya está así que salimos
	}
	delete [] pszTemp;
}

delete [] m_pszRegex;
m_pszRegex=new m4char_t [size+1];
m_iRegexSize=size;

strcpy (m_pszRegex,regex);

if (!m_bIsSensitive){
	RegexUpr(m_pszRegex);
}


if (!m_bCompileInitialized)
CompileInitialize();
m_oCompiled.used = 0;
m_oCompiled.fastmap_accurate = 0;
m_oCompiled.uses_registers = 0;
translate = m_oCompiled.translate;
pattern = m_oCompiled.buffer;
alloc = m_oCompiled.allocated;
if (alloc == 0 || pattern == NULL)
{
	alloc = 256;
	pattern = (m4char_t*)malloc(alloc);
	if (!pattern)
		goto out_of_memory;
}
pattern_offset = 0;
starts_base = 0;
num_jumps = 0;
current_level = 0;
SET_LEVEL_START;
num_open_registers = 0;
next_register = 1;
paren_depth = 0;
beginning_context = 1;
op = -1;
/* we use Rend dummy to ensure that pending jumps are updated (due to
low priority of Rend) before exiting the loop. */
pos = 0;
while (op != Rend)
{
	if (pos >= size)
		op = Rend;
	else
	{
		NEXTCHAR(ch);
		if (translate)
			ch = translate[(m4uchar_t)ch];
		op = m_regexp_plain_ops[(m4uchar_t)ch];
		if (op == Rquote)
		{
			NEXTCHAR(ch);
			op = m_regexp_quoted_ops[(m4uchar_t)ch];
			if (op == Rnormal && m_regexp_ansi_sequences)
				ANSI_TRANSLATE(ch);
		}
	}
	level = m_regexp_precedences[op];
	/* printf("ch='%c' op=%d level=%d current_level=%d curlevstart=%d\n",
	ch, op, level, current_level, CURRENT_LEVEL_START); */
	if (level > current_level)
	{
		for (current_level++; current_level < level; current_level++)
			SET_LEVEL_START;
		SET_LEVEL_START;
	}
	else
		if (level < current_level)
		{
			current_level = level;
			for (;num_jumps > 0 &&
				future_jumps[num_jumps-1] >= CURRENT_LEVEL_START;
			num_jumps--)
				PUT_ADDR(future_jumps[num_jumps-1], pattern_offset);
		}
		switch (op)
		{
		case Rend:
			break;
		case Rnormal:
normal_char:
			opcode = Cexact;
store_opcode_and_arg: /* opcode & ch must be set */
			SET_LEVEL_START;
			ALLOC(2);
			STORE(opcode);
			STORE(ch);
			break;
		case Ranychar:
			opcode = Canychar;
store_opcode:
			SET_LEVEL_START;
			ALLOC(1);
			STORE(opcode);
			break;
		case Rquote:
			return "abort";
			/*NOTREACHED*/
		case Rbol:
			if (!beginning_context)
				if (m_regexp_context_indep_ops)
					goto op_error;
				else
					goto normal_char;
				opcode = Cbol;
				goto store_opcode;
		case Reol:
			if (!((pos >= size) ||
				((m_iRegexpSyntax & RE_NO_BK_VBAR) ?
				(m_pszRegex[pos] == '\174') :
			(pos+1 < size && m_pszRegex[pos] == '\134' &&
				m_pszRegex[pos+1] == '\174')) ||
				((m_iRegexpSyntax & RE_NO_BK_PARENS)?
				(m_pszRegex[pos] == ')'):
			(pos+1 < size && m_pszRegex[pos] == '\134' &&
				m_pszRegex[pos+1] == ')'))))
				if (m_regexp_context_indep_ops)
					goto op_error;
				else
					goto normal_char;
				opcode = Ceol;
				goto store_opcode;
				break;
		case Roptional:
			if (beginning_context)
				if (m_regexp_context_indep_ops)
					goto op_error;
				else
					goto normal_char;
				if (CURRENT_LEVEL_START == pattern_offset)
					break; /* ignore empty patterns for ? */
				ALLOC(3);
				INSERT_JUMP(CURRENT_LEVEL_START, Cfailure_jump,
					pattern_offset + 3);
				break;
		case Rstar:
		case Rplus:
			if (beginning_context)
				if (m_regexp_context_indep_ops)
					goto op_error;
				else
					goto normal_char;
				if (CURRENT_LEVEL_START == pattern_offset)
					break; /* ignore empty patterns for + and * */
				ALLOC(9);
				INSERT_JUMP(CURRENT_LEVEL_START, Cfailure_jump,
					pattern_offset + 6);
				INSERT_JUMP(pattern_offset, Cstar_jump, CURRENT_LEVEL_START);
				if (op == Rplus)  /* jump over initial failure_jump */
					INSERT_JUMP(CURRENT_LEVEL_START, Cdummy_failure_jump,
					CURRENT_LEVEL_START + 6);
				break;
		case Ror:
			ALLOC(6);
			INSERT_JUMP(CURRENT_LEVEL_START, Cfailure_jump,
				pattern_offset + 6);
			if (num_jumps >= MAX_NESTING)
				goto too_complex;
			STORE(Cjump);
			future_jumps[num_jumps++] = pattern_offset;
			STORE(0);
			STORE(0);
			SET_LEVEL_START;
			break;
		case Ropenpar:
			SET_LEVEL_START;
			if (next_register < RE_NREGS)
			{
				m_oCompiled.uses_registers = 1;
				ALLOC(2);
				STORE(Cstart_memory);
				STORE(next_register);
				open_registers[num_open_registers++] = next_register;
				next_register++;
			}
			paren_depth++;
			PUSH_LEVEL_STARTS;
			current_level = 0;
			SET_LEVEL_START;
			break;
		case Rclosepar:
			if (paren_depth <= 0)
				goto parenthesis_error;
			POP_LEVEL_STARTS;
			current_level = m_regexp_precedences[Ropenpar];
			paren_depth--;
			if (paren_depth < num_open_registers)
			{
				m_oCompiled.uses_registers = 1;
				ALLOC(2);
				STORE(Cend_memory);
				num_open_registers--;
				STORE(open_registers[num_open_registers]);
			}
			break;
		case Rmemory:
			if (ch == '0')
				goto bad_match_register;
			assert(ch >= '0' && ch <= '9');
			m_oCompiled.uses_registers = 1;
			opcode = Cmatch_memory;
			ch -= '0';
			goto store_opcode_and_arg;
		case Rextended_memory:
			NEXTCHAR(ch);
			if (ch < '0' || ch > '9')
				goto bad_match_register;
			NEXTCHAR(a);
			if (a < '0' || a > '9')
				goto bad_match_register;
			ch = 10 * (a - '0') + ch - '0';
			if (ch <= 0 || ch >= RE_NREGS)
				goto bad_match_register;
			m_oCompiled.uses_registers = 1;
			opcode = Cmatch_memory;
			goto store_opcode_and_arg;
		case Ropenset:
			{
				m4int_t complement,prev,offset,range,firstchar;
				
				SET_LEVEL_START;
				ALLOC(1+256/8);
				STORE(Cset);
				offset = pattern_offset;
				for (a = 0; a < 256/8; a++)
					STORE(0);
				NEXTCHAR(ch);
				if (translate)
					ch = translate[(m4uchar_t)ch];
				if (ch == '\136')
				{
					complement = 1;
					NEXTCHAR(ch);
					if (translate)
						ch = translate[(m4uchar_t)ch];
				}
				else
					complement = 0;
				prev = -1;
				range = 0;
				firstchar = 1;
				while (ch != '\135' || firstchar)
				{
					firstchar = 0;
					if (m_regexp_ansi_sequences && ch == '\134')
					{
						NEXTCHAR(ch);
						ANSI_TRANSLATE(ch);
					}
					if (range)
					{
						for (a = prev; a <= ch; a++)
							SETBIT(pattern, offset, a);
						prev = -1;
						range = 0;
					}
					else
						if (prev != -1 && ch == '-')
							range = 1;
						else
						{
							SETBIT(pattern, offset, ch);
							prev = ch;
						}
						NEXTCHAR(ch);
						if (translate)
							ch = translate[(m4uchar_t)ch];
				}
				if (range)
					SETBIT(pattern, offset, '-');
				if (complement)
				{
					for (a = 0; a < 256/8; a++)
						pattern[offset+a] ^= 0xff;
				}
				break;
			}
		case Rbegbuf:
			opcode = Cbegbuf;
			goto store_opcode;
		case Rendbuf:
			opcode = Cendbuf;
			goto store_opcode;
		case Rwordchar:
			opcode = Csyntaxspec;
			ch = Sword;
			goto store_opcode_and_arg;
		case Rnotwordchar:
			opcode = Cnotsyntaxspec;
			ch = Sword;
			goto store_opcode_and_arg;
		case Rwordbeg:
			opcode = Cwordbeg;
			goto store_opcode;
		case Rwordend:
			opcode = Cwordend;
			goto store_opcode;
		case Rwordbound:
			opcode = Cwordbound;
			goto store_opcode;
		case Rnotwordbound:
			opcode = Cnotwordbound;
			goto store_opcode;
		default:
			return "abort";
	}
	beginning_context = (op == Ropenpar || op == Ror);
    }
	if (starts_base != 0)
		goto parenthesis_error;
	assert(num_jumps == 0);
	ALLOC(1);
	STORE(Cend);
	SET_FIELDS;
	m_bIsCompiled=M4_TRUE; //salimos con éxito, la damos por compilada
	return NULL;
	
op_error:
	SET_FIELDS;
	return "Badly placed special character";
	
bad_match_register:
	SET_FIELDS;
	return "Bad match register number";
	
hex_error:
	SET_FIELDS;
	return "Bad hexadecimal number";
	
parenthesis_error:
	SET_FIELDS;
	return "Badly placed parenthesis";
	
out_of_memory:
	SET_FIELDS;
	return "Out of memory";
	
ends_prematurely:
	SET_FIELDS;
	return "Regular expression ends prematurely";
	
too_complex:
	SET_FIELDS;
	return "Regular expression too complex";
}
#undef CHARAT
#undef NEXTCHAR
#undef GETHEX
#undef ALLOC
#undef STORE
#undef CURRENT_LEVEL_START
#undef SET_LEVEL_START
#undef PUSH_LEVEL_STARTS
#undef POP_LEVEL_STARTS
#undef PUT_ADDR
#undef INSERT_JUMP
#undef SETBIT
#undef SET_FIELDS

//--------------------------------
//CompileFastmapAux
//--------------------------------
void ClRegExp::CompileFastmapAux(m4char_t *code, m4int_t pos, m4char_t *visited, m4char_t *can_be_null, m4char_t *fastmap)
{
	m4int_t a, b, syntaxcode;
	
	if (visited[pos])
		return;  /* we have already been here */
	visited[pos] = 1;
	for (;;)
		switch (code[pos++])
	{
      case Cend:
		  *can_be_null = 1;
		  return;
      case Cbol:
      case Cbegbuf:
      case Cendbuf:
      case Cwordbeg:
      case Cwordend:
      case Cwordbound:
      case Cnotwordbound:
		  break;
      case Csyntaxspec:
		  syntaxcode = code[pos++];
		  for (a = 0; a < 256; a++)
			  if (SYNTAX(a) == syntaxcode)
				  fastmap[a] = 1;
		  return;
      case Cnotsyntaxspec:
		  syntaxcode = code[pos++];
		  for (a = 0; a < 256; a++)
			  if (SYNTAX(a) != syntaxcode)
				  fastmap[a] = 1;
		  return;
      case Ceol:
		  fastmap['\n'] = 1;
		  if (*can_be_null == 0)
			  *can_be_null = 2;  /* can match null, but only at end of buffer*/
		  return;
      case Cset:
		  for (a = 0; a < 256/8; a++)
			  if (code[pos + a] != 0)
				  for (b = 0; b < 8; b++)
					  if (code[pos + a] & (1 << b))
						  fastmap[(a << 3) + b] = 1;
					  pos += 256/8;
					  return;
      case Cexact:
		  fastmap[(m4uchar_t)code[pos]] = 1;
		  return;
      case Canychar:
		  for (a = 0; a < 256; a++)
			  if (a != '\n')
				  fastmap[a] = 1;
			  return;
      case Cstart_memory:
      case Cend_memory:
		  pos++;
		  break;
      case Cmatch_memory:
		  for (a = 0; a < 256; a++)
			  fastmap[a] = 1;
		  *can_be_null = 1;
		  return;
      case Cjump: 
      case Cdummy_failure_jump:
      case Cupdate_failure_jump:
      case Cstar_jump:
		  a = (m4uchar_t)code[pos++];
		  a |= (m4uchar_t)code[pos++] << 8;
		  pos += (m4int_t)(short)a;
		  if (visited[pos])
		  {
		  /* argh... the regexp contains empty loops.  This is not
		  good, as this may cause a failure stack overflow when
			  matching.  Oh well. */
			  /* this path leads nowhere; pursue other paths. */
			  return;
		  }
		  visited[pos] = 1;
		  break;
      case Cfailure_jump:
		  a = (m4uchar_t)code[pos++];
		  a |= (m4uchar_t)code[pos++] << 8;
		  a = pos + (m4int_t)(short)a;
		  CompileFastmapAux(code, a, visited, can_be_null, fastmap);
		  break;
      default:
		  //abort();  /* probably some opcode is missing from this switch */
		  return;
		  /*NOTREACHED*/
	}
}

//------------------------------------
//DoCompileFastmap
//------------------------------------
m4int_t ClRegExp::DoCompileFastmap(m4char_t *buffer, m4int_t used, m4int_t pos, m4char_t *can_be_null, m4char_t *fastmap)
{
	m4char_t small_visited[512], *visited;
	
	if (used <= sizeof(small_visited))
		visited = small_visited;
	else
    {
		visited = (m4char_t*)malloc(used);
		if (!visited)
			return 0;
    }
	*can_be_null = 0;
	memset(fastmap, 0, 256);
	memset(visited, 0, used);
	CompileFastmapAux(buffer, pos, visited, can_be_null, fastmap);
	if (visited != small_visited)
		free(visited);
	return 1;
}

//------------------------------------
//CompileFastmap
//------------------------------------
void ClRegExp::CompileFastmap(void)
{
	if (!m_oCompiled.fastmap || m_oCompiled.fastmap_accurate)
		return;
	assert(m_oCompiled.used > 0);
	if (!DoCompileFastmap(m_oCompiled.buffer, m_oCompiled.used, 0, &m_oCompiled.can_be_null,
			     m_oCompiled.fastmap))
				 return;
	if (m_oCompiled.buffer[0] == Cbol)
		m_oCompiled.anchor = 1;   /* begline */
	else
		if (m_oCompiled.buffer[0] == Cbegbuf)
			m_oCompiled.anchor = 2; /* begbuf */
		else
			m_oCompiled.anchor = 0; /* none */
		m_oCompiled.fastmap_accurate = 1;
}

#define INITIAL_FAILURES  128  /* initial # failure points to allocate */
#define MAX_FAILURES     4100  /* max # of failure points before failing */

//-----------------------
//_Match2
//-----------------------
m4int_t ClRegExp::_Match2(m4char_t *string1, m4int_t size1, m4char_t *string2, m4int_t size2, m4int_t pos, m4int_t mstop)
{
	struct failure_point { m4char_t *text, *partend, *code; }
    *failure_stack_start, *failure_sp, *failure_stack_end,
		initial_failure_stack[INITIAL_FAILURES];
	m4char_t *code, *translate, *text, *textend, *partend, *part_2_end;
	m4char_t *regstart_text[RE_NREGS], *regstart_partend[RE_NREGS];
	m4char_t *regend_text[RE_NREGS], *regend_partend[RE_NREGS];
	m4int_t a, b, ch, reg, regch, match_end;
	m4char_t *regtext, *regpartend, *regtextend;

/* #define PREFETCH					\
	MACRO_BEGIN						\
    if (text == partend)				\
	{							\
	if (text == textend)				\
	goto fail;					\
	text = string2;					\
	partend = part_2_end;				\
	}							\
	MACRO_END

  */

#define PREFETCH	MACRO_BEGIN			    if (text == partend)		{					if (text == textend)			goto fail;			text = string2;					partend = part_2_end;				}						MACRO_END

/* #define NEXTCHAR(var)				\
	MACRO_BEGIN					\
    PREFETCH;					\
    (var) = (m4uchar_t)*text++;		\
    if (translate)				\
	(var) = (m4uchar_t)translate[(var)];	\
	MACRO_END
*/
	
#define NEXTCHAR(var)		MACRO_BEGIN		    PREFETCH;		    (var) = (m4uchar_t)*text++;    if (translate)			(var) = (m4uchar_t)translate[(var)];	MACRO_END


	if (pos < 0 || size1 < 0 || size2 < 0 || mstop < 0) return -4;
	if (mstop > size1 + size2)return -4;
	if (pos > mstop)return -4;
	
	if (pos <= size1)
    {
		text = string1 + pos;
		if (mstop <= size1)
		{
			partend = string1 + mstop;
			textend = partend;
		}
		else
		{
			partend = string1 + size1;
			textend = string2 + mstop - size1;
		}
		part_2_end = string2 + mstop - size1;
    }
	else
    {
		text = string2 + pos - size1;
		partend = string2 + mstop - size1;
		textend = partend;
		part_2_end = partend;
    }
	
	if (m_oCompiled.uses_registers && m_poRegs != NULL)
		for (a = 0; a < RE_NREGS; a++)
			regend_text[a] = NULL;
		
		code = m_oCompiled.buffer;
		translate = m_oCompiled.translate;
		failure_stack_start = failure_sp = initial_failure_stack;
		failure_stack_end = initial_failure_stack + INITIAL_FAILURES;
		
#if 0
		/* Search2 has already done this, and otherwise we get little benefit
		from this.  So I'll leave this out. */
		if (m_oCompiled.fastmap_accurate && !m_oCompiled.can_be_null &&
			text != textend &&
			!m_oCompiled.fastmap[translate ?
			(m4uchar_t)translate[(m4uchar_t)*text] :
		(m4uchar_t)*text])
			return -1;  /* it can't possibly match */
#endif
		
continue_matching:
		for (;;)
		{
			switch (*code++)
			{
			case Cend:
				if (partend != part_2_end)
					match_end = (int) (text - string1);
				else
					match_end = (int) (text - string2 + size1);
				if (m_poRegs)
				{
					m_poRegs->start[0] = pos;
					m_poRegs->end[0] = match_end;
					if (!m_oCompiled.uses_registers)
					{
						for (a = 1; a < RE_NREGS; a++)
						{
							m_poRegs->start[a] = -1;
							m_poRegs->end[a] = -1;
						}
					}
					else
					{
						for (a = 1; a < RE_NREGS; a++)
						{
							if (regend_text[a] == NULL)
							{
								m_poRegs->start[a] = -1;
								m_poRegs->end[a] = -1;
								continue;
							}
							if (regstart_partend[a] != part_2_end)
								m_poRegs->start[a] = (int) (regstart_text[a] - string1);
							else
								m_poRegs->start[a] = (int) (regstart_text[a] - string2 + size1);
							if (regend_partend[a] != part_2_end)
								m_poRegs->end[a] = (int) (regend_text[a] - string1);
							else
								m_poRegs->end[a] = (int) (regend_text[a] - string2 + size1);
						}
					}
				}
				if (failure_stack_start != initial_failure_stack)
					free((m4char_t *)failure_stack_start);
				return match_end - pos;
			case Cbol:
				if (text == string1 || text[-1] == '\n') /* text[-1] always valid */
					break;
				goto fail;
			case Ceol:
				if (text == string2 + size2 ||
					(text == string1 + size1 ?
					(size2 == 0 || *string2 == '\n') :
				*text == '\n'))
					break;
				goto fail;
			case Cset:
				NEXTCHAR(ch);
				if (code[ch/8] & (1<<(ch & 7)))
				{
					code += 256/8;
					break;
				}
				goto fail;
			case Cexact:
				NEXTCHAR(ch);
				if (ch != (m4uchar_t)*code++)
					goto fail;
				break;
			case Canychar:
				NEXTCHAR(ch);
				if (ch == '\n')
					goto fail;
				break;
			case Cstart_memory:
				reg = *code++;
				regstart_text[reg] = text;
				regstart_partend[reg] = partend;
				break;
			case Cend_memory:
				reg = *code++;
				regend_text[reg] = text;
				regend_partend[reg] = partend;
				break;
			case Cmatch_memory:
				reg = *code++;
				if (regend_text[reg] == NULL)
					goto fail;  /* or should we just match nothing? */
				regtext = regstart_text[reg];
				regtextend = regend_text[reg];
				if (regstart_partend[reg] == regend_partend[reg])
					regpartend = regtextend;
				else
					regpartend = string1 + size1;
				
				for (;regtext != regtextend;)
				{
					NEXTCHAR(ch);
					if (regtext == regpartend)
						regtext = string2;
					regch = (m4uchar_t)*regtext++;
					if (translate)
						regch = (m4uchar_t)translate[regch];
					if (regch != ch)
						goto fail;
				}
				break;
			case Cstar_jump:
			/* star is coded as:
			1: failure_jump 2
			... code for operand of star
			star_jump 1
			2: ... code after star
			We change the star_jump to update_failure_jump if we can determine
			that it is safe to do so; otherwise we change it to an ordinary
			jump.
			plus is coded as
			jump 2
			1: failure_jump 3
			2: ... code for operand of plus
			star_jump 1
			3: ... code after plus
			For star_jump considerations this is processed identically
				to star. */
				a = (m4uchar_t)*code++;
				a |= (m4uchar_t)*code++ << 8;
				a = (m4int_t)(short)a;
				{
					m4char_t map[256], can_be_null;
					m4char_t *p1, *p2;
					int iPos;

					p1 = code + a + 3; /* skip the failure_jump */
					assert(p1[-3] == Cfailure_jump);
					p2 = code;
					/* p1 points inside loop, p2 points to after loop */
					iPos = (int)(p2 - m_oCompiled.buffer);
					if (!DoCompileFastmap(m_oCompiled.buffer, m_oCompiled.used, iPos, &can_be_null, map))
						goto make_normal_jump;
						/* If we might introduce a new update point inside the loop,
						we can't optimize because then update_jump would update a
					wrong failure point.  Thus we have to be quite careful here. */
loop_p1:
					/* loop until we find something that consumes a character */
					switch (*p1++)
					{
					case Cbol:
					case Ceol:
					case Cbegbuf:
					case Cendbuf:
					case Cwordbeg:
					case Cwordend:
					case Cwordbound:
					case Cnotwordbound:
						goto loop_p1;
					case Cstart_memory:
					case Cend_memory:
						p1++;
						goto loop_p1;
					case Cexact:
						ch = (m4uchar_t)*p1++;
						if (map[ch])
							goto make_normal_jump;
						break;
					case Canychar:
						for (b = 0; b < 256; b++)
							if (b != '\n' && map[b])
								goto make_normal_jump;
							break;
					case Cset:
						for (b = 0; b < 256; b++)
							if ((p1[b >> 3] & (1 << (b & 7))) && map[b])
								goto make_normal_jump;
							p1 += 256/8;
							break;
					default:
						goto make_normal_jump;
					}
					/* now we know that we can't backtrack. */
					while (p1 != p2 - 3)
					{
						switch (*p1++)
						{
						case Cend:
							//abort();  /* we certainly shouldn't get this inside loop */
							return -3;
							/*NOTREACHED*/
						case Cbol:
						case Ceol:
						case Canychar:
						case Cbegbuf:
						case Cendbuf:
						case Cwordbeg:
						case Cwordend:
						case Cwordbound:
						case Cnotwordbound:
							break;
						case Cset:
							p1 += 256/8;
							break;
						case Cexact:
						case Cstart_memory:
						case Cend_memory:
						case Cmatch_memory:
						case Csyntaxspec:
						case Cnotsyntaxspec:
							p1++;
							break;
						case Cjump:
						case Cstar_jump:
						case Cfailure_jump:
						case Cupdate_failure_jump:
						case Cdummy_failure_jump:
							goto make_normal_jump;
						default:
							printf("regexpr.c: processing star_jump: unknown op %d\n", p1[-1]);
							break;
						}
					}
					goto make_update_jump;
				}
make_normal_jump:
				/* printf("changing to normal jump\n"); */
				code -= 3;
				*code = Cjump;
				break;
make_update_jump:
				/* printf("changing to update jump\n"); */
				code -= 2;
				a += 3;  /* jump to after the Cfailure_jump */
				code[-1] = Cupdate_failure_jump;
				code[0] = a & 0xff;
				code[1] = a >> 8;
				/* fall to next case */
					case Cupdate_failure_jump:
						failure_sp[-1].text = text;
						failure_sp[-1].partend = partend;
						/* fall to next case */
					case Cjump:
						a = (m4uchar_t)*code++;
						a |= (m4uchar_t)*code++ << 8;
						code += (m4int_t)(short)a;
						break;
					case Cdummy_failure_jump:
					case Cfailure_jump:
						if (failure_sp == failure_stack_end)
						{
							if (failure_stack_start != initial_failure_stack)
								goto error;
							failure_stack_start = (struct failure_point *)
								malloc(MAX_FAILURES * sizeof(*failure_stack_start));
							failure_stack_end = failure_stack_start + MAX_FAILURES;
							memcpy((m4char_t *)failure_stack_start, (m4char_t *)initial_failure_stack,
								INITIAL_FAILURES * sizeof(*failure_stack_start));
							failure_sp = failure_stack_start + INITIAL_FAILURES;
						}
						a = (m4uchar_t)*code++;
						a |= (m4uchar_t)*code++ << 8;
						a = (m4int_t)(short)a;
						if (code[-3] == Cdummy_failure_jump)
						{ /* this is only used in plus */
							assert(*code == Cfailure_jump);
							b = (m4uchar_t)code[1];
							b |= (m4uchar_t)code[2] << 8;
							failure_sp->code = code + (m4int_t)(short)b + 3;
							failure_sp->text = NULL;
							code += a;
						}
						else
						{
							failure_sp->code = code + a;
							failure_sp->text = text;
							failure_sp->partend = partend;
						}
						failure_sp++;
						break;
					case Cbegbuf:
						if (text == string1)
							break;
						goto fail;
					case Cendbuf:
						if (size2 == 0 ? text == string1 + size1 : text == string2 + size2)
							break;
						goto fail;
					case Cwordbeg:
						if (text == string2 + size2)
							goto fail;
						if (size2 == 0 && text == string1 + size1)
							goto fail;
						if (SYNTAX(text == string1 + size1 ? *string1 : *text) != Sword)
							goto fail;
						if (text == string1)
							break;
						if (SYNTAX(text[-1]) != Sword)
							break;
						goto fail;
					case Cwordend:
						if (text == string1)
							goto fail;
						if (SYNTAX(text[-1]) != Sword)
							goto fail;
						if (text == string2 + size2)
							break;
						if (size2 == 0 && text == string1 + size1)
							break;
						if (SYNTAX(*text) == Sword)
							goto fail;
						break;
					case Cwordbound:
					/* Note: as in gnu regexp, this also matches at the beginning
						and end of buffer. */
						if (text == string1 || text == string2 + size2 ||
							(size2 == 0 && text == string1 + size1))
							break;
						if ((SYNTAX(text[-1]) == Sword) ^
							(SYNTAX(text == string1 + size1 ? *string2 : *text) == Sword))
							break;
						goto fail;
					case Cnotwordbound:
					/* Note: as in gnu regexp, this never matches at the beginning
						and end of buffer. */
						if (text == string1 || text == string2 + size2 ||
							(size2 == 0 && text == string1 + size1))
							goto fail;
						if (!((SYNTAX(text[-1]) == Sword) ^
							(SYNTAX(text == string1 + size1 ? *string2 : *text) == Sword)))
							goto fail;
						break;
					case Csyntaxspec:
						NEXTCHAR(ch);
						if (SYNTAX(ch) != (m4uchar_t)*code++)
							goto fail;
						break;
					case Cnotsyntaxspec:
						NEXTCHAR(ch);
						if (SYNTAX(ch) != (m4uchar_t)*code++)
							break;
						goto fail;
					default:
						//abort();
						return -3;
						/*NOTREACHED*/
	}
    }
	//abort();
	return -3;
	/*NOTREACHED*/
	
fail:
	if (failure_sp != failure_stack_start)
    {
		failure_sp--;
		text = failure_sp->text;
		if (text == NULL)
			goto fail;
		partend = failure_sp->partend;
		code = failure_sp->code;
		goto continue_matching;
    }
	if (failure_stack_start != initial_failure_stack)
		free((m4char_t *)failure_stack_start);
	return -1;
	
error:
	if (failure_stack_start != initial_failure_stack)
		free((m4char_t *)failure_stack_start);
	return -2;
}

#undef PREFETCH
#undef NEXTCHAR
#undef PUSH_FAILURE

//-----------------------
//Match
//-----------------------
m4int_t ClRegExp::Match(m4char_t *string, m4int_t size, m4int_t pos)

{
	return Match2(string, size, (m4char_t *)NULL, 0, pos, size);
}

//-----------------------
//_Search2
//-----------------------
m4int_t ClRegExp::_Search2(m4char_t *string1, m4int_t size1, m4char_t *string2, m4int_t size2, m4int_t pos, m4int_t range, m4int_t mstop)
{
	m4char_t *fastmap, *translate, *text, *partstart, *partend;
	m4int_t dir, ret;
	m4char_t anchor;

	
	if (size1 < 0 || size2 < 0 || pos < 0 || mstop < 0)  return -4;
	if (pos + range < 0 || pos + range > size1 + size2)  return -4;
	if (pos > mstop) return -4;

	fastmap = m_oCompiled.fastmap;
	translate = m_oCompiled.translate;
	if (fastmap && !m_oCompiled.fastmap_accurate)
		CompileFastmap();
	anchor = m_oCompiled.anchor;
	if (m_oCompiled.can_be_null == 1) /* can_be_null == 2: can match null at eob */
		fastmap = NULL;
	if (range < 0)
    {
		dir = -1;
		range = -range;
    }
	else
		dir = 1;
	if (anchor == 2)
		if (pos != 0)
			return -1;
		else
			range = 0;
		for (; range >= 0; range--, pos += dir)
		{
			if (fastmap)
			{
				if (dir == 1)
				{ /* searching forwards */
					if (pos < size1)
					{
						text = string1 + pos;
						if (pos + range > size1)
							partend = string1 + size1;
						else
							partend = string1 + pos + range;
					}
					else
					{
						text = string2 + pos - size1;
						partend = string2 + pos + range - size1;
					}
					partstart = text;
					if (translate)
						while (text != partend &&
							!fastmap[(m4uchar_t)
							translate[(m4uchar_t)*text]])
							text++;
						else
							while (text != partend && !fastmap[(m4uchar_t)*text])
								text++;
							pos += (int) (text - partstart);
							range -= (int) (text - partstart);
							if (pos == size1 + size2 && m_oCompiled.can_be_null == 0)
								return -1;
				}
				else
				{ /* searching backwards */
					if (pos <= size1)
					{
						text = string1 + pos;
						partstart = string1 + pos - range;
					}
					else
					{
						text = string2 + pos - size1;
						if (range < pos - size1)
							partstart = string2 + pos - size1 - range;
						else
							partstart = string2;
					}
					partend = text;
					if (translate)
						while (text != partstart &&
							!fastmap[(m4uchar_t)
							translate[(m4uchar_t)*text]])
							text--;
						else
							while (text != partstart &&
								!fastmap[(m4uchar_t)*text])
								text--;
							pos -= (int) (partend - text);
							range -= (int) (partend - text);
				}
			}
			if (anchor == 1)
			{ /* anchored to begline */
				if (pos > 0 &&
					(pos <= size1 ? string1[pos - 1] :
				string2[pos - size1 - 1]) != '\n')
					continue;
			}
			if (pos < 0 || pos > size1 + size2) return -4;
			ret = _Match2(string1, size1, string2, size2, pos, mstop);
			if (ret >= 0)
				return pos;
			if (ret == -2)
				return -2;
		}
		return -1;
}
//-----------------------
//Search
//-----------------------
m4int_t ClRegExp::Search(m4char_t *string, m4int_t size, m4int_t startpos, m4int_t range)
{
	return Search2(string, size, (m4char_t *)NULL, 0,
		startpos, range, size);
}


//-----------------------
//Search
//-----------------------
m4int_t ClRegExp::Search2(m4char_t *string1, m4int_t size1, m4char_t *string2, m4int_t size2, m4int_t pos, m4int_t range, m4int_t mstop){

	if (!m_bIsSensitive){ 
		
		m4char_t *pszString1, *pszString2;
		
		pszString1=new m4char_t [size1+1];
		
		if (string2){
			pszString2=new m4char_t [size2+1];
		}
		else{
			pszString2=0;
		}
		
		strcpy (pszString1,string1);
		if (string2){
			strcpy (pszString2,string2);
		}
		
		// UNICODE LOCALE
		M4StrUpr(pszString1);
		if (string2){
			M4StrUpr(pszString2);
		}

		m4int_t ret=_Search2(pszString1, size1, pszString2, size2, pos, range, mstop);
		delete []pszString1;
		delete []pszString2;
		return ret;
	}
	return _Search2(string1, size1, string2, size2, pos, range, mstop);
	
}

//-----------------------
//Match2
//-----------------------
m4int_t ClRegExp::Match2(m4char_t *string1, m4int_t size1, m4char_t *string2, m4int_t size2, m4int_t pos, m4int_t mstop){

		if (!m_bIsSensitive){ 
		
		m4char_t *pszString1, *pszString2;
		
		pszString1=new m4char_t [size1+1];
		
		if (string2){
			pszString2=new m4char_t [size2+1];
		}
		else{
			pszString2=0;
		}
		
		strcpy (pszString1,string1);
		if (string2){
			strcpy (pszString2,string2);
		}
		
		// UNICODE LOCALE
		M4StrUpr(pszString1);
		if (string2){
			M4StrUpr (pszString2);
		}

		m4int_t ret=_Match2(pszString1, size1, pszString2, size2, pos, mstop);
		delete []pszString1;
		delete []pszString2;
		return ret;
	}
	return _Match2(string1, size1, string2, size2, pos, mstop);

}

//--------------------------------
//RegexUpr 
//--------------------------------
void ClRegExp::RegexUpr (char *pszRegex){

	//to make insensitive searches possible.

	while (*pszRegex)
	{
		if (*pszRegex=='\\' && (*(pszRegex+1)=='w' || *(pszRegex+1)=='W' || *(pszRegex+1)=='b' || *(pszRegex+1)=='B') )
		{
			pszRegex += 2;
		}
		else
		{
			// UNICODE LOCALE
			/* Bug 0126055
			Hay que convertir todos los caracteres a mayúsculas
			*/
			size_t	iBytesEaten = 0 ;
			M4ChrUpr(pszRegex, iBytesEaten);

			pszRegex += iBytesEaten;
		}
	}
}


