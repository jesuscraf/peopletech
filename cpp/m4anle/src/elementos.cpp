// Elementos.cpp: implementation of the CElementos class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dinarray.h"
#include "lectura.h"
#include "runtime.h"
#include "error.h"
#include "pilaerrores.h"
#include "escritura.h"
#include "pila.h"
//#include "pilapro.h"
#include "elementos.h"
#include "compobjects.h"
#include "comptype.h"
#include "pilaobligatorios.h"
#include "utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TOPE_ERRORES	10000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElementos::CElementos(long max_error, short errorlevel, ICPropBag * pPBTypesrec, ICPropBag * pPBPropertiesgrouprec, ICPropBag * pPBObjectsgrouprec, ICPropBag * pPBObjectsrec, ICPropBag * pPBMessagesrec)
{		
	// Constructor de la clase CElementos aquí se
	// incializa las variables miembro para comenzar
	// el análisis
	m_padre= -1;	// Nodo padre	
	m_pila=0;		// Nivel de la pila.
	m_ObjPath="";	// Path de objetos
	m_AliasPath="";	// Path de alias

	// Comprobamos el número máximo de errores e 
	// inicializamos la variable tope
	if(!max_error)
		m_tope = TOPE_ERRORES;
	else
		m_tope=max_error;

	m_Errorlevel=errorlevel;

	m_Cpobjects.CargaPb(pPBObjectsrec, pPBObjectsgrouprec,pPBPropertiesgrouprec);
	m_CComType.cargapb(pPBTypesrec);
	m_Crun.Cargar(pPBObjectsrec, pPBPropertiesgrouprec);
	m_CMessages.Cargar(pPBMessagesrec);
	m_PilaOblig.Cargar(&m_Crun);
	
}

CElementos::~CElementos()
{

}

// Función para crear una nueva variable de tipo
// pila para ir añadiendo a la pila de objetos
void CElementos::CreaEstr(pila*& estruc)
{
	estruc = new pila;
}


// Método para determinar si el alias del objeto es correcto
short CElementos::CompAlias(CString palab, char &ult, CString &texterror)
{
	CString csaux="";

	csaux = palab;

	// Aqui podemos comprobar que el alias no se
	// repite entre hermanos

	while (!m_leer.Compr(ult,CR_NL))
	{
		palab = m_leer.Lpalabra();
		csaux = csaux + ult + palab;
		ult = m_leer.Lultimo();			
	}


//	if ((csaux.Find(' ')==-1) && (csaux.Find("\\")==-1) && (csaux.Find("/")==-1))
	if ((csaux.Find("\\")==-1) && (csaux.Find("/")==-1))
	{
		texterror = csaux;
		return 1;
	}
	else
	{
		texterror = " un espacio en blanco o \\ o /.";
		return 0;
	}

}


// Método para ir apilando los objetos
short CElementos::Apilar(int li_obj, CString alias, int linea, CString nombobj, short allprop, short allobj)
{
	pila *est_pila;
	short ls_resu=1;


	m_AliasPath= m_AliasPath + "/" + alias;

	CreaEstr(est_pila);

	est_pila->id_end = 0;
	est_pila->id_obj = li_obj;
	est_pila->id_objpadre = m_padre;
	est_pila->alias_objeto = alias;
	est_pila->path_obj=m_AliasPath;
	est_pila->nom_obj = nombobj;
	est_pila->nlinea = linea;
	est_pila->allproperties = allprop;
	est_pila->allobject = allobj;


	m_pilar.CargEstr(est_pila);


	ls_resu = m_PilaOblig.NewObjeto(nombobj,alias,m_pila, m_padre);
	if (li_obj>0)
	{
		// El error es cuando tengo más de uno objeto y tengo 
		// puesto un limite para el mismo.
		m_Cpobjects.IntroduOblig(li_obj,&m_PilaOblig,m_pila);
	}
	

	m_padre = m_pila;
	
	++m_pila;

	return ls_resu;

}


// Cuando no encontramos el objeto ni su alias lo
// apilamos
short CElementos::ApilarError()
{
	int li_ob=-1;
	CString csnom="ERROR";
	CString csali="ERROR";
	int lin_act=m_leer.Numlinea();
	short ls_resu=1;

	ls_resu=Apilar(li_ob,csali,lin_act,csnom,1,1);
	return ls_resu;

}


