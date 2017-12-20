#ifndef __SOFTTRANS__H__
#define __SOFTTRANS__H__

//*******************Enums del array de translación de caracteres

#define ENG_PSC_AFM_WIDTHS_ARRAY_SIZE 256

enum ENG_SOFT_TRANS_TYPE {	
	ENG_SOFT_TRANS_TYPE_EMPTY='-',	//Valor vacio
	ENG_SOFT_TRANS_TYPE_I_2_5='I',	//Tipo I_2_5
	ENG_SOFT_TRANS_TYPE_128='8',		 //Tipo 128
	ENG_SOFT_TRANS_TYPE_POSTNET='P',		 //Tipo PostNet
	ENG_SOFT_TRANS_TYPE_PSC_MIXED='M'		 //Tipo PostScript Mixed
} ;

enum ENG_SOFT_TRANS_INDEX {
	
	//***Valores comunes a todos tipos de translations

	//Solo se admiten 255 valores de translacion, (la pos 0 se ocupa con el tipo)

	ENG_TRANS_ARRAY_SIZE=256,	

	//Indice para leer el tipo de bloque (ENG_SOFT_TRANS_TYPE_I_2_5, etc)

	ENG_TRANS_TYPE_POS=0,	

	//***Indices para Interleaved 2 of 5

	ENG_TRANS_I_2_5_START=32,	
	ENG_TRANS_I_2_5_STOP=33,	
	//Pos comienzo del digito-valor 00. El 01, 02, etc van consecutivos,  hasta el 99
	ENG_TRANS_I_2_5_ZERO=100,	
	//Count del numero de digitos, desde ENG_TRANS_I_2_5_ZERO (00) hasta el (99)
	ENG_TRANS_I_2_5_DIGIT_COUNT=100, 
	//Count del total de chars empleados en I_2_5
	ENG_TRANS_I_2_5_COUNT=102, 
	
	//***Indices para 128

	//---Solo para C, digitos 00 a 99

	ENG_TRANS_128_C_ZERO_POS=1,			//Como Interleaved ...
	ENG_TRANS_128_C_DIGIT_COUNT=100,	//... hay 100 valores

	//---Comunes a set A, B y C, chars de control

	ENG_TRANS_128_FNC1=101,
	ENG_TRANS_128_START_A,
	ENG_TRANS_128_START_B,
	ENG_TRANS_128_START_C,
	ENG_TRANS_128_STOP,

	//---FNC y otros de control no disponibles en C (FNC1 si, en todos)

	ENG_TRANS_128_A_FNC2=106,
	ENG_TRANS_128_B_FNC2,
	ENG_TRANS_128_A_FNC3,
	ENG_TRANS_128_B_FNC3,
	ENG_TRANS_128_A_FNC4,
	ENG_TRANS_128_B_FNC4,
	ENG_TRANS_128_A_SHIFT,
	ENG_TRANS_128_B_SHIFT,

	//---Cambios de Set

	ENG_TRANS_128_B_TO_A=114,
	ENG_TRANS_128_B_TO_C,
	ENG_TRANS_128_A_TO_B,
	ENG_TRANS_128_A_TO_C,
	ENG_TRANS_128_C_TO_A,
	ENG_TRANS_128_C_TO_B,

	//---Mapeos ASCII (rangos porque no podemos meter mas de 255 translation values)

	ENG_TRANS_128_A_CTRL=120,		//ASCII 0-31, chars de control (solo A)

	ENG_TRANS_128_A_BLANK,			//ASCII 32-47 blancos y punt (A y B)
	ENG_TRANS_128_B_BLANK,

	ENG_TRANS_128_A_DIGIT,			//ASCII 48-57 digitos (A y B)
	ENG_TRANS_128_B_DIGIT,

	ENG_TRANS_128_A_PUNCT,			//ASCII 58-64 mas puntuadores (A y B)
	ENG_TRANS_128_B_PUNCT,

	ENG_TRANS_128_A_MAY,			//ASCII 65-90 mayusculas (A y B)
	ENG_TRANS_128_B_MAY,

	ENG_TRANS_128_A_PUNCT2,			//ASCII 91-95 mas puntuadores (A y B)
	ENG_TRANS_128_B_PUNCT2,

	ENG_TRANS_128_B_ASC96,			//ASCII 96 (solo B)

	ENG_TRANS_128_B_MIN,			//ASCII 97-122, minusculas (solo B)

	ENG_TRANS_128_B_123,			//ASCII 123-127, llaves y otros (solo B)

	//---Total de entradas usadas

	ENG_TRANS_128_COUNT = 133,

	//***Indices para PostNet
	
	ENG_TRANS_POSTNET_START = 1,
	ENG_TRANS_POSTNET_STOP = 2,
	ENG_TRANS_POSTNET_ZERO = 3,
	ENG_TRANS_POSTNET_DIGIT_COUNT =10,

	ENG_TRANS_POSTNET_COUNT = ENG_TRANS_POSTNET_DIGIT_COUNT+2,

	//***Indices para ENG_SOFT_TRANS_TYPE_PSC_MIXED

	ENG_TRANS_PSC_MIXED_SHIFT_IN = 1,
	ENG_TRANS_PSC_MIXED_SHIFT_OUT = 2,

	ENG_TRANS_PSC_MIXED_EURO = 3,

	ENG_TRANS_PSC_MIXED_COUNT = 3

} ;

#endif
