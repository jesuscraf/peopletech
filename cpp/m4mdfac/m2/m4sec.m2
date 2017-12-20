BUILD: 814.000.000
ID_T3: M4SEC
ID_STREAM_TYPE: 104

SENTENCE_NUMBER: 5

	ID_SENTENCE: SYS_T3_MASK
	APISQL_1: SELECT @T3_SEC = (1*M.T3_SEC_READ)+(2*M.T3_SEC_WRITE)+(4*M.T3_SEC_DELETE)+(8*M.T3_SEC_UPDATE)+(16*M.T3_SEC_EXE), @VERSION = V.DT_MD_VERSION FROM M4RCH_T3_MASK M, M4RCH_MD_VERSION V WHERE M.ID_T3 = V.ID_T3 AND V.ID_MD_TYPE = '2' AND M.ID_MAKS = @ID_MASK AND M.ID_T3 = @ID_T3
	APISQL_2: NULL
	APISQL_3: NULL
	APISQL_4: NULL

	ID_SENTENCE: SYS_NODE_MASK
	APISQL_1: SELECT @ID_NODE = A.ID_NODE, @T3_SEC = (1*A.T3_SEC_READ)+(2*A.T3_SEC_WRITE)+(4*A.T3_SEC_DELETE)+(8*A.T3_SEC_UPDATE)+(16*A.T3_SEC_EXE)+(32*A.DNY_FILTER), @SEC_FILTER = B.SEC_FILTER FROM M4RCH_NODE_MASK A, M4RCH_NODE_MASK1 B WHERE A.ID_T3 = B.ID_T3 AND A.ID_NODE = B.ID_NODE AND A.ID_MASK = B.ID_MASK AND A.ID_MASK = @ID_MASK AND A.ID_T3 = @ID_T3 ORDER BY A.ID_NODE
	APISQL_2: NULL
	APISQL_3: NULL
	APISQL_4: NULL

	ID_SENTENCE: SYS_ITEM_MASK
	APISQL_1: SELECT @ID_NODE = ID_NODE, @ID_ITEM = ID_ITEM, @T3_SEC = (1*T3_SEC_READ)+(2*T3_SEC_WRITE)+(4*T3_SEC_DELETE)+(8*T3_SEC_UPDATE)+(16*T3_SEC_EXE)+(32*T3_SEC_CONTINUE) FROM M4RCH_ITEM_MASK WHERE ID_MASK = @ID_MASK AND ID_T3 = @ID_T3 ORDER BY ID_NODE, ID_ITEM
	APISQL_2: NULL
	APISQL_3: NULL
	APISQL_4: NULL

	ID_SENTENCE: SYS_FILTER_MASK
	APISQL_1: SELECT @ID_NODE = ID_NODE, @ID_FILTER = ID_FILTER, @IS_APPLICABLE = IS_APPLICABLE FROM M4RCH_FILTER_MASK WHERE ID_MASK = @ID_MASK AND ID_T3 = @ID_T3 ORDER BY ID_NODE, ID_FILTER
	APISQL_2: NULL
	APISQL_3: NULL
	APISQL_4: NULL

	ID_SENTENCE: SYS_T3_CONNECTOR_MASK
	APISQL_1: SELECT @ID_NODE = ID_NODE, @ID_T3_USED = ID_T3_USED, @ID_NODE_USED = ID_NODE_USED, @IS_APPLICABLE = IS_APPLICABLE FROM M4RCH_T3_CON_MASK WHERE ID_MASK = @ID_MASK AND ID_T3 = @ID_T3 ORDER BY ID_NODE, ID_T3_USED, ID_NODE_USED
	APISQL_2: NULL
	APISQL_3: NULL
	APISQL_4: NULL

