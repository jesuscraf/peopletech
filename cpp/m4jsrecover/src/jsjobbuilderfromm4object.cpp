//## begin module%37E602220252.cm preserve=no
//## end module%37E602220252.cm

//## begin module%37E602220252.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%37E602220252.cp

//## Module: jsjobbuilderfromm4object%37E602220252; Package body
//## Subsystem: M4JSRecover::src%37F8B6A102CB
//## Source file: C:\Trabajo\Server\m4jsrecover\version\src\jsjobbuilderfromm4object.cpp

//## begin module%37E602220252.additionalIncludes preserve=no
//## end module%37E602220252.additionalIncludes

//## begin module%37E602220252.includes preserve=yes
#include <m4var.hpp>
#include <jscompoundjob.hpp>
#include <m4jsres.hpp>
#include "node.hpp"
//## end module%37E602220252.includes

// jsjobbuilderfromm4object
#include <jsjobbuilderfromm4object.hpp>
// jsjobbuilder
#include <jsjobbuilder.hpp>

class ClAccess;

//## begin module%37E602220252.declarations preserve=no
//## end module%37E602220252.declarations

//## begin module%37E602220252.additionalDeclarations preserve=yes
//## end module%37E602220252.additionalDeclarations


// Class ClJSJobBuilderFromM4Object 



ClJSJobBuilderFromM4Object::ClJSJobBuilderFromM4Object ()
  //## begin ClJSJobBuilderFromM4Object::ClJSJobBuilderFromM4Object%938073478.hasinit preserve=no
  //## end ClJSJobBuilderFromM4Object::ClJSJobBuilderFromM4Object%938073478.hasinit
  //## begin ClJSJobBuilderFromM4Object::ClJSJobBuilderFromM4Object%938073478.initialization preserve=yes
  :ClJSJobBuilder()
  //## end ClJSJobBuilderFromM4Object::ClJSJobBuilderFromM4Object%938073478.initialization
{
  //## begin ClJSJobBuilderFromM4Object::ClJSJobBuilderFromM4Object%938073478.body preserve=yes
   SetSource();
  //## end ClJSJobBuilderFromM4Object::ClJSJobBuilderFromM4Object%938073478.body
}



//## Other Operations (implementation)
m4return_t ClJSJobBuilderFromM4Object::SetSource (ClAccess *ai_poSource)
{
  //## begin ClJSJobBuilderFromM4Object::SetSource%938073479.body preserve=yes
   if ( ai_poSource != m_poSource ) 
	  m_poSource = ai_poSource;

   return M4_SUCCESS;
  //## end ClJSJobBuilderFromM4Object::SetSource%938073479.body
}