// Método principal para analizar el fichero.
long CElementos::Compilar(const char *pfichero,CPilaErrores *pilaerror, BOOL genlog, const char *pfilelog /* = NULL*/)
{


	// Palabra encontrada
	CString palab = "";
	// Palabra encontrada en mayusculas
	CString segpalab = "";
	// Alias que recibe el objeto
	CString csalias="";
	// Nivel en el que nos encontramos.
	int nivel = 1;
	// Texto del error producido
	CString tipoerror="";
	// Número de línea en que nos encontramos.
	int lin=0;
	// ID de la propiedad encontrada.
	int li_prop=0;
	// ID del objeto encontrado.
	int li_obje=0;
	// ID del objeto auxiliar.
	int aux_obje=0;
	// Posición del . en el fichero pasado a analizar.
	int posinc=0;
	// Texto del error producido al comprobar el tipo de la propiedad.
	CString texterror="";
	// Nombre de la propiedad encontrada.
	CString csnomprop="";
	// Nombre del objeto encontrado
	CString csnomobje="";
	// Número de errores encontrados
	long numerror=0;
	// Para saber en que linea nos encontramos
	int linea_actual = 0;
	// Variable auxiliar para operaciones
	int li_aux = 0;
	int li_aux2 = 0;
	CString csnomaux="";
	char csnomaux2[20];
	CString csnomaux3="";
	// Variable auxilar para decter que hay cadenas despues
	// del END o antes del BEGIN
	int li_errcad=0;
	// Variable para terminar la aplicacion
	int li_finaliza=0;

	// Estas variables son de la segunda fase
	tpprop s_prop;
	info_error st_info;
	short allprop=0;
	short allobj=0;
	short ls_ret=0;
	CString csaux="";
	CString ls_expresion="";
	BOOL lb_endvalidat=FALSE;
	CString nomb_objpadre="";
	int n_error=0;
	short li_resu=0;
	CString li_err;
	CString contnull;
	CHK_METODO st_chkmetodo;
	int li_objmet=0;
	int li_numetodos=0;
	CString ls_cadobj;
	int li_aux4=0;
	int li_errn = 0;


	if(pfilelog) //Se ha especificado un numbre de fichero para el .log
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		_splitpath( pfichero, drive, dir, fname, ext );
		m_NombreFich = drive;
		m_NombreFich += dir;
		m_NombreFich += pfilelog;
	}
	else //No se ha especificado fichero de salida se toma el fichero por defecto
	{
		m_NombreFich = pfichero;
		posinc= m_NombreFich.Find('.');
		++posinc;
		m_NombreFich = m_NombreFich.Mid(0,posinc);
		m_NombreFich= m_NombreFich + "log";
	}

	CEscritura	*error = new CEscritura(m_NombreFich, pilaerror, &m_CMessages);


	error->Enable(FALSE); //Los errores se escriben al final
	if(genlog)
		error->Inicial();

	// Apertura del fichero	
 	if (m_leer.Apertura(pfichero))
	{
		//procesamiento gnral de la presentacion
		ProcessPresentation(error,pilaerror);
		// Verificamos que los métodos y propiedades runtime son correctas.
		//!!!  SOLO DE MOMENTOOOO
		//AnalyzeRunTime(error);

		// Esto es cuando se termina y quedan objetos por cerrar.						
		if ((nivel>1) && ((error->Compabierto())!=m_tope))
		{
			li_aux=m_pila;
			for (int i=0;i<li_aux;++i)
			{
				li_aux2=m_pilar.BusEnd(i);
				if (li_aux2)
				{
					csnomaux = m_pilar.DarNomObjeto(i);
					if (csnomaux=="ERROR")
						csnomaux=" no identificado ";
					_itoa( li_aux2, csnomaux2, 10  );
					csnomaux3 = csnomaux2;
					//tipoerror = "Nº Error:23. El objeto "+csnomaux+" de la línea "+csnomaux3+" no tiene END.";
					if (i>0)
						nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(i));
					else
						nomb_objpadre ="";
					tipoerror = m_CMessages.TextMessage("23");
					tipoerror = MsgTexto (tipoerror,csnomaux,"",li_aux2,"","","23");

					error->Escribe(li_aux2,tipoerror,23,csnomaux,nomb_objpadre,"","");
				}				
			}
		}
		
	} // endif apertura.
	else
	{
		//tipoerror = "Se ha producido un error en la apertura del fichero.";		
		tipoerror = m_CMessages.TextMessage("202");
		error->WriteToFile(0,tipoerror,"","","","");
		// Se produce un error.
	}

	// Obtenemos el número de errores producidos durante el analisis.
	long num_err = ShowNumErrors(error, genlog);

	delete error;
	return num_err;

}

void CElementos::ProcessPresentation(CEscritura *error, CPilaErrores *pilaerror)

