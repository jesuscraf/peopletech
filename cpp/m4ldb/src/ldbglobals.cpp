
#include "m4stl.hpp"
#include "ldbglobals.hpp"
#include "m4objglb.hpp"
#include "m4unicode.hpp"
#include "m4cryptc.hpp"
#include <sys/stat.h>



#define	M4CH_DUMMY_B1	"\xC\xD\x5\x9\xA\xA\xF\x1\x6\x7\xA\xD\x7\xE\xA\xB\x9\xB\x4\x4\x5\x7\xF\xD\xA\x6\xC\xA\x2\xB\xB\xD\xE\xA\x5\x1\x9\x9\x1\x5\xC\xB\x9\xA\xA\x8\x8\xB\xC\x4\x6\x4\x4\xC\xE\xE\x4\xD\x4\x5\xC\xD\x2\x0\xD\x5\x8\xD\xB\xA\x3\x3\x7\x9\x1\xE\x4\x5\x2\xF\xC\x2\xD\xA\x7\xD\xB\x8\x5\xA\x2\x7\xA\xF\x1\xF\xD\xA\x6\x6\x4\xD\xE\x8\x0\xA\x6\x1\x8\x7\xD\xE\xC\x3\x6\xE\x7\xD\x9\x6\x8\x9\xF\xE\x8\x6\x1\x6\x3"
#define	M4CH_DUMMY_K	"\x8\xA\x3\x7\x2\x8\x9\x2\x2\xF\xC\x5\x2\x6\x4\xA\x2\x8\xD\xE\xA\x3\x1\xC\x4\x1\xC\x6\x5\x8\x4\x9\xB\xA\xA\xA\xE\xF\x1\x2\x5\x2\x2\x8\x9\x4\x6\x9\xC\x3\x5\x1\x1\x9\x3\x5\x6\x7\x2\xA\x3\x8\xC\x0\x3\x4\x6\x7\x8\xA\x3\x1\x6\x8\x7\x1\x2\x7\xF\xE\x2\x1\x4\x1\x6\x4\x8\x7"

static	m4char_t	s_acDummy[] = "\xA\x6\xA\xE\x1\x5\xA\xB\x9\x2\xD\x2\x8\x1\xF\x9\x3\x6\x3\xC\x1\xA\xE\x2\xD\x1\xE\x3\x1\xE\x2\xC\x6\xD\x4\x6\x2\xA\xE\x9\x6\x7\xD\x5\x8\x3\x5\x1\x2\xC\x9\xB\x4\xB\x4\xC\x3\xA\x8\x7\x4\x6\x3\x0\xD\x5\x8\xD\xB\xA\x3\x3\x7\x9\x1\xE\x4\x5\x2\xF\xC\x2\xD\xA\x7\xD\xB\x8\x5\xA\x2\x7\xA\xF\x1\xF\xD\xA\x6\x6\x4\xD\xE\x8\x0\x8\xA\x6\xB\xB\xB\xE\xD\x1\x9\x7\x2\x2\xF\x1\x9\x2\xA\x9\x2\x2\x6\xB\x6" ;



///////////////////////////////
//Set de la Password del Usuario. Se obtiene de base de datos.
m4return_t M4SetUserKey(m4pcchar_t ai_pUserKey)
{
	m4int_t		iLength = 0 ;
	m4int_t		iDummyLen = 0;
	m4pchar_t	pcUserKeyDecrypted = NULL;
	
	if( ai_pUserKey == NULL || *ai_pUserKey == '\0' )
	{
		return M4_SUCCESS;
	}

	if( DecryptTwoWay( ai_pUserKey, strlen( ai_pUserKey ) * sizeof( m4char_t ), M4CH_DUMMY_K + 23, strlen( M4CH_DUMMY_K + 23 ) * sizeof( m4char_t ), pcUserKeyDecrypted, iLength ) != M4_SUCCESS )
	{
		return M4_ERROR;
	}

	iDummyLen = sizeof(s_acDummy) - 23;
	if (iDummyLen < 2 * ( iLength + 1))
	{
		return M4_ERROR;
	}
	
	memcpy(s_acDummy + 23, pcUserKeyDecrypted, iLength + 1);
	memcpy(s_acDummy + 23 + iLength + 1, pcUserKeyDecrypted, iLength + 1);
	delete pcUserKeyDecrypted;

	return M4_SUCCESS;
}


m4return_t	M4DeConvert( m4pcchar_t ai_pccInString, m4pchar_t & ao_pcOutString )
{

	m4return_t	iResult ;
	m4int_t		iLength ;

	iResult = DecryptTwoWaySecondKey( ai_pccInString, strlen( ai_pccInString ) * sizeof( m4char_t ), M4CH_DUMMY_B1 + 23, strlen( M4CH_DUMMY_B1 + 23 ) * sizeof( m4char_t ), ao_pcOutString, iLength ) ;

	return( iResult ) ;
}