m4return_t ClJSJobBuilderFromM4Object::BuildJobs (m4bool_t ai_bBuildCancellations)
{
  //## begin ClJSJobBuilderFromM4Object::BuildJobs%939977802.body preserve=yes
	m4VariantType vIdJob        ;/* Identificador del trabajo                 */
	m4VariantType vIdExec       ;/* Identificador de ejecución                */
	m4VariantType vSchedDate    ;/* Fecha de ejecución prevista               */
	m4VariantType vResult       ;/* Resultado del método LN4                  */
	ClJSJob * pTrabajo          ;/* Trabajo que se construye                  */
	ClNode * poNode             ;/* Puntero al nodo principal del M4O         */
	M4ClString stError          ;/* Parámetro para los errores                */     
	m4uint32_t uiNumError = 0   ;/* Codigo de error a generar                 */
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */

	/* Pedimos un puntero al nodo principal del M4Object                      */ 
	poNode = &(m_poSource -> Node ["N4_JS_SCHED_DATES"]);

	if ( poNode != NULL)
	{
		/* Nos posicionamos en el primer registro del bloque                   */
		poNode->RecordSet.Current.Begin();

		/* Recorremos todos los registros del bloque                           */
		// while( uiNumError == 0 && !poNode->RecordSet.Current.Index.IsEOF() )
		 while( uiNumError == 0 && (poNode->RecordSet.Current.Index != M4DM_EOF_INDEX) )
		{
		   /* Metemos los parámetros que nos va a rellenar el método           */
			if (poNode->RecordSet.Current.Item[ "GET_TASK_EXE" ].StackPush (vIdJob) == M4_SUCCESS && 
				poNode->RecordSet.Current.Item[ "GET_TASK_EXE" ].StackPush (vIdExec) == M4_SUCCESS &&
				poNode->RecordSet.Current.Item[ "GET_TASK_EXE" ].StackPush (vSchedDate) == M4_SUCCESS)
			{
				/* Llamada al método GET_TASK_EXE                                */
				if (poNode->RecordSet.Current.Item[ "GET_TASK_EXE" ].Call() == M4_SUCCESS )
				{
					/* Recuperamos los parámetros                                 */
					if (poNode->RecordSet.Current.Item[ "GET_TASK_EXE" ].StackPop (vIdJob) == M4_SUCCESS && 
						poNode->RecordSet.Current.Item[ "GET_TASK_EXE" ].StackPop (vIdExec) == M4_SUCCESS &&
					    poNode->RecordSet.Current.Item[ "GET_TASK_EXE" ].StackPop (vSchedDate) == M4_SUCCESS &&
						poNode->RecordSet.Current.Item[ "GET_TASK_EXE" ].StackPop (vResult) == M4_SUCCESS)
					{
						/* Comprobamos el resultado del método                     */
						if (vResult.Data.DoubleData == 0)
						{
   							pTrabajo = new ClJSCompoundJob( (m4uint_t) vIdJob.Data.DoubleData,
															(m4uint_t) vIdExec.Data.DoubleData);

	   						if (pTrabajo != NULL)
							{
								M4ClTimeStamp tsSchedTime ;/* Fecha de ejecución     */

								/* Convertimos el double del variant en un timestamp */
								tsSchedTime.set( vSchedDate.Data.DoubleData);
								pTrabajo->SetScheduledTime( tsSchedTime);
   
								if (ai_bBuildCancellations == M4_FALSE)
								{
									/* Completamos la estructura del trabajo          */
									if (BuildJobStructure( pTrabajo) == M4_SUCCESS)
									{
										/* Construcción de los conectores, los         */
										/* parámetros irán en la construcción de la    */
										/* estructura                                  */
										if ( BuildParameterConnectors( pTrabajo) == M4_SUCCESS)
										{
											if ( pTrabajo->PropagateAllParamValues() == M4_SUCCESS )
											{
												/* Si hemos hecho todo bien, incluimos   */
												/* el trabajo en la lista                */
												m_lProducts.insert(m_lProducts.begin(), pTrabajo);
											}
											else
											{
												uiNumError = M4_JS_REC_EXECUTING_METHOD;
												stError = "JSJob->PropagateAllParamValues";
											}
										}
										else
										{
											uiNumError = M4_JS_REC_EXECUTING_METHOD;
											stError = "Builder->BuildParameterConnectors";
										}
									} 
									else 
									{
										uiNumError = M4_JS_REC_EXECUTING_METHOD;
										stError = "Builder->BuildJobStructure";
									} /* endif */
								}
								else
								{
									/* Incluimos el trabajo en la lista               */
									m_lProducts.insert(m_lProducts.begin(), pTrabajo);
								}

								/* Si ha habido algún error, borramos el trabajo     */
								if (uiNumError != 0)
									delete pTrabajo;

								/* Pasamos al registro siguiente							  */
								poNode->RecordSet.Current.Next();
							}
							else 
							{
								stError = "ClJSCompoundJob";
								uiNumError = M4_JS_REC_BUILDING_INSTANCE;
	   						}
						}
   						else
   						{
							stError = "C4_JS_SCHED_DATES.GET_TASK_EXE";
							uiNumError = M4_JS_REC_EXECUTING_METHOD;
   						}
	   				}
					else
   					{
	 					stError = "C4_JS_SCHED_DATES.GET_TASK_EXE";
 						uiNumError = M4_JS_REC_POPPING_PARAMETERS;
	   				}
				}
   				else
	   			{
					stError = "C4_JS_SCHED_DATES.GET_TASK_EXE";
   					uiNumError = M4_JS_REC_CALLING_METHOD;
	   			}
			}
			else
			{
	  			stError = "C4_JS_SCHED_DATES.GET_TASK_EXE";
	  			uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
			} 
		} /* End while */
	}
	else
	{
		stError = "C4_JS_SCHED_DATES.N4_JS_SCHED_DATES";
		uiNumError = M4_JS_REC_ACCESSING_NODE;
	}

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClJSJobBuilderFromM4Object::BuildJobs";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClJSJobBuilderFromM4Object::BuildJobs%939977802.body
}

