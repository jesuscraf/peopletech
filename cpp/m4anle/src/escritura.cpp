// Escritura.cpp: implementation of the CEscritura class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "error.h"
#include "dinarray.h"
#include "pilaerrores.h"
#include "escritura.h"
#include "m4unicode.hpp"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEscritura::CEscritura(CString pfichero, CPilaErrores *pila, CMessages * pMessages)
{
	m_pFile   = NULL;
	m_NumError = 0;
	m_NombreFich = pfichero;
	m_enable = TRUE; //Por defecto esta habilitado para escritura
	m_pila = pila;
	m_pMessages = pMessages;
	m_firsterr = TRUE;
}

CEscritura::~CEscritura()
{
	Cierrfich();
}

short CEscritura::Inicial()
{
	if (Apertura(m_NombreFich))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


short CEscritura::Apertura(CString pfichero)
{
	// Si ya habia un fichero abierto
	Cierrfich();

	eUniFileEncodingType_t eEncoding = M4UniNative;
	m_pFile = M4Fopen( pfichero, M4UniWrite, eEncoding );

	// Si puede abrir el fichero para escritura
	if( m_pFile != NULL )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// Cierra el fichero abierto
void CEscritura::Cierrfich()
{
	if ( m_pFile != NULL )
	{
		fclose( m_pFile );
		m_pFile = NULL;
	}
}

void CEscritura::Escribe(int linea, CString texto, int codigo, CString objeto, CString padre, CString propiedad, CString propval)
{	
	//Se almacena el error en la pila
	if((m_pila) && (linea))
	{
		m_pila->AddError(linea, texto, codigo, objeto, padre, propiedad, propval);
	}

	++m_NumError;

	if(!m_pFile) //El fichero no ha llegado a abrirse
	{
		return;
	}

	if(!m_enable)// Si no esta habilitado no se escribe
	{
		return;
	}

	//Escribir en el fichero
	WriteToFile(linea,texto,objeto,padre,propiedad,propval);

}

void CEscritura::WriteToFile(int linea, CString texto, CString objeto, CString padre, CString propiedad, CString propval)
{	

	char buffer[50];
	CString clinea="";
	CString cresul="";
	CString cpadding = "        ";
	int iPadding;


	buffer[0] = 0;

	if (m_firsterr)
	{
		m_firsterr = FALSE;
		cresul = m_pMessages->TextMessage("201") + "\n";
		//cresul="Nºlínea       Error\n" ;
		fwrite(cresul, cresul.GetLength(), 1, m_pFile);
		cresul="_______       _____\n";
		fwrite(cresul, cresul.GetLength(), 1, m_pFile);
	}
	if (linea)
	{
		_itoa( linea, buffer, 10  );
		clinea = buffer;
		cresul = buffer;
		// Aseguramos que el texto quede alineado independiente del número de línea
		iPadding = cpadding.GetLength() - cresul.GetLength();
		if (iPadding > 0) cresul += cpadding.Left(iPadding);
		cresul = cresul + ":     " + texto + "\n";
	}
	else
	{
		cresul = "\n" + texto + "\n";
	}

	fwrite(cresul, cresul.GetLength(), 1, m_pFile);
}


void CEscritura::Enable(BOOL enable)
{
	m_enable = enable;
}

long CEscritura::GenFileFromPila()
{

	//devuelve num de errores malos
int num,i;
CError err;
long badError = 0;

	num = m_pila->GetCount();
	if(!num)
		return 0;

	for(i=1; i<=num; i++)
	{
		m_pila->GetError(i, &err);

		//gemma..Quitar msg vacios
		if (strcmp(err.m_texto,"") != 0)
		{
			WriteToFile(err.m_linea, err.m_texto, err.m_objeto, err.m_padre, err.m_propiedad, err.m_propval);
		}
		else
		{
			badError ++;
		}
	}

	return badError;
}