{
	// Palabra encontrada
	CString palab = "";
	// Palabra encontrada en mayusculas
	CString segpalab = "";
	// Ultimo caracter leido
	char ult;
	// Alias que recibe el objeto
	CString csalias="";
	// Nivel en el que nos encontramos.
	int nivel = 1;
	// Texto del error producido
	CString tipoerror="";
	// Número de línea en que nos encontramos.
	int lin=0;
	// ID de la propiedad encontrada.
	int li_prop=0;
	// ID del objeto encontrado.
	int li_obje=0;
	// ID del objeto auxiliar.
	int aux_obje=0;
	// Posición del . en el fichero pasado a analizar.
	int posinc=0;
	// Texto del error producido al comprobar el tipo de la propiedad.
	CString texterror="";
	// Nombre de la propiedad encontrada.
	CString csnomprop="";
	// Nombre del objeto encontrado
	CString csnomobje="";
	// Número de errores encontrados
	long numerror=0;
	// Para saber en que linea nos encontramos
	int linea_actual = 0;
	// Variable auxiliar para operaciones
	int li_aux = 0;
	int li_aux2 = 0;
	CString csnomaux="";
	CString csnomaux3="";
	// Variable auxilar para decter que hay cadenas despues
	// del END o antes del BEGIN
	int li_errcad=0;
	// Variable para terminar la aplicacion
	int li_finaliza=0;

	// Estas variables son de la segunda fase
	tpprop s_prop;
	info_error st_info;
	short allprop=0;
	short allobj=0;
	short ignore=0;
	short ls_ret=0;
	CString csaux="";
	CString ls_expresion="";
	BOOL lb_endvalidat=FALSE;
	CString nomb_objpadre="";
	int n_error=0;
	short li_resu=0;
	CString li_err;
	CString contnull;
	CHK_METODO st_chkmetodo;
	int li_objmet=0;
	int li_numetodos=0;
	CString ls_cadobj;
	int li_aux4=0;
	int li_errn = 0;
    bool begin = true;

	int iNumBegin = 0;

	
	do {	
		palab = m_leer.Lpalabra();
		ult = m_leer.Lultimo();
		// Si es distindo de cadena vacia.
		if (begin)
		{
			if (palab.IsEmpty() && m_leer.Finfich()) 
			{
				tipoerror = m_CMessages.TextMessage("44");
				tipoerror = MsgTexto (tipoerror,"","",0,"","","1");
				error->Escribe(1,tipoerror,44,"","","","");

			}
			begin = false;
		}
	//	else
	//	{
		
		if (palab=="END")
			iNumBegin --;

		if (palab=="BEGIN")
			iNumBegin ++;

	 	if (m_leer.Finfich() && (iNumBegin > 0))
		{

			tipoerror = m_CMessages.TextMessage("45");
			tipoerror = MsgTexto (tipoerror,"","",linea_actual,"","","45");
			error->Escribe(linea_actual,tipoerror,45,"","","","");
		}


		if (!palab.IsEmpty())
		{
			//if (palab=="END")
			//	iNumBegin --;
		

			if (li_finaliza)
			{
				m_leer.Termfich();
				tipoerror = m_CMessages.TextMessage("1");
				tipoerror = MsgTexto (tipoerror,"","",0,"","","1");
				//tipoerror = "Nº Error: 1. Exite una cadena de caracteres detras del último END.";
				error->Escribe(1,tipoerror,1,"","","","");

			}
			else
			{
				// Si encontramos un Begin
				segpalab = palab;
				segpalab.MakeUpper();

				if (segpalab=="BEGIN")
				{
					// Comprobamos que el ultimo caracter
					// leido es distinto de fin de linea.

					//iNumBegin++;

					++nivel;
					li_errcad=1;
				
					if (m_leer.Compr(ult,ESPACIO_BLANCO)) // modificado !leer.Compr(ult,2)
					{						
						
						while (m_leer.Compr(ult,TAB_BLANCO))
							ult = (char)m_leer.Leecaract();

						palab = m_leer.Lpalabra();

						if (palab=="END")
							iNumBegin --;

						ult = m_leer.Lultimo();
						if ((palab.IsEmpty()) && (!m_leer.Compr(ult,CR_NL)))
						{
							// Se produce un error
							// porque no hemos encontrado
							// el objeto
							linea_actual=m_leer.Numlinea();
							++linea_actual;
							if ((error->Compabierto())==m_tope)
								m_leer.Termfich();
							else
							{
								tipoerror = m_CMessages.TextMessage("2");
								
								//tipoerror = "Nº Error:2. Exite más de un espacio en blanco después del Begin.";
								if (m_padre>0)
									nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
								else
									nomb_objpadre = "";
								//tipoerror = MsgTexto (tipoerror,nomb_objpadre,"",linea_actual,"","","2");
								//error->Escribe(linea_actual,tipoerror,2,"",nomb_objpadre,"","");							
								do
								{
									palab = m_leer.Lpalabra();
									ult = m_leer.Lultimo();
								}	
								while ((palab.IsEmpty()) && (!m_leer.Compr(ult,CR_NL)));
							}
						}
					
						//if ((!leer.Compr(ult,2)) && (!leer.Finfich()))
						if (!m_leer.Finfich())
						{
							aux_obje =m_Cpobjects.BuscObj(palab, allprop, allobj, ignore);

							if ((palab == "Contextblk") || (palab == "TraductProp"))
							{
								//de momento
									while (m_leer.Compr(ult,TAB_BLANCO))
										ult = (char)m_leer.Leecaract();

									palab = m_leer.Lpalabra();
									ult = m_leer.Lultimo();
									//es el alias
									//++linea_actual;
					
									bool end = false;
									int numBegin = 0;
									int numEnd = 0;
									
									iNumBegin--;
									while (!end)
									{
										do
										{
											palab = m_leer.Lpalabra();
											if (palab == "BEGIN") numBegin ++;
											if (palab == "END") 
											{
												numEnd ++;
												if (numEnd == (numBegin + 1))
												{
													--nivel;
													end = true;
												}
												
											}
											ult = m_leer.Lultimo();
										}	
										while ((palab.IsEmpty()) && (!m_leer.Compr(ult,CR_NL)));

									//	if (m_leer.Compr(ult,CR_NL)) ++linea_actual;
									}

							}

							else
							{ //gemma
							if (!aux_obje)
							{
								if (m_padre>0)
								{
		// Comprobar que es heredado de su padre
									ls_ret = m_pilar.DarAllObj(m_padre);
									if (ls_ret)
									{
										if ((m_pilar.DarNomObjeto(m_padre))!="ERROR")
										{
											aux_obje=-1;
											allprop = 1;
											allobj = 1;
										}
									}
								}
							}
							if (aux_obje)
							{
								li_obje = aux_obje;
								csnomobje = palab;
								linea_actual = m_leer.Numlinea();
								if (!m_leer.Compr(ult,2))
									++linea_actual;

							// Tenemos que guardar todo aunque no sea correcto
							// el alias.

								if (m_leer.Compr(ult,1))
								{
									palab = m_leer.Lpalabra();
									ult = m_leer.Lultimo();
									csalias="ERROR";
									if (palab=="END")
										iNumBegin --;
									if (palab.GetLength()==0 || (palab == ""))
									{
										if ((error->Compabierto())==m_tope)
											m_leer.Termfich();
										else
										{
											//tipoerror = "Nº Error:3. Existe más de un espacio en blanco después de la declaración del objeto " + csnomobje;	
											//tipoerror = m_CMessages.TextMessage("3");
											tipoerror = m_CMessages.TextMessage("6");
											if (m_padre>0)
												nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
											else
												nomb_objpadre = "";
											
											//if (m_leer.Compr(ult,2))
											//{
												tipoerror = MsgTexto (tipoerror,csnomobje,nomb_objpadre,linea_actual,"","","6");	
											//	error->Escribe(linea_actual,tipoerror,3,csnomobje,nomb_objpadre,"","");
												error->Escribe(linea_actual,tipoerror,6,csnomobje,nomb_objpadre,"","");
											//}
		//										if (!leer.Compr(ult,2))
		//											leer.Finalinea();
										}
									}			
						   			else
									{
										if (!CompAlias(palab, ult, texterror)) 
										{
											if ((error->Compabierto())==m_tope)
												m_leer.Termfich();
											else
											{
												//tipoerror = "Nº Error:4. El objeto " + csnomobje + " tiene en el alias " + texterror;
												tipoerror = m_CMessages.TextMessage("4");													
												if (m_padre>0)
													nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
												else
													nomb_objpadre = "";
												tipoerror = MsgTexto (tipoerror,csnomobje,nomb_objpadre,linea_actual,"","","4");
												error->Escribe(linea_actual,tipoerror,4,csnomobje,nomb_objpadre,"","");											
											}
										}
										else
										{
											csalias = texterror;
										}

										if (!m_leer.Compr(ult,2))
											m_leer.Finalinea();	
									}


								// Aqui viene el resto

									li_resu=Apilar(li_obje,csalias,linea_actual,csnomobje, allprop, allobj);

									if (li_resu<=0)
									{														
										if (m_padre>0)
											nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
										else
											nomb_objpadre = "";
										switch (li_resu)
										{
										case 0:
											//tipoerror = "Nº Error:24. El objeto " + csnomobje + " no puede ser contenido más veces por " + nomb_objpadre;
											//tipoerror = m_CMessages.TextMessage("24");												
											li_err = "24";
											li_errn = 24;
											break;
										case -1:
											//tipoerror = "Nº Error:25. El objeto " + csnomobje + " tiene el alias repetido con un hermano.";
											//tipoerror = m_CMessages.TextMessage("25");
											li_err = "25";
											li_errn = 25;
											break;
										case -2:
											//tipoerror = "Nº Error:26. El objeto " + csnomobje + " no puede ser contenido más veces por " + nomb_objpadre + " y tiene el alias repetido con un hermano";
											//tipoerror = m_CMessages.TextMessage("26");
											li_err = "26";
											li_errn = 26;
											break;
										}
										tipoerror = m_CMessages.TextMessage(li_err);
										tipoerror = MsgTexto (tipoerror,csnomobje,nomb_objpadre,linea_actual,"","",li_err);
										error->Escribe(linea_actual,tipoerror,li_errn,csnomobje,nomb_objpadre,"","");
									}
									if (m_padre>0)
									{
								// Comprobar que es heredado de su padre
										li_aux2 = m_pilar.DarObjeto(m_pilar.DarPadre(m_padre));

										if (!(m_Cpobjects.BusCont(li_aux2,csnomobje)))
										{
											if ((error->Compabierto())==m_tope)
												m_leer.Termfich();
											else
											{												
												csnomaux = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
												//tipoerror = "Nº Error:5. El objeto " + csnomobje + " no puede ser contenido por " + csnomaux;
												tipoerror = m_CMessages.TextMessage("5");
												//if (m_padre>0)
												//	nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
												//else
												//	nomb_objpadre = "";
												tipoerror = MsgTexto (tipoerror,csnomobje,csnomaux,linea_actual,"","","5");
												error->Escribe(linea_actual,tipoerror,5,csnomobje,csnomaux,"","");
											}
										}																		
									}									
								}
								else
								{
								// Error debe haber un espacio en blanco
								// despues del objeto.
									if ((error->Compabierto())==m_tope)
										m_leer.Termfich();
									else
									{
										li_resu=Apilar(li_obje,"ERROR",linea_actual,csnomobje, allprop, allobj);

										if (li_resu<=0)
										{										
											if (m_padre>0)
												nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
											else
												nomb_objpadre = "";
											switch (li_resu)
											{
											case 0:
												//tipoerror = "Nº Error:24. El objeto " + csnomobje + " no puede ser contenido más veces por " + nomb_objpadre;
												li_err = "24";
												li_errn = 24;
												break;
											case -1:
												//tipoerror = "Nº Error:25. El objeto " + csnomobje + " tiene el alias repetido con un hermano.";
												li_err = "25";
												li_errn = 25;
												break;
											case -2:
												//tipoerror = "Nº Error:26. El objeto " + csnomobje + " no puede ser contenido más veces por " + nomb_objpadre + " y tiene el alias repetido con un hermano";
												li_err = "26";
												li_errn = 26;
												break;
											}		
											tipoerror = m_CMessages.TextMessage(li_err);
											tipoerror = MsgTexto (tipoerror,csnomobje,nomb_objpadre,linea_actual,"","",li_err);
											error->Escribe(linea_actual,tipoerror,li_errn,csnomobje,nomb_objpadre,"","");
										}
										//apilarerror();
										//li_obje = -1;
										//csnomobje = "ERROR";		
										//tipoerror = "Nº Error:6. El objeto " + palab + " no tiene alias.";
										if (m_padre>0)
											nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
										else
											nomb_objpadre = "";
										tipoerror = m_CMessages.TextMessage("6");
										tipoerror = MsgTexto (tipoerror,palab,nomb_objpadre,linea_actual,"","","6");
										error->Escribe(linea_actual,tipoerror,6,palab,nomb_objpadre,"","");
									}									
								}

							// Leemos hasta el final de 
							// línea pero en realidad
							// tenemos que seguir.
								if (!m_leer.Compr(ult,2))
									m_leer.Finalinea();
							}
							else
							{
							// Error. Debido a que no
							// se encuentra el objeto.
								if ((error->Compabierto())==m_tope)
									m_leer.Termfich();
								else
								{
									if (!m_leer.Compr(ult,2))
										m_leer.Finalinea();
									ApilarError();
									li_obje = -1;
									csnomobje = "ERROR";
									//tipoerror = "Nº Error:7. El objeto " + palab + " no es correcto.";
									if (m_padre>0)
										nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
									else
										nomb_objpadre = "";
									tipoerror = m_CMessages.TextMessage("7");
									tipoerror = MsgTexto (tipoerror,palab,nomb_objpadre,m_leer.Numlinea(),"","","7");
									error->Escribe(m_leer.Numlinea(),tipoerror,7,palab,nomb_objpadre,"","");
								}
							}
						}//gemma

						} 
						else
						{
						// Se produce un error
						// porque no hemos encontrado
						// el objeto
							if ((error->Compabierto())==m_tope)
								m_leer.Termfich();
							else
							{
								li_obje = -1;
								csnomobje = "ERROR";
								ApilarError();
								//tipoerror = "Nº Error:8. Exite más de un espacio en blanco o el objeto no ha sido declarado.";
								if (m_padre>0)
									nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
								else
									nomb_objpadre = "";
								tipoerror = m_CMessages.TextMessage("8");
								tipoerror = MsgTexto (tipoerror,"","",m_leer.Numlinea(),"","","8");
								error->Escribe(m_leer.Numlinea(),tipoerror,8,"",nomb_objpadre,"","");
								if (!m_leer.Compr(ult,2))
									m_leer.Finalinea();
							}
						}
					}
					else
					{
						if ((error->Compabierto())==m_tope)
							m_leer.Termfich();
						else
						{
							li_obje = -1;
							csnomobje = "ERROR";
							ApilarError();
							//tipoerror = "Nº Error:9. El objeto no ha sido declarado.";
							if (m_padre>0)
								nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
							else
								nomb_objpadre = "";
							tipoerror = m_CMessages.TextMessage("9");
							tipoerror = MsgTexto (tipoerror,"","",m_leer.Numlinea(),"","","9");
							error->Escribe(m_leer.Numlinea(),tipoerror,9,"",nomb_objpadre,"","");
							if (!m_leer.Compr(ult,2))
								m_leer.Finalinea();
						}						
					// Se produce un error debido
					// a que es fin de linea y no hay
					// declarado un objeto.
					}

				} // Fin del if de BEGIN
				else
				{
					if (!li_errcad)
					{
						if ((error->Compabierto())==m_tope)
							m_leer.Termfich();
						else
						{
							//tipoerror = "Nº Error:10. Existe una cadena de caracteres erroneos antes del Begin.";
							if (m_padre>0)
								nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
							else
								nomb_objpadre = "";
							tipoerror = m_CMessages.TextMessage("10");
							tipoerror = MsgTexto (tipoerror,"","",m_leer.Numlinea(),"","","10");
							error->Escribe(m_leer.Numlinea(),tipoerror,10,"",nomb_objpadre,"","");
							if (!m_leer.Compr(ult,2))	
								m_leer.Finalinea();
						}
					}

				// En el caso de que no estemos en
				// el primer nivel
					else
					{
						segpalab = palab;
						segpalab.MakeUpper();
						if (segpalab=="END")
						{
						// Comprobar si hay
						// algun begin
							//iNumBegin --;

							if (nivel<=1)
							{
								linea_actual = m_leer.Numlinea(); 
								if ((error->Compabierto())==m_tope)
									m_leer.Termfich();
								else
								{
									if (m_leer.Finfich())
										linea_actual = linea_actual + 1; 

									//tipoerror = "Nº Error:11. Hay mas de un End para los Begin encontrados.";
									tipoerror = m_CMessages.TextMessage("11");
									tipoerror = MsgTexto (tipoerror,"","",linea_actual,"","","11");
									error->Escribe(linea_actual,tipoerror,11,"","","","");
		///**********OJO				
									if ((!m_leer.Compr(ult,2)) && (!m_leer.Finfich()))
									{
										if ((error->Compabierto())==m_tope)
											m_leer.Termfich();
										else
										{
											//tipoerror = "Nº Error:12. Hay una cadena de caracteres detras del END.";									
											tipoerror = m_CMessages.TextMessage("12");
											tipoerror = MsgTexto (tipoerror,"","",linea_actual,"","","12");
											error->Escribe(linea_actual,tipoerror,12,"","","","");

											m_leer.Finalinea();
										}
									}
								}
							// Error debido a que nos
							// hemos encontrado un END
							// sin un Begin.
							}
							else
							{
							// Se realizara mas adelante la
							// comprobación.
							//if (Comprob(ult,2))
							//{
								--nivel;
								if (nivel>1)
								{
									m_pilar.EncEnd(m_padre);
									// Aqui viene la comprobación de los objetos y 
									//propiedades obligatorias.
									li_aux4 = m_PilaOblig.CheckObjObligatorios(m_padre);

									if (li_aux4)
									{
										if ((error->Compabierto())==m_tope)
											m_leer.Termfich();
										else
										{
											for (int obj=1;obj<=li_aux4;++obj)
											{
												m_PilaOblig.GetObjObligatorio(m_padre,obj,ls_cadobj);

											//tipoerror = "Nº Error:27. El objeto " + m_pilar.DarNomObjeto(m_padre) + " no tiene definido el objeto obligatorio " + ls_cadobj;
											if (m_padre>0)
												nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
											else
												nomb_objpadre = "";													
											tipoerror = m_CMessages.TextMessage("27");
											tipoerror = MsgTexto (tipoerror,ls_cadobj,(m_pilar.DarNomObjeto(m_padre)),(m_pilar.DarLineas(m_padre)),"","","27");
											error->Escribe((m_pilar.DarLineas(m_padre)),tipoerror,27,(m_pilar.DarNomObjeto(m_padre)),nomb_objpadre,"","");
											}
										}
									}
									li_aux4 = m_PilaOblig.CheckPropObligatorias(m_padre);
									if (li_aux4)
									{
										if ((error->Compabierto())==m_tope)
											m_leer.Termfich();
										else
										{
											for (int obj=1;obj<=li_aux4;++obj)
											{
												m_PilaOblig.GetPropObligatoria(m_padre,obj,ls_cadobj);
											//tipoerror = "Nº Error:28. El objeto " + m_pilar.DarNomObjeto(m_padre) + " no tiene definida la propiedad obligatoria " + ls_cadobj;
											if (m_padre>0)
												nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
											else
												nomb_objpadre = "";
											tipoerror = m_CMessages.TextMessage("28");
											tipoerror = MsgTexto (tipoerror,(m_pilar.DarNomObjeto(m_padre)),nomb_objpadre,(m_pilar.DarLineas(m_padre)),ls_cadobj,"","28");
											error->Escribe((m_pilar.DarLineas(m_padre)),tipoerror,28,(m_pilar.DarNomObjeto(m_padre)),nomb_objpadre,"","");
											}
												
										}
									}

									m_padre = m_pilar.DarPadre(m_padre);
									m_AliasPath = m_pilar.DarPathobj(m_padre);
									//obj_path = pilar.darpathobj(m_padre);
									// Nombre del objeto es alias_path
									//alias_path = pilar.DarPathalias(m_padre);

								}
								else
								{
									short NotFinfich = 1;
									if ((!m_leer.Compr(ult,2)) && (!(m_leer.Finfich())))
									{
										NotFinfich = m_leer.Leecaract();
										ult = m_leer.Lultimo();
										while ((m_leer.Compr(ult,CR_NL_TAB_BLANCO)) && (NotFinfich))
										{
											NotFinfich = m_leer.Leecaract();
											ult = m_leer.Lultimo();
										}

										if (NotFinfich)
										{
											m_leer.Termfich();
											linea_actual = m_leer.Numlinea() +1; 
											//tipoerror = "Nº Error:15. Hay una cadena de caracteres o espacios en blanco detras del END.";											
											tipoerror = m_CMessages.TextMessage("15");
											tipoerror = MsgTexto (tipoerror,"","",linea_actual,"","","15");
											error->Escribe(linea_actual,tipoerror,15,"","","","");
										}

									}
						
											
								}
								if (nivel==1)
								{
									m_pilar.EncEnd(m_padre);
									// Aqui viene la comprobación de los objetos y 
									//propiedades obligatorias.

									li_aux4 = m_PilaOblig.CheckObjObligatorios(m_padre);

									if (li_aux4)
									{
										if ((error->Compabierto())==m_tope)
											m_leer.Termfich();
										else
										{
											for (int obj=1;obj<=li_aux4;++obj)
											{
												m_PilaOblig.GetObjObligatorio(m_padre,obj,ls_cadobj);

											//tipoerror = "Nº Error:27. El objeto " + m_pilar.DarNomObjeto(m_padre) + " no tiene definido el objeto obligatorio " + ls_cadobj;

											if (m_padre>0)
												nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
											else
												nomb_objpadre = "";													
											tipoerror = m_CMessages.TextMessage("27");
											tipoerror = MsgTexto (tipoerror,ls_cadobj,(m_pilar.DarNomObjeto(m_padre)),(m_pilar.DarLineas(m_padre)),"","","27");
											error->Escribe((m_pilar.DarLineas(m_padre)),tipoerror,27,(m_pilar.DarNomObjeto(m_padre)),nomb_objpadre,"","");
											}
										}
									}

									li_aux4 = m_PilaOblig.CheckPropObligatorias(m_padre);
									if (li_aux4)
									{
										if ((error->Compabierto())==m_tope)
											m_leer.Termfich();
										else
										{
											for (int obj=1;obj<=li_aux4;++obj)
											{
												m_PilaOblig.GetPropObligatoria(m_padre,obj,ls_cadobj);
											//tipoerror = "Nº Error:28. El objeto " + m_pilar.DarNomObjeto(m_padre) + " no tiene definida la propiedad obligatoria " + ls_cadobj;
											if (m_padre>0)
												nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
											else
												nomb_objpadre = "";													
											tipoerror = m_CMessages.TextMessage("28");
											tipoerror = MsgTexto (tipoerror,(m_pilar.DarNomObjeto(m_padre)),nomb_objpadre,(m_pilar.DarLineas(m_padre)),ls_cadobj,"","28");
											error->Escribe((m_pilar.DarLineas(m_padre)),tipoerror,28,(m_pilar.DarNomObjeto(m_padre)),nomb_objpadre,"","");
											}
												
										}
									}
									li_finaliza=1;

								}
								if ((!m_leer.Compr(ult,CR_NL)) && (!m_leer.Finfich()))
								{
									if ((error->Compabierto())==m_tope)
										m_leer.Termfich();
									else
									{
										
										short NotFinfich;

										NotFinfich = m_leer.Leecaract();
										ult = m_leer.Lultimo();
										while ((m_leer.Compr(ult,CR_NL_TAB_BLANCO)) && (NotFinfich))
										{
											NotFinfich = m_leer.Leecaract();
											ult = m_leer.Lultimo();
										}

										if (NotFinfich)
										{
											m_leer.Termfich();
											linea_actual = m_leer.Numlinea() +1; 
											//tipoerror = "Nº Error:15. Hay una cadena de caracteres o espacios en blanco detras del END.";											
											tipoerror = m_CMessages.TextMessage("15");
											tipoerror = MsgTexto (tipoerror,"","",linea_actual,"","","15");
											error->Escribe(linea_actual,tipoerror,15,"","","","");
										}
										m_leer.Finalinea();
									}
								}
		// Aqui es donde esta END
								//leer.Finalinea();
							//}
							
							}

						} // Final del END
						else
						{
						// Comprobar si es
						// una propiedad
							bool ExistIgualIzq = false;
							bool ExistIgualDch = false;
							csnomprop = palab;

							if (palab.Find('=') > 0)
							{
								csnomprop = palab.Left(palab.Find('='));
								ExistIgualIzq = true;
							}

							li_prop=m_Cpobjects.EncProp(li_obje,csnomprop,s_prop);
						// 1 la propiedad es contenida dentro del objeto
						// -1 puede ser cualquier cosa
						// 0 no se ha encontrado la propiedad en el objeto y por tanto
						// daría error con que la ha producido.
							if (li_prop>0)
							{
								csaux="";
								ls_expresion = "";
								lb_endvalidat = FALSE;

								
								if ((m_leer.Compr(ult,1)) || (ExistIgualIzq))
								{
									if (!ExistIgualIzq)
									{
										palab = m_leer.Lpalabra();

										if (palab.GetLength() > 1)
										{
											palab = palab.Right( palab.GetLength() - 1);
											ExistIgualDch = true;
										}
										ult = m_leer.Lultimo();
									}
									if ((palab=="=") || (ExistIgualIzq) || (ExistIgualDch))
									{
									// Aqui viene la validacion
									// del valor de la propiedad
									// ahora solo vamos a leer
									// hasta el fin de línea.
										if ((m_leer.Compr(ult,ESPACIO_BLANCO)) || (ExistIgualIzq) || (ExistIgualDch))
										{
											if (ExistIgualIzq)
											{
												if ((palab.GetLength() - palab.Find('=') - 1) > 0)
													palab = palab.Right(palab.GetLength() - palab.Find('=')-1);
												else
													palab = m_leer.Lpalabra();
												ExistIgualIzq = false;
											}
											else
												if (!ExistIgualDch)
													palab = m_leer.Lpalabra();

											ExistIgualDch = false;	
											ult = m_leer.Lultimo();

											if ((palab.IsEmpty()) && (!m_leer.Compr(ult,2)))
											{
												if ((error->Compabierto())==m_tope)
													m_leer.Termfich();
												else
												{
													//tipoerror = "Nº Error:17. Existe en la propiedad más de un espacio en blanco después del =.";														
													if (m_padre>0)
														nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
													else
														nomb_objpadre = "";
													tipoerror = m_CMessages.TextMessage("17");
													tipoerror = MsgTexto (tipoerror,"","",(m_leer.Numlinea()+1),csnomprop,"","17");
													error->Escribe((m_leer.Numlinea()+1),tipoerror,17,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,csnomprop,"");
													do
													{
														palab=m_leer.Lpalabra();
														ult=m_leer.Lultimo();
													} while ((palab.IsEmpty()) && (!m_leer.Compr(ult,CR_NL)));
												}
											}

											if (!m_leer.Finfich())
											{											
												if (palab.GetLength()==0)
												{
													if ((error->Compabierto())==m_tope)
														m_leer.Termfich();
													else
													{
														//tipoerror = "Nº Error:18. No tiene asignado el valor la propiedad " + csnomprop;												

														if (m_padre>0)
															nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
														else
															nomb_objpadre = "";												
														tipoerror = m_CMessages.TextMessage("18");
														tipoerror = MsgTexto (tipoerror,"","",m_leer.Numlinea(),csnomprop,"","18");
														error->Escribe(m_leer.Numlinea(),tipoerror,18,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,csnomprop,"");
													}

												}
																		
												else
												{
													csaux = palab;
													while (!m_leer.Compr(ult,CR_NL))
													{
														palab = m_leer.Lpalabra();
														if (palab=="END")
															iNumBegin --;
														csaux = csaux + ult + palab;
														ult = m_leer.Lultimo();			
													}

													csaux.TrimRight();
													contnull = s_prop.Type;
													contnull.MakeUpper();
		// Para el tipo								
													if (contnull!="NULL")
													{
		// Fidel
		// Falta el Errorlevel porque cambia
		// la forma de llamar a las funciones

														li_aux2 = m_CComType.comptype(s_prop.Type,csaux,&st_info);
														ls_expresion = s_prop.Constraint.Expression;
														lb_endvalidat = s_prop.Constraint.Endvalidate;
														if (li_aux2<=0)	
														{
															if ((error->Compabierto())==m_tope)
																m_leer.Termfich();
															else
															{
																if (li_aux2==0)
																{
																	//tipoerror = "Nº Error:" + st_info.Code + ". La propiedad " + csnomprop + " tiene el error: " + st_info.Message;														
																	if (m_padre>0)
																		nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
																	else
																		nomb_objpadre = "";
																	tipoerror = m_CMessages.TextMessage(st_info.Code);
																	tipoerror = MsgTexto (tipoerror,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,m_leer.Numlinea(),csnomprop,csaux,st_info.Code);
																	n_error = atoi(st_info.Code);
																	error->Escribe(m_leer.Numlinea(),tipoerror,n_error,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,csnomprop,csaux);
																}
																else
																{
																	//tipoerror = "Nº Error:13. La propiedad " + csnomprop + " no tiene el tipo declarado.";
																	if (m_padre>0)
																		nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
																	else
																		nomb_objpadre = "";														
																	tipoerror = m_CMessages.TextMessage("13");
																	tipoerror = MsgTexto (tipoerror,"","",m_leer.Numlinea(),csnomprop,csaux,"13");
																	error->Escribe(m_leer.Numlinea(),tipoerror,13,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,csnomprop,csaux);
																}
															}	
															if (!m_leer.Compr(ult,CR_NL))
																m_leer.Finalinea();
														}											  
													}										  						
												} //fin del else cuando la longitud es mayor que 0
											}
											else
											{	
 												if (!m_leer.Compr(ult,CR_NL))
													m_leer.Finalinea();
											}
										}
										else
										{
											if ((error->Compabierto())==m_tope)
												m_leer.Termfich();
											else
											{
												if (!m_leer.Compr(ult,CR_NL))
													m_leer.Finalinea();
												//tipoerror = "Nº Error:19. Debe haber un espacio en blanco despues =.";
												if (m_padre>0)
													nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
												else
													nomb_objpadre = "";
												tipoerror = m_CMessages.TextMessage("19");
												tipoerror = MsgTexto (tipoerror,"","",m_leer.Numlinea(),csnomprop,"","19");
												error->Escribe(m_leer.Numlinea(),tipoerror,19,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,csnomprop,"");
											}
										}
									}
									else
									{
										if ((error->Compabierto())==m_tope)
											m_leer.Termfich();
										else
										{
											if (!m_leer.Compr(ult,CR_NL))
												m_leer.Finalinea();																				
											//tipoerror = "Nº Error:20. Para la propiedad " + csnomprop + " debe haber un = o un espacio en blanco antes del =.";
											if (m_padre>0)
												nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
											else
												nomb_objpadre = "";
											tipoerror = m_CMessages.TextMessage("20");
											tipoerror = MsgTexto (tipoerror,"","",m_leer.Numlinea(),csnomprop,"","20");
											error->Escribe(m_leer.Numlinea(),tipoerror,20,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,csnomprop,"");

										}
									// Error debido a que
									// debe existir un
									// = .
									// leer hasta fin de linea
									}
								}
								else
								{
									if ((error->Compabierto())==m_tope)
										m_leer.Termfich();
									else
									{
										if (!m_leer.Compr(ult,CR_NL))
											m_leer.Finalinea();
										//tipoerror = "Nº Error:21. Debe haber un espacio en blanco antes =.";
										if (m_padre>0)
											nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
										else
											nomb_objpadre = "";
										tipoerror = m_CMessages.TextMessage("21");
										tipoerror = MsgTexto (tipoerror,"","",m_leer.Numlinea(),"","","21");
										error->Escribe(m_leer.Numlinea(),tipoerror,21,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,"","");
									}
								// Error debido 
								// a que debe existir
								// un blanco.
								// leer hasta fin de linea.
								}
								if (li_obje>0)
								{
									if (!(m_PilaOblig.NewPropiedad((m_pila-1),csnomprop,csaux,&ls_expresion,lb_endvalidat)))
									{
										if ((error->Compabierto())==m_tope)
											m_leer.Termfich();
										else
										{
											//tipoerror = "Nº Error: " + s_prop.Constraint.Code + " .La propiedad " + csnomprop + " tiene el error " + s_prop.Constraint.Message ;
											if (m_padre>0)
												nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
											else
												nomb_objpadre = "";
											tipoerror = m_CMessages.TextMessage(s_prop.Constraint.Code);
											tipoerror = MsgTexto (tipoerror,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,(m_pilar.DarLineas(m_padre)),csnomprop,csaux,s_prop.Constraint.Code);
											n_error = atoi(s_prop.Constraint.Code);
											error->Escribe((m_pilar.DarLineas(m_padre)),tipoerror,n_error,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,csnomprop,csaux);
										}
									}
								}
							}

							else
							{
								if (li_prop==0)
								{		
							// Error debido a que no
							// se encuentra en el objeto o que
							// dicha propiedad no se valida
							// de propiedades
									if ((error->Compabierto())==m_tope)
										m_leer.Termfich();
									else
									{
										lin = m_leer.Numlinea();
										++lin;
										//tipoerror = "Nº Error:22. La propiedad "+ palab + " no encontrada en el objeto " + m_pilar.DarNomObjeto(m_padre);
										if (m_padre>0)
											nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(m_padre));
										else
											nomb_objpadre = "";
										tipoerror = m_CMessages.TextMessage("22");
										tipoerror = MsgTexto (tipoerror,m_pilar.DarNomObjeto(m_padre),"",m_leer.Numlinea(),palab,"","22");
										error->Escribe(lin,tipoerror,22,m_pilar.DarNomObjeto(m_padre),nomb_objpadre,palab,"");
										if (!m_leer.Compr(ult,2))
											m_leer.Finalinea();
									}
								}
								else
								{
									if (!m_leer.Compr(ult,2))
										m_leer.Finalinea();
								}
							}// Aqui debemos tener un =									
						}// Para el end
					} // Si es distinto de primer nivel
				} // Si es distinto de Begin
			}// Si es li_finaliza	
		//	}
		}// Si la palabra no es nula

	} while (!m_leer.Finfich());


}
void CElementos::AnalyzeRunTime(CEscritura *error)
{

	// Palabra encontrada
	CString palab = "";
	// Palabra encontrada en mayusculas
	CString segpalab = "";
	// Ultimo caracter leido
	CString csalias="";
	// Nivel en el que nos encontramos.
	int nivel = 1;
	// Texto del error producido
	CString tipoerror="";
	// Número de línea en que nos encontramos.
	int lin=0;
	// ID de la propiedad encontrada.
	int li_prop=0;
	// ID del objeto encontrado.
	int li_obje=0;
	// ID del objeto auxiliar.
	int aux_obje=0;
	// Posición del . en el fichero pasado a analizar.
	int posinc=0;
	// Texto del error producido al comprobar el tipo de la propiedad.
	CString texterror="";
	// Nombre de la propiedad encontrada.
	CString csnomprop="";
	// Nombre del objeto encontrado
	CString csnomobje="";
	// Para convertir el número de errores encontrados.
	long numerror=0;
	// Para saber en que linea nos encontramos
	int linea_actual = 0;
	// Variable auxiliar para operaciones
	int li_aux = 0;
	int li_aux2 = 0;
	CString csnomaux="";
	CString csnomaux3="";
	// Variable auxilar para decter que hay cadenas despues
	// del END o antes del BEGIN
	int li_errcad=0;
	// Variable para terminar la aplicacion
	int li_finaliza=0;

	// Estas variables son de la segunda fase
	tpprop s_prop;
	info_error st_info;
	short allprop=0;
	short allobj=0;
	short ignore=0;
	short ls_ret=0;
	CString csaux="";
	CString ls_expresion="";
	BOOL lb_endvalidat=FALSE;
	CString nomb_objpadre="";
	int n_error=0;
	short li_resu=0;
	CString li_err;
	CString contnull;
	CHK_METODO st_chkmetodo;
	int li_objmet=0;
	int li_numetodos=0;
	CString ls_cadobj;
	int li_aux4=0;
	int li_errn = 0;


		li_numetodos = m_PilaOblig.GetNumMetodos();

		if ((li_numetodos) && ((error->Compabierto())!=m_tope))
		{
			for (int j=1;j<=li_numetodos;++j)
			{
				if (!(m_PilaOblig.CheckMetodo(j,&st_chkmetodo)))
				{
					li_objmet = st_chkmetodo.objeto;
					csnomaux =  m_pilar.DarNomObjeto(li_objmet);
					//tipoerror = "Nº Error:36. El objeto "+csnomaux+" tiene un error en la propiedad "+ st_chkmetodo.propiedad;
					if (st_chkmetodo.failpath)
					{
						li_err="36";
						li_errn=36;
						//tipoerror = tipoerror + " donde el path no es válido ";
					}
					if (st_chkmetodo.failruntimeproperty)
					{
						//tipoerror = tipoerror + ", una propiedad de runtime es incorrecta ";
						if (li_errn==36)
						{
							li_err="39";
							li_errn=39;
						}
						else
						{
							li_err="37";
							li_errn=37;
						}
					}

					if (st_chkmetodo.failruntimemethod)
					{
						//tipoerror = tipoerror + ", un método es incorrecto";
						if (li_errn==36)
						{
							li_err="40";
							li_errn=40;
						}
						else
						{
							if (li_errn==37)
							{
								li_err="42";
								li_errn=42;
							}
							else
							{
								if (li_errn==39)
								{
									li_err="41";
									li_errn=41;
								}
								else
								{
									li_err="38";
									li_errn=38;
								}
							}
						}
					}
					//tipoerror = tipoerror + ".";
					if (li_objmet>0)
						nomb_objpadre = m_pilar.DarNomObjeto(m_pilar.DarPadre(li_objmet));
					else
						nomb_objpadre ="";
					tipoerror = m_CMessages.TextMessage(li_err);
					tipoerror = MsgTexto (tipoerror,csnomaux,"",0,st_chkmetodo.propiedad,"",li_err);
					error->Escribe((m_pilar.DarLineas(li_objmet)),tipoerror,li_errn,csnomaux,nomb_objpadre,st_chkmetodo.propiedad,st_chkmetodo.valor);
				}

			}
		}

}

