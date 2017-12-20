// clsLN4.cpp: implementation of the clsLN4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "clsln4.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Definicion de colores
#define LN4_TEXT_CODE		"\\cf1 "
#define LN4_REMARK_CODE		"\\cf2 "
#define LN4_KEYWORD_CODE	"\\cf3 "
#define LN4_FUNCTION_CODE	"\\cf4 "
#define LN4_CONSTANT_CODE	"\\cf5 "
#define LN4_OPERATOR_CODE	"\\cf6 "
#define LN4_NUMERIC_CODE	"\\cf7 "
#define LN4_STRING_CODE		"\\cf8 "
//#define LN4_CONCEPT_CODE	"\\cf9 "
#define LN4_NORMALBACK_CODE "\\highlight00 "
#define LN4_BREAKPOINT_CODE	"\\highlight09 "
#define LN4_ACTIVELINE_CODE	"\\highlight10 "
#define LN4_CALLERLINE_CODE "\\highlight11 "

#define LN4_UNDEFINED	0
#define LN4_TEXT		1
#define LN4_REMARK		2
#define LN4_KEYWORD		3
#define LN4_FUNCTION	4
#define LN4_CONSTANT	5
#define LN4_OPERATOR	6
#define LN4_NUMERIC		7
#define LN4_STRING		8
#define LN4_CONCEPT		9
#define LN4_NORMALBACK	10
#define LN4_BREAKPOINT	11
#define LN4_ACTIVELINE	12
#define LN4_CALLERLINE	13

#define LN4_FORECODE_LENGTH 5
#define LN4_BACKCODE_LENGTH 13

#define CALLER_SELECTION_MARK 6

const int iNumColors = 11;

const long patColor[iNumColors] = {
		RGB(0,0,0),			// LN4_TEXT_COLOR
		RGB(0,128,0),		// LN4_REMARK_COLOR
		RGB(0,0,255),		// LN4_KEYWORD_COLOR
		RGB(0,0,128),		// LN4_FUNCTION_COLOR
		RGB(128,128,0),		// LN4_CONSTANT_COLOR
		RGB(128,0,0),		// LN4_OPERATOR_COLOR
		RGB(255,128,128),	// LN4_NUMERIC_COLOR
		RGB(0,128,128),		// LN4_STRING_COLOR
//		RGB(128,128,128),	// LN4_CONCEPT_COLOR
		RGB(255,192,192),	// LN4_BREAKPOINT
		RGB(255,255,0),		// LN4_ACTIVE_LINE
		RGB(0, 255, 0)		// LN4_CALLERLINE
};

const int patKind[iNumColors] = {
		0,	// Text
		0,	// Text
		0,	// Text
		0,	// Text
		0,	// Text
		0,	// Text
		0,	// Text
		0,  // Text
//		0,	// Text
		1,	// Mark
		-1,	// Highlight
		6	// Mark
};

const int iNumKeyWordsLN4 = 20;

CString cKeyWordsLN4[iNumKeyWordsLN4] = {
		"Condition",
		"Do",
		"Else",
		"Elseif",
		"End",
		"Endif",
		"FinFor",
		"For",
		"If",
		"Next",
		"Purge",
		"Return",
		"StoreFor",
		"Target",
		"Then",
		"To",
		"Until",
		"Wend",
		"While",
		"WriteLine"
};

const int iNumOperatorsLN4 = 3;

CString cOperatorsLN4[iNumOperatorsLN4] = {
		"And",
		"Not",
		"Or"
};

const int iNumConstantsLN4 = 338;