m4return_t ClJSJobBuilderFromM4Object::BuildJobStructure (ClJSJob *ai_pMainJob)
{
  //## begin ClJSJobBuilderFromM4Object::BuildJobStructure%940243353.body preserve=yes
	m4VariantType vIdJob        ;/* Identificador del trabajo                 */
	m4VariantType vResult       ;/* Resultado del método LN4                  */
	m4uint_t uiLocalId = 1      ;/* Identificador local de cada subtarea      */
	ClJSSimpleJob * pTarea      ;/* Trabajo que se construye                  */
	ClNode * poNode             ;/* Puntero al nodo principal del M4O         */
	M4ClString stError          ;/* Parámetro para los errores                */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */

   /* Puntero al nodo secundario del M4Object                                */ 
   poNode = &(m_poSource->Node ["N4_JS_SUBTASK_DEF"]);
   if (poNode != NULL)
   {
      /* Nos colocamos en el primer registro                                 */
      poNode->RecordSet.Current.Begin();
   
      /* Recorremos el conjunto de registros                                 */
      // while (!poNode->RecordSet.Current.Index.IsEOF() && uiNumError == 0)
	  while ((poNode->RecordSet.Current.Index != M4DM_EOF_INDEX) && uiNumError == 0)
      {
         /* Metemos los parámetros que nos va a rellenar el método           */
		if (poNode->RecordSet.Current.Item[ "GET_SUBTASK_DATA" ].StackPush (vIdJob) == M4_SUCCESS )
		{
            /* Llamamos al método GET_JOB_DATA                               */
			if (poNode->RecordSet.Current.Item[ "GET_SUBTASK_DATA" ].Call() == M4_SUCCESS )
            {
               /* Recuperamos los parámetros y el resultado                  */
        		if ( poNode->RecordSet.Current.Item[ "GET_SUBTASK_DATA" ].StackPop (vIdJob) == M4_SUCCESS && 
         			poNode->RecordSet.Current.Item[ "GET_SUBTASK_DATA" ].StackPop (vResult) == M4_SUCCESS)
				{
					/* Comprobamos que el resultado es correcto                */
					if (vResult.Data.DoubleData == 0)
					{
						// Construimos un trabajo simple siempre que la tarea sea una 
						// tarea simple y no compuesta (localId = 0).
						if (vIdJob.Data.DoubleData != 0)
						{
							pTarea = new ClJSSimpleJob( (m4uint_t) vIdJob.Data.DoubleData);
         
         					if (pTarea != NULL)
         					{
									/* Construimos los parámetros                        */
									if ( BuildJobParameters( pTarea) == M4_SUCCESS )
									{
										/* Asociamos la tarea al trabajo principal        */
										if (ai_pMainJob->AddTask( pTarea, uiLocalId) == M4_SUCCESS)
										{
											uiLocalId ++;
											poNode->RecordSet.Current.Next();
										}
										else
										{
											stError = "ClJSCompoundJob::AddTask";
											uiNumError = M4_JS_REC_EXECUTING_METHOD;
										}
									}
									else
									{
										stError = "ClJSJobBuilderFromM4Object::BuildJobParameters";
										uiNumError = M4_JS_REC_EXECUTING_METHOD;
									}

									if (uiNumError != 0)
										delete pTarea;
         					}
							else 
							{
         							stError = "ClJSSimpleJob";
									uiNumError = M4_JS_REC_BUILDING_INSTANCE;
         					}
						}
						else
						{
							// Se contruyen los parametros de la tarea compuesta.
							if (BuildJobParameters(ai_pMainJob) == M4_SUCCESS )
							{
								poNode->RecordSet.Current.Next();
							}
							else
							{
								stError = "ClJSJobBuilderFromM4Object::BuildJobParameters";
								uiNumError = M4_JS_REC_EXECUTING_METHOD;
							}
						}
         			}
         			else
         			{
						stError = "C4_JS_SCHED_DATES.GET_SUBTASK_DATA";
						uiNumError = M4_JS_REC_EXECUTING_METHOD;
         			}
         		}
         		else
         		{
					stError = "C4_JS_SCHED_DATES.GET_SUBTASK_DATA";
					uiNumError = M4_JS_REC_POPPING_PARAMETERS;
         		}
         	}
         	else
         	{
   				stError = "C4_JS_SCHED_DATES.GET_SUBTASK_DATA";
				uiNumError = M4_JS_REC_CALLING_METHOD;
         	}
         }
         else
         {
			stError = "C4_JS_SCHED_DATES.GET_SUBTASK_DATA";
            uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
         } 
      } /* endwhile */
   } 
   else
   {
		stError = "C4_JS_SCHED_DATES.N4_JS_SUBTASK_DEF";
		uiNumError = M4_JS_REC_ACCESSING_NODE;
   } /* endif */

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClJSJobBuilderFromM4Object::BuildJobStructure";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClJSJobBuilderFromM4Object::BuildJobStructure%940243353.body
}

