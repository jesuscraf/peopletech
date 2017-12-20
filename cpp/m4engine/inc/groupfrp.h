//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             groupfrp.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition: VIew word documment attached
//
//
//==============================================================================

#ifndef __GROUPFRP_H__
#define __GROUPFRP_H__

//Para el multimap
#include "m4stl.hpp"
#include "clfile.h"
#include "clstr.h"
#include "m4ipcs.hpp"
#include "formattp.h"

#include "pi.h"
#include "physfrp.h"


class ClENG_ReportOuputRepository ;
class ClENG_GlobalGroup ;
class ClENG_BreakGroup ;
class ClENG_BreakList ;
class M4ClBuffer ;
class ClENG_Expression ;

//=================================================================ClENG_GlobalGroup

class ClENG_GlobalGroup
{
	friend class ClENG_ReportOuputRepository ;
	friend class ClENG_BreakGroup ;
MIT_PRIVATE:
	
	ClENG_ReportOuputRepository *m_poOwner ;	//Link al owner

	StMIT_Inspector* m_poInsp;	//Simple link

	ClENG_Path *m_oPhysPath ;	//Link, NULL if the physical directory is not created.
	ClMIT_Str   m_oRelativePath ; //Parameter in order to create m_oPhysPath

	struct StSecondFileList {
		//TRUE: this public file is common for all breaks (published for all)
		//FALSE: this public file is NOT common for all breaks (breaks holds references)
		m4bool_t m_bSwForAllBreaks ;
		ClENG_PhysicalFile *m_poFile ;
	} ;
	
	//key: FileName (without path) value: link to file
	//key is converted to lower, in order to avoid case-sensitive issues.
	typedef map<ClMIT_Str, StSecondFileList, less<ClMIT_Str> > FileList_t;
	FileList_t m_oFileList;

MIT_PRIVATE:
	
	//All methods are controlled by facade classes

	//---------------------------------------Initialize--------------------------------

	ClENG_GlobalGroup();
	virtual ~ClENG_GlobalGroup() { ClENG_GlobalGroup::End() ; }
	virtual void End() ;

	virtual size_t GetSizeof() { return sizeof(ClENG_GlobalGroup) ; }

	//The physical path is not created at Init time. The first calling to 
	//	GetGlobalReference will do it !
	
	m4bool_t Init( 
		ClENG_ReportOuputRepository *ai_poOwner,
		const m4char_t * ai_pcRelativePath,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	);


	//---------------------------------------Gets--------------------------------------

	const m4char_t *GetRelativePath() { return m_oRelativePath.InChar(0) ; }
	ClENG_ReportOuputRepository *GetOutputRepository() { return m_poOwner ;	}

	//---------------------------------------Flow--------------------------------------

	//ai_pcFileNameId is both the key for global reference, and the physical file name.
	//-If the reference exists, the link is returned and ao_SwCreated=FALSE. May be is
	//	open, may be not (depends on the use that the client does).
	//-If the reference doesn't exist, a new reference will be created. May be the file
	//	physically exists, may be not; if doesn't exist a new one empty file is created and
	//	ao_SwCreated=TRUE, or if it exists  ao_SwCreated=FALSE.
	//	The link  is returned, and the file remains in close state.
	//-A NULL is returned for error, and a error will be thrown.
	//-ao_SwCreated is optional.
	//-ai_bSwCommonForAllBreaks. Only is taking into account if the reference is created 
	//	(first call for this file). 
	//	-If it's false (not for all), and the first previous calling for this 
	//		file said was true 	(common for all), a true value (common for all) is forced.
	//	-If it's true (common for all) and a previous call said false (not for all), 
	//		a error will be thrown and a true is forced.
	//	-If after all, it's false (not for all), the m_oGlobalDependencies of the current 
	//		break will be updated (forced to true if there are not current break). 
	//		If it's true (common for all), nathing happens.

	ClENG_PhysicalFile * GetGlobalReference(
		const m4char_t *ai_pcFileNameId, 
		m4bool_t ai_bSwCommonForAllBreaks, 
		m4bool_t *ao_SwCreated) ;

	m4bool_t Remove() ;
} ;

//=================================================================ClENG_BreakGroup

class ClENG_BreakGroup
{
	friend class ClENG_ReportOuputRepository ;
	friend class ClENG_BreakList ;
	friend class ClENG_GlobalGroup ;

MIT_PUBLIC:

	enum STATE { CURRENT_BREAK, WAITING, CLOSED, REMOVED } ;
	
	typedef vector<ClENG_PhysicalFile *> MainDataList_t;

MIT_PRIVATE:

	STATE m_State ;
	
	StMIT_Inspector* m_poInsp;	//Simple link

	ClENG_BreakList *m_poOwner ;	//Link al owner

	//If PreparePublish was called, it has the publishId.
	//-1 (test with==) if PreparePublish was not called for this break.