CString cConstantsLN4[iNumConstantsLN4] = {
		"ABORT",
		"ASCENDING",
		"ATT_SYS_ADD_NEW_SLICE",
		"ATT_SYS_AUX_ITEM_ID",
		"ATT_SYS_BLOB_DESCRIPTION",
		"ATT_SYS_BLOB_MASK",
		"ATT_SYS_CREATE_BLOB_FILE",
		"ATT_SYS_CREATE_SLICE",
		"ATT_SYS_END_DATE",
		"ATT_SYS_FIRST_SLICE",
		"ATT_SYS_LAST_SLICE",
		"ATT_SYS_OLD_VALUE",
		"ATT_SYS_SET_BLOB_DESCRIPTION",
		"ATT_SYS_SLICE_NUMBER",
		"ATT_SYS_SPLIT_SLICE",
		"ATT_SYS_START_DATE",
		"ATT_SYS_TOTALIZE_ITEMS",
		"ATT_SYS_TOTALIZE_SLICES",
		"AUTOLOAD_BLOCK",
		"AUTOLOAD_NODESAYS",
		"AUTOLOAD_OFF",
		"AUTOLOAD_PRG",
		"BACK",
		"BEGIN",
		"BTN_ABORT_RETRY_IGNORE",
		"BTN_OK",
		"BTN_OK_CANCEL",
		"BTN_RETRY_CANCEL",
		"BTN_YES_NO",
		"BTN_YES_NO_CANCEL",
		"CANCEL",
		"CASE_DISTINCT",
		"CASE_DISTINCT_OR_NULL",
		"CASE_EQUAL",
		"CASE_EQUAL_OR_NULL",
		"CASE_GREATER",
		"CASE_GREATER_OR_EQUAL",
		"CASE_GREATER_OR_EQUAL_OR_NULL",
		"CASE_GREATER_OR_NULL",
		"CASE_REGULAR_EXPRESSION",
		"CASE_REGULAR_EXPRESSION_OR_NULL",
		"CASE_SMALLER",
		"CASE_SMALLER_OR_EQUAL",
		"CASE_SMALLER_OR_EQUAL_OR_NULL",
		"CASE_SMALLER_OR_NULL",
		"COMMIT",
		"CR",
		"DEBUGINFOLOG",
		"DESCENDING",
		"DISTINCT",
		"DISTINCT_OR_NULL",
		"DOUBLE_QUOTE",
		"END",
		"EQUAL",
		"EQUAL_OR_NULL",
		"ERRORLOG",
		"EXECUTE_POSTVALIDATION",
		"FALSE",
		"GREATER",
		"GREATER_OR_EQUAL",
		"GREATER_OR_EQUAL_OR_NULL",
		"GREATER_OR_NULL",
		"IGNORE",
		"INSTANCE_GLOBAL_SHARED",
		"INSTANCE_LOCAL_SHARED",
		"INSTANCE_NOT_SHARED",
		"LONG_DATE_FORMAT",
		"LOWERCASE",
		"M4_ABORT",
		"M4_ASCENDING",
		"M4_ATT_SYS_ADD_NEW_SLICE",
		"M4_ATT_SYS_AUX_ITEM_ID",
		"M4_ATT_SYS_BLOB_DESCRIPTION",
		"M4_ATT_SYS_BLOB_MASK",
		"M4_ATT_SYS_CREATE_BLOB_FILE",
		"M4_ATT_SYS_CREATE_SLICE",
		"M4_ATT_SYS_END_DATE",
		"M4_ATT_SYS_FIRST_SLICE",
		"M4_ATT_SYS_LAST_SLICE",
		"M4_ATT_SYS_OLD_VALUE",
		"M4_ATT_SYS_SET_BLOB_DESCRIPTION",
		"M4_ATT_SYS_SLICE_NUMBER",
		"M4_ATT_SYS_SPLIT_SLICE",
		"M4_ATT_SYS_START_DATE",
		"M4_ATT_SYS_TOTALIZE_ITEMS",
		"M4_ATT_SYS_TOTALIZE_SLICES",
		"M4_AUTOLOAD_BLOCK",
		"M4_AUTOLOAD_NODESAYS",
		"M4_AUTOLOAD_OFF",
		"M4_AUTOLOAD_PRG",
		"M4_BACK",
		"M4_BEGIN",
		"M4_BOOKMARK_PK",
		"M4_BOOKMARK_POSITION",
		"M4_BTN_ABORT_RETRY_IGNORE",
		"M4_BTN_OK",
		"M4_BTN_OK_CANCEL",
		"M4_BTN_RETRY_CANCEL",
		"M4_BTN_YES_NO",
		"M4_BTN_YES_NO_CANCEL",
		"M4_CANCEL",
		"M4_CASE_DISTINCT",
		"M4_CASE_DISTINCT_OR_NULL",
		"M4_CASE_EQUAL",
		"M4_CASE_EQUAL_OR_NULL",
		"M4_CASE_GREATER",
		"M4_CASE_GREATER_OR_EQUAL",
		"M4_CASE_GREATER_OR_EQUAL_OR_NULL",
		"M4_CASE_GREATER_OR_NULL",
		"M4_CASE_REGULAR_EXPRESSION",
		"M4_CASE_REGULAR_EXPRESSION_OR_NULL",
		"M4_CASE_SMALLER",
		"M4_CASE_SMALLER_OR_EQUAL",
		"M4_CASE_SMALLER_OR_EQUAL_OR_NULL",
		"M4_CASE_SMALLER_OR_NULL",
		"M4_COMMIT",
		"M4_COMPLETE_WEEK",
		"M4_CR",
		"M4_CSTYPE_BACK",
		"M4_CSTYPE_BOTH",
		"M4_CSTYPE_FRONT",
		"M4_CSTYPE_MAPPED",
		"M4_CSTYPE_REAL_BOTH",
		"M4_DAY",
		"M4_DEBUGINFOLOG",
		"M4_DEFAULT_CS_TIMEOUT",
		"M4_DESCENDING",
		"M4_DISTINCT",
		"M4_DISTINCT_OR_NULL",
		"M4_DOUBLE_QUOTE",
		"M4_ENABLE_TIMEOUT_WINDOW",
		"M4_ENCODING_ANSI", 
		"M4_ENCODING_NATIVE",
		"M4_ENCODING_UTF8",
		"M4_END",
		"M4_EQUAL",
		"M4_EQUAL_OR_NULL",
		"M4_ERROR",
		"M4_ERRORLOG",
		"M4_EXECUTE_OPERATIONS",
		"M4_EXECUTE_POSTVALIDATION",
		"M4_FALSE",
		"M4_GENERIC_CS_TIMEOUT",
		"M4_GREATER",
		"M4_GREATER_OR_EQUAL",
		"M4_GREATER_OR_EQUAL_OR_NULL",
		"M4_GREATER_OR_NULL",
		"M4_HOUR",
		"M4_IGNORE",
		"M4_INSTANCE_GLOBAL_SHARED",
		"M4_INSTANCE_LOCAL_SHARED",
		"M4_INSTANCE_NOT_SHARED",
		"M4_ITEM_AUXILIAR_ITEM",
		"M4_ITEM_DMD",
		"M4_ITEM_DMD_COMPONENT",
		"M4_ITEM_DMD_CROSSTAB",
		"M4_ITEM_DMD_FIELD",
		"M4_ITEM_ID",
		"M4_ITEM_INDEX",
		"M4_ITEM_INTERNAL_TYPE",
		"M4_ITEM_IS_PK",
		"M4_ITEM_M4TYPE",
		"M4_ITEM_NAME",
		"M4_ITEM_NOT_NULL",
		"M4_ITEM_NUMBER",
		"M4_ITEM_ORDER",
		"M4_ITEM_PRECISION",
		"M4_ITEM_READ_FIELD_ID",
		"M4_ITEM_READ_OBJECT_ALIAS",
		"M4_ITEM_READ_OBJECT_ID",
		"M4_ITEM_RULE_AUTOGENERATED",
		"M4_ITEM_RULE_CODE",
		"M4_ITEM_RULE_ID",
		"M4_ITEM_RULE_END_DATE",
		"M4_ITEM_RULE_NUMBER",
		"M4_ITEM_RULE_START_DATE",
		"M4_ITEM_SCALE",
		"M4_ITEM_SCOPE",
		"M4_ITEM_SLICE_TOTALIZE",
		"M4_ITEM_SYNONYM",
		"M4_ITEM_TYPE",
		"M4_ITEM_WRITE_FIELD_ID",
		"M4_ITEM_WRITE_OBJECT_ALIAS",
		"M4_ITEM_WRITE_OBJECT_ID",
		"M4_KEEP_ONE_RECORD",
		"M4_KEEP_RECORDS",
		"M4_KEEP_SLICES",
		"M4_LANG_DEVELOPMENT",
		"M4_LANG_LOGON",
		"M4_LONG_DATE_FORMAT",
		"M4_LOWERCASE",
		"M4_M4OBJECT_CREATION_TYPE",
		"M4_M4OBJECT_ENVIRONMENT_VARS",
		"M4_M4OBJECT_ID",
		"M4_M4OBJECT_INSTANCE_HANDLE",
		"M4_M4OBJECT_INSTANCE_NUMBER",
		"M4_M4OBJECT_NAME",
		"M4_M4OBJECT_NODE_NUMBER",
		"M4_MATH_E",
		"M4_MATH_PI",
		"M4_MINUS_INF",
		"M4_MINUTE",
		"M4_MONTH",
		"M4_NEGATIVE",
		"M4_NEW_LINE",
		"M4_NEXT",
		"M4_NEXT_CS_TIMEOUT",
		"M4_NO",
		"M4_NODE_ID",
		"M4_NODE_INDEX",
		"M4_NODE_IS_REAL_ROOT",
		"M4_NODE_IS_ROOT",
		"M4_NODE_MAX_NUM_ROWS_DB",
		"M4_NODE_NAME",
		"M4_NODE_PARENT",
		"M4_NODE_READ_OBJECT_ID",
		"M4_NODE_SENTENCE",
		"M4_NODE_TYPE",
		"M4_NODE_WRITE_OBJECT_ID",
		"M4_OK",
		"M4_OPEN_MODE_AUTOLOAD_BLOCK",
		"M4_OPEN_MODE_AUTOLOAD_NODESAYS",
		"M4_OPEN_MODE_AUTOLOAD_PRG",
		"M4_OPEN_MODE_LOAD",
		"M4_OPEN_MODE_NO_LOAD",
		"M4_OP_DELETED",
		"M4_OP_INSERTED",
		"M4_OP_MODIFIED",
		"M4_OP_UPDATED",
		"M4_ORGANIZATION_L2_TYPE_ANY",
		"M4_ORGANIZATION_L2_TYPE_FATHER",
		"M4_ORGANIZATION_L2_TYPE_MONO",
		"M4_ORGANIZATION_L2_TYPE_MULTI",
		"M4_ORGANIZATION_L2_TYPE_NONE",
		"M4_PLUS_INF",
		"M4_POSITIVE",
		"M4_REGULAR_EXPRESSION",
		"M4_REGULAR_EXPRESSION_OR_NULL",
		"M4_RESET_NEXT_TIMEOUT",
		"M4_RETRY",
		"M4_ROLLBACK",
		"M4_ROLLBACK_RESUME",
		"M4_SCOPE_ALL",
		"M4_SCOPE_BLOCK",
		"M4_SCOPE_NODE",
		"M4_SCOPE_REGISTER",
		"M4_SECOND",
		"M4_SEND_BRANCH",
		"M4_SEND_NO_BRANCH",
		"M4_SEND_RESET",
		"M4_SHORT_DATE_FORMAT",
		"M4_SMALLER",
		"M4_SMALLER_OR_EQUAL",
		"M4_SMALLER_OR_EQUAL_OR_NULL",
		"M4_SMALLER_OR_NULL",
		"M4_SUCCESS",
		"M4_TAB",
		"M4_TOTAL_AVG",
		"M4_TOTAL_COUNT",
		"M4_TOTAL_FIRST",
		"M4_TOTAL_LAST",
		"M4_TOTAL_MAX",
		"M4_TOTAL_MIN",
		"M4_TOTAL_SUM",
		"M4_TRIM_ALL",
		"M4_TRIM_LEFT",
		"M4_TRIM_RIGHT",
		"M4_TRUE",
		"M4_TYPE_ALL",
		"M4_TYPE_CONCEPT",
		"M4_TYPE_FIELD",
		"M4_TYPE_METHOD",
		"M4_TYPE_PROPERTY",
		"M4_UNCHANGED",
		"M4_UPPERCASE",
		"M4_VARIANT_TYPE_DATE",
		"M4_VARIANT_TYPE_NULL",
		"M4_VARIANT_TYPE_NUMBER",
		"M4_VARIANT_TYPE_STRING",
		"M4_WARNINGLOG",
		"M4_WEEK",
		//"M4_WEEKDAY",
		"M4_YEAR",
		"M4_YES",
		"M4_ZERO",
		"MINUS_INF",
		"NEGATIVE",
		"NEW_LINE",
		"NEXT",
		"NO",
		"OK",
		"OP_DELETED",
		"OP_INSERTED",
		"OP_MODIFIED",
		"OP_UPDATED",
		"PLUS_INF",
		"POSITIVE",
		"REGULAR_EXPRESSION",
		"REGULAR_EXPRESSION_OR_NULL",
		"RETRY",
		"ROLLBACK",
		"SCOPE_ALL",
		"SCOPE_BLOCK",
		"SCOPE_NODE",
		"SCOPE_REGISTER",
		"SEND_BRANCH",
		"SEND_NO_BRANCH",
		"SEND_RESET",
		"SHORT_DATE_FORMAT",
		"SMALLER",
		"SMALLER_OR_EQUAL",
		"SMALLER_OR_EQUAL_OR_NULL",
		"SMALLER_OR_NULL",
		"TAB",
		"TOTAL_AVG",
		"TOTAL_COUNT",
		"TOTAL_FIRST",
		"TOTAL_LAST",
		"TOTAL_MAX",
		"TOTAL_MIN",
		"TOTAL_SUM",
		"TRIM_ALL",
		"TRIM_LEFT",
		"TRIM_RIGHT",
		"TRUE",
		"TYPE_ALL",
		"TYPE_CONCEPT",
		"TYPE_FIELD",
		"TYPE_METHOD",
		"TYPE_PROPERTY",
		"UNCHANGED",
		"UPPERCASE",
		"VARIANT_TYPE_DATE",
		"VARIANT_TYPE_NULL",
		"VARIANT_TYPE_NUMBER",
		"VARIANT_TYPE_STRING",
		"WARNINGLOG",
		"YES",
		"ZERO"
};