m4return_t ClJSJobBuilderFromM4Object::BuildJobParameters (ClJSJob *ai_pJob)
{
  //## begin ClJSJobBuilderFromM4Object::BuildJobParameters%941187550.body preserve=yes
	m4VariantType vAmbito        ;/* Tipo de parámetro (Entrada / Salida)     */
	m4VariantType vNombre        ;/* Nombre del parámetro                     */
	m4VariantType vTipo          ;/* Tipo de parámetro (Cadena/Número/Fecha)  */
	m4VariantType vValor         ;/* Valor del parámetro (Si lo tiene)        */
	m4VariantType vResult        ;/* Resultado del método LN4                 */
	ClJSParameter * pParametro   ;/* Parámetro que se construye               */
	ClNode * poNode              ;/* Puntero al nodo principal del M4O        */
	M4ClString stError           ;/* Parámetro para los errores               */
	M4ClString stNombreParm      ;/* Nombre del parámetro                     */
	M4ClString stNombreLocal = "";/* Nombre local del parámetro               */
	m4uint32_t uiNumError = 0    ;/* Código de error a generar                */
	m4return_t res = M4_SUCCESS  ;/* Resultado del método                     */
	ClJSParameter::eParameterType eTipoParm ;/* Tipo de parámetro             */

	/* Puntero al nodo secundario del M4Object                                */ 
	poNode = &(m_poSource->Node ["N4_JS_SUBTASK_PARAM"]);
	if (poNode != NULL)
	{
      /* Nos colocamos en el primer registro                                 */
      poNode->RecordSet.Current.Begin();
   
      /* Recorremos el conjunto de registros                                 */
      // while (!poNode->RecordSet.Current.Index.IsEOF() && uiNumError == 0)
	  while (poNode->RecordSet.Current.Index != M4DM_EOF_INDEX && uiNumError == 0)
      {
         /* Metemos los parámetros que nos va a rellenar el método           */
         if (	poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPush (vAmbito) == M4_SUCCESS &&
				poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPush (vNombre) == M4_SUCCESS &&
				poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPush (vTipo)   == M4_SUCCESS &&
				poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPush (vValor)  == M4_SUCCESS )
         {
            /* Llamamos al método GET_PARAM_DATA                             */
            if (poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].Call() == M4_SUCCESS )
            {
               /* Recuperamos los parámetros y el resultado                  */
         	   if (	poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPop (vAmbito) == M4_SUCCESS && 
         			poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPop (vNombre) == M4_SUCCESS &&
         			poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPop (vTipo)   == M4_SUCCESS &&
         			poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPop (vValor)  == M4_SUCCESS &&
         			poNode->RecordSet.Current.Item[ "GET_PARAM_DATA" ].StackPop (vResult) == M4_SUCCESS )
               {
					/* Comprobamos que el resultado es correcto                */
					if (vResult.Data.DoubleData == 0)
					{
						/* Construimos un parámetro									  */
						if (!strcmp( vTipo.Data.PointerChar, "STRING") ||
						!strcmp( vTipo.Data.PointerChar, "EXECUTABLE"))
							eTipoParm = ClJSParameter::TypeString;
						else if (!strcmp( vTipo.Data.PointerChar, "NUMBER"))
							eTipoParm = ClJSParameter::TypeNumber;
						else
							eTipoParm = ClJSParameter::TypeDate;

						stNombreParm = vNombre.Data.PointerChar;

						pParametro = new ClJSParameter( stNombreParm,
														stNombreLocal,
														eTipoParm,
														vAmbito.Data.DoubleData == 1 ? ClJSParameter::FlowInput : ClJSParameter::FlowOutput );
         
         				if (pParametro != NULL)
         				{
								/* Asignación del valor al trabajo                   */
								if ( (vValor.Data.PointerChar != NULL) && (strcmp( vValor.Data.PointerChar, "")!=0))
								{
									if ( pParametro->SetValue( vValor.Data.PointerChar ) != M4_SUCCESS)
									{
										stError = "ClJSParameter::SetValue";
										uiNumError = M4_JS_REC_EXECUTING_METHOD;
									}
								}

								/* Asociamos el parámetro al trabajo                 */
								if (uiNumError == 0)
								{
									if (ai_pJob->AddParameter( pParametro) == M4_SUCCESS)
									{
										poNode->RecordSet.Current.Next();
									}
									else
									{
										stError = "ClJSJob::AddParameter";
										uiNumError = M4_JS_REC_EXECUTING_METHOD;
									}
								}

								if (uiNumError != 0)
									delete pParametro;
         				}
						else 
						{
         					stError = "ClJSParameter";
							uiNumError = M4_JS_REC_BUILDING_INSTANCE;
						}
         			} 
         			else
         			{
						stError = "C4_JS_SCHED_DATES.GET_PARAM_DATA";
						uiNumError = M4_JS_REC_EXECUTING_METHOD;
         			}
         		}
         		else
         		{
    				stError = "C4_JS_SCHED_DATES.GET_PARAM_DATA";
					uiNumError = M4_JS_REC_POPPING_PARAMETERS;
         		}
         	}
         	else
         	{
   				stError = "C4_JS_SCHED_DATES.GET_PARAM_DATA";
   				uiNumError = M4_JS_REC_CALLING_METHOD;
         	}
         }
         else
         {
   			stError = "C4_JS_SCHED_DATES.GET_PARAM_VALUE";
            uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
         } 
      } /* endwhile */
   } 
   else
   {
		stError = "C4_JS_SCHED_DATES.N4_JS_SUBTASK_PARAM";
		uiNumError = M4_JS_REC_ACCESSING_NODE;
   } /* endif */

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClJSJobBuilderFromM4Object::BuildJobParameters";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClJSJobBuilderFromM4Object::BuildJobParameters%941187550.body
}

