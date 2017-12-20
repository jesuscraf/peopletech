
#include "cldates.hpp"
#include "cllgadap.hpp"

#include "medvarin.hpp"

#include "clldb.hpp"
#include "clload.hpp"
#include "defmain.hpp"
#include "cllconn.hpp"
#include "cldinarr.hpp"
#include "cltblmem.hpp"
#include "clpersbs.hpp"
#include "clmtsent.hpp"

ClLoad::ClLoad (ClGlobVars *ai_pGlobVars, m4bool_t ai_bIsInternal, ClPersistBase *ai_pMainPB) : ClStmt (ai_pGlobVars, ai_bIsInternal)
{
//	m_eExtendedState = closed;
	m_InitCol = -1;
	
	m_EndCol = -1;

	m_bVerifyData = M4_FALSE;
	m_bVerifyDTApp = M4_FALSE;
	m_bVerifyDTCorr = M4_FALSE;
	m_bVerifyOperation = M4_FALSE;
	m_bAllowPreviousIni = M4_FALSE;
	m_eVerifyOrg = M4LDB_UNDEF_ORGANIZATION;
	m_indForLgcTbl = M4LDB_NULL_INDEX;
	m_pvcIndParVal = NULL;
	m_pAuxLoad = NULL;
	m_pAuxRS = NULL;
	m_pColumns = NULL;
	m_pMainPB = ai_pMainPB;
}

	
ClLoad::~ClLoad (void)
{
	Destroy ();
}

	
void ClLoad::InitStt (void)
{
	if (m_pAuxLoad)
		m_pAuxLoad->InitStt();

	ClStmt::InitStt();
}
	

m4return_t ClLoad::Open (m4int32_t ai_idRConn, ClMetaSentence *ai_pMtSent, m4bool_t ai_bMtSentOwner, ClDinArr *ai_pParam)
{
	m4return_t result;

	m4int32_t idRConn = ai_idRConn;

	ClDinArr *pParam = ai_pParam;

	if (ai_pMtSent) {
		if (idRConn == M4LDB_NULL_INDEX) {
			idRConn = m_pGlobVars->GetIdRConn ();
		}

		m4int32_t hTIStm = m_pGlobVars->GetpMCR ()->GetNodeReadSentenceHandle (m_pGlobVars->GethNode ());
		
		if (hTIStm) {
			m4int32_t maxNumCol = m_pGlobVars->GetpMCR ()->GetSentenceMaxNumCol (hTIStm);

			m4int32_t numCol = ai_pMtSent->GetNumItemCol ();


			if ((maxNumCol) &&(numCol > maxNumCol))
			{
				if (m_InitCol == -1){
					m_InitCol =0;
					m_EndCol = maxNumCol;
				}
				else{
					if (m_InitCol+maxNumCol<=numCol){
						m_EndCol=m_InitCol+maxNumCol;
					}
					else{
						m_EndCol=numCol;
					}
				}
				if (m_EndCol<numCol)
				{
					// Nos hace falta una Load auxiliar.
					m_pAuxLoad = new ClLoad (m_pGlobVars);
					
					m_pAuxLoad->m_InitCol = m_EndCol;

					result = m_pAuxLoad->Open (idRConn, ai_pMtSent, M4_FALSE, ai_pParam);

					if (result == M4_ERROR) {
						return (M4_ERROR);
					}

					pParam = m_pAuxLoad->GetpParams ();
				}
			}
		}
	}

	result = ClStmt::Open (M4LDB_SELECT, idRConn, ai_pMtSent, ai_bMtSentOwner, pParam);
	
	return (result);
}