m4return_t	M4DataConvert( m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t & ao_pcOutString, m4int_t &ao_riCopied )
{

	m4return_t	iResult ;

	iResult = EncryptTwoWaySecondKey( ai_pccInString, ai_iInLength, s_acDummy + 23, strlen( s_acDummy + 23 ) * sizeof( m4char_t ), ao_pcOutString, ao_riCopied ) ;

	return( iResult ) ;
}

m4return_t	M4DataDeConvert( m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t & ao_pcOutString, m4int_t &ao_riCopied )
{

	m4return_t	iResult ;

	iResult = DecryptTwoWaySecondKey( ai_pccInString, ai_iInLength, s_acDummy + 23, strlen( s_acDummy + 23 ) * sizeof( m4char_t ), ao_pcOutString, ao_riCopied ) ;

	return( iResult ) ;
}


m4return_t _M4CryptFile(m4pchar_t ai_pFileIn,m4pchar_t ai_pFileOut,m4bool_t ai_bCrypt)
{
	m4int32_t fileinlen = 0,fileoutlen=0;
	m4int_t retsize=0;
	m4pchar_t bufferin=0,bufferout=0;
	m4return_t ret = M4_SUCCESS;

	// UNICODE FILE
	int iLength = -1 ;
	char* pcFileName = M4CppToANSI( ai_pFileIn, iLength ) ;

	struct stat stFile;
	if (stat (pcFileName, &stFile) == 0) {
		fileinlen  = stFile.st_size;
	}

	delete [] pcFileName ;

    ifstream ifs;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	M4Open( ifs, ai_pFileIn, M4UniReadBinary, eEncoding );

	if ((ifs.rdbuf()->is_open()==0)||(fileinlen==0)){
		return M4_ERROR;
	}

	// Buffer que contenga el fichero original.
	bufferin = new m4char_t [fileinlen];
	if (bufferin==0){
		// Compruebo por si acaso,que esto puede ser muy grande
		return M4_ERROR;
	}
	
	ifs.read(bufferin,fileinlen);
	ifs.close();

	if (ai_bCrypt){
		ret = M4DataConvert(bufferin,fileinlen,bufferout,retsize);
	}
	else {
		ret = M4DataDeConvert(bufferin,fileinlen,bufferout,retsize);
	}

	delete [] bufferin;

	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}

	ofstream ofs;

	// UNICODE FILE
	eEncoding = M4UniANSI ;
	M4Open( ofs, ai_pFileOut, M4UniWriteBinary, eEncoding );

	if (ofs.rdbuf()->is_open()==0){
		delete [] bufferout;
		return M4_ERROR;
	}

	ofs.write(bufferout,retsize);

	ofs.close();

	delete [] bufferout;

	return M4_SUCCESS;

}

m4return_t	M4ConvertFile	   (m4pchar_t ai_pFileIn, m4pchar_t ai_pFileOut)
{
	return _M4CryptFile(ai_pFileIn,ai_pFileOut,M4_TRUE);
}
m4return_t	M4DeConvertFile	   (m4pchar_t ai_pFileIn, m4pchar_t ai_pFileOut)
{
	return _M4CryptFile(ai_pFileIn,ai_pFileOut,M4_FALSE);
}


m4pvoid_t ConvertValue (m4VariantType *aio_pValue, m4int32_t ai_m4Type)
{
	m4int8_t cppType = aio_pValue->GetType ();

	if (cppType == M4CL_CPP_TYPE_NULL) {
		return (NULL);
	}

	switch (ai_m4Type) {

		default:
			if ((cppType != M4CL_CPP_TYPE_STRING) && 
				(cppType != M4CL_CPP_TYPE_STRING_VAR) &&
				(cppType != M4CL_CPP_TYPE_STRING_POOL)){
				ClVariantConversor oConv;
				oConv.ToCharString (*aio_pValue);
			}
			//Ojo con el pValue, porque si se produce un ResizePool
			//el puntero obtenido aqui es inválido. Seria bueno que Pipo nos
			//pasase Variants de Tipo OWNER
			return (aio_pValue->Data.PointerChar);
			break;

		case M4CL_M4_TYPE_DATE:
		case M4CL_M4_TYPE_TIMESTAMP:
			if (cppType != M4CL_CPP_TYPE_DATE) {
				ClVariantConversor oConv;
				oConv.ToDate (*aio_pValue);
			}
			return ((m4pvoid_t) &aio_pValue->Data.DoubleData);
			break;

		case M4CL_M4_TYPE_NUMBER:
			if (cppType != M4CL_CPP_TYPE_NUMBER) {
				ClVariantConversor oConv;
				oConv.ToDouble (*aio_pValue);
			}
			return ((m4pvoid_t) &aio_pValue->Data.DoubleData);
			break;
	}

	return (NULL);
}