m4return_t ClJSJobBuilderFromM4Object::BuildParameterConnectors (ClJSJob *ai_pMainJob)
{
  //## begin ClJSJobBuilderFromM4Object::BuildParameterConnectors%941187551.body preserve=yes
	m4VariantType vJobOrigen    ;/* Identificador local del trabajo origen    */
	m4VariantType vJobDestino   ;/* Identificador local del trabajo destino   */
	m4VariantType vParmOrigen   ;/* Nombre del parámetro origen               */
	m4VariantType vParmDestino  ;/* Nombre del parámetro destino              */
	m4VariantType vResult       ;/* Resultado del método LN4                  */
	ClNode * poNode             ;/* Puntero al nodo principal del M4O         */
	M4ClString stError          ;/* Parámetro para los errores                */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */

   /* Puntero al nodo secundario del M4Object                                */ 
   poNode = &(m_poSource->Node ["N4_JS_PARAM_FLOW"]);
   if (poNode != NULL)
   {
      /* Nos colocamos en el primer registro                                 */
      poNode->RecordSet.Current.Begin();
	
   
      /* Recorremos el conjunto de registros                                 */
      // while (!poNode->RecordSet.Current.Index.IsEOF() && (uiNumError == 0))
	  while (poNode->RecordSet.Current.Index != M4DM_EOF_INDEX && (uiNumError == 0))
      {
         /* Metemos los parámetros que nos va a rellenar el método           */
         if (	poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPush (vJobOrigen)   == M4_SUCCESS &&
				poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPush (vParmOrigen)  == M4_SUCCESS &&
				poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPush (vJobDestino)  == M4_SUCCESS &&
				poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPush (vParmDestino) == M4_SUCCESS )
         {
            /* Llamamos al método GET_CONNECTOR_DATA                         */
            if (poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].Call() == M4_SUCCESS )
            {
               /* Recuperamos los parámetros y el resultado                  */
         	   if (	poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPop (vJobOrigen)   == M4_SUCCESS && 
         			poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPop (vParmOrigen)  == M4_SUCCESS &&
         			poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPop (vJobDestino)	 == M4_SUCCESS &&
         			poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPop (vParmDestino) == M4_SUCCESS &&
         			poNode->RecordSet.Current.Item[ "GET_CONNECTOR_DATA" ].StackPop (vResult)      == M4_SUCCESS )
               {
					/* Comprobamos que el resultado es correcto                */
         			if (vResult.Data.DoubleData == 0)
					{
						/* Construimos un parámetro									  */
						if ( ai_pMainJob->AddParameterConnection( (m4uint_t) vJobOrigen.Data.DoubleData,
																	vParmOrigen.Data.PointerChar,
																	(m4uint_t) vJobDestino.Data.DoubleData,
																	vParmDestino.Data.PointerChar) == M4_SUCCESS)
         				{
								poNode->RecordSet.Current.Next();
						}
						else
						{
							stError = "ClJSJob::AddParameterConnection";
							uiNumError = M4_JS_REC_EXECUTING_METHOD;
         				}
         			} 
         			else
         			{
         				stError = "C4_JS_SCHED_DATES.GET_CONNECTOR_DATA";
        		        uiNumError = M4_JS_REC_EXECUTING_METHOD;
         			}
         		}
         		else
         		{
      				stError = "C4_JS_SCHED_DATES.GET_CONNECTOR_DATA";
					uiNumError = M4_JS_REC_POPPING_PARAMETERS;
         		}
         	}
         	else
         	{
   				stError = "C4_JS_SCHED_DATES.GET_CONNECTOR_DATA";
   				uiNumError = M4_JS_REC_CALLING_METHOD;
         	}
         }
         else
         {
			stError = "C4_JS_SCHED_DATES.GET_CONNECTOR_DATA";
            uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
         } 
      } /* endwhile */
   } 
   else
   {
		stError = "C4_JS_SCHED_DATES.N4_JS_PARAM_FLOW";
		uiNumError = M4_JS_REC_ACCESSING_NODE;
   } /* endif */

   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClJSJobBuilderFromM4Object::BuildParameterConnectors";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClJSJobBuilderFromM4Object::BuildParameterConnectors%941187551.body
}

