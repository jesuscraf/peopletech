// Lectura.cpp: implementation of the CLectura class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lectura.h"

#include <stdlib.h>
#include <afx.h>
#include "m4stl.hpp"
#include "m4unicode.hpp"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLectura::CLectura()
{
	m_pFile = NULL;
	m_nCaract=0;
	m_nLineas=0;
	m_NuevaLinea=1;
}

CLectura::~CLectura()
{
	Cierrfich();
}

// Apertura del fichero, con el nombre del fichero
// con la ruta completa.

short CLectura::Apertura(const char *pfichero)
{
	// Si ya habia un fichero abierto
	Cierrfich();

	eUniFileEncodingType_t eEncoding = M4UniNative;
	m_pFile = M4Fopen( pfichero, M4UniRead, eEncoding );

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
void CLectura::Cierrfich()
{
	if ( m_pFile != NULL )
	{
		fclose( m_pFile );
		m_pFile = NULL;
	}
}

// Lectura de un caracter, devuelve 0 en caso de que
// sea fin de fichero.
short CLectura::Leecaract()
{
	if (m_Termina)
	{
		m_Caracter = fgetc( m_pFile );

		if ( m_Caracter == EOF )
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}


// Comprueba de que tipo es el caracter leeido
short CLectura::Compr(char caract, short tipo)
{
	switch (tipo)
	{
		// En el caso 1 es un blanco
	case ESPACIO_BLANCO:
		if (caract==' ')
			return 1;
		else
			break;
		// En el caso 2 es un retorno o nueva linea
	case CR_NL:
		if ((caract=='\n') || (caract=='\r'))
		{
			return 1;
		}
		else
			break;
		// En el caso 3 es un tabulador, retorno, nueva linea
		// o blanco
	case CR_NL_TAB_BLANCO:
		if ((caract=='\n') || (caract=='\r') )
		{
			return 1;
		}
		if ((caract=='\t') || (caract==' '))
			return 1;
		else
			break;

	// En el caso 4 es un tabulador
	case TAB:
		if (caract=='\t')
			return 1;
		else
			break;
	case TAB_BLANCO:
		if ((caract=='\t') || (caract==' '))
		{
			return 1;
		}
		else
			break;
	// En otro caso no es niguno de los anteriores
	default : return 0;
	}
	return 0;
}





// Comprueba de que tipo es el caracter leeido
short CLectura::Comprob(char caract, short tipo)
{
	switch (tipo)
	{
		// En el caso 1 es un blanco
	case ESPACIO_BLANCO:
		if (caract==' ')
			return 1;
		else
			break;
		// En el caso 2 es un retorno o nueva linea
	case CR_NL:
		if ((caract=='\n') || (caract=='\r'))
		{
			++m_nLineas;
			++m_NuevaLinea;
			return 1;
		}
		else
			break;
		// En el caso 3 es un tabulador, retorno, nueva linea
		// o blanco
	case CR_NL_TAB_BLANCO:
		if ((caract=='\n') || (caract=='\r') )
		{
			++m_NuevaLinea;
			++m_nLineas;
			return 1;
		}
		if ((caract=='\t') || (caract==' '))
			return 1;
		else
			break;

	// En el caso 4 es un tabulador
	case TAB:
		if (caract=='\t')
			return 1;
		else
			break;
	// En otro caso no es niguno de los anteriores
	default : return 0;
	}
	return 0;
}

// Lee hasta fin de linea o fichero.

short CLectura::Finalinea()
{
	short fin=1;
	m_Termina = 1;

	while ((Leecaract()) && (fin))
		{
			if (Comprob(m_Caracter,CR_NL))
			{
				fin = 0;
				m_Termina=0;
			}
		}

	if (fin)
	{
		m_Termina=1;
		return 0;
	}
	else
		return 1;
}


// Lectura de una palabra

CString CLectura::Lpalabra()
{
	
	
	CString cadena="";
	cadena.Empty();
	if(cadena.IsEmpty()==0)
	{
		cadena="";
		cadena.Empty();
	}
	//CString cadena="";

	m_Termina = 1;
		
	if (Leecaract()){

		// Estamos en el caso de que leemos
		// dos tabuladores seguidos o dos 
		// blancos seguidos y no es una
		// nueva linea.
		if ((!m_NuevaLinea) && ((Comprob(m_Caracter,ESPACIO_BLANCO)) || (Comprob(m_Caracter,TAB))) )
			m_Termina = 0;
		else
		{
		do {
			// Ignora todos los caracteres de blancos
			// Retorno, tabulador y cambio de línea
			// si todavía no ha leido ninguna cadena.
			if (!m_nCaract)
			{ 
				if (!Comprob(m_Caracter,CR_NL_TAB_BLANCO))
				{
					++m_nCaract;
					m_NuevaLinea=0;
					cadena=cadena + m_Caracter;
				}
			}
			else
			{
				if ((!Comprob(m_Caracter,CR_NL)) && !(Comprob(m_Caracter,ESPACIO_BLANCO)))
				{
					// Ignora todos los tabuladores
					// cuando se empieza con una nueva linea.

					if ( ( (Comprob(m_Caracter,TAB_BLANCO)) && (!cadena.IsEmpty()) ) || (!Comprob(m_Caracter,TAB)) )
					{
						cadena= cadena + m_Caracter;
						m_NuevaLinea=0;
					}
				}
				else
					m_Termina=0;
			}
		}while ((Leecaract()) && (m_Termina));
		}
	}
//	pcadena=cadena.GetBuffer(cadena.GetLength());
	return cadena;
}


CString CLectura::Palabra(const char *pfichero)
{

	CString palab = "";
	CString frase = "";
	char ult;
	CString izqu;
	CString dere;

	if (Apertura(pfichero))
	{
		do {

			palab=Lpalabra();
			ult=Lultimo();
			if (!palab.IsEmpty())
			{
				if (Comprob(ult,CR_NL))
				{
					izqu = palab.Left(1);
					dere = palab.Right(1);
					frase = frase + palab;
				}
				else
				{
					if (!(m_Termina))
						frase = frase + (palab + ult);
					else
						frase = frase + palab;
				}
			}
		} while (!Finfich());

	}

	Cierrfich();
	return frase;
}