const int  iNumFunctionsLN4 = 295;

CString cFunctionsLN4[iNumFunctionsLN4] = {
		"Abs",
		"AddAsPartner",
		"AddFilter",
		"AddMonths",
		"AddRegister",
		"AddSortFilter",
		"AddYears",
		"AppEndDate",
		"ApplyBookMark",
		"ApplyTable",
		"AppRunDate",
		"AppStartDate",
		"Ascii",
		"Begin",
		"BeginDBTransaction",
		"BeginTransaction",
		"BeginVMTransaction",
		"Bound",
		"BSearch",
		"Call",
		"CallNode",
		"ChannelAttribCall",
		"ChannelAttribGetSliceValue",
		"ChannelAttribGetValue",
		"ChannelCall",
		"CheckModify",
		"CheckPointBlock",
		"CheckPointChannel",
		"CheckPointNode",
		"CheckPointRegister",
		"Chr",
		"clcCheckSyntax",
		"clcCompile",
		"clcExecuteJIT",
		"clcExecuteLN4JIT",
		"clcGetPolish",
		"clcOldTItoIDTI",
		"clcPrepareJIT",
		"clcReleaseJIT",
		"clcRestoreSource",
		"clcSourceToId",
		"clcSourceToNumber",
		"clcSourceToString",
		"CleanFilter",
		"ClearFiles",
		"CleanLiterals",
		"CloneRegister",
		"CloseChannel",
		"CloseFile",
		"CommonTime",
		"CommonTimeStamp",
		"CompileBlock",
		"ConcatFile",
		"ConfigCSTimeOut",
		"ConvertCase",
		"CopyRegister",
		"Count",
		"CreateOrgChart",
		"CrossTab",
		"curExchangeRecord",
		"curExchangeRecordRounded",
		"curGetCurrencyExchangeRounded",
		"curGetDecimals",
		"curGetExchange",
		"curGetExchangeCurrency",
		"curGetExchangeRounded",
		"DataConvert",
		"Date30",
		"Date30Ex",
		"DateAdd",
		"DateDiff",
		"DateToAllNumbers",
		"DateToNumbers",
		"DayOfTheWeek",
		"DaysOfMonth",
		"DaysOfYear",
		"DBTransactionNumber",
		"DefineInstance",
		"DefineInstanceEx",
		"DeleteAllRegisters",
		"DeleteRegister",
		"Delete_blk",
		"Delete_prg",
		"DestroyAllRegisters",
		"DestroyBlock",
		"DestroyFile",
		"DestroyRegister",
		"DialogBox",
		"Div",
		"DumpChannel",
		"DumpNode",
		"DumpRegister",
		"DumpStatistics",
		"DxCompare",
		"DxDate",
		"DxDate30",
		"DxDate30Ex",
		"DxDateIso",
		"DxDateIsoD",
		"DxDateIsoTS",
		"DxDay",
		"DxDaysOfMonth",
		"DxInc",
		"DxMonth",
		"DxRound",
		"DxRound50",
		"DxRoundCent",
		"DxTruncaCent",
		"DxYear",
		"End",
		"EndDBTransaction",
		"EndTransaction",
		"EndVMTransaction",
		"ExecuteDirectSQL",
		"ExecuteGroup",
		"ExecuteRealSQL",
		"ExecuteSQL",
		"ExistNodeItemIndex",
		"FindRegister",
		"Flatten",
		"Format",
		"Fraction",
		"GetArgument",
		"GetArgumentNumber",
		"GetAutoloadMode",
		"GetBookMark",
		"GetChannelData",
		"GetChannelId",
		"GetChannelNumberOfNodes",
		"GetChannelValue",
		"GetCSTimeOut",
		"GetCurrent",
		"GetEndDate",
		"GetErrorString",
		"GetFilterArgument",
		"GetHandle",
		"GetLog",
		"GetLogSize",
		"GetNewNotPersist",
		"GetNodeAutoloadMode",
		"GetNodeData",
		"GetNodeIndex",
		"GetNodeItemArgument",
		"GetNodeItemArgumentNumber",
		"GetNodeItemArgumentPos",
		"GetNodeItemData",
		"GetNodeItemId",
		"GetNodeItemIndex",
		"GetNodeItemInternalType",
		"GetNodeItemM4Type",
		"GetNodeItemPrecision",
		"GetNodeItemPrecisionEx",
		"GetNodeItemReadFieldId",
		"GetNodeItemReadObjectAlias",
		"GetNodeItemReadObjectId",
		"GetNodeItemScale",
		"GetNodeItemScope",
		"GetNodeItemSliceTotalize",
		"GetNodeItemType",
		"GetNodeItemWritefieldId",
		"GetNodeItemWriteObjectAlias",
		"GetNodeItemWriteObjectId",
		"GetNodeNumberOfItems",
		"GetNodeReadObjectId",
		"GetNodeWriteObjectId",
		"GetProperty",
		"GetRegistryValue",
		"GetRuleData",
		"GetRunDate",
		"GetRunningEndDate",
		"GetRunningRunDate",
		"GetRunningStartDate",
		"GetServerValue",
		"GetSliceMode",
		"GetStartDate",
		"GetThisItemId",
		"GetThisNodeId",
		"GetUniqueID",
		"GetValue",
		"GetVariantType",
		"GoNext",
		"GoPrevious",
		"IndexOf",
		"InitFile",
		"InsertChannel",
		"InsertRegister",
		"Insert_blk",
		"Insert_prg",
		"IsBlockUpdated",
		"IsDataCacheable",
		"IsDeleted",
		"IsEOF",
		"IsNew",
		"IsNewAndDeleted",
		"IsNodeUpdated",
		"IsNull",
		"IsUpdated",
		"IsUpdatedDB",
		"JoinSlices",
		"LastIndexOf",
		"Length",
		"Load",
		"LoadFromFile",
		"Load_blk",
		"Load_prg",
		"LocalTest",
		"Log",
		"LookUp",
		"MatchRegExp",
		"Max",
		"MergeSentences",
		"MessageBox",
		"Mid",
		"Min",
		"Months",
		"MoveTo",
		"MoveToEOF",
		"NULL",
		"NullValue",
		"NumbersToDate",
		"OpenChannel",
		"OpenChannelEx",
		"OpenChannelEx",
		"OpenFile",
		"Percentage",
		"Persist",
		"PersistToFile",
		"Persist_tree",
		"Pivot",
		"Power",
		"prInitApplyValue",
		"prSetDates",
		"prSetPeriods",
		"PurgeLog",
		"ReadFile",
		"RefreshFilter",
		"Release",
		"ReleaseBookMark",
		"RemoveAsPartner",
		"RemoveFilter",
		"Replace",
		"ReplaceLiterals",
		"ResetExecuteGroup",
		"Rnd",
		"Round",
		"SendBlockMark",
		"SendNodeMark",
		"SendRegisterMark",
		"SetAutoloadMode",
		"SetChannelValue",
		"SetCSTimeOut",
		"SetCSTimeoutFunction",
		"SetEndDate",
		"SetFilterArgument",
		"SetInserted",
		"SetLoadTimeOut",
		"SetLog",
		"SetNewNotPersist",
		"SetNodeAutoloadMode",
		"SetPriority",
		"SetProperty",
		"SetRunDate",
		"SetSliceMode",
		"SetStartDate",
		"SetValue",
		"SetValuePriority",
		"Sleep",
		"Sort",
		"StrIn",
		"sttGetLevel",
		"sttPersistToFile",
		"sttSetLabel",
		"TickCount",
		"ToDate",
		"Today",
		"TodayGMT",
		"TodayNow",
		"TodayNowGMT",
		"ToDouble",
		"ToInt",
		"ToString",
		"TransferApplyValue",
		"Trim",
		"Truncate",
		"UndoBlock",
		"UndoChannel",
		"UndoNode",
		"UndoRegister",
		"Unflatten",
		"Unload",
		"Update_blk",
		"Update_prg",
		"Years",
		"YTDSearch",
		"YTDSearchFiltered"
};