m4return_t ClJSJobBuilderFromM4Object::ReadScheduledJobs (m4uint_t &ai_uiMaxNumJobs)
{
  //## begin ClJSJobBuilderFromM4Object::ReadScheduledJobs%941096725.body preserve=yes
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */
	M4ClString stError = ""     ;/* Mensajes de error                         */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */

	// Bug 0291201. Construir el mapa de trabajaos aunque falle el método que 
	// recupera las tareas pendientes de ser ejeuctadas.
	// Esto nos va a permitir desmarcarlas posteriormente.
	res = InitializeScheduledJobsNode(ai_uiMaxNumJobs);
	if ( res != M4_SUCCESS )
	{
		uiNumError = M4_JS_REC_EXECUTING_METHOD;
		stError = "ClJSJobBuilderFromM4Object->InitializeScheduledJobsNode";
	}

	if ( BuildJobs( M4_FALSE) != M4_SUCCESS )
	{
		uiNumError = M4_JS_REC_EXECUTING_METHOD;
		stError = "ClJSJobBuilderFromM4Object->BuildJobs";
	}
	
	/* Información de errores                                                 */
   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClJSJobBuilderFromM4Object::ReadScheduledJobs";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClJSJobBuilderFromM4Object::ReadScheduledJobs%941096725.body
}