	m4uint32_t m_uiPublishGroupId ;
	ClENG_Chn *m_poPublishChn ;	//NULL or link to object if m_uiPublishGroupId!=-1

	m4uint32_t m_uiBreakId ;
	ClENG_Path *m_oPath ;	//Link
	ClMIT_Str m_oPathMask ;

	//List to all main data files (currently in proccess included)
	MainDataList_t m_oMainDataList;

	//List for all global (NOT global to break, global to break), private for this break.
	//The total global files are the global files marked as CommonForAllBreaks and the
	//	global of this list.
	typedef map<ClENG_PhysicalFile *, ClENG_PhysicalFile *, less<ClENG_PhysicalFile *> > GlobalDependencies_t;
	GlobalDependencies_t m_oGlobalDependencies ;

	//key: FileId (String from Dvc Layer) value: link to file
	typedef map<ClMIT_Str, ClENG_PhysicalFile *, less<ClMIT_Str> > GlobalToBreakList_t;
	GlobalToBreakList_t m_oGlobalToBreakList;


	//Main file for break (like main for report). Useful for clients, they can mark an 
	//	file as main for the break.
	//The marked file **MUST** states as a break file, may be as global to break, may be 
	//	as main data.
	//If there isn't main, the member will be set to NULL:
	//Mark a file as main can be useful in order to navigation purposes.

	ClENG_PhysicalFile *m_poMain ;	//Link

	//Flag run-time de break.
	//Reset to default at Init.

	m4bool_t m_bRT_DelBreakFlag ;
	
MIT_PRIVATE:
	//All methods are controlled by facade classes

	//---------------------------------------Initialize--------------------------------

	ClENG_BreakGroup();
	virtual ~ClENG_BreakGroup() { ClENG_BreakGroup::End() ; }
	virtual void End() ;

	virtual size_t GetSizeof() { return sizeof(ClENG_BreakGroup) ; }

	m4bool_t Init( 
		ClENG_BreakList * ai_poOwner,
		m4uint32_t ai_uiBreakId,
		ClENG_Path *ai_oPath,
		ClMIT_Str *ai_oAlternativeMaskName,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	);


	//---------------------------------------Gets--------------------------------------

	ClENG_BreakList *GetBreakList() { return m_poOwner ; } 
	ClENG_Path *GetPath() { return m_oPath ; }
	m4uint32_t GetBreakId() { return m_uiBreakId ; }

	STATE GetState() { return m_State ; }

	//Only for go-through purposes

	void GetMainDataIt( MainDataList_t::iterator &ao_ItBeg, MainDataList_t::iterator &ao_ItEnd ) 
		{ ao_ItBeg = m_oMainDataList.begin() ; ao_ItEnd = m_oMainDataList.end() ; }

	void GetRelativeMaskFromGlobalToBreakToFlush(ClMIT_Str *ao_Str)  ;

	//---------------------------------------Flow--------------------------------------

	//NULL if error 
	ClENG_PhysicalFile * CreateMainDataFile(const char *ai_pcFileName) ;


	//It creates a new one empty file (overwrite if exists) 
	//NULL if error (pe, Id exists.). Message will be thrown.
	//The real file name will be different from ai_pcFileId, it well be built with 
	//	ai_pcFileName and MaskName. Preserve modes are taking into account.
	//ai_pcFileId contains both, a part of file name, and the extension.

	ClENG_PhysicalFile * NewGlobalReference(const m4char_t *ai_pcFileId) ;

	//Get an existing reference. NULL if doesn't exist. A message will be thrown.
	ClENG_PhysicalFile * GetGlobalReference(const m4char_t *ai_pcFileId) ;

	//Close the break, a IsReadyToClose calling was done and the result was TRUE.
	//From WAINTING or CURRENT_BREAK state, changes to CLOSE status. For other states,
	//	nothing happens.
	//Proccess a close status: may be the main data files will be printed, may be all files
	//	will be published, may be interest will be declared for all files, anf may be the 
	//	break will be deleted (in this case, a change to REMOVED status occurs).
	//Returns false on error.
	
	m4bool_t CloseBreak(m4bool_t ai_bIsValidOuput) ;


	//From CURRENT_BREAK state, changes to WAITING state. For other states, nothing happens.
	void ChangeToWaiting() ;

	//From a CLOSED state, delete all files and directories for BREAK_DIR mode 
	//	(not delete the memory objects!), and changes to REMOVED state.
	//Forom other states, nothing happens.
	//Return false on error.

	m4bool_t RemoveBreak() ;

	//Only takes effect if there are not a previous calling to PreparePublishGroup 
	//	(m_uiPublishGroupId==-1)
	//Call to the methos channel in order to get the handler and the methos fill the 
	//	required data from channel. 
	//The object store a reference to the chnnel in order to execute the PublishFunction
	//To be called out od this module because the timing must be synchronized wit the
	//	channel data movements.

	void PreparePublishGroup(ClENG_Chn *ai_poChn, const m4char_t *ai_pcReportId) ;


