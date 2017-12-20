// Declaración de la clase Operador. Clase base para todos los tipo de operadores
//##ModelId=3973073C03C5
class COperador
{
public:
	//##ModelId=3973073D0019
	COperador();
	//##ModelId=3973073D0012
	virtual ~COperador();

	//##ModelId=3973073D000F
	virtual void SetParam1(char *par);
	//##ModelId=3973073D0005
	virtual void SetParam2(char *par);
	//##ModelId=3973073C03E5
	virtual void SetParam3(char *par);
	//##ModelId=3973073C03DC
	virtual void SetParam4(char *par);
	//##ModelId=3973073C03D9
	virtual void SetParam5(char *par);
	//##ModelId=3973073C03CF
	virtual int Opera(char **res)=0;
};


// Declaración de la clase COLen
//##ModelId=3973073C022B
class COLen : public COperador
{
public:
	//##ModelId=3973073C0249
	COLen();
	//##ModelId=3973073C0242
	~COLen();

private:
	//##ModelId=3973073C0236
	char *m_str;

public:
	//##ModelId=3973073C0240
	void SetParam1(char *par);
	//##ModelId=3973073C0237
	int Opera(char **res);
};


// Declaración de la clase COPos
//##ModelId=3973073902D1
class COPos : public COperador
{
public:
	//##ModelId=3973073902FB
	COPos();
	//##ModelId=3973073902FA
	~COPos();

private:
	//##ModelId=3973073902E5
	char *m_substr;
	//##ModelId=3973073902DC
	char *m_str;

public:
	//##ModelId=3973073902F2
	void SetParam1(char *par);
	//##ModelId=3973073902F0
	void SetParam2(char *par);
	//##ModelId=3973073902E6
	int Opera(char **res);
};

// Declaración de la clase COUpper
//##ModelId=3973074E01CC
class COUpper : public COperador
{
public:
	//##ModelId=3973074E01EC
	COUpper();
	//##ModelId=3973074E01EB
	~COUpper();

private:
	//##ModelId=3973074E01D7
	char *m_str;

public:
	//##ModelId=3973074E01E2
	void SetParam1(char *par);
	//##ModelId=3973074E01E0
	int Opera(char **res);
};

// Declaración de la clase COLower
//##ModelId=3973074501E7
class COLower : public COperador
{
public:
	//##ModelId=397307450209
	COLower();
	//##ModelId=397307450208
	~COLower();

private:
	//##ModelId=3973074501F2
	char *m_str;

public:
	//##ModelId=397307450206
	void SetParam1(char *par);
	//##ModelId=3973074501FC
	int Opera(char **res);
};

// Declaración de la clase COFupper
//##ModelId=39730744007E
class COFupper : public COperador
{
public:
	//##ModelId=39730744009D
	COFupper();
	//##ModelId=39730744009C
	~COFupper();

private:
	//##ModelId=397307440089
	char *m_str;

public:
	//##ModelId=397307440094
	void SetParam1(char *par);
	//##ModelId=397307440092
	int Opera(char **res);
};

// Declaración de la clase COLeft
//##ModelId=39730735014E
class COLeft : public COperador
{
public:
	//##ModelId=397307350180
	COLeft();
	//##ModelId=397307350179
	~COLeft();

private:
	//##ModelId=397307350162
	char *m_str;
	//##ModelId=397307350159
	long m_pos;

public:
	//##ModelId=397307350177
	void SetParam1(char *par);
	//##ModelId=39730735016E
	void SetParam2(char *par);
	//##ModelId=39730735016C
	int Opera(char **res);
};

// Declaración de la clase CORight
//##ModelId=397307410340
class CORight : public COperador
{
public:
	//##ModelId=39730741036A
	CORight();
	//##ModelId=397307410369
	~CORight();

private:
	//##ModelId=397307410354
	char *m_str;
	//##ModelId=39730741034B
	long m_pos;

public:
	//##ModelId=397307410360
	void SetParam1(char *par);
	//##ModelId=39730741035E
	void SetParam2(char *par);
	//##ModelId=397307410355
	int Opera(char **res);
};

// Declaración de la clase COMid
//##ModelId=39730741020A
class COMid : public COperador
{
public:
	//##ModelId=397307410248
	COMid();
	//##ModelId=397307410247
	~COMid();

private:
	//##ModelId=397307410228
	char *m_str;
	//##ModelId=397307410220
	long m_pos1;
	//##ModelId=39730741021F
	long m_pos2;

public:
	//##ModelId=39730741023D
	void SetParam1(char *par);
	//##ModelId=397307410235
	void SetParam2(char *par);
	//##ModelId=397307410233
	void SetParam3(char *par);
	//##ModelId=397307410229
	int Opera(char **res);
};

// Declaración de la clase COTraductString
//##ModelId=3973073503D9
class COTraductString : public COperador
{
public:
	//##ModelId=39730736002F
	COTraductString();
	//##ModelId=39730736002E
	~COTraductString();

private:
	//##ModelId=39730736000F
	char *m_str;
	//##ModelId=397307360006
	char *m_cad1;
	//##ModelId=397307360005
	char *m_cad2;
	//##ModelId=3973073503E4
	char *m_cad3;

public:
	//##ModelId=397307360025
	void SetParam1(char *par);
	//##ModelId=397307360023
	void SetParam2(char *par);
	//##ModelId=39730736001B
	void SetParam3(char *par);
	//##ModelId=397307360019
	void SetParam4(char *par);
	//##ModelId=397307360010
	int Opera(char **res);
};

