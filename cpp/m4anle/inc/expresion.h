// Declaración de la Clase CExpresion

#define MAX_EVALUACIONES	255
#define MAX_OPERADORES		255

//##ModelId=397307430325
class CExpresion
{
public:
// Constructor y destructor
	//##ModelId=39730743039F
	CExpresion();
	//##ModelId=39730743039E
	~CExpresion();

// Declaración de Datos Miembros
private:
	//##ModelId=397307430357
	CEvaluacion	*m_pevaluaciones[MAX_EVALUACIONES];
	//##ModelId=397307430344
	int	m_operadores[MAX_OPERADORES];
	//##ModelId=397307430343
	int m_num_evaluaciones;
	//##ModelId=39730743033A
	int m_num_operadores;
	//##ModelId=397307430339
	char *m_expresion;
	//##ModelId=39730743032F
	BOOL m_definida;

// Declaración de Funciones Miembros
public:
	//##ModelId=397307430393
	int Define(char *valor, char *tipo, char *exp);
	//##ModelId=39730743038A
	BOOL Evalua(void);

private:
	//##ModelId=397307430380
	int Busca_Evaluacion(char **eva, int n);
	//##ModelId=397307430377
	int Busca_Operador(int n);
	//##ModelId=39730743036C
	BOOL Opera(BOOL operador1, int op_logico, BOOL operador2);
	//##ModelId=397307430363
	void FormateaExpresion(char *exp);
	//##ModelId=397307430361
	BOOL CheckExpresion(char *expresion);
};