#define StringChar '"'

#define CommentChar '\''

// Visual C tiene un bug en depuración con las funciones inline si coinciden en nombre
// ¡¡ Coge la primera que pilla aunque esten en un scope distinto !!
// Por ello se utiliza un prefijo en todas ellas
inline bool LN4_NeedCharEscape (TCHAR aChar)
{
	return (aChar == '\n' || aChar == '\t');
	// { y } también necesitan carácter de escape pero, como es necesario
	// realizar el procesamiento de estos caracteres y el procesamiento se realiza
	// al final de todo, lo eliminamos de esta lista
}

inline bool LN4_IsDigit (TCHAR value)
{
	return (value >= '0' && value <='9');
}

inline bool LN4_IsOperator (TCHAR value)
{
	return (value == '=' || value == '+' || value == '-' || value == '*' || value == '/');
}

inline bool LN4_IsSeparator (TCHAR value)
{
	return (value == '"' || value == '\'' || value == '\r' || value == '\n' ||
			value ==' ' || value =='\t' || value =='(' || value ==')' || value == ',' ||
			value == ':' || LN4_IsOperator(value) || value == '[' || value == ']' ||
			value == '{' || value == '}' );
}

inline int LN4_SearchKeyWordAux(CString & candidate, CString * tableKey, int iniPos, int finPos)
{
	int medPos;

	if (iniPos > finPos)
	{
		return -1;
	}
	else if (iniPos == finPos)
	{
		return (candidate == tableKey[iniPos] ? iniPos : -1);
	}
	else
	{
		medPos = (iniPos + finPos) / 2;
		switch (candidate.Compare(tableKey[medPos]))
		{
		case -1:
			return LN4_SearchKeyWordAux(candidate, tableKey, iniPos, medPos - 1);
		case 0:
			return medPos;
		default: // 1
			return LN4_SearchKeyWordAux(candidate, tableKey, medPos + 1, finPos);
		}
	}
}

