// Declaración de la Clase CEvaluacion

//##ModelId=397307430176
class CEvaluacion
{
public:
// Constructor y destructor
	//##ModelId=3973074301C8
	CEvaluacion();
	//##ModelId=3973074301C7
	~CEvaluacion();

// Declaración de Datos Miembros
private:
	//##ModelId=39730743019E
	char *m_expresion;
	//##ModelId=397307430194
	CArbol m_arbol;
	//##ModelId=397307430180
	BOOL m_definida;

// Declaración de Funciones Miembros
public:
	//##ModelId=3973074301BD
	int Define(char *valor, char *exp);
	//##ModelId=3973074301B4
	BOOL Resuelve(void);

private:
	//##ModelId=3973074301AB
	int Busca_Operador(char *str, CNodo *parent=NULL);
	//##ModelId=39730743019F
	int GetFirstOp(char *str, char **pstr);
};
