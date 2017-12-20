
#ifndef M4DETREE_HPP
#define M4DETREE_HPP

#include "stdafx.h"


#define M4DELETE(X)		if (X) delete X; X = NULL;
#define M4DELETEV(X)		if (X) delete [] X; X = NULL;


#define M4TREE_DEFAULT_NODE         0
#define M4TREE_DEFAULT_PROPERTY     0
#define M4TREE_ALL_LEVEL            0

#define M4TREE_CURRENT_NODE         0
#define M4TREE_ROOT_NODE			1
#define M4TREE_FIRST_NODE			1
#define M4TREE_NEXT_NODE			2

#define M4TREE_PARENT				0
#define M4TREE_FIRST_CHILD			1
#define M4TREE_LAST_CHILD			2
#define M4TREE_NEXT_SIBLING			3
#define M4TREE_PREV_SIBLING			4


// Errores.
#define M4TREE_NO_ERROR				0
#define M4TREE_NO_TREE				-1000
#define M4TREE_NO_NODE				-1001
#define M4TREE_NO_STYLE				-1002
#define M4TREE_NO_PROPERTY			-1003
#define M4TREE_NO_MEMORY  			-1004
#define M4TREE_NO_FILE    			-1005


// Propiedad Estilo del árbol.
#define M4TREE_TREE_VERT			1
#define M4TREE_TREE_HORZ			2
#define M4TREE_TREE_DIAG			3


// Propiedad Apariencia del árbol.
#define M4TREE_TREE_FLAT			1
#define M4TREE_TREE_SHADOW			2
#define M4TREE_TREE_3D				3
#define M4TREE_NO_BORDER			4


// Propiedad Tipo de línea.
#define M4TREE_LINK_SOLID			1
#define M4TREE_LINK_DASH			2
#define M4TREE_LINK_DOT				3
#define M4TREE_LINK_NULL			4


// Propiedad Estilo de línea.
#define M4TREE_LINK_ANGLED			1
#define M4TREE_LINK_DIRECT			2


// Rango de la barra de scroll.
#define M4TREE_SCROLL_MIN 			0
#define M4TREE_SCROLL_MAX 			30000


#define M4TREE_SIZE_LIST_NODE		1024
#define M4TREE_SIZE_LIST_STYLE		256
#define M4TREE_SIZE_STYLE			8


//Definición de los prefijos de cada control.
#define M4TREE_CONTROL_A	'A'	// Todo el nodo.
#define M4TREE_CONTROL_T	'T'	// Texto.
#define M4TREE_CONTROL_P	'P'	// Imagen.

//Propiedades comunes a todos los controles.
#define M4TREE_PROP_X	"_X"	// Origen X del control respecto al origen del nodo.
#define M4TREE_PROP_Y	"_Y"	// Origen Y del control respecto al origen del nodo.
#define M4TREE_PROP_CX	"_CX"	// Anchura del control.
#define M4TREE_PROP_CY	"_CY"	// Altura del control.

//Propiedades específicas del nodo.
#define M4TREE_PROP_CVO	"_CVO"	// Controles visibles para el estado NODO ABIERTO.
#define M4TREE_PROP_CVC	"_CVC"	// Controles visibles para el estado NODO CERRADO.
#define M4TREE_PROP_CVN	"_CVN"	// Controles visibles para el estado NODO SIN HIJOS.
#define M4TREE_PROP_BC	"_BC"	// Color de fondo.
#define M4TREE_PROP_ST	"_ST"	// Estilo del nodo.

//Propiedades específicas de un control texto.
#define M4TREE_PROP_TT	"_TT"	// Texto.
#define M4TREE_PROP_FC	"_FC"	// Color del texto.
#define M4TREE_PROP_FN	"_FN"	// Nombre de la fuente.
#define M4TREE_PROP_FS	"_FS"	// Tamaño de la fuente.
#define M4TREE_PROP_FW	"_FW"	// Anchura de la fuente.
#define M4TREE_PROP_FI	"_FI"	// Bandera de cursiva.
#define M4TREE_PROP_FU	"_FU"	// Bandera de subrayado.
#define M4TREE_PROP_FK	"_FK"	// Bandera de tachado.

//Propiedades específicas de un control bitmap.
#define M4TREE_PROP_BT	"_BT"	// Bitmap.


#endif