inline int LN4_SearchKeyWord (CString & candidate, CString * tableKey, int iNumKeys)
{
/*	for (int iCount=0; (iCount< iNumKeys) && (candidate > tableKey[iCount]); iCount++);
	return ((iCount < iNumKeys) && (candidate == tableKey[iCount]) ? iCount : -1);*/
	return LN4_SearchKeyWordAux (candidate, tableKey, 0, iNumKeys -1);
}

inline int LN4_SearchEndLine (CString & text, int iIniPos, int iLastPos)
{
	int iActualPos = iIniPos;
	TCHAR aChar;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		if (aChar == '\n' || aChar == '\r')
		{
			return (iActualPos -1);
		}
		else
		{
			iActualPos++;
		}
	}
	return (iLastPos - 1);
}

inline int LN4_SearchChar (CString & text, int iIniPos, int iLastPos, TCHAR charSearch)
{
	int iActualPos = iIniPos;

	for (; (iActualPos < iLastPos) && (charSearch != text[iActualPos]); iActualPos++);
	return (iActualPos == iLastPos ? -1 : iActualPos);
}

inline int LN4_SearchCharUntilEndLine (CString & text, int iIniPos, int iLastPos, TCHAR charSearch)
{
	int iActualPos = iIniPos;
	TCHAR aChar;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		if (aChar == charSearch)
		{
			return iActualPos;
		}
		else if (aChar == '\n' || aChar == '\r')
		{
			return (iActualPos -1);
		}
		else
		{
			iActualPos++;
		}
	}
	return (iLastPos - 1);
}
inline int LN4_SearchSeparators(CString & text, int iIniPos, int iLastPos)
{
	int iActualPos = iIniPos;
	TCHAR aChar;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		if (LN4_IsSeparator(aChar))
		{
			return (iActualPos -1);
		}
		else
		{
			iActualPos++;
		}
	}
	return (iLastPos - 1);
}