	//Only takes effect if a previous calling to PreparePublishGroup was done 
	//	(m_uiPublishGroupId!=-1)
	//Publish all break files.

	m4bool_t PublishGroup() ;


	//---------------------------------Other set methods--------------------------------

	void  SetAsMainFile(ClENG_PhysicalFile *ai_File) { m_poMain=ai_File; }

	//See m_poMain; it can be NULL.
	ClENG_PhysicalFile * GetMainFile() { return m_poMain; }


	//Get the mask in order to access from a flush file to a global to break file. It is a 
	//	mask because contains the root path of the file name.
	//Example: FileId					= M4_Index.html
	//		   RelativeMask returned	= c:\temp\BRK_1\TEST_
	//		   REal file name			= c:\temp\BRK_1\TEST_M4_Index.html	

	const m4char_t * GetPathMask() { return m_oPathMask.InChar(0) ; }

	//Methods to manage the runtime DelBreak flag.
	//DelThisBreak take into account the UseDelBreak flag.
	m4bool_t IsDelBreak() { return m_bRT_DelBreakFlag ; }
	void SetDelBreak(m4bool_t ai_bDelBreak) { m_bRT_DelBreakFlag=ai_bDelBreak ; }
	m4bool_t DelThisBreak() ;
	


MIT_PRIVATE:

	//May be the file exists, may be not, but you should open it with OVERWRITE, W or R_W mode.
	//The returned file is returned in RELATIVE to the break path flavour.
	//The PRESERVE_MOVE_NAME mode is banned, all names must share the same root name in order 
	// to template replace issues.

	m4bool_t NewGlobalToBreakFile(
		ClMIT_Str *ao_Path, m4bool_t * ao_bExist, const m4char_t *ai_pcFileName
	)  ;

	
	//Get the PathMask, but in a relative flavour, in order to access from 
	//	a global to break to a flush.

	m4bool_t ComputeRelativeMask(ClMIT_Str *ai_oAlternativeMaskName) ;

	//true if has some file openned or if any file has a hanging PI
	//From state CLOSED or REMOVED, it is false.
	//From state CURRENT_BREAK or WAITING, it has to check.

	m4bool_t HasSomethingPending() ;


	//Test if a CloseBreak is suitable. This doens't change the status, the calling to 
	//	CloseBreak is necesary.
	//From WAITING or CURRENT_BREAK:
	//	- NOT HasSomethingPending. 
	//		-A change from CURRENT_BREAK --> CLOSED is suitable, TRUE is returned.
	//		-A change from WAITING       --> CLOSED is suitable, TRUE is returned.
	//	- HasSomethingPending. 
	//		-A change from CURRENT_BREAK --> WAITING is suitable, but not to close. 
	//											FALSE is returned.
	//		-The WAITING status remain.
	//ai_SwOk is set to FALSE on error. A error rise if the WAITING state is choosen and the 
	//	ai_bSwDelThisBreak is true.  

	m4bool_t IsReadyToClose(m4bool_t *ai_SwOk) ;


	void GetFileList(ClENG_BreakGroup::MainDataList_t *ai_List, 
		m4bool_t ai_bSwMainDataFiles, m4bool_t ai_bGloablToBreakFiles,
		m4bool_t ai_bGlobalFiles ) ;

} ;



//=================================================================ClENG_BreakList

class ClENG_BreakList
{
	friend class ClENG_ReportOuputRepository ;
	friend class ClENG_BreakGroup ;
	friend class ClENG_GlobalGroup ;

MIT_PUBLIC:

	enum BREAK_MODE { BREAK_TO_FILE, BREAK_TO_DIR } ;
	enum DUPLEX_MODE { SHARING_DUPLEX, EVEN_ODD_DUPLEX, ATOMIC_DUPLEX } ;

MIT_PRIVATE:
	
	StMIT_Inspector* m_poInsp;	//Simple link
	ClENG_ReportOuputRepository *m_poOwner ;	//link

	//General flow modifiers

	BREAK_MODE	m_BreakMode ;
	DUPLEX_MODE m_DuplexMode ;

	//Name file modifiers

	m4int16_t m_iNumOfSecChars ;
	m4int16_t m_iNumOfPageChars ;
	m4char_t  m_cSeparator ; //0, doesn't apply
	m4char_t  m_cBreakSeparator ; //0, doesn't apply
	ClMIT_Str m_oExtension ;

	//Break output management

	m4int16_t m_iOffset ;
	m4bool_t m_bUseDelBreak ;
	m4bool_t m_bDefaultDelBreak ;
	m4bool_t m_bConsecutiveBreaks ;

	//Real flush layout count, may be equals to parameterized LayotPages, or may be equals to
	//	parameterized LayotPages plus one phantom page (for EVEN_ODD_DUPLEX mode, with a greater 
	//	than one and odd LayoutPageCount).
	m4int16_t m_PageLayoutCount ;
	m4bool_t  m_bPhantomPage ;

