//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ttf2pfa.h
// Project:			 RFNBuilder
// Author:           Meta Software M.S. , S.A
// Date:			 20/01/2000
// Language:         C++
// Operating System: ALL
// Design Document:  
//
// Propietary:		Manuel Lazcano Perez
// Modifications:	Who && Date
//
// Definition: This module defines the conversion fron True Type Format
//		       files to PFA (Adobe Type 1) format files for Postscript.
//
//==============================================================================

#define TTF_UCHAR unsigned char
#define TTF_WORD unsigned short
#define TTF_SHORT signed short
#define TTF_UINT unsigned int

#define TTF_ONOFF	0x01
#define TTF_XSHORT	0x02
#define TTF_YSHORT	0x04
#define TTF_REPEAT	0x08
#define TTF_XSAME	0x10
#define TTF_YSAME	0x20

#define ARG_1_AND_2_ARE_WORDS		0x0001
#define ARGS_ARE_XY_VALUES 			0x0002
#define XY_BOUND_TO_GRID			0x0004
#define WE_HAVE_A_SCALE		 		0x0008
#define MORE_COMPONENTS				0x0020
#define WE_HAVE_AN_X_AND_Y_SCALE	0x0040
#define WE_HAVE_A_TWO_BY_TWO		0x0080
#define WE_HAVE_INSTRUCTIONS		0x0100
#define USE_MY_METRICS				0x0200

//======================================================================

//Este bloque define las macros ntohl y ntohs, que convierten enteros
// long y short del orden de los procesadores 68000 y de los ficheros 
// True Type al orden que utilice la plataforma. Para cambiar la 
// definicion segun la plataforma, basta incluir el #define _BIG_ENDIAN
// o no, para controlar el conjunto de definiciones que se aplicaran.

#if defined(_BIG_ENDIAN)
#define	ntohl(x)	(x)
#define	ntohs(x)	(x)
#else
#define ntohs(x) \
    ((TTF_WORD)((((TTF_WORD)(x) & 0x00ff) << 8) | \
                (((TTF_WORD)(x) & 0xff00) >> 8))) 
#define ntohl(x) \
    ((TTF_UINT)((((TTF_UINT)(x) & 0x000000ffU) << 24) | \
                (((TTF_UINT)(x) & 0x0000ff00U) <<  8) | \
                (((TTF_UINT)(x) & 0x00ff0000U) >>  8) | \
                (((TTF_UINT)(x) & 0xff000000U) >> 24)))  
#endif

//======================================================================

typedef struct StShort2 {
	TTF_SHORT	m_Upper;
	TTF_WORD	m_Lower;
} TTF_FIXED ;

typedef struct StLongHorMetric {
	TTF_WORD	m_AdvanceWidth;
	TTF_WORD	m_iLSB;
} LONGHORMETRIC;

typedef struct StTTF_HHea {
	TTF_UCHAR	m_Version[4];
	TTF_SHORT	m_Ascender,	m_Descender, m_LineGap;
	TTF_WORD	m_AdvanceWidthMax;
	TTF_SHORT	m_MinLSB, m_MinRSB, m_MaxExtent;
	TTF_SHORT	m_CaretSlopeRise, m_CaretSlopeRun;
	TTF_SHORT	m_Reserved[5];
	TTF_SHORT	m_MetricDataFormat;
	TTF_WORD	m_NumberOfHMetrics;
} TTF_HHEA;

typedef struct StDirEntryTTF {
	m4char_t	m_cTag[4];
	TTF_UINT	m_Checksum;
	TTF_UINT	m_Offset;
	TTF_UINT	m_Length;
} TTF_DIR_ENTRY ;

typedef struct StDirectoryTTF {
	TTF_UINT		m_SFntVersion;
	TTF_WORD		m_NumTables;
	TTF_WORD		m_SearchRange;
	TTF_WORD		m_EntrySelector;
	TTF_WORD		m_RangeShift;
	TTF_DIR_ENTRY	m_List;
} TTF_DIRECTORY ;

typedef struct StNameRecTTF {
	TTF_WORD	m_PlatformID;
	TTF_WORD	m_EncodingID;
	TTF_WORD	m_LanguageID;
	TTF_WORD	m_NameID;
	TTF_WORD	m_StringLength;
	TTF_WORD	m_StringOffset;
} TTF_NAME_REC;

