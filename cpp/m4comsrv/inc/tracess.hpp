
#ifndef TRACESS_HPP_
#define TRACESS_HPP_

#include <clssimplementacion.hpp>

class ClTraceSS : public ClSSImplementation
{
public:
	ClTraceSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator ) ;

    ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

    m4return_t Initialize ();

};



class ClLogSS : public ClSSImplementation
{

    typedef enum {ENABLE_ERRORS = ClSSImplementation::TOTAL_COMMANDS, DISABLE_ERRORS , ENABLE_WARNINGS , DISABLE_WARNINGS , ENABLE_DEBUG_LOG , DISABLE_DEBUG_LOG, TOTAL_COMMANDS } eLogSSCommands;

public:
	ClLogSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator ) ;

	m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

	m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

	m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

    m4return_t Initialize ();
};

class ClLDBInspectorSS : public ClSSImplementation
{
    typedef enum { SET_DEBUG_LEVEL = ClSSImplementation :: TOTAL_COMMANDS, SET_DEBUG_DETAIL_LEVEL, VM_DEBUG_ENABLED, SET_VM_MAX_DEBUG_PERIOD, ENABLE_JAVA_TRACE, DISABLE_JAVA_TRACE, SET_JAVA_TRACE, ENABLE_TRANSPORT_DUMP, DISABLE_TRANSPORT_DUMP, FLUSH_SENTENCE_CACHE, FLUSH_DICTIONARY_CACHE, RETRY_CONNECTION} eLDBCommandsID;
    typedef enum { DEBUG_LEVEL, DEBUG_DETAIL_LEVEL } eLDBParamsID;

public:
	ClLDBInspectorSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator ) ;

	m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

	m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

	m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

    m4return_t Initialize ();
};


#endif