m4return_t ClLoad::Defuse (ClLConn *ai_pLConn)
{
	m4return_t result;

	if (m_pAuxLoad) {
		result = m_pAuxLoad->Defuse (ai_pLConn);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	result = ClStmt::Defuse (ai_pLConn);

	return (result);
}


m4return_t ClLoad::Destroy (void)
{
	m4return_t result;

	if (m_pAuxLoad) {
		result = m_pAuxLoad->Destroy ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

    if (m_pAuxRS) {
        m_pAuxRS->Detach();
	}

	if (m_pvcIndParVal) {
		while (!m_pvcIndParVal->empty ()) {

			PStIndPar pIndPar = (PStIndPar) m_pvcIndParVal->back ();

			m_pvcIndParVal->pop_back ();

			delete pIndPar;
		}
		delete m_pvcIndParVal;
	}
	m_pvcIndParVal = NULL;

	if (m_pAuxRS) {
		delete m_pAuxRS;
	}
	m_pAuxRS = NULL;

	if ((m_pColumns) && 
		(!m_pAuxLoad)) {
		delete m_pColumns;
	}

	m_pColumns = NULL;
	if (m_pAuxLoad) {
		delete m_pAuxLoad;
	}
	m_pAuxLoad = NULL;

	result = ClStmt::Destroy ();

	return (result);
}


m4return_t ClLoad::Close (void)
{
	m4return_t result;

	if (m_pAuxLoad) {
		result = m_pAuxLoad->Close ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

    if (m_pAuxRS) {
        m_pAuxRS->Detach();
	}

	result = ClStmt::Close ();

	return (result);
}


m4return_t ClLoad::Prepare (string &ao_stStmSQL)
{
	m4return_t result;

	if (m_pAuxLoad){
		if (m_pAuxLoad->Prepare (ao_stStmSQL)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	ao_stStmSQL = M4LDB_EMPTY;
		
	//Bug 0081719
	//Tendremos un Flag que permitira la ordenacion de parametros en el traductor.
	//Si hay m_pAuxLoad significa que tenemos una sentencia repetida por limitación de columnas
	//La reordenacion de parametros solo es necesaria una vez, la primera.
	if (!m_pAuxLoad) {
		m_pMtSent->SetSortParamsColumnsLimitation (M4_TRUE);
	}

	result = m_pMtSent->TranslateSentence (ao_stStmSQL, m_InitCol, m_EndCol);

	m_pMtSent->SetSortParamsColumnsLimitation (M4_FALSE);

	if (result != M4_ERROR) {
		result = ClStmt::Prepare ((m4pchar_t) ao_stStmSQL.c_str ());
	}

	return (result);
}


m4return_t 
ClLoad::BindParam (m4bool_t ai_bUseOldValue)
{
	if (m_pAuxLoad) {
		if (m_pAuxLoad->BindParam ()!=M4_SUCCESS){
			return (M4_ERROR);
		}
	}
	else{
		if (m_pMtSent->CreateParams(m_pParam, ai_bUseOldValue)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	// Por simetría lo meto en todas. Mas adelante lo quitare, si merece la pena ( Habria que tocar el _BindParam)
	m_oVParam.reserve(10);
	if (m_pMtSent->CreateVirtualParams(m_oVParam,m_InitCol,m_EndCol)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (_BindParam(ai_bUseOldValue)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return (M4_SUCCESS);
}
m4return_t
ClLoad::_BindParam (m4bool_t ai_bUseOldValue)
{
	//Iteramos por el m_oVParam.
	m4int32_t numPar = m_oVParam.size();
	m4int32_t indPar;
	
	for (m4int32_t CurrentPar=0; CurrentPar<numPar; CurrentPar++) {
		indPar= m_oVParam[CurrentPar];
		// Chapuza para evitar el problema del INTERNAL_PAR en seguridad.
		if (ClStmt::BindParam (CurrentPar+ 1, indPar, (m4bool_t)((ai_bUseOldValue) && (!m_pParam->GetInfoIsOwn  (indPar))) ? M4_TRUE : M4_FALSE)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return (M4_SUCCESS);

}


m4return_t
ClLoad::_BindCol ()
{
	//Iteramos por el m_oVColumns.
	m4int32_t numCol = m_oVColumns.size();
	m4int32_t indCol;
	ClDinArr *pColumns = GetpColumns();
	for (m4int32_t CurrentCol=0; CurrentCol<numCol; CurrentCol++) {
		indCol= m_oVColumns[CurrentCol];

		// UNICODE DB
		if (BindResult (CurrentCol + 1, pColumns->GetDataPointerPointer(indCol), pColumns->GetMaxLenPointer(indCol), pColumns ->GetInfoM4Type (indCol),pColumns->GetLengthPointer (indCol), pColumns ->GetInfoPrecFld (indCol) + 1)!=M4_SUCCESS){
			return (M4_ERROR);
		}
	}
	return (M4_SUCCESS);
}


m4return_t
ClLoad::BindCol (void)
{
	// Si tenemos Load Auxiliar, ella hace el CreateColumns.
	if (m_pAuxLoad) {
		if (m_pAuxLoad->BindCol ()!=M4_SUCCESS){
			return (M4_ERROR);
		}
	}
	else{		
		if (m_pColumns==0){
			m_pColumns  = new ClDinArr (m_pGlobVars);
		}
		
		if (m_pMtSent->CreateColumns(m_pColumns)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	// Por simetría lo meto en todas. Mas adelante lo quitare, si merece la pena ( Habria que tocar el _BindCol)

	m_oVColumns.reserve(10);

	if (m_pMtSent->CreateVirtualColumns(m_oVColumns,m_InitCol,m_EndCol)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (_BindCol()!=M4_SUCCESS){
		return M4_ERROR;
	}

	return (M4_SUCCESS);
}


m4id_t ClLoad::GetIdLgcTbl (void) 
{
	if (m_pMainPB) {
		return (m_pMainPB->GetIdLgcTbl ());
	}

	return (M4LDB_EMPTY);
}


m4int32_t ClLoad::GetIndLgcTbl (void) 
{
	if (m_pMainPB) {
		return (m_pMainPB->GetIndLgcTbl ());
	}

	return (M4LDB_NULL_INDEX);
}


m4return_t ClLoad::ComposePK (string &ai_onPKString, string &ai_oiPKString)
{
	if (m_pMainPB) {
		return (m_pMainPB->ComposePK (ai_onPKString, ai_oiPKString));
	}

	return (M4_SUCCESS);
}