typedef struct StNameTTF {
	TTF_WORD		m_Format;
	TTF_WORD		m_NumberOfNameRecords;
	TTF_WORD		m_Offset;
	TTF_NAME_REC	m_NameRecords;
} TTF_NAME ;

typedef struct StHeadTTF {
	TTF_UINT	m_Version;
	TTF_UINT	m_FontRevision;
	TTF_UINT	m_ChecksumAdjust;
	TTF_UINT	m_MagicNo;
	TTF_WORD	m_Flags;
	TTF_WORD	m_UnitsPerEm;
	TTF_UCHAR	m_Created[8];
	TTF_UCHAR	m_Modified[8];
	TTF_WORD	m_iMinX, m_iMinY, m_iMaxX, m_iMaxY;
	TTF_WORD	m_MacStyle, m_LowestRecPPEM;
	TTF_SHORT	m_FontDirection, m_IndexToLocFormat, m_GlyphDataFormat;
} TTF_HEAD ;

typedef struct StKernTTF {
	TTF_WORD	m_Version;
	TTF_WORD	m_nTables;
} TTF_KERN ;

typedef struct StKernSubTTF {
	TTF_WORD	m_Version, m_Length, m_Coverage;
	TTF_WORD	m_nPairs, m_SearchRange, m_EntrySelector, m_RangeShift;
} TTF_KERN_SUB;

typedef struct StKernEntryTTF {
	TTF_WORD	m_Left, m_Right;
	TTF_WORD	m_Value;
} TTF_KERN_ENTRY;

typedef struct StCMapFmt0TTF {
	TTF_WORD	m_Format;
	TTF_WORD	m_Length;
	TTF_WORD	m_Version;
	TTF_UCHAR	m_GlyphIdArray[256];
} TTF_CMAP_FMT0;

typedef struct StCMapFmt4TTF {
	TTF_WORD	m_Format;
	TTF_WORD	m_Length;
	TTF_WORD	m_Version;
	TTF_WORD	m_SegCountX2;
	TTF_WORD	m_SearchRange;
	TTF_WORD	m_EntrySelector;
	TTF_WORD	m_RangeShift;
} TTF_CMAP_FMT4;

typedef struct StCMapEntryTTF {
	TTF_WORD	m_PlatformID;
	TTF_WORD	m_EncodingID;
	TTF_UINT	m_Offset;
} TTF_CMAP_ENTRY;

typedef struct StCMapTTF {
	TTF_WORD		m_Version;
	TTF_WORD		m_NumberOfEncodingTables;
	TTF_CMAP_ENTRY	m_EncodingTable[1];
} TTF_CMAP ;

typedef struct StGlyfTTF {
	TTF_SHORT	m_NumberOfContours;
	TTF_WORD	m_iMinX, m_iMinY, m_iMaxX, m_iMaxY;
} TTF_GLYF ;

typedef struct StMaxPTTF {
	TTF_UINT	m_Version;
	TTF_WORD	m_NumGlyphs, m_MaxPoints, m_MaxContours;
	TTF_WORD	m_MaxCompositePoints, m_MaxCompositeContours;
	TTF_WORD	m_MaxZones, m_MaxTwilightPoints, m_MaxStorage;
	TTF_WORD	m_MaxFunctionDefs, m_MaxInstructionsDefs;
	TTF_WORD	m_MaxSizeOfInstructions, m_MaxComponentElements;
	TTF_WORD	m_MaxComponentDepth;
} TTF_MAXP ;

typedef struct StPostHeadTTF {
	TTF_UINT	m_FormatType;
	TTF_FIXED		m_ItalicAngle;
	TTF_WORD	m_UnderlinePosition;
	TTF_WORD	m_UnderlineThickness;
	TTF_UINT	m_IsFixedPitch;
	TTF_UINT	m_MinMemType42;
	TTF_UINT	m_MaxMemType42;
	TTF_UINT	m_MinMemType1;
	TTF_UINT	m_MaxMemType1;
	TTF_WORD	m_NumGlyphs;
	TTF_WORD	m_GlyphNameIndex;
} TTF_POST_HEAD ;


typedef struct StGlyph {
	m4int_t  m_iCharNo; /* Encoding of glyph */
    m4int_t  m_iUnicode; /* Unicode value of glyph */
	m4int_t  m_iMinX, m_iMinY, m_iMaxX, m_iMaxY;
    m4int_t  m_iWidth, m_iLSB;
    m4char_t *m_pcName;  /* Postscript name of glyph */
} Glyph;