long  CElementos::ShowNumErrors(CEscritura *error, BOOL writeFich)
{
	
	long numerror=0;
	CString tipoerror="";
	// Para convertir el número de errores encontrados.
	char csner[20];
	long badError = 0;


	// Obtenemos el número de errores producidos durante el analisis.
	numerror = error->Compabierto();
	if (numerror)
	{
		

		if(writeFich)// Si hay que generar fichero .log
		{
			badError = error->GenFileFromPila();
		

			ltoa(numerror - badError,csner,10);
			//tipoerror = "Nº de errores: ";
			tipoerror = m_CMessages.TextMessage("203");
			tipoerror = tipoerror + csner;
			tipoerror = tipoerror + " ";
			//tipoerror = tipoerror + " errores.";
			tipoerror = tipoerror + m_CMessages.TextMessage("204");
			error->WriteToFile(0,tipoerror,"","","","");
		
		}
		return (numerror - badError);
	}
	else
		return 0;




}

CString CElementos::MsgTexto(CString cadena, CString objeto, CString objeto_padre, int num_linea, CString propiedad, CString value_propiedad, CString codigo_error)
{

	char *buf =NULL;
	char csnom[20];
	

	// objeto %O
	// objeto_padre %A
	// num_linea %L
	// propiedad %P
	// value_propiedad %V
	// codigo_error %C

	// Sustituyo el nombre de Objeto
	if (objeto!="")
	{
		ChgStr(&buf, cadena, "%O",objeto);
		if (buf!=NULL)
		{
			cadena = buf;
			delete buf;
		}
	}

	// Sustituyo el nombre del objeto padre
	if (objeto_padre!="")
	{
		ChgStr(&buf, cadena, "%A",objeto_padre);
		if (buf!=NULL)
		{
			cadena = buf;
			delete buf;
		}
	}

	//Sustituyo el número de línea
	if (num_linea>0)
	{
		_itoa( num_linea, csnom, 10  );
		ChgStr(&buf, cadena, "%L",csnom);
		if (buf!=NULL)
		{
			cadena = buf;
			delete buf;
		}
	}

	//Sustituyo el nombre de la propiedad
	if (propiedad!="")
	{
		ChgStr(&buf, cadena, "%P",propiedad);
		if (buf!=NULL)
		{
			cadena = buf;
			delete buf;
		}
	}

	// Sustituyo el valor de la propiedad
	if (value_propiedad!="")
	{
		ChgStr(&buf, cadena, "%V",value_propiedad);
		if (buf!=NULL)
		{
			cadena = buf;
			delete buf;
		}
	}
	// Sustituyo el código de error.
	if (codigo_error!="")
	{
		ChgStr(&buf, cadena, "%C",codigo_error);
		if (buf!=NULL)
		{
			cadena = buf;
			delete buf;
		}
	}
	
	return cadena;
}