	//key: BreakId, value: BreakGroup object
	typedef map<m4uint32_t, ClENG_BreakGroup *, less<m4uint32_t> > BreakList_t;
	BreakList_t m_oBreakList;

	//Links to waiting state breaks
	typedef list<ClENG_BreakGroup *> WaitingBreaks_t ;
	WaitingBreaks_t m_oWaitingBreaks ;

	//Link (NULL if there are not any open)
	ClENG_BreakGroup * m_poCurrentBreak ;

	//Vector to in-proccess Flush (the referenced files are also included in m_oMainDataList).
	//Index is equal to FlushLayoutPage.
	//The redirect list has the index to the redirected page (for index i, a value of i is for
	// no-redirected, another value is for redirected).
	//m_oRedirectList is always the same, m_oCurrentFlush can contains NULL if there is not
	//	a break openned.
	//m_oPrintedPages stores the real count for pages printed in the file. The way to access the
	//	data it's the same that CurrentFlush, it uses RedirectMechanism.
	//m_SharingFlushPages stores for each FlushPage (the index), how many others pages
	//	are sharing the same file (the FluhPageIndex counts!).


	ClENG_PhysicalFile ** m_oCurrentFlush ;
	m4int16_t *m_oRedirectList;
	m4uint32_t *m_oPrintedPages ;
	m4uint32_t *m_SharingFlushPages ;

	//Aux proccess

	m4uint32_t m_uiCurrentBreakSequence ; //Count for file-names (++  in NewBreak)
	m4uint32_t m_uiPhysicalPage ;	//Count for file-names in ATOMIC_DUPLEX mode (++ in CloseFlush).
	m4uint32_t m_uiPhysicalPagePerBreak ;	//Same above, but reset by break (++ in CloseFlush, =0 in NewBreak   )
	m4uint32_t m_uiDeletedBreaks ;	//Count for deleted breaks (useful with consecutive breaks) 
	m4uint32_t m_uiPublishedBreaks ;	//Count for published breaks 

	//  UNICODE REPORTS
	m4bool_t m_bIsUTF16;


MIT_PRIVATE:
	//All methods are controlled by facade classes

	//---------------------------------------Initialize--------------------------------

	ClENG_BreakList();
	virtual ~ClENG_BreakList() { ClENG_BreakList::End() ; }
	virtual void End() ;

	virtual size_t GetSizeof() { return sizeof(ClENG_BreakList) ; }

	m4bool_t Init( 
		
		ClENG_ReportOuputRepository *ai_poOwner,

		//General flow modifiers

		BREAK_MODE	ai_BreakMode, 
		DUPLEX_MODE ai_DuplexMode,

		//Name file modifiers

		m4int16_t ai_iNumOfSecChars,
		m4int16_t ai_iNumOfPageChars,
		m4char_t  ai_cSeparator,
		m4char_t  ai_cBreakSeparator,
		const m4char_t *ai_pcExtension,

		//Break output management

		m4int16_t ai_iOffset,
		m4bool_t ai_bUseDelBreak,
		m4bool_t ai_bDefaultDelBreak,
		m4bool_t ai_bConsecutiveBreaks,

		//Flush management
		m4int16_t ai_PageLayoutCount,

		// UNICODE REPORTS
		m4bool_t ai_bIsUTF16,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	);


	//---------------------------------------Gets--------------------------------------

	ClENG_ReportOuputRepository *GetOutputRepository() { return m_poOwner ;	}

	BREAK_MODE	GetBreakMode() { return m_BreakMode ; }
	DUPLEX_MODE GetDuplexMode() { return m_DuplexMode ; }

	m4bool_t IsRedirected(m4int16_t ai_iPage) { return m_oRedirectList[ai_iPage]!=ai_iPage ; } 

	m4int16_t GetRealPageLayoutCount() { return m_PageLayoutCount ; }
	m4bool_t  IsPhantomPage() { return m_bPhantomPage ; }

	//Doesn't includes deleted breaks
	m4uint32_t GetBreakCount() { return  m_oBreakList.size()-m_uiDeletedBreaks ; }

	m4uint32_t GetPublishedCount() { return  m_uiPublishedBreaks; }

	//Includes deleted breaks
	m4uint32_t GetHardBreakCount() { return  m_oBreakList.size() ; }
	
	m4uint32_t GetDeletedBreakCount() { return  m_uiDeletedBreaks ; }

	m4bool_t IsUseDelBreak() { return m_bUseDelBreak ; }
	m4bool_t DefaultDelBreak() { return m_bDefaultDelBreak ; }

	//NULL if there aren't any not deleted group
	ClENG_BreakGroup *GetFirstNotDeleted() ;

	//---------------------------------------Flow---------------------------------------

	//For SHARING_DUPLEX & EVEN_ODD_DUPLEX, this open new flush files 
	//	(how much files and redirection issues depends on sharing mode). 
	//	In this case, ao_bNewFlushFilesOpenned (if it's not null) will be set to TRUE.