typedef struct StAuxParams {
	
	m4int16_t m_piEncoding[256]; /* inverse of glyph[].m_iCharNo */
	m4int_t   m_iFileTTF;
	m4int_t	  m_iNumGlyphs;
	m4int_t   m_iLongOffsets;
	m4int_t   m_iNCurves;
	m4int16_t m_iCMapNSegs;
	m4int16_t *m_piCMapDeltaID;
	m4int16_t *m_piCMapRangeOffsetID;
	m4int16_t m_iPsFmt3;
	m4int16_t m_iUnicode;
	m4float_t m_fScaleFactor;

	m4char_t m_pcUnknownGlyph[4];
	m4char_t m_pcNameBuffer[2000];
	m4char_t *m_pcFileBuffer;
	m4char_t *m_pcNameFields[8];

	Glyph *m_pGlyphList;	
	FILE *m_pFilePFA, *m_pFileAFM;
	
	TTF_DIRECTORY *m_DirTTF;
	TTF_DIR_ENTRY *m_DirEntry ;	
	TTF_NAME	  *m_NameTable;
	TTF_NAME_REC  *m_NameRecord;
	TTF_HEAD	  *m_HeadTable;
	TTF_HHEA	  *m_HHeaTable;
	TTF_KERN	  *m_KernTable;
	TTF_CMAP	  *m_CMapTable;
	LONGHORMETRIC *m_HMTXTable;
	TTF_GLYF	  *m_GlyphTable;
	TTF_MAXP	  *m_MaxPTable;
	TTF_POST_HEAD *m_PostTable;
	TTF_UCHAR	  *m_GlyphStart;
	TTF_WORD	  *m_ShortLocalTable;
	TTF_UINT	  *m_LongLocalTable;
	TTF_WORD	  *m_CMapSegStart;
	TTF_WORD	  *m_CMapSegEnd;

} StAuxParamsTTF;

static char *ISOLatin1Encoding[256] = {
    ".null",          ".notdef",        ".notdef",        ".notdef",
    ".notdef",        ".notdef",        ".notdef",        ".notdef",
    ".notdef",        ".notdef",        ".notdef",        ".notdef",
    ".notdef",        "CR",             ".notdef",        ".notdef",
    ".notdef",        ".notdef",        ".notdef",        ".notdef",
    ".notdef",        ".notdef",        ".notdef",        ".notdef",
    ".notdef",        ".notdef",        ".notdef",        ".notdef",
    ".notdef",        ".notdef",        ".notdef",        ".notdef",
    "space",          "exclam",         "quotedbl",       "numbersign",
    "dollar",         "percent",        "ampersand",      "quoteright",
    "parenleft",      "parenright",     "asterisk",       "plus",
    "comma",          "hyphen",         "period",         "slash",
    "zero",           "one",            "two",            "three",
    "four",           "five",           "six",            "seven",
    "eight",          "nine",           "colon",          "semicolon",
    "less",           "equal",          "greater",        "question",
    "at",             "A",              "B",              "C",
    "D",              "E",              "F",              "G",
    "H",              "I",              "J",              "K",
    "L",              "M",              "N",              "O",
    "P",              "Q",              "R",              "S",
    "T",              "U",              "V",              "W",
    "X",              "Y",              "Z",              "bracketleft",
    "backslash",      "bracketright",   "asciicircum",    "underscore",
    "grave",          "a",              "b",              "c",
    "d",              "e",              "f",              "g",
    "h",              "i",              "j",              "k",
    "l",              "m",              "n",              "o",
    "p",              "q",              "r",              "s",
    "t",              "u",              "v",              "w",
    "x",              "y",              "z",              "braceleft",
    "bar",            "braceright",     "asciitilde",     ".notdef",
    ".notdef",        ".notdef",        "quotesinglbase", "florin",
    "quotedblbase",   "ellipsis",       "dagger",         "daggerdbl",
    "circumflex",     "perthousand",    "Scaron",         "guilsinglleft",
    "OE",             ".notdef",        ".notdef",        ".notdef",
    ".notdef",        "quoteleft",      "quoteright",     "quotedblleft",
    "quotedblright",  "bullet",         "endash",         "emdash",
    "tilde",          "trademark",      "scaron",         "guilsinglright",
    "oe",             ".notdef",        ".notdef",        "Ydieresis",
    "nbspace",        "exclamdown",     "cent",           "sterling",
    "currency",       "yen",            "brokenbar",      "section",
    "dieresis",       "copyright",      "ordfeminine",    "guillemotleft",
    "logicalnot",     "sfthyphen",      "registered",     "macron",
    "degree",         "plusminus",      "twosuperior",    "threesuperior",
    "acute",          "mu",             "paragraph",      "periodcentered",
    "cedilla",        "onesuperior",    "ordmasculine",   "guillemotright",
    "onequarter",     "onehalf",        "threequarters",  "questiondown",
    "Agrave",         "Aacute",         "Acircumflex",    "Atilde",
    "Adieresis",      "Aring",          "AE",             "Ccedilla",
    "Egrave",         "Eacute",         "Ecircumflex",    "Edieresis",
    "Igrave",         "Iacute",         "Icircumflex",    "Idieresis",
    "Eth",            "Ntilde",         "Ograve",         "Oacute",
    "Ocircumflex",    "Otilde",         "Odieresis",      "multiply",
    "Oslash",         "Ugrave",         "Uacute",         "Ucircumflex",
    "Udieresis",      "Yacute",         "Thorn",          "germandbls",
    "agrave",         "aacute",         "acircumflex",    "atilde",
    "adieresis",      "aring",          "ae",             "ccedilla",
    "egrave",         "eacute",         "ecircumflex",    "edieresis",
    "igrave",         "iacute",         "icircumflex",    "idieresis",
    "eth",            "ntilde",         "ograve",         "oacute",
    "ocircumflex",    "otilde",         "odieresis",      "divide",
    "oslash",         "ugrave",         "uacute",         "ucircumflex",
    "udieresis",      "yacute",         "thorn",          "ydieresis"
};

