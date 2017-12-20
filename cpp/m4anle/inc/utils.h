// Rutinas de apoyo utiles

// Elimina de una cadena los espacios en blanco
#ifdef __cplusplus
extern "C"{
#endif
char * CleanSp(char *des, LPCSTR src);
char * CleanChr(char *str, char chr);
char * ChgStr(char **des, LPCSTR str, LPCSTR old, LPCSTR chg);
char * FindChr(char *str, char c);
char * FindChr2(char *str, char c);
char * FindChr3(char *str, char c);
char * FindStr(char *str, LPCSTR sub);
char * FindStr2(char *str, LPCSTR sub);
char * FindStr3(char *str, LPCSTR sub);
char *FindStrPos(LPCSTR str, LPCSTR substr, int n);
int StrCount(LPCSTR str, LPCSTR substr);
char *GetStringFromTo(char **des, LPSTR org, LPCSTR substr1, int int1, LPCSTR substr2, int int2);
char *GetListStringFromTo(char **des, LPSTR org, LPCSTR substr1, int int1, LPCSTR substr2, int int2);
char *FindLast(LPCSTR str, LPCSTR substr);
void Trace(LPCSTR mensaje, LPCSTR dump);
#ifdef __cplusplus
}
#endif