TI_NUMBER: 6

	ID_TI: SYS_BASE
	ID_READ_SENTENCE: NULL

	ID_TI: SYS_T3_MASK
	ID_READ_SENTENCE: SYS_T3_MASK

	ID_TI: SYS_NODE_MASK
	ID_READ_SENTENCE: SYS_NODE_MASK

	ID_TI: SYS_ITEM_MASK
	ID_READ_SENTENCE: SYS_ITEM_MASK

	ID_TI: SYS_FILTER_MASK
	ID_READ_SENTENCE: SYS_FILTER_MASK

	ID_TI: SYS_T3_CONNECTOR_MASK
	ID_READ_SENTENCE: SYS_T3_CONNECTOR_MASK

INHERIT_NUMBER: 5

	ID_TI: SYS_T3_MASK
	ID_TI_BASE: SYS_BASE
	ID_LEVEL: 1

	ID_TI: SYS_NODE_MASK
	ID_TI_BASE: SYS_BASE
	ID_LEVEL: 1

	ID_TI: SYS_ITEM_MASK
	ID_TI_BASE: SYS_BASE
	ID_LEVEL: 1

	ID_TI: SYS_FILTER_MASK
	ID_TI_BASE: SYS_BASE
	ID_LEVEL: 1

	ID_TI: SYS_T3_CONNECTOR_MASK
	ID_TI_BASE: SYS_BASE
	ID_LEVEL: 1

ITEM_NUMBER: 26

	ID_TI: SYS_BASE
	ID_ITEM: LOAD
	ID_ITEM_TYPE: 1
	ID_SCOPE_TYPE: 2
	ID_M4_TYPE: 6
	PREC: 4
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 2
	
	ID_TI: SYS_T3_MASK
	ID_ITEM: ID_MASK
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 2
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_MASK
	ID_ITEM: ID_T3
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 2
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_MASK
	ID_ITEM: T3_SEC
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 6
	PREC: 1
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_MASK
	ID_ITEM: VERSION
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 5
	PREC: 0
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_NODE_MASK
	ID_ITEM: ID_MASK
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_NODE_MASK
	ID_ITEM: ID_T3
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_NODE_MASK
	ID_ITEM: ID_NODE
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_NODE_MASK
	ID_ITEM: T3_SEC
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 6
	PREC: 1
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_NODE_MASK
	ID_ITEM: SEC_FILTER
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 3
	PREC: -1
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_ITEM_MASK
	ID_ITEM: ID_MASK
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_ITEM_MASK
	ID_ITEM: ID_T3
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_ITEM_MASK
	ID_ITEM: ID_NODE
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_ITEM_MASK
	ID_ITEM: ID_ITEM
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_ITEM_MASK
	ID_ITEM: T3_SEC
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 6
	PREC: 1
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_FILTER_MASK
	ID_ITEM: ID_MASK
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_FILTER_MASK
	ID_ITEM: ID_T3
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_FILTER_MASK
	ID_ITEM: ID_NODE
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_FILTER_MASK
	ID_ITEM: ID_FILTER
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_FILTER_MASK
	ID_ITEM: IS_APPLICABLE
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 6
	PREC: 1
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_CONNECTOR_MASK
	ID_ITEM: ID_MASK
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_CONNECTOR_MASK
	ID_ITEM: ID_T3
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_CONNECTOR_MASK
	ID_ITEM: ID_T3_USED
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_CONNECTOR_MASK
	ID_ITEM: ID_NODE
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_CONNECTOR_MASK
	ID_ITEM: ID_NODE_USED
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 1
	PREC: 30
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0
	
	ID_TI: SYS_T3_CONNECTOR_MASK
	ID_ITEM: IS_APPLICABLE
	ID_ITEM_TYPE: 3
	ID_SCOPE_TYPE: 3
	ID_M4_TYPE: 6
	PREC: 1
	SCALE: 0
	DEFAULT_VALUE: NULL
	ID_EXE_LEVEL: 0

RULE_NUMBER: 1

	ID_TI: SYS_BASE
	ID_ITEM: LOAD
	ID_METHOD: SYSLOAD