static char *adobe_StandardEncoding[256] = {
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    "space","exclam","quotedbl","numbersign",
    "dollar","percent","ampersand","quoteright",
    "parenleft","parenright","asterisk","plus",
    "comma","hyphen","period","slash",
    "zero","one","two","three",
    "four","five","six","seven",
    "eight","nine","colon","semicolon",
    "less","equal","greater","question",
    "at","A","B","C","D","E","F","G",
    "H","I","J","K","L","M","N","O",
    "P","Q","R","S","T","U","V","W",
    "X","Y","Z","bracketleft",
    "backslash","bracketright","asciicircum","underscore",
    "grave","a","b","c","d","e","f","g",
    "h","i","j","k","l","m","n","o",
    "p","q","r","s","t","u","v","w",
    "x","y","z","braceleft",
    "bar","braceright","asciitilde",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef","exclamdown","cent","sterling",
    "fraction","yen","florin","section",
    "currency","quotesingle","quotedblleft","guillemotleft",
    "guilsinglleft","guilsinglright","fi","fl",
    ".notdef","endash","dagger","daggerdbl",
    "periodcentered",".notdef","paragraph","bullet",
    "quotesinglbase","quotedblbase","quotedblright","guillemotright",
    "ellipsis","perthousand",".notdef","questiondown",
    ".notdef","grave","acute","circumflex",
    "tilde","macron","breve","dotaccent",
    "dieresis",".notdef","ring","cedilla",
    ".notdef","hungarumlaut","ogonek","caron",
    "emdash",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef","AE",".notdef","ordfeminine",
    ".notdef",".notdef",".notdef",".notdef",
    "Lslash","Oslash","OE","ordmasculine",
    ".notdef",".notdef",".notdef",".notdef",
    ".notdef","ae",".notdef",".notdef",
    ".notdef","dotlessi",".notdef",".notdef",
    "lslash","oslash","oe","germandbls",
    ".notdef",".notdef",".notdef",".notdef"
};

