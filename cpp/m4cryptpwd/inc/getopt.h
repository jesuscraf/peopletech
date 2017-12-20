#ifdef __cplusplus
extern "C" {
#endif

extern char *optarg;    /* pointer to the start of the option argument  */ 
extern int   optind;       /* number of the next argv[] to be evaluated    */ 
extern int   opterr;       /* non-zero if a question mark should be returned */

int getopt(int argc, char *argv[], char *opstring);

#ifdef __cplusplus
}
#endif


