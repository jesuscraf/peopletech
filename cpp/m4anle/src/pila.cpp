// pila.cpp: implementation of the Cpila class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pila.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cpila::Cpila()
{
	
}

Cpila::~Cpila()
{
	// Destruimos la pila de objetos
	// antes de destruir la clase
	BorrEstr(m_DApila);

}

// Borramos la pila de objetos construida
void Cpila::BorrEstr(DinArray& m_DApila)
{
	pila* aux;
	void * lv_aux;

	// Nos posicionamos al inicio de la pila
	m_DApila.inicializa();

	do
	{
		// Pedimos el registro actual
		lv_aux = m_DApila.actual();

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Lo borramos
		delete aux;
		// Seguimos con el siguiente registro
	} while (m_DApila.next());


}

// Buscamos dentro de la pila para una posicion determinada
// si ha encontrado un End o no
int Cpila::BusEnd(int posicion)
{
	pila* aux;
	void * lv_aux;
	int padre=0;
	int li_aux=0;

	if (posicion >= 0)
	{
		// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo id_end que nos
		// indica si se ha encontrado o no un End.
		padre = aux->id_end;

		if (padre)
			// Devolvemos 0 si se ha encontrado un End
			return 0;
		else
		{
			// Recogemos el valor del registro nlinea que
			// nos indica el número de línea que se encuentra
			// el objeto.
			li_aux=aux->nlinea;
			// Devolvemos el número de línea del objeto.
			return li_aux;
		}
	}

	return li_aux;

}

// Buscamos el padre para una posición determinada de
// objeto.
int Cpila::DarPadre(int posicion)
{
	pila* aux;
	void * lv_aux;
	int padre=0;

	if (posicion >= 0)
	{
		// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo id_objpadre que nos
		// indica el padre del objeto.
		padre = aux->id_objpadre;
	}	
	// Devolvemos el padre encontrado.
	return padre;
	
}

// Nos devuelve el identificador de objeto para una
// posición determinada.
int Cpila::DarObjeto(int posicion)
{
	pila* aux;
	void * lv_aux;
	int padre=0;

	if (posicion >= 0)
	{
		// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo id_obj que nos
		// indica el identificador del objeto.
		padre = aux->id_obj;
	}

	// Devolvemos el id del objeto.
	return padre;
}

// Nos devuelve el nombre del objeto para una
// posición determinada.
CString Cpila::DarNomObjeto(int posicion)
{
	pila* aux;
	void * lv_aux;
	CString padre;

	if (posicion >= 0)
	{
		// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo nom_obj que nos
		// indica el nombre del objeto.
		padre = aux->nom_obj;
	}

	// Devolvemos el nombre del objeto encontrado.
	return padre;
}

// Nos devuelve el número de línea de un objeto para una
// posición determinada.
int Cpila::DarLineas(int posicion)
{
	pila* aux;
	void * lv_aux;
	int padre=0;

	if (posicion >= 0)
	{
		// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo nlinea que nos
		// indica la linea donde se encuentra el objeto.
		padre = aux->nlinea;
	}

	// Devolvemos el número de linea encontrada.
	return padre;
}

// Nos devuelve el valor de allobject de un objeto para una
// posición determinada.
short Cpila::DarAllObj(int posicion)
{
	pila* aux;
	void * lv_aux;
	short padre=0;

	if (posicion >= 0)
	{
	// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo allobject que nos
		// indica si el objeto tiene todos los objetos.
		padre = aux->allobject;
	}

	// Devolvemos el valor de allobject encontrado.
	return padre;
}

// Nos devuelve el valor de allpro de un objeto para una
// posición determinada.
short Cpila::DarAllPro(int posicion)
{
	pila* aux;
	void * lv_aux;
	short padre=0;

	
	if (posicion >= 0)
	{
		// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo allproperties que nos
		// indica si el objeto tiene todas las propiedades.
		padre = aux->allproperties;
	}
	// Devolvemos el valor que nos indica si tiene
	// todas las propiedades
	return padre;
}

// Nos devuelve el Path de objetos de un objeto para una
// posición determinada.
CString Cpila::DarPathobj(int posicion)
{
	pila* aux;
	void * lv_aux;
	CString patho="";

	
	if (posicion>= 0)
	{
		// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo path_obj que nos
		// indica si el path del objeto.
		patho = aux->path_obj;
	}

	// Devolvemos el paht del objeto.
	return patho;
}

// Nos devuelve el alias de un objeto para una
// posición determinada.
CString Cpila::DarPathalias(int posicion)
{
	pila* aux;
	void * lv_aux;
	CString patho="";

	if (posicion>= 0)
	{
		// Recogemos el registro para la posición determina
		lv_aux = m_DApila[posicion];

		// Lo convertimos a tipo pila
		aux = (pila *) lv_aux;

		// Recogemos el valor del registo nom_obj que nos
		// indica si el alias del objeto.
		patho = aux->nom_obj;
	}

	// Devolvemos el alias del objeto
	return patho;
}

// Modificamos el valor de id_end ya que hemos encontrado
// un end para la pospila determinada.
void Cpila::EncEnd(int pospila)
{
	pila* aux;
	void * lv_aux;

	// Recogemos el registro para la posición determina.
	lv_aux = m_DApila[pospila];

	// Lo convertimos a tipo pila
	aux = (pila *) lv_aux;

	// Le pasamos el valor que indica que hemos encontrado
	// un end.
	aux->id_end=1;

	// Modificamos de la pila dicho registro
	m_DApila.modifica(pospila,aux);
}

// Carga la estructura determinada que le pasamos en la
// pila.
void Cpila::CargEstr(pila* estruc)
{
	pila* aux;

	// Recogemos la estructura
	aux = estruc;
	// Se la añadimos
	m_DApila.anade ( aux );
}
//*****************
// No hace nada.
void Cpila::DarEstr(int pospila)
{
	pila* aux;
	void * lv_aux;

	lv_aux = m_DApila[pospila];

	aux = (pila *) lv_aux;
}