// Declaración de la clase COIn
//##ModelId=3973074E00DC
class COIn : public COperador
{
public:
	//##ModelId=3973074E010E
	COIn();
	//##ModelId=3973074E0106
	~COIn();

private:
	//##ModelId=3973074E00F1
	char *m_val;
	//##ModelId=3973074E00F0
	char *m_grupo;

public:
	//##ModelId=3973074E0104
	void SetParam1(char *par);
	//##ModelId=3973074E00FC
	void SetParam2(char *par);
	//##ModelId=3973074E00FA
	int Opera(char **res);
};

// Declaración de la clase COMayor
//##ModelId=397307380397
class COMayor : public COperador
{
public:
	//##ModelId=3973073803C0
	COMayor();
	//##ModelId=3973073803BF
	~COMayor();

private:
	//##ModelId=3973073803A3
	long m_par1;
	//##ModelId=3973073803A2
	long m_par2;

public:
	//##ModelId=3973073803B6
	void SetParam1(char *par);
	//##ModelId=3973073803AD
	void SetParam2(char *par);
	//##ModelId=3973073803AB
	int Opera(char **res);
};

// Declaración de la clase COMenor
//##ModelId=3973073E0025
class COMenor : public COperador
{
public:
	//##ModelId=3973073E0045
	COMenor();
	//##ModelId=3973073E0044
	~COMenor();

private:
	//##ModelId=3973073E0030
	long m_par1;
	//##ModelId=3973073E002F
	long m_par2;

public:
	//##ModelId=3973073E003C
	void SetParam1(char *par);
	//##ModelId=3973073E003A
	void SetParam2(char *par);
	//##ModelId=3973073E0031
	int Opera(char **res);
};

// Declaración de la clase COMayorIgual
//##ModelId=397307350267
class COMayorIgual : public COperador
{
public:
	//##ModelId=39730735028F
	COMayorIgual();
	//##ModelId=397307350287
	~COMayorIgual();

private:
	//##ModelId=397307350272
	long m_par1;
	//##ModelId=397307350271
	long m_par2;

public:
	//##ModelId=397307350285
	void SetParam1(char *par);
	//##ModelId=39730735027C
	void SetParam2(char *par);
	//##ModelId=397307350273
	int Opera(char **res);
};

// Declaración de la clase COMenorIgual
//##ModelId=3973073901B8
class COMenorIgual : public COperador
{
public:
	//##ModelId=3973073901E0
	COMenorIgual();
	//##ModelId=3973073901D8
	~COMenorIgual();

private:
	//##ModelId=3973073901C3
	long m_par1;
	//##ModelId=3973073901C2
	long m_par2;

public:
	//##ModelId=3973073901D6
	void SetParam1(char *par);
	//##ModelId=3973073901CE
	void SetParam2(char *par);
	//##ModelId=3973073901CC
	int Opera(char **res);
};

// Declaración de la clase COIgual
//##ModelId=3973074D0398
class COIgual : public COperador
{
public:
	//##ModelId=3973074D03D4
	COIgual();
	//##ModelId=3973074D03CB
	~COIgual();

private:
	//##ModelId=3973074D03B6
	char *m_par1;
	//##ModelId=3973074D03AD
	char *m_par2;

public:
	//##ModelId=3973074D03C2
	void SetParam1(char *par);
	//##ModelId=3973074D03C0
	void SetParam2(char *par);
	//##ModelId=3973074D03B7
	int Opera(char **res);
};

// Declaración de la clase CODistinto
//##ModelId=3973073800B2
class CODistinto : public COperador
{
public:
	//##ModelId=3973073800DA
	CODistinto();
	//##ModelId=3973073800D3
	~CODistinto();

private:
	//##ModelId=3973073800BE
	char *m_par1;
	//##ModelId=3973073800BD
	char *m_par2;

public:
	//##ModelId=3973073800D1
	void SetParam1(char *par);
	//##ModelId=3973073800C8
	void SetParam2(char *par);
	//##ModelId=3973073800C6
	int Opera(char **res);
};


// Declaración de la clase COGetStringFromTo
//##ModelId=397307380257
class COGetStringFromTo : public COperador
{
public:
	//##ModelId=39730738029F
	COGetStringFromTo();
	//##ModelId=39730738029E
	~COGetStringFromTo();

private:
	//##ModelId=397307380276
	char *m_par1;
	//##ModelId=397307380275
	char *m_par2;
	//##ModelId=39730738026C
	int  m_par3;
	//##ModelId=39730738026B
	char *m_par4;
	//##ModelId=397307380262
	int  m_par5;

public:
	//##ModelId=397307380296
	void SetParam1(char *par);
	//##ModelId=397307380294
	void SetParam2(char *par);
	//##ModelId=39730738028C
	void SetParam3(char *par);
	//##ModelId=39730738028A
	void SetParam4(char *par);
	//##ModelId=397307380281
	void SetParam5(char *par);
	//##ModelId=39730738027F
	int Opera(char **res);
};


// Declaración de la clase COperando
//##ModelId=3973073E0115
class COperando : public COperador
{
public:
	//##ModelId=3973073E013D
	COperando(char *par);
	//##ModelId=3973073E0135
	~COperando();

private:
	//##ModelId=3973073E0129
	char *m_operando;

public:
	//##ModelId=3973073E0133
	void SetParam1(char *par);
	//##ModelId=3973073E012A
	int Opera(char **res);
};
