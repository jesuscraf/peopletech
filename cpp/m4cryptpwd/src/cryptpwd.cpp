//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              cryptpwd 
// File:                cryptpwd.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                1998/9/2
// Language:            C++
// Operating System:    WINDOWS
// 
// Definition:
//
//  Una utilidad que permite encryptar una password para su
//	uso como pwd de acceso a la base de datos. Como salida 
//	genera un archivo .reg que puede ser incluido en el proceso
//	de build para generar las claves necesarias en el registro.
//
//==============================================================================


#include <stdio.h>
#include <string.h>
#include "cldefine.hpp"
#include "m4cryptc.hpp"
#include "m4regdef.hpp"

#if defined(WINDOWS) || defined(_WINDOWS)
#include "getopt.h"
#else	//UNIX
#include <stdlib.h>	// int getopt(int argc, char **argv, char *optstring);
#endif


#define	M4CH_DUMMY		"\xD\x5\x8\xD\xB\xA\x3\x3\x7\x9\x1\xE\x4\x5\x2\xF\xC\x2\xD\xA\x7\xD\xB\x8\x5\xA\x2\x7\xA\xF\x1\xF\xD\xA\x6\x6\x4\xD\xE\x8"

static void usage(char* progname)
{
    fprintf(stderr, "Usage: %s [-d dsn] [-s server] [-u user] -p password [-o file.reg]\n", progname);
}

static char		g_dsn[256]    = "ORA32_INT_01";
static char		g_server[256] = "MANANOTCP";
static char		g_user[256]   = "m3cpt";
static char		g_pwd[256];				// La pwd de acceso a DB
static char		* g_pwd_enc = NULL;		// La pwd encriptada
static char		g_reg_file[256];		// El archivo .reg a generar

int main(int argc, char* argv[])
{

#	define ERR_ARGS				1	// Argumento no reconocido
#	define ERR_NO_PWD			2	// No se ha especificado ninguna contraseña
#	define ERR_REG_FILE			3	// No es posible crear el archivo .reg
#	define ERR_ENCRIPT_PWD		4	// El algoritmo de encriptación ha fallado

	m4uint32_t	iResult = M4_SUCCESS;
	m4int_t		iLength = 0;


    int opt_char;
    while ((opt_char = getopt(argc, argv, "d:s:u:p:o:h")) != EOF) {
        switch (opt_char) {
        case 'd':
			if (optarg)
				strcpy(g_dsn, optarg);
            break;
		case 's':
            if (optarg)
				strcpy(g_server, optarg);
            break;
        case 'u':
			if (optarg)
				strcpy(g_user, optarg);
            break;
        case 'p':
			if (optarg)
				strcpy(g_pwd, optarg);
            break;
        case 'o':
			if (optarg)
				strcpy(g_reg_file, optarg);
            break;
        case '?':
		case 'h':
        default:
            usage(argv[0]);
            exit(ERR_ARGS);
        };
    }

	// Debe especificar una password
	if (strlen(g_pwd) == 0) {
		fprintf(stderr, "You must specify a password\n");
        exit(ERR_NO_PWD);
	}

	FILE* p_reg = stdout;
	if (strlen(g_reg_file) > 0) {
		// UNICODE FILE revised
		p_reg = fopen(g_reg_file, "w");
		if (p_reg == NULL) {
			perror("Could not open '%s'\n");
			exit(ERR_REG_FILE);
		}
	}
	
	iResult = EncryptTwoWay(g_pwd, strlen(g_pwd), M4CH_DUMMY, strlen(M4CH_DUMMY), g_pwd_enc, iLength);
	if (iResult != M4_SUCCESS)
	{
		fprintf(stderr, "Error en el algoritmo de encriptación. \n");
		exit(ERR_ENCRIPT_PWD);
	}

	fprintf(stdout, "Texto de entrada: %s\n", g_pwd);
	fprintf(stdout, "Texto de salida: %s\n", g_pwd_enc);

	// Generar la información para el registro
	fprintf(p_reg, "REGEDIT4\n\n");
	fprintf(p_reg, "[HKEY_LOCAL_MACHINE\\%s%s%s]\n", M4_NEW_REGEDIT_KEY, M4_NEW_REGEDIT_KEY_SERVER, M4_NEW_REGEDIT_KEY_CVM_LDB);
	fprintf(p_reg, "\"ConnectionString\"=\"DSN=%s;SRVR=%s;UID=%s;PWD=%%s\"\n", g_dsn, g_server, g_user);
	fprintf(p_reg, "\"DBPassword\"=\"%s\"\n", g_pwd_enc);

	delete g_pwd_enc;

	return 0;
}