static char *mac_glyph_names[258] = {
    ".notdef",         ".null",          "CR",
    "space",          "exclam",         "quotedbl",       "numbersign",
    "dollar",         "percent",        "ampersand",      "quotesingle",
    "parenleft",      "parenright",     "asterisk",       "plus",
    "comma",          "hyphen",         "period",         "slash",
    "zero",           "one",            "two",            "three",
    "four",           "five",           "six",            "seven",
    "eight",          "nine",           "colon",          "semicolon",
    "less",           "equal",          "greater",        "question",
    "at",             "A",              "B",              "C",
    "D",              "E",              "F",              "G",
    "H",              "I",              "J",              "K",
    "L",              "M",              "N",              "O",
    "P",              "Q",              "R",              "S",
    "T",              "U",              "V",              "W",
    "X",              "Y",              "Z",              "bracketleft",
    "backslash",      "bracketright",   "asciicircum",    "underscore",
    "grave",          "a",              "b",              "c",
    "d",              "e",              "f",              "g",
    "h",              "i",              "j",              "k",
    "l",              "m",              "n",              "o",
    "p",              "q",              "r",              "s",
    "t",              "u",              "v",              "w",
    "x",              "y",              "z",              "braceleft",
    "bar",            "braceright",     "asciitilde",     "Adieresis",
    "Aring",          "Ccedilla",       "Eacute",         "Ntilde",
    "Odieresis",      "Udieresis",      "aacute",         "agrave",
    "acircumflex",    "adieresis",      "atilde",         "aring",
    "ccedilla",       "eacute",         "egrave",         "ecircumflex",
    "edieresis",      "iacute",         "igrave",         "icircumflex",
    "idieresis",      "ntilde",         "oacute",         "ograve",
    "ocircumflex",    "odieresis",      "otilde",         "uacute",
    "ugrave",         "ucircumflex",    "udieresis",      "dagger",
    "degree",         "cent",           "sterling",       "section",
    "bullet",         "paragraph",      "germandbls",     "registered",
    "copyright",      "trademark",      "acute",          "dieresis",
    "notequal",       "AE",             "Oslash",         "infinity",
    "plusminus",      "lessequal",      "greaterequal",   "yen",
    "mu",             "partialdiff",    "summation",      "product",
    "pi",             "integral",       "ordfeminine",    "ordmasculine",
    "Omega",          "ae",             "oslash",         "questiondown",
    "exclamdown",     "logicalnot",     "radical",        "florin",
    "approxequal",    "increment",      "guillemotleft",  "guillemotright",
    "ellipsis",       "nbspace",        "Agrave",         "Atilde",
    "Otilde",         "OE",             "oe",             "endash",
    "emdash",         "quotedblleft",   "quotedblright",  "quoteleft",
    "quoteright",     "divide",         "lozenge",        "ydieresis",
    "Ydieresis",      "fraction",       "currency",       "guilsinglleft",
    "guilsinglright", "fi",             "fl",             "daggerdbl",
    "periodcentered", "quotesinglbase", "quotedblbase",   "perthousand",
    "Acircumflex",    "Ecircumflex",    "Aacute",         "Edieresis",
    "Egrave",         "Iacute",         "Icircumflex",    "Idieresis",
    "Igrave",         "Oacute",         "Ocircumflex",    "applelogo",
    "Ograve",         "Uacute",         "Ucircumflex",    "Ugrave",
    "dotlessi",       "circumflex",     "tilde",          "macron",
    "breve",          "dotaccent",      "ring",           "cedilla",
    "hungarumlaut",   "ogonek",         "caron",          "Lslash",
    "lslash",         "Scaron",         "scaron",         "Zcaron",
    "zcaron",         "brokenbar",      "Eth",            "eth",
    "Yacute",         "yacute",         "Thorn",          "thorn",
    "minus",          "multiply",       "onesuperior",    "twosuperior",
    "threesuperior",  "onehalf",        "onequarter",     "threequarters",
    "franc",          "Gbreve",         "gbreve",         "Idot",
    "Scedilla",       "scedilla",       "Cacute",         "cacute",
    "Ccaron",         "ccaron",         "dmacron"
};

//======================================================================

m4bool_t HandleName(StAuxParamsTTF *ao_pStAuxParams);
m4bool_t HandleCMap (StAuxParamsTTF* ao_pStAuxParams);
m4bool_t HandleHead (StAuxParamsTTF* ao_pStAuxParams);
m4bool_t HandlePost(StAuxParamsTTF* ao_pStAuxParams);
void HandleHMTX(StAuxParamsTTF* ao_pStAuxParams);
void HandleKern(StAuxParamsTTF* ao_pStAuxParams);

void DrawGlyf(m4int_t ai_iGlyphNo, m4int_t ai_iParent, StAuxParamsTTF* ao_pStAuxParams);
void DoGlyf(m4int_t ai_iGlyphNo, StAuxParamsTTF* ao_pStAuxParams);

m4int_t UnicodeToWin31 (m4int_t ai_iUnival);
m4float_t f2dot14 (m4int16_t ai_iX);
