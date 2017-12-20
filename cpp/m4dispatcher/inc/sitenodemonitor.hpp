#include <m4thread.hpp>
#include <sitenodeinfo.hpp>
#include <clientapi.hpp>


class ClSiteHealthVerifier;

// -----------------------------------------------------------------------------------------------------------------------------
// -- Class ClRunningSrvMonitor.
// -- Monitorizes a single running server, checking if they are able to service client requests.
// -----------------------------------------------------------------------------------------------------------------------------

class ClRunningSrvMonitor: public M4Thread
{

	public:

		ClRunningSrvMonitor( const ClSiteNodeInformation &ai_oSiteNode, ClSiteHealthVerifier* ai_pHealthMonitor );
		~ClRunningSrvMonitor();

	private:

		// -- Involved server node.
		ClSiteNodeInformation m_oSiteNode;
		
		// -- Current monitor objects container.
		ClSiteHealthVerifier* m_pSiteHealthVerifier;

		// -- Clear text polling user password.
		M4ClString m_strPwd;

		// -- Common client API for all monitors.
		static ClClientAPI*	s_poClientAPI;

		// -- Current session Id.
		m4handle_t m_hSessionId;

		// -- Polling trace file.
		static FILE* s_pfPollingTraceFile;

		// -- Synch object.
		static ClCriticalSection m_oTraceSynch;

	protected:

		// -- Thread working method.
		void Run();

		// -- Activity log helper method.
		void _DumpPollingTrace( m4pchar_t ai_pcFormat,  ... );

	private:

		// -- Gets decrypted polling user password.
		m4return_t	_GetPwd( M4ClString& ao_strPwd );

		// -- Returns the client API instance.
		ClClientAPI* _GetClientAPI();

		// -- Logs in to current server.
		m4return_t _ServerLogin();

		// -- Logs out from current server.
		m4return_t _ServerLogout();

		// -- Calculates execution times for each target server executor.
		m4return_t _CalculateExecutorsTime( m4millisec_t& ao_iRemainingTime );

		// -- Launchs a single OLTP execution.
		m4return_t _LaunchOLTPExecution();

		// -- Launchs a single XML execution.
		m4return_t _LaunchXMLExecution();

		void _SleepNoLock( m4millisec_t ai_iMsecs );
};



// -----------------------------------------------------------------------------------------------------------------------------
// -- Class ClSiteHealthVerifier.
// -- Running server monitor objects container.
// -----------------------------------------------------------------------------------------------------------------------------

typedef map< m4string_t, ClRunningSrvMonitor*, less <m4string_t> > TMonitorsMap;

class ClSiteHealthVerifier
{

	public:

		ClSiteHealthVerifier( m4bool_t ai_bEnabled, m4millisec_t ai_iDelay, m4bool_t ai_bDumpPollingTrace, m4uint32_t ai_iTimeout, m4millisec_t ai_iExecutorDelay );
		~ClSiteHealthVerifier();
		
		// -- Adds a new server node to monitorize.
		m4return_t AddServerMonitor( const ClSiteNodeInformation &ai_oSiteNode );

		// -- Stops monitorizing a server node.
		m4return_t RemoveServerMonitor( const ClSiteNodeInformation &ai_oSiteNode );

		// -- Stops monitorizing all server nodes.
		void RemoveAllServerMonitors();

		// -- Member getters.
		m4millisec_t GetDelay();

		m4uint32_t GetTimeout();

		m4bool_t GetDumpPollingTrace();

		m4millisec_t GetExecutorDelay();

		// -- Member setters.
		void SetDumpPollingTrace( m4bool_t ai_bEnable );

	private:

		// -- Polling mechanism switch.
		m4bool_t m_bEnabled;

		// -- Server monitor objects container.
		TMonitorsMap m_oRunningSrvMonitors;

		// -- Polling delay.
		m4millisec_t m_iDelay;

		// -- Trace active.
		m4bool_t m_bDumpPollingTrace;

		// -- Polling timeout.
		m4uint32_t m_iTimeout;

		// -- Synch object.
		static ClCriticalSection m_oSynch;

		// -- Executor polling delay.
		m4millisec_t  m_iExecutorDelay;

};


inline m4millisec_t ClSiteHealthVerifier::GetDelay()
{
	return m_iDelay;
}

inline m4bool_t	ClSiteHealthVerifier::GetDumpPollingTrace()
{
	return m_bDumpPollingTrace;
}

inline m4uint32_t ClSiteHealthVerifier::GetTimeout()
{
	return m_iTimeout;
}

inline m4millisec_t ClSiteHealthVerifier::GetExecutorDelay()
{
	return m_iExecutorDelay;
}

inline void ClSiteHealthVerifier::SetDumpPollingTrace( m4bool_t ai_bEnable )
{
	m_bDumpPollingTrace = ai_bEnable;
}