	m4bool_t NewBreak(m4bool_t *ao_bNewFlushFilesOpenned=NULL) ;

	//For SHARING_DUPLEX & EVEN_ODD_DUPLEX, this closes any flush file openned. 
	//	In this case, a calling to the function IsGoingToCloseFlushSet() BEFORE the calling to
	//	CloseBreak will return TRUE (the calling is made before because usually you want to
	//	update the files before they will be closed).
	//This calling change the break to a CLOSED or WAITING status, and test for other breaks
	//	int WAITING status; if now they are ready to close, they will be closed. When a break
	//	closes, all print, publish, notify and remove issues are managed.

	m4bool_t CloseBreak(m4bool_t ai_bIsValidOuput) ;

	//For ATOMIC mode, open new flush files
	//	In this case, ao_bNewFlushFilesOpenned (if it's not null) will be set to TRUE.
	m4bool_t NewFlush(m4bool_t *ao_bNewFlushFilesOpenned=NULL) ;

	//ai_poBuffer is an array of m_PageLayoutCount items, it contains M4ClBuff references. 
	//	This method updates the PI list and flush the buffers to the file. 
	//	The buffers MUST be updated.
	//For ATOMIC mode, close the flush files
	//	In this case, a calling to the function IsGoingToCloseFlushSet() BEFORE the calling to
	//	CloseBreak will return TRUE (the calling is made before because usually you want to
	//	update the files before they will be closed).
	m4bool_t CloseFlush(M4ClBuffer **ai_poBuffArray) ;

	//For SHARING_DUPLEX & EVEN_ODD_DUPLEX, the flush files set changes every break loop, for
	//	ATOMIC mode this changes every flush loop.

	ClENG_PhysicalFile * GetFlushFile(m4int16_t ai_iFlushPage) 
		{ return m_oCurrentFlush[ai_iFlushPage] ; }

	//TRUE if ai_iFlushPageA & ai_iFlushPageB share file.

	m4bool_t ShareFileWith(m4int16_t ai_iFlushPageA, m4int16_t ai_iFlushPageB) 
		{ return m_oRedirectList[ai_iFlushPageA]==m_oRedirectList[ai_iFlushPageB] ; } 

	//Get the printed pages into the file that ai_iFlushPage belongs to. It is the real 
	//	count for the file, it depends on redirected issues. The value updates in the
	//	CloseFlush method.
	//For example, if we have 4 pages, and 2 files ("Even.txt", "Odd.txt"), and the pages 
	//	0-2 flushes dato to "Even.txt", the 1-3 flushes to "Odd.txt", and there was 3
	//	CloseFlush() callings, a GetPrintedPages(0)==GetPrintedPages(2)==6 (the pages 
	//	printed into "Even.txt", 3 pages for flush page 0, and 3 more for flush page 2). 
	//Can be called for all states.

	m4uint32_t GetPrintedPages(m4int16_t ai_iFlushPage) 
		{ return m_oPrintedPages[ m_oRedirectList[ai_iFlushPage] ] ; }

	//Get how many flush pages sharing a same file. The file is the file where 
	//	ai_FlushPage flushes.
	//Taking  into account the even-odd above example, a 
	//	GetHowManyPagesSharingFile(0)==GetHowManyPagesSharingFile(1)==2, because in the
	//  "Even.txt" flushes 2 pages. This value is always 2, it's not dependant on 
	//	NewFlush-CloseFlush issues.
	
	m4uint32_t GetHowManyPagesSharingFile(m4int16_t ai_iFlushPage) 
		{ return m_SharingFlushPages[ai_iFlushPage] ; }

	//This method is like GetPrintedPages, but the returned value takes into account 
	//	only the printed pages for the ai_iFlushPage. For example, a 
	//	GetPrintedPagesOnlyForPage(0)==3 because page 0 flushes 3 pages into the file 
	//	"Even.txt".

	m4uint32_t GetPrintedPagesOnlyForPage(m4int16_t ai_iFlushPage) 
		{ return	m_oPrintedPages[ m_oRedirectList[ai_iFlushPage] ] /
					m_SharingFlushPages[ai_iFlushPage] ; }

	//This method is like GetPrintedPagesOnlyForPage, but it returns 0 if the FlushPage
	//	doesn't shere file with the page ai_iFlushReference.

	m4uint32_t GetPrintedPagesOnlyForPage(
		m4int16_t ai_iFlushPage, m4int16_t ai_iFlushReference ) 
		{	return ShareFileWith(ai_iFlushPage,ai_iFlushReference) ?
					GetPrintedPagesOnlyForPage(ai_iFlushPage) : 0 ; }
		
	//Get the file-ordinal for the ai_iFlushPage page. For instance, the 0 layout page in
	//	the file "even.txt" has the 0 ordinal in this file, and the 2 layout page has the 
	//	1 ordinal in this file (1, because it follows to page 0 in "even.txt" file).