inline int LN4_SearchNumber(CString & text, int iIniPos, int iLastPos)
{
	int iActualPos = iIniPos;
	TCHAR aChar = '\0';
	bool bPointUsed = false;
	bool bAnyDigit = false;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		if (LN4_IsDigit(aChar))
		{
			iActualPos++;
			bAnyDigit = true;
			continue;
		}
		else if (aChar == '.')
		{
			if (bPointUsed)
			{
				return (-1);
			}
			else
			{
				iActualPos++;
				bPointUsed = true;
				continue;
			}
		}
		else if (LN4_IsSeparator(aChar))
		{
			return (bAnyDigit ? iActualPos - 1 : -1);
		}
		else
		{
			return (-1);
		}
	}
	return (bAnyDigit ? iLastPos - 1 : -1);
}

inline int LN4_SearchEndMultilineComment(CString & text, int iIniPos, int iLastPos, bool & bInMultiline)
{
	int iActualPos = iIniPos + 1;
	TCHAR aCharPrev;
	TCHAR aChar;

	if (iActualPos < iLastPos)
	{
		aCharPrev = text[iIniPos];
		if (aCharPrev == '\n')
		{
			bInMultiline = true;
			return (iIniPos);
		}
		while (iActualPos < iLastPos)
		{
			aChar = text[iActualPos];
			if (aChar == '\n')
			{
				bInMultiline = true;
				return (iActualPos);
			}
			else if ((aCharPrev == '*') && (aChar == '/'))
			{
				bInMultiline = false;
				return (iActualPos);
			}
			else
			{
				iActualPos++;
			}
			aCharPrev = aChar;
		}
	}
	return (iLastPos);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

clsLN4::clsLN4()
{
	int i;

	m_eType = M4PT_LN4;

	m_cKeyWords = new CString[iNumKeyWordsLN4];
	for (i=0; i < iNumKeyWordsLN4; i++)
	{
		m_cKeyWords[i] = cKeyWordsLN4[i];
		m_cKeyWords[i].MakeUpper();
	}
	m_iNumKeyWords = iNumKeyWordsLN4;

	m_cOperators = new CString[iNumOperatorsLN4];
	for (i=0; i < iNumOperatorsLN4; i++)
	{
		m_cOperators[i] = cOperatorsLN4[i];
		m_cOperators[i].MakeUpper();
	}
	m_iNumOperators = iNumOperatorsLN4;

	m_cConstants = new CString[iNumConstantsLN4];
	for (i=0; i < iNumConstantsLN4; i++)
	{
		m_cConstants[i] = cConstantsLN4[i];
		m_cConstants[i].MakeUpper();
	}
	m_iNumConstants = iNumConstantsLN4;

	m_cFunctions = new CString[iNumFunctionsLN4];
	for (i=0; i < iNumFunctionsLN4; i++)
	{
		m_cFunctions[i] = cFunctionsLN4[i];
		m_cFunctions[i].MakeUpper();
	}
	m_iNumFunctions = iNumFunctionsLN4;
	m_patColor = new long[iNumColors];
	for (i=0; i < iNumColors; i++)
	{
		m_patColor[i] = patColor[i];
	}
	m_iNumColors = iNumColors;
}

clsLN4::~clsLN4()
{
	delete [] m_cKeyWords;
	delete [] m_cOperators;
	delete [] m_cConstants;
	delete [] m_cFunctions;
	delete [] m_patColor;
}

CString clsLN4::GetColorTable()
{
	CString csTable("");
	CString csColor;

	for (int j=0; j < m_iNumColors; j++)
	{
		csColor.Format("\\red%d\\green%d\\blue%d;", GetRValue(m_patColor[j]), GetGValue(m_patColor[j]), GetBValue(m_patColor[j]));
		csTable += csColor;
	}
	return csTable;
}

void clsLN4::Parse(clsTextInfo * oTextInfo, long lStartLine, CString & sWorkText, VARIANT_BOOL * bPostProcess, long * lEndLine)
{
	int iActualPos = 0;
	int iAuxPos;
	int iFinal;
	bool bTokenNumber = false;
	bool bTokenPrevNumber = false;
	bool bPreviousHighlighted = true;
	bool bInMultiLineComment;
	int iLastHighlight = LN4_UNDEFINED;
	int iLastBackHighlight = LN4_UNDEFINED;
	bool bAnalyzeLineStart = true;
	int iParameters = 0;
	int iBrackets = 0;
	int iBrackets2 = 0;
	long iActualLine = lStartLine;

	TCHAR aChar;
	TCHAR aChar2;
	CString csAux;

	if (iActualLine < 0) iActualLine = 0;

	bInMultiLineComment = ((iActualLine > 0) && (oTextInfo->LineInfo(iActualLine - 1)->get_LineStatus() != 0));

	iFinal = sWorkText.GetLength();
	// Se parsea el texto
	for (; iActualPos < iFinal; iActualPos++)
	{
		if (bAnalyzeLineStart)
		{
			iParameters = 0;
			iBrackets = 0;
			iBrackets2 = 0;

			if (iActualLine == oTextInfo->get_ActiveLine())
			{
				if (iLastBackHighlight != LN4_ACTIVELINE)
				{
					sWorkText.Insert(iActualPos, LN4_ACTIVELINE_CODE);
					iActualPos += LN4_BACKCODE_LENGTH;
					iFinal += LN4_BACKCODE_LENGTH;
					iLastBackHighlight = LN4_ACTIVELINE;
				}
			}
			else if (oTextInfo->LineInfo(iActualLine)->get_SelectionMark() == CALLER_SELECTION_MARK)
			{
				if (iLastBackHighlight != LN4_CALLERLINE)
				{
					sWorkText.Insert(iActualPos, LN4_CALLERLINE_CODE);
					iActualPos += LN4_BACKCODE_LENGTH;
					iFinal += LN4_BACKCODE_LENGTH;
					iLastBackHighlight = LN4_CALLERLINE;
				}
			}
			else if (oTextInfo->LineInfo(iActualLine)->get_BreakPointMark() > 0)
			{
				if (iLastBackHighlight != LN4_BREAKPOINT)
				{
					sWorkText.Insert(iActualPos, LN4_BREAKPOINT_CODE);
					iActualPos += LN4_BACKCODE_LENGTH;
					iFinal += LN4_BACKCODE_LENGTH;
					iLastBackHighlight = LN4_BREAKPOINT;
				}
			}
			else
			{
				if (iLastBackHighlight != LN4_NORMALBACK)
				{
					sWorkText.Insert(iActualPos, LN4_NORMALBACK_CODE);
					iActualPos += LN4_BACKCODE_LENGTH;
					iFinal += LN4_BACKCODE_LENGTH;
					iLastBackHighlight = LN4_NORMALBACK;
				}
			}
			bAnalyzeLineStart = false;
		}

		if (bInMultiLineComment)
		{
			iAuxPos = LN4_SearchEndMultilineComment(sWorkText, iActualPos, iFinal, bInMultiLineComment);

			if (iLastHighlight != LN4_REMARK)
			{
				sWorkText.Insert(iActualPos, LN4_REMARK_CODE);
				iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
				iFinal += LN4_FORECODE_LENGTH;
				iLastHighlight = LN4_REMARK;
			}
			else
			{
				iActualPos = iAuxPos;
			}

			// Si termina el comentario multilinea. iActualPos apunta a / (del */) => continue para salta al siguiente
			// si no aputa al \n => lo procesa
			if (!bInMultiLineComment || (iActualPos >= iFinal)) continue;
		}

		aChar = sWorkText[iActualPos];
		bTokenPrevNumber = bTokenNumber;
		bTokenNumber = false;

		if (LN4_NeedCharEscape(aChar) || aChar =='\r')
		{
			if (aChar == '\n')
			{
				if ((* bPostProcess) && (oTextInfo->LineInfo(iActualLine)->get_LineStatus() == (bInMultiLineComment ? 1:0)))
				{
					// Estamos en un postprocesado (por comentarios)
					// y se ha llegado a un línea donde no hay cambio de estado
					// => Optimizamos y cortamos aquí
					sWorkText.Delete(iActualPos, iFinal - iActualPos);
					* bPostProcess = FALSE;
					* lEndLine = iActualLine;
					return;
				}
				oTextInfo->LineInfo(iActualLine++)->put_LineStatus(bInMultiLineComment ? 1:0);
				bAnalyzeLineStart = true;
			}
			// Se sustituira al final (Es mucho más eficiente. Mejoras del 40% en tiempo)
			continue;
		}
		else if (aChar == '\'' || aChar == '/')
		{
			aChar2 = (iActualPos + 1 < iFinal ? sWorkText[iActualPos +1] : '\0');
			if (aChar == '\'' || aChar2 == '/') // Comentario de tipo ' o de tipo //
			{
				iAuxPos = LN4_SearchEndLine(sWorkText, iActualPos + 1, iFinal);
				if (iLastHighlight != LN4_REMARK)
				{
					sWorkText.Insert(iActualPos, LN4_REMARK_CODE);
					iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_REMARK;
				}
				else
				{
					iActualPos = iAuxPos;
				}
			}
			else if (aChar2 == '*') //Comentario multilinea
			{
				if (iLastHighlight != LN4_REMARK)
				{
					sWorkText.Insert(iActualPos, LN4_REMARK_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_REMARK;
				}

				iActualPos++; // Estabamos en el / => Ahora nos posicionamos en el * y continuamos
				bInMultiLineComment = true;

			}
			else //Operador numérico
			{
				if (iLastHighlight != LN4_OPERATOR)
				{
					sWorkText.Insert(iActualPos, LN4_OPERATOR_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal +=LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_OPERATOR;
				}
			}
		}
		else if (aChar == StringChar)
		{
			iAuxPos = LN4_SearchCharUntilEndLine (sWorkText, iActualPos+1, iFinal, StringChar);
			if (iLastHighlight != LN4_STRING)
			{
				sWorkText.Insert(iActualPos, LN4_STRING_CODE);
				iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
				iFinal += LN4_FORECODE_LENGTH;
				iLastHighlight = LN4_STRING;
			}
			else
			{
				iActualPos = iAuxPos;
			}
		}
		else if (aChar =='(')
		{
			iParameters++;
			if (iLastHighlight != LN4_KEYWORD)
			{
				sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
				iActualPos += LN4_FORECODE_LENGTH;
				iFinal +=LN4_FORECODE_LENGTH;
				iLastHighlight = LN4_KEYWORD;
			}
		}
		else if (aChar ==')')
		{
			if (iParameters > 0)
			{
				iParameters--;
				if (iLastHighlight != LN4_KEYWORD)
				{
					sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal +=LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_KEYWORD;
				}
			}
			else
			{
				if (iLastHighlight != LN4_TEXT)
				{
					sWorkText.Insert(iActualPos, LN4_TEXT_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_TEXT;
				}
			}
		}
		else if (aChar =='[')
		{
			iBrackets++;
			if (iLastHighlight != LN4_KEYWORD)
			{
				sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
				iActualPos += LN4_FORECODE_LENGTH;
				iFinal +=LN4_FORECODE_LENGTH;
				iLastHighlight = LN4_KEYWORD;
			}
		}
		else if (aChar ==']')
		{
			if (iBrackets > 0)
			{
				iBrackets--;
				if (iLastHighlight != LN4_KEYWORD)
				{
					sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal +=LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_KEYWORD;
				}
			}
			else
			{
				if (iLastHighlight != LN4_TEXT)
				{
					sWorkText.Insert(iActualPos, LN4_TEXT_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_TEXT;
				}
			}
		}
		else if (aChar =='{')
		{
			iBrackets2++;
			if (iLastHighlight != LN4_KEYWORD)
			{
				sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
				iActualPos += LN4_FORECODE_LENGTH;
				iFinal +=LN4_FORECODE_LENGTH;
				iLastHighlight = LN4_KEYWORD;
			}
		}
		else if (aChar =='}')
		{
			if (iBrackets2 > 0)
			{
				iBrackets2--;
				if (iLastHighlight != LN4_KEYWORD)
				{
					sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal +=LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_KEYWORD;
				}
			}
			else
			{
				if (iLastHighlight != LN4_TEXT)
				{
					sWorkText.Insert(iActualPos, LN4_TEXT_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_TEXT;
				}
			}
		}
		else if (aChar ==',')
		{
			if (iParameters > 0)
			{
				if (iLastHighlight != LN4_KEYWORD)
				{
					sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal +=LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_KEYWORD;
				}
			}
			else
			{
				if (iLastHighlight != LN4_TEXT)
				{
					sWorkText.Insert(iActualPos, LN4_TEXT_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_TEXT;
				}
			}
		}
		else if (aChar == '=' || aChar == '+' || aChar == '*' || aChar == '<' || aChar == '>')
		{
			if (iLastHighlight != LN4_OPERATOR)
			{
				sWorkText.Insert(iActualPos, LN4_OPERATOR_CODE);
				iActualPos += LN4_FORECODE_LENGTH;
				iFinal +=LN4_FORECODE_LENGTH;
				iLastHighlight = LN4_OPERATOR;
			}
		}
		/*selse if (aChar =='@' || aChar =='#')
		{
			iAuxPos = LN4_SearchEndWord(sWorkText, iActualPos, iFinal);
			if (iLastHighlight != LN4_CONCEPT)
			{
				sWorkText.Insert(iActualPos, LN4_CONCEPT_CODE);
				iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
				iFinal += LN4_FORECODE_LENGTH;
				iLastHighlight = LN4_CONCEPT;
			}
			else
			{
				iActualPos = iAuxPos;
			}
		}*/
		else if ((aChar == '-') || LN4_IsDigit(aChar)) // Evaluar número
		{
			if (bTokenPrevNumber && (aChar == '-')) // Si el token anterior era un número el - es operador
			{
				iAuxPos = -1;
			}
			else
			{
				iAuxPos = LN4_SearchNumber(sWorkText, ((aChar == '-') ? iActualPos+1 : iActualPos), iFinal);
			}

			if (iAuxPos >=0)
			{
				if (iLastHighlight != LN4_NUMERIC)
				{
					sWorkText.Insert(iActualPos, LN4_NUMERIC_CODE);
					iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_NUMERIC;
					bTokenNumber = true;
				}
				else
				{
					iActualPos = iAuxPos;
				}
			}
			else if (aChar == '-') // Es un operador
			{
				if (iLastHighlight != LN4_OPERATOR)
				{
					sWorkText.Insert(iActualPos, LN4_OPERATOR_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal +=LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_OPERATOR;
				}
			}
			else
			{
				if (iLastHighlight != LN4_TEXT)
				{
					sWorkText.Insert(iActualPos, LN4_TEXT_CODE);
					iActualPos += LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_TEXT;
				}
				iActualPos = LN4_SearchSeparators(sWorkText, iActualPos, iFinal);
			}
		}
		else if (aChar == ':')
		{
			if (iLastHighlight != LN4_KEYWORD)
			{
				sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
				iActualPos += LN4_FORECODE_LENGTH;
				iFinal +=LN4_FORECODE_LENGTH;
				iLastHighlight = LN4_KEYWORD;
			}
		}
		else if (aChar != ' ' && aChar != '\r')
		{
			iAuxPos = LN4_SearchSeparators(sWorkText, iActualPos, iFinal);
			csAux = sWorkText.Mid(iActualPos, iAuxPos - iActualPos + 1);
			csAux.MakeUpper();
			if (LN4_SearchKeyWord(csAux,m_cOperators, m_iNumOperators) >=0)
			{
				if (iLastHighlight != LN4_OPERATOR)
				{
					sWorkText.Insert(iActualPos, LN4_OPERATOR_CODE);
					iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_OPERATOR;
				}
				else
				{
					iActualPos = iAuxPos;
				}
			}
			else if (LN4_SearchKeyWord(csAux,m_cKeyWords, m_iNumKeyWords) >=0)
			{
				if (iLastHighlight != LN4_KEYWORD)
				{
					sWorkText.Insert(iActualPos, LN4_KEYWORD_CODE);
					iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_KEYWORD;
				}
				else
				{
					iActualPos = iAuxPos;
				}
			}
			else if (LN4_SearchKeyWord(csAux,m_cFunctions, m_iNumFunctions) >=0)
			{
				if (iLastHighlight != LN4_FUNCTION)
				{
					sWorkText.Insert(iActualPos, LN4_FUNCTION_CODE);
					iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_FUNCTION;
				}
				else
				{
					iActualPos = iAuxPos;
				}
			}
			else if (LN4_SearchKeyWord(csAux,m_cConstants, m_iNumConstants) >=0)
			{
				if (iLastHighlight != LN4_CONSTANT)
				{
					sWorkText.Insert(iActualPos, LN4_CONSTANT_CODE);
					iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_CONSTANT;
				}
				else
				{
					iActualPos = iAuxPos;
				}
			}
			else
			{
				if (iLastHighlight != LN4_TEXT)
				{
					sWorkText.Insert(iActualPos, LN4_TEXT_CODE);
					iActualPos = iAuxPos + LN4_FORECODE_LENGTH;
					iFinal += LN4_FORECODE_LENGTH;
					iLastHighlight = LN4_TEXT;
				}
				else
				{
					iActualPos = iAuxPos;
				}
			}
		}
	}

	*bPostProcess = (oTextInfo->LineInfo(iActualLine)->get_LineStatus() != (bInMultiLineComment ? 1 : 0));

	oTextInfo->LineInfo(iActualLine)->put_LineStatus(bInMultiLineComment ? 1 : 0);

	*lEndLine = iActualLine;
}

VARIANT_BOOL clsLN4::GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, CString & sDesc)
{
	if (Id < 0 || Id >= m_iNumColors)
	{
		return FALSE;
	}
	else
	{
		*Color = m_patColor[Id];
		*DefaultColor = patColor[Id];
		*iKind = patKind[Id];
		sDesc.LoadString(m_lBaseResources + Id);

		return TRUE;
	}
}

VARIANT_BOOL clsLN4::SetColorInfo(short Id, long Color)
{
	if (Id < 0 || Id >= m_iNumColors)
	{
		return FALSE;
	}
	else
	{
		m_patColor[Id] = Color;
		return TRUE;
	}
}