NODE_NUMBER: 5

	ID_NODE: SYS_T3_MASK
	ID_TI: SYS_T3_MASK
	IS_ROOT: 1

	ID_NODE: SYS_NODE_MASK
	ID_TI: SYS_NODE_MASK
	IS_ROOT: 0

	ID_NODE: SYS_ITEM_MASK
	ID_TI: SYS_ITEM_MASK
	IS_ROOT: 0

	ID_NODE: SYS_FILTER_MASK
	ID_TI: SYS_FILTER_MASK
	IS_ROOT: 0

	ID_NODE: SYS_T3_CONNECTOR_MASK
	ID_TI: SYS_T3_CONNECTOR_MASK
	IS_ROOT: 0

CONNECTOR_NUMBER: 4

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_NODE_MASK
	ID_CONNECTION_TYPE: 1

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_ITEM_MASK
	ID_CONNECTION_TYPE: 1

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_FILTER_MASK
	ID_CONNECTION_TYPE: 1

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_T3_CONNECTOR_MASK
	ID_CONNECTION_TYPE: 1

ITEM_CONN_NUMBER: 12

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_NODE_MASK
	ID_ITEM_USED: LOAD
	ID_ITEM: LOAD
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 2
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 2
	CALL_ORDER: 1
	ID_CSTYPE: 2

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_NODE_MASK
	ID_ITEM_USED: ID_MASK
	ID_ITEM: ID_MASK
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 0
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 0
	CALL_ORDER: 1
	ID_CSTYPE: 3

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_NODE_MASK
	ID_ITEM_USED: ID_T3
	ID_ITEM: ID_T3
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 0
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 0
	CALL_ORDER: 1
	ID_CSTYPE: 3

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_ITEM_MASK
	ID_ITEM_USED: LOAD
	ID_ITEM: LOAD
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 2
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 2
	CALL_ORDER: 2
	ID_CSTYPE: 2

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_ITEM_MASK
	ID_ITEM_USED: ID_MASK
	ID_ITEM: ID_MASK
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 0
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 0
	CALL_ORDER: 2
	ID_CSTYPE: 3

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_ITEM_MASK
	ID_ITEM_USED: ID_T3
	ID_ITEM: ID_T3
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 0
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 0
	CALL_ORDER: 2
	ID_CSTYPE: 3

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_FILTER_MASK
	ID_ITEM_USED: LOAD
	ID_ITEM: LOAD
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 2
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 2
	CALL_ORDER: 3
	ID_CSTYPE: 2

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_FILTER_MASK
	ID_ITEM_USED: ID_MASK
	ID_ITEM: ID_MASK
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 0
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 0
	CALL_ORDER: 3
	ID_CSTYPE: 3

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_FILTER_MASK
	ID_ITEM_USED: ID_T3
	ID_ITEM: ID_T3
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 0
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 0
	CALL_ORDER: 3
	ID_CSTYPE: 3

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_T3_CONNECTOR_MASK
	ID_ITEM_USED: LOAD
	ID_ITEM: LOAD
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 2
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 2
	CALL_ORDER: 4
	ID_CSTYPE: 2

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_T3_CONNECTOR_MASK
	ID_ITEM_USED: ID_MASK
	ID_ITEM: ID_MASK
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 0
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 0
	CALL_ORDER: 4
	ID_CSTYPE: 3

	ID_NODE_USED: SYS_T3_MASK
	ID_NODE: SYS_T3_CONNECTOR_MASK
	ID_ITEM_USED: ID_T3
	ID_ITEM: ID_T3
	ID_PRECEDENCE_TYPE: 2
	ID_SPIN_TYPE: 0
	ID_RELSHIP_TYPE: 1
	ID_CONTEXT_TYPE: 0
	CALL_ORDER: 4
	ID_CSTYPE: 3

INDEX_NUMBER: 0

INDEX_ITEM_NUMBER: 0