	m4int16_t GetOrdinalInFile(m4int16_t ai_iFlushPage) ;

	//See CloseBreak & CloseFlush
	m4bool_t IsGoingToCloseFlushSet() ;

	m4bool_t RemoveAllBreaks() ;

MIT_PRIVATE:

	//---------------------------------------Aux---------------------------------------

	//The returned path exists, it is created. It's returned in RELATIVE to the root flavour.
	m4bool_t NewBreakDirectory(ClMIT_Str *ao_Path) ;

	//May be the file exists, may be not, but you should open it with OVERWRITE, W or R_W mode.
	//The returned file is returned in RELATIVE to the root flavour.
	m4bool_t NewBreakFile(ClMIT_Str *ao_Path, m4bool_t *ao_bExist, m4int16_t ai_iFlushPage)  ;

	m4bool_t GetNumOfSecCharsAddString( ClMIT_Str *ao_Str ) ;
	m4bool_t GetDuplexAddString( ClMIT_Str *ao_Str, m4int16_t ai_iNPage ) ;

	m4bool_t OpenFlushFiles() ;
	void CloseFlushFiles() ;

	//See GetNavigationPathMask at  ClENG_ReportOuputRepository

	void GetNavigationPathMask(ClMIT_Str *ao_Str, m4bool_t ai_bOnlyMainMode) ;

	static void CollatePathMask(ClMIT_Str *ao_Mask, ClMIT_Str *ai_File) ;

} ;

//=================================================================ClENG_ReportOuputRepository

class ClENG_ReportOuputRepository
{
	friend class ClENG_BreakList ;
	friend class ClENG_BreakGroup ;
	friend class ClENG_GlobalGroup ;
	friend class ClENG_GlobalGroup ;
MIT_PUBLIC:

	enum PRESERVE_MODE { NO_PRESERVE, PRESERVE_FIX_NAME, PRESERVE_MOVE_NAME, PRESERVE_DIR } ;

MIT_PRIVATE:
	
	StMIT_Inspector* m_poInsp;	//Simple link

	enum STATE { NOT_INIT, OUT_OF_BREAK, OUT_OF_FLUSH, PROCESSING_FLUSH } ;
	STATE m_State ;

	DECLARE_EXTERNAL_FILE m_DeclareExternalMode ;
	
	//Class

	ClENG_PhysicalFileSet m_oFileSet ;
	ClENG_GlobalGroup m_oGlobalGroup ;
	ClENG_BreakList m_oBreaks ;

	ClMIT_Str m_oMaskName ;

	//File expression object for break output
	ClENG_Expression* m_oFileExpressionObj ;

	//General flow modifiers

	m4bool_t	m_MonoBreak ;
	PRESERVE_MODE m_PreserveMode ;

	//PI manager

	ClENG_PI_Manager m_oPI_Manager ;

	//Main file for report. Useful for clients, they can mark an file as main for the report.
	//The marked file **MUST** states as a global group file.
	//If there isn't main, the member will be set to NULL:
	//Mark a file as main can be useful in order to navigation purposes.

	ClENG_PhysicalFile *m_poMain ;	//Link

	//Useful for GetNavigationPathMask. "" is equal to inactive
	
	ClMIT_Str m_oWebPath ;

	ClMIT_Str  m_oPrinter ;
	m4bool_t m_bDeleteOnExit ;

MIT_PUBLIC:

	//Aux

	static ClMutex m_oMutexPreserveMove ;


MIT_PUBLIC:

	//---------------------------------------Initialize--------------------------------

	ClENG_ReportOuputRepository();
	virtual ~ClENG_ReportOuputRepository() { ClENG_ReportOuputRepository::End() ; }

	//Execute the RemoveOutput if m_bDeleteOnExit
	virtual void End() ;

	virtual size_t GetSizeof() { return sizeof(ClENG_ReportOuputRepository) ; }

	m4bool_t Init( 
		
		//General flow modifiers

		m4bool_t ai_MonoBreak,
		ClENG_BreakList::BREAK_MODE ai_BreakMode,
		ClENG_BreakList::DUPLEX_MODE ai_DuplexMode,
		
		//Root Path modifiers
		
		const m4char_t * ai_pcPathMask,
		m4bool_t ai_bIsFile,
		m4bool_t ai_bCreateDir,

		//File expression for break output
		const m4char_t * ai_pcFileExpression,
		// Acceso al canal de datos para evaluación de expresiones
		const ClMIT_Chn * ai_poChnData,
		
		//PRESERVE_DIR is a root path modifier, other modes are file name modifiers

		PRESERVE_MODE ai_PreserveMode,
		
		//Name file modifiers

		m4int16_t ai_iNumOfSecChars,
		m4int16_t ai_iNumOfPageChars,
		m4char_t  ai_cSeparator,
		m4char_t  ai_cBreakSeparator,
		const m4char_t *ai_pcExtension,

		//Break output management

		m4int16_t ai_iOffset,
		m4bool_t ai_bUseDelBreak,
		m4bool_t ai_bDefaultDelBreak,
		m4bool_t ai_bConsecutiveBreaks,
		const m4char_t *ai_pcToPrinter,

		//Flush management

		m4int16_t ai_PageLayoutCount,

		//Report output management

		m4bool_t ai_bDeleteOnExit,
		DECLARE_EXTERNAL_FILE ai_DeclareExternalMode,

		//Other specifics purposes

		const m4char_t *ai_pcWebPath, 

		// UNICODE REPORTS
		m4bool_t aiUTF16_BOM,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	);