m4return_t ClJSJobBuilderFromM4Object::ReadCancelledJobs ()
{
  //## begin ClJSJobBuilderFromM4Object::ReadCancelledJobs%941096726.body preserve=yes
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */
	M4ClString stError = ""     ;/* Mensajes de error                         */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */

	if ( InitializeCancelledJobsNode() == M4_SUCCESS )
	{
		if ( BuildJobs( M4_TRUE) != M4_SUCCESS )
		{
			uiNumError = M4_JS_REC_EXECUTING_METHOD;
			stError = "ClJSJobBuilderFromM4Object->BuildJobs";
		}
	}
	else
	{
		uiNumError = M4_JS_REC_EXECUTING_METHOD;
		stError = "ClJSJobBuilderFromM4Object->InitializeCancelledJobsNode";
	}

	/* Información de errores                                                 */
   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClJSJobBuilderFromM4Object::ReadCancelledJobs";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;

  //## end ClJSJobBuilderFromM4Object::ReadCancelledJobs%941096726.body
}

m4return_t ClJSJobBuilderFromM4Object::InitializeScheduledJobsNode (m4uint_t &ai_uiMaxNumJobs)
{
  //## begin ClJSJobBuilderFromM4Object::InitializeScheduledJobsNode%941120255.body preserve=yes
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */
	M4ClString stError = ""     ;/* Mensajes de error                         */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */

	m4VariantType vQueremos     ;/* Parámetro para LN4: num trabajos pedidos  */
	m4VariantType vQuedan       ;/* Parámetro para LN4: num trabajos restantes*/
	m4VariantType vRes          ;/* Parámetro para LN4: resultado métodos     */
	ClNode * poNode             ;/* Puntero al nodo principal del M4Object    */

	/* Pedimos un puntero al nodo principal del M4Object                      */
	poNode = &(m_poSource-> Node ["N4_JS_SCHED_DATES"]);
	if ( poNode )
	{
	   /* Ponemos en la pila el número de trabajos que queremos recuperar     */
		vQueremos.Type = M4CL_CPP_TYPE_NUMBER;
		vQueremos.Data.DoubleData = ai_uiMaxNumJobs;
		if ( poNode->RecordSet.Current.Item[ "GET_NEXT_DATES" ].StackPush(vQueremos) == M4_SUCCESS &&
			 poNode->RecordSet.Current.Item[ "GET_NEXT_DATES" ].StackPush(vQuedan) == M4_SUCCESS)
		{
			/* Llamamos al método GET_NEXT_DATES                                */
			if (poNode->RecordSet.Current.Item[ "GET_NEXT_DATES" ].Call() == M4_SUCCESS )
			{
				/* Recuperamos de la pila el resultado del método, que es el     */
				/* número de trabajos que quedan pendientes                      */
				if (poNode->RecordSet.Current.Item[ "GET_NEXT_DATES" ].StackPop (vQuedan) == M4_SUCCESS &&
					poNode->RecordSet.Current.Item[ "GET_NEXT_DATES" ].StackPop (vRes) == M4_SUCCESS)
				{
					if ( vRes.Data.DoubleData == 0 )
					{
						m_uiRemainingJobs = (m4uint_t) vQuedan.Data.DoubleData;
					}
					else
					{
						stError = "C4_JS_SCHED_DATES.GET_NEXT_DATES";
						uiNumError = M4_JS_REC_EXECUTING_METHOD;
					}
				}
				else
				{
					stError = "C4_JS_SCHED_DATES.GET_NEXT_DATES";
					uiNumError = M4_JS_REC_POPPING_PARAMETERS;
				}
			}
			else 
			{
				stError = "C4_JS_SCHED_DATES.GET_NEXT_DATES";
				uiNumError = M4_JS_REC_CALLING_METHOD;
			}
		}
		else
		{
			uiNumError = M4_JS_REC_PUSHING_PARAMETERS;
		}
   }

	/* Información de errores                                                 */
   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClJSJobBuilderFromM4Object::InitializeScheduledJobsNode";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClJSJobBuilderFromM4Object::InitializeScheduledJobsNode%941120255.body
}

