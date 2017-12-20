//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlutint.hpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:			 21/10/2015
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines Unit Testing API and utilities.
//
//==============================================================================

#ifndef _M4XLUTINT_H_
#define _M4XLUTINT_H_

#include "m4xlapi_dll.hpp"
#include "m4xlhelper.hpp"
#include <m4types.hpp>
#include <m4var.hpp>
#include <execontx.hpp>
#include <libxl.h>

using namespace libxl;


// -----------------------------------------------------------------------------
// Class ClUTConfig
// -----------------------------------------------------------------------------
class ClUTConfig
{
	public:

		ClUTConfig( ClVMRunContext*	ai_pVMCtx );
		~ClUTConfig();

		int		GetScanXEmpty();
		bool	GetTouchScanned();
		bool	GetTraceActive();
		char*	GetTraceFile();

		static const char*	c_sIemCfgScanXEmpty;
		static const char*	c_sItemCfgTouchScanned;
		static const char*	c_sItemCfgTraceActive;
		static const char*	c_sItemCgfTraceFile;
		static const int	c_iScanTillLastCell;
		static const int	c_iScanTillXEmpty;

	private:

		ClVMRunContext*		m_pVMCtx;
		int					m_iScanXEmpty;
		bool				m_bTouchScanned;
		bool				m_bTraceActive;
		char*				m_pcTraceFile;
};


// -----------------------------------------------------------------------------
// Class ClWbook
// -----------------------------------------------------------------------------
class ClWbook
{
	public:

		ClWbook( const char* ai_pcPath, ClUTConfig* ai_pCfg );
		~ClWbook();
		
		bool		operator==( const ClWbook& ai_Wbook );
		bool		IsXml();
		ClUTConfig*	GetCfg();
		Sheet*		GetXlWs( int ai_iIdx );
		bool		GetIsLoaded();
		bool		SaveXlWb();

	private:

		const char*	m_pcPath;
		bool		m_bIsXML;
		bool		m_bIsLoaded;
		ClUTConfig*	m_pCfg;
		Book*		m_pXlWb;

		m4return_t	loadXlWb();
		m4return_t	releaseXlWb();
};


// -----------------------------------------------------------------------------
// Class ClWsheet
// -----------------------------------------------------------------------------
class ClWsheet
{
	public:

		typedef enum { MSLIM_MAX_ROW, MSLIM_MAX_COL } eMSLimit_t;

		ClWsheet( ClWbook* ai_pWbook, int ai_iIdx );
		~ClWsheet();

		bool		IsEqual( ClWsheet* ai_pWsheet, int ai_iFR, int ai_iFC, int ai_iLR, int ai_iLC, char* ai_pcIgnoreList );
		ClWbook*	GetWbook();
		bool		GetIsLoaded();
		Sheet*		GetXlWs();

	private:

		ClWbook*	m_pWbook;
		int			m_iIdx;
		bool		m_bIsLoaded;
		bool		m_bWritePending;
		Sheet*		m_pXlWs;

		bool		outMSLimit( eMSLimit_t ai_iLimit, int ai_iVal );
		void		dbgTouchCell( int ai_iRow, int ai_iCol, char* ai_pcMsg );
		void		dbgSaveWb();
		void		dbgInitTrace();
		void		dbgWriteTrace( m4pcchar_t ai_pccFormat, ... );
		bool		iteStatEndCond( int ai_iIdx, int ai_iLast, eMSLimit_t ai_iLimit );
		m4return_t	loadXlWs();
		int			parseCellList( char* ai_pcCells, CellPosSet_t& ao_setCells);
};


// -----------------------------------------------------------------------------
// Class ClCell
// -----------------------------------------------------------------------------
class ClCell
{
	public:

		typedef	enum eBorderEdges { BDR_EDGE_TOP, BDR_EDGE_RIGHT, BDR_EDGE_BOTTOM, BDR_EDGE_LEFT } eBorderEdges_t;

		ClCell( ClWsheet* ai_pWsheet );
		~ClCell();

		void	Set( int ai_iRow, int ai_iCol );
		bool	operator==( const ClCell& ai_Cell );
		bool	operator!=( const ClCell& ai_Cell );
		bool	IsEmpty();

	private:

		ClWsheet*		m_pWsheet;
		int				m_iRow;
		int				m_iCol;
		const wchar_t*	m_pwcComment;
		CellType		m_iType;
		Format*			m_pFormat;
		BorderStyle		m_aiBorder[ 4 ];
		FillPattern		m_iPattern;
		Color			m_iForeground;
		Color			m_iBackground;
};


// -----------------------------------------------------------------------------
// Class ClWbook
// -----------------------------------------------------------------------------
class ClDeleteThem
{
	public:
		
		ClDeleteThem();
		~ClDeleteThem();

		ClWbook*	m_pWbook1;
		ClWbook*	m_pWbook2;
		ClWsheet*	m_pWsheet1;
		ClWsheet*	m_pWsheet2;
};

#endif