	//---------------------------------------Gets--------------------------------------
	
	void GetRootPath( ClMIT_Str *ao_Str ) { m_oFileSet.GetRootPath(ao_Str) ; }

	void GetMaskName( ClMIT_Str *ao_Str ) { ao_Str->StrCpy(m_oMaskName) ; }

	m4bool_t HasBreakFileExpression() { return m_oFileExpressionObj != NULL ; }

	enum EXPRESSION_RESULT { EXPRESSION_ERROR, EXPRESSION_OK, NO_EXPRESSION } ;
	//Resuelve la expresión asociada al nombre del archivo.
	EXPRESSION_RESULT ResolveExpressionFile(ClMIT_Str *ao_ExpressionResult);

	m4bool_t IsMonoBreak() { return m_MonoBreak ; }
	PRESERVE_MODE GetPreserveMode() { return m_PreserveMode ; }

	ClENG_BreakList::BREAK_MODE	GetBreakMode() { return m_oBreaks.GetBreakMode() ; }
	ClENG_BreakList::DUPLEX_MODE GetDuplexMode() { return m_oBreaks.GetDuplexMode() ; }

	m4int16_t GetRealPageLayoutCount() { return m_oBreaks.GetRealPageLayoutCount() ; }
	m4bool_t  IsPhantomPage() { return m_oBreaks.IsPhantomPage() ; }

	//Gets a path mask with info in order to navigate through the files. This string doesn't
	//	have information valuable for this DLL, it's useful for VB messages.
	//Gets the path taking into account the files created. The returned value can be different
	//	at different calling times.
	//This works this way:
	//	-There is a main file at report level?
	//		-YES. This file path is returned.
	//		-NO. There is at most 1 break ?
	//			-NO. "" is returned.
	//			-YES. Has the first break a main file ?
	//				-YES. There is only a break ?
	//					-YES. The main break file is returned.
	//					-NO. For all breaks with main file, all its main files are compared in 
	//						order to make a path mask (fe : "c:\temp\reports\test*\main.html" )
	//				-NO. All data files for all breaks are taking into account in order 
	//						to make a path mask (fe : "c:\temp\reports\test*.pcl" )
	//Before the output will be returned, if there are a WebPath specified, the ouput mask
	//	will be tranlated to the web path form: the webpath string will be extracted from the
	//	begin of the path, an all \ will be translated to /

	void GetNavigationPathMask(ClMIT_Str *ao_Str) ;

	//It gets the navigation pathmask for the report root; like GetNavigationPathMask, 
	//	it can be the absolute report root, or the webpath form.

	void GetNavigationRootPathMask(ClMIT_Str *ao_Str) ;

	//Gets a relative path (NOT url) in order to access from a main flush file to a global
	//	file. It's NOT necessary a flush exits. The path ends with \ (if it's not "")

	void GetRelativePathFromFlushToGlobal( ClMIT_Str *ao_poStr ) ;

	//Gets a relative path (NOT url) in order to access from a main flush file to a global
	//	to break file. It's NOT necessary a break exits The path ends with \ (if it's not "")

	void GetRelativePathFromFlushToGlobalToBreak( ClMIT_Str *ao_poStr ) ;


	//See ClENG_BreakGroup.
	//Gives the value for the current break; false and "" if there aren't.

	m4bool_t GetRelativeMaskFromGlobalToBreakToFlush( ClMIT_Str *ao_poStr ) ;

	//See ClENG_BreakGroup, executes on current break.
	m4bool_t GetMainDataIt( 
		ClENG_BreakGroup::MainDataList_t::iterator &ao_ItBeg, 
		ClENG_BreakGroup::MainDataList_t::iterator &ao_ItEnd ) ;


	m4bool_t IsUseDelBreak() { return m_oBreaks.IsUseDelBreak() ; }
	m4bool_t DefaultDelBreak() { return m_oBreaks.DefaultDelBreak() ; }

	//---------------------------------Flow methods--------------------------------------

	//See ...........
	
	m4bool_t NewBreak(m4bool_t *ao_bNewFlushFilesOpenned=NULL) ;
	m4bool_t CloseBreak(m4bool_t ai_bIsValidOuput) ;

