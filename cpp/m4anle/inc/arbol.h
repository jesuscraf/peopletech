// Declaración de las clases CNodo y CArbol

//##ModelId=397307410001
class CNodo
{
public:
	//##ModelId=3973074100DE
	CNodo();
	//##ModelId=3973074100DD
	~CNodo();

	//##ModelId=39730741008F
	CNodo *m_nodo1;
	//##ModelId=397307410079
	CNodo *m_nodo2;
	//##ModelId=39730741005D
	CNodo *m_nodo3;
	//##ModelId=397307410049
	CNodo *m_nodo4;
	//##ModelId=397307410035
	CNodo *m_nodo5;
	//##ModelId=39730741001F
	COperador *m_operador;
	//##ModelId=39730741000B
	char *m_resultado;

	//##ModelId=3973074100BF
	int Inserta(CNodo *parent, CNodo *newnodo);
	//##ModelId=397307410097
	int Opera(void);
};


//##ModelId=397307440395
class CArbol
{
public:
	//##ModelId=3973074403D2
	CArbol();
	//##ModelId=3973074403D1
	~CArbol();

private:
	//##ModelId=3973074403AB
	CNodo *m_raiz;

public:
	//##ModelId=3973074403C7
	int Inserta(CNodo *parent, CNodo *newnodo);
	//##ModelId=3973074403BE
	BOOL Resuelve(void);
	//##ModelId=3973074403B3
	int Limpia(void);
};