m4return_t ClJSJobBuilderFromM4Object::InitializeCancelledJobsNode ()
{
  //## begin ClJSJobBuilderFromM4Object::InitializeCancelledJobsNode%941120256.body preserve=yes
	m4return_t res = M4_SUCCESS ;/* Resultado del método                      */
	M4ClString stError = ""     ;/* Mensajes de error                         */
	m4uint32_t uiNumError = 0   ;/* Código de error a generar                 */
	m4VariantType vQueremos     ;/* Parámetro para LN4: num trabajos pedidos  */
	m4VariantType vRes          ;/* Parámetro para LN4: resultado métodos     */
	ClNode * poNode             ;/* Puntero al nodo principal del M4Object    */

	/* Pedimos un puntero al nodo principal del M4Object                      */
	poNode = &(m_poSource-> Node ["N4_JS_SCHED_DATES"]);
	if ( poNode )
	{
		/* Llamamos al método TASKS_TO_BE_INTERRUPTED                       */
		if (poNode->RecordSet.Current.Item[ "TASKS_TO_BE_INTERRUPTED" ].Call() == M4_SUCCESS )
		{
			/* Recuperamos de la pila el resultado del método                   */
			if ( poNode->RecordSet.Current.Item[ "TASKS_TO_BE_INTERRUPTED" ].StackPop (vRes) == M4_SUCCESS)
			{
				if (vRes.Data.DoubleData == -1) 
				{
					stError = "C4_JS_SCHED_DATES.TASKS_TO_BE_INTERRUPTED";
					uiNumError = M4_JS_REC_EXECUTING_METHOD;
				}
			}
			else
			{
				stError = "C4_JS_SCHED_DATES.TASKS_TO_BE_INTERRUPTED";
				uiNumError = M4_JS_REC_POPPING_PARAMETERS;
			}
		}
		else 
		{
			stError = "C4_JS_SCHED_DATES.TASKS_TO_BE_INTERRUPTED";
			uiNumError = M4_JS_REC_CALLING_METHOD;
		}
   }

	/* Información de errores                                                 */
   if (uiNumError > 0)
   {
      BL_BEGIN( ERRORLOG, uiNumError);
      BL_OBJ << stError << "ClJSJobBuilderFromM4Object::InitializeCancelledJobsNode";
      BL_END;
      res = M4_ERROR;
   } /* endif */

	return res;
  //## end ClJSJobBuilderFromM4Object::InitializeCancelledJobsNode%941120256.body
}

// Additional Declarations
  //## begin ClJSJobBuilderFromM4Object%3773A3520184.declarations preserve=yes
  //## end ClJSJobBuilderFromM4Object%3773A3520184.declarations

//## begin module%37E602220252.epilog preserve=yes
//## end module%37E602220252.epilog