	m4bool_t NewFlush(m4bool_t *ao_bNewFlushFilesOpenned=NULL) ;
	m4bool_t CloseFlush(M4ClBuffer **ai_poBuffArray) ;

	m4bool_t IsGoingToCloseFlushSet() { return m_oBreaks.IsGoingToCloseFlushSet(); }

	m4uint32_t GetPrintedPages(m4int16_t ai_iFlushPage) 
		{ return m_oBreaks.GetPrintedPages(ai_iFlushPage) ; }

	m4int16_t GetOrdinalInFile(m4int16_t ai_iFlushPage) 
		{ return m_oBreaks.GetOrdinalInFile(ai_iFlushPage) ; }

	m4uint32_t GetPublishedBreakCount() { return  m_oBreaks.GetPublishedCount(); }

	m4bool_t ShareFileWith(m4int16_t ai_iFlushPageA, m4int16_t ai_iFlushPageB) 
		{ return m_oBreaks.ShareFileWith(ai_iFlushPageA, ai_iFlushPageB) ; } 

	m4uint32_t GetHowManyPagesSharingFile(m4int16_t ai_iFlushPage) 
		{ return m_oBreaks.GetHowManyPagesSharingFile(ai_iFlushPage)  ; }

	m4uint32_t GetPrintedPagesOnlyForPage(m4int16_t ai_iFlushPage) 
		{ return m_oBreaks.GetPrintedPagesOnlyForPage(ai_iFlushPage) ; }

	m4uint32_t GetPrintedPagesOnlyForPage(
		m4int16_t ai_iFlushPage, m4int16_t ai_iFlushReference ) 
		{	return m_oBreaks.GetPrintedPagesOnlyForPage(ai_iFlushPage, ai_iFlushReference) ; }

	//..........ClENG_BreakList

	//Returns NULL on error.
	//If ao_bRedirected is not NULL, it will be updated with the redirected flag.

	ClENG_PhysicalFile * GetFlushFile(m4int16_t ai_iFlushPage, m4bool_t *ao_bRedirected=NULL) ;

	//See ClENG_GlobalGroup::GetGlobalReference
	//Returns NULL on error.
	ClENG_PhysicalFile * GetGlobalReference(
		const m4char_t *ai_pcFileNameId, m4bool_t ai_bSwCommonForAllBreaks, 
		m4bool_t *ao_SwCreated=NULL) ;

	//See ClENG_BreakGroup::NewGlobalReference
	//Returns NULL on error.
	ClENG_PhysicalFile * NewGlobalToBreakReference( 
		const m4char_t *ai_pcFileId) ;

	//See ClENG_BreakGroup::GetGlobalReference
	//Returns NULL on error.
	ClENG_PhysicalFile * GetGlobalToBreakReference(const m4char_t *ai_pcFileId) ;

	//See ClENG_BreakGroup::PreparePublishGroup
	//If there aren't current break it returns FALSE. 
	m4bool_t PreparePublishGroup(ClENG_Chn *ai_poChn, const m4char_t *ai_pcReportId) ;

	//See ClENG_BreakGroup::IsDelBreak
	m4bool_t IsDelBreak() ;

	//See ClENG_BreakGroup::SetDelBreak
	void SetDelBreak(m4bool_t ai_bDelBreak) ;

	//-------------------------Bridge to PI methods--------------------------------------

	StPi_t * AddNewPi( 
		const char *ai_pcIdPI, m4int16_t ai_iPage, m4uint32_t ai_Offset, size_t ai_NBytes) ;

	m4bool_t ExtractPi( const char *ai_pcIdPI, StPi_t *ao_poData ) ;

	StPi_t * FindPi( const char *ai_pcIdPI, m4bool_t ai_bSwErrorIfNotFound=M4_FALSE) ;

	void GetPiIterators( 
		ClENG_PI_Manager::PI_List_t::iterator &ao_Beg, 
		ClENG_PI_Manager::PI_List_t::iterator &ao_End )  ;

	//---------------------------------Other set methods--------------------------------

	void  SetAsMainFile(ClENG_PhysicalFile *ai_File) { m_poMain=ai_File; }

	//See m_poMain; it can be NULL.
	ClENG_PhysicalFile * GetMainFile() { return m_poMain; }

	void  SetAsMainFileForCurrentBreak(ClENG_PhysicalFile *ai_File) ;

MIT_PRIVATE:
	//---------------------------------Aux methods--------------------------------------

	//Build the directory if it's necesary
	static m4bool_t GetRootPath( ClMIT_Str *ao_RootPath, ClMIT_Str *ao_NameMask,
		const m4char_t * ai_pcPathMask, m4bool_t ai_bIsFile, m4bool_t ai_bCreateDir,
		PRESERVE_MODE ai_PreserveMode )  ;

	static const char * GetTraceString(STATE ai_State) ;

	//Remove all cretaed files (use RemoveBreak)
	m4bool_t RemoveOuput() ;

};



#endif
