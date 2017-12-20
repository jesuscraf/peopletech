//______________________________________
// 
//       Fichero de recursos

#ifdef _WINDOWS
#include <windows.h>
#endif
#include <string.h>

	typedef struct {
              int   key;
  			   char *data;
			} t_element;


	t_element g_recursosobl[] = {  
    {2000, "NO TIENE PERMISO SOBRE ESTE OBJETO"},
    {2100, "Fallo al reservar memoria de un objeto."},
    {2110, "Match erroneo."},
    {2140, "No hay más handles disponibles."},
    {2150, "Handle de objeto a NULL."},
    {2151, "No hay más handles disponibles."},
    {2152, "Se ha sobrepasado el numero de handles permitidos"},
    {2160, "Error de escritura en un bloque."},
    {2161, "Objeto a NULL."},
    {2162, "Se intenta asignar una cadena a nula."},
    {2170, "No se puede borrar un bloque que contengan subbloques o propiedades."},
    {2175, "Bloque no encontrado."},
    {2180, "Error en el índice del array de bloques."},
    {2181, "En el fichero falta la palabra versión"},
    {2183, "Se espera un retorno de carro en el fichero. "},
    {2184, "Se espera un BEGIN en el fichero."},
    {2188, "Esperaba un \"=\"  en el fichero."},
    {2189, "Se espera un REM o un salto carro en el fichero."},
    {2190, "Falta la clase."},
    {2191, "Falta la instancia"},
    {2192, "Falta texto en el fichero."},
    {2193, "Error mientras se copiaba los bloques."},
    {2299, "Se ha producido un error sintáxtico en un casting.  "},
    {2300, "Se intento asignar una propiedad nula."},
    {2305, "Asignar propiedad o instancia de bloque vacía."},
    {2310, "Cadena muy larga."},
    {2315, "Handle mal definido."},
    {2320, "Se intenta modificar una propiedad inexistente."},
    {2330, "Se intenta buscar una cadena nula."},
    {2331, "No hay propiedades."},
    {2340, "Objeto no encontrado."},
    {2345, "Tipo de busqueda equivocado."},
    {2350, "Tipo de propiedad erroneo."},
    {2500, "No se puede generar un bloque ya existente."},
    {3100, "Versión no definida."},
    {3110, "Bloque a NULL."},
    {3115, "No se puede borrar el bloque raiz."},
    {3120, "No hay  más handles disponibles."},
    {3121, "Tipo de objeto es erroneo."},
    {3140, "Se ha pasado un path a nulo."},
    {3141, "Path no encontrado."},
    {3143, "Path erroneo."},
    {3190, "Falta comillas."},
    {3191, "Formato de número erroneo."},
    {3200, "Se ha intentado asignar un handle incorrecto."},
    {3201, "Se ha pasado un parametro null."},
    {3210, "Se ha pasado un buffer muy grande."},
    {3211, "Se ha pasado una cadena pequeña."},
    {3220, "Se ha intentado usar un handle erroneo."},
    {3230, "Fichero nulo."},
    {3300, "Resultado truncado."},
    {3301, "Hay que escribir el fichero priméro."},
    {3400, "Handle de match erroneo."},
    {3450, "Error cuando se intentó reparar el array de handles."},
    {10000, "Se ha corrompido el tipo de un handle."},
    {10001, "Busqueda de math erronea."}    };

int g_recursos_len_obl = 56;

int LoadString_obl(int ai_ikey, char * lpBuffer,int nBufferMax )
{
	int i;
	size_t min_len;

	for ( i=0; i< g_recursos_len_obl; i++)
	{
		if ( g_recursosobl[i].key  == ai_ikey)
		{
			min_len = strlen(g_recursosobl[i].data)+1;
			if ( nBufferMax < min_len)
			{
				min_len = nBufferMax;
			}
			memcpy(lpBuffer , g_recursosobl[i].data, min_len-1);
			lpBuffer[min_len-1] = 0;
			return (int) min_len;
		}
	}
	return 0;
}
