#! /usr/bin/perl 
#################################
# Entrada:	Fichero *.prj por STDIN
# Salida:	Devuelve en STDOUT la cadena '<path>/makefile' si se ejecuto con
#           exito. El valor de <path> es relativo a la posicion del *.prj
# Efecto:	Crea el fichero 'makefile' correspondiente al *.prj
#################################

use English;
use File::Basename;
use Cwd;
use Cwd 'abs_path'; 

$g_n_script_root_dir = abs_path(dirname($PROGRAM_NAME));


#------------------------
# main
&Setup;

my( %PRJDATA ) = &ReadProjectFile;
my( %MAKEDATA ) = &ProcessForPlatform( %PRJDATA );
&WriteMakefile( %MAKEDATA );


sub Setup
{
	$| = 1;

    # find out the value of the variable $_platform
	$_platform = `$g_n_script_root_dir/n-platform.sh`;
	chomp( $_platform );
	$_pwd = cwd();
	chomp( $_pwd );
		
	# set options.
	@_opts = @ARGV;
	$_nprjname = shift(@_opts);
	$_nprjname =~ s/(.+)\..*/$1/;
}


sub ReadProjectFile
{
    my( %D ); # data store
    my( $line ) = 0;
	my( $aux );
	my( $ev_string ) = "";
	$_no_optimize_list = "" ;
	
	# A continuacion se ponen las opciones -D como variables
	# para poder ser consultadas con un eval.
	foreach ( @_opts )  {
		if ( /^-D(.*)$/ ) {
			my( $macro,$def ) = split( /=/,$1 );
			&Error(0,"Error de sintaxis en macro <$_>" ) if $macro !~ /^[A-Z0-9_]+$/;
			$def = 1 if ( $def eq '' );
			$ev_string .= " \$$macro=$def;"; 
		}
	}
	eval $ev_string;
	&Error(0,"Mala sintaxis de macros (opcion -D) ") if $@ ne '';

	# Ahora procesamos el fichero *.prj linea a linea
	while( <STDIN> ) {  
		$line++;
		next if /^\s*#/; #Skip comments
		next if /^\s*$/; #Skip blank lines

		# Evaluacion condicional [] segun maquina para los ficheros prj
		if ( s/^\s*\[(.*)\]// ) {
			next if ($_platform !~ /$1/i); #Skip if not for this pltf
		}
	
	    # Evaluacion condicional {} para las lineas de los ficheros prj
		if ( s/\{(.*)\}// ) {
			my( $cond ) = "$1";
			&Error( $line,"No se admite '=' en expr. entre {}, use '=='" ) if $cond =~ /[^=]=[^=]/;
			my( $aux ) = eval $cond;
			&Error( $line,"Error de sintaxis en expr. entre {}" ) if $@ ne '';
			next if ! $aux ; # salta linea si no se cumple la cond. entre {}
		}

		chomp;

		# expand variables $PLATFORM y $COMPILER
		s/\$PLATFORM/$_platform/g; 
		s/\$COMPILER/$D{'C_COMPILER'}/g; 

		# expands extra variables
		s/\$ARCHTYPE/$_archtype/g;
		s/\$THREADTYPE/$_threadtype/g; 
		s/\$LINKTYPE/$_linktype/g; 
		s/\$NPRJNAME/$_nprjname/g; 
		s/\$DB_VERSION/$_dbversion/g; 
		s/\$ENCODING/$_encoding/g; 
		s/\$APPTARGET/$_apptarget/g; 

		if      ( /^\s*EXE\s*=\s*([A-Za-z0-9_.\-\*]+)/ ) {
			$D{'EXE'} = $1; 
		} elsif ( /^\s*COMPILER\s*=\s*([A-Za-z0-9_.\-+]+)/ ) {
			( $D{'COMPILER'},$D{'C_COMPILER'} ) = &NormalizeCompiler( $1 );
			my( $d ) = $D{'C_COMPILER'};
			$d =~ tr/a-z/A-Z/;
			$D{'CCFLAGS'} .= " -D_$d";
		} elsif ( /^\s*INC\s*=\s*([A-Za-z0-9_.\-\/]+)/ ) {
			$aux = $1;
			$D{'INCDIRS'} .= " -I$aux";
		} elsif ( /^\s*SRC\s*=\s*([A-Za-z0-9_.\-\/]+)/ ) {
			$aux = $1;
			$D{'SRCS'} .= " $aux";
		} elsif ( /^\s*DEP\s*=\s*([A-Za-z0-9_.\-\/]+)/ ) {
		} elsif ( /^\s*LIBS?\s*=\s*([A-Za-z0-9_.\-\/\+]+)/ ) {
			$D{'LIBS'} .= " -l$1";
		} elsif ( /^\s*LIBDIR\s*=\s*([A-Za-z0-9_.\$\-\/]+)/ ) {
			$aux = $1;
			$D{'LIBDIRS'} .= " -L$aux";
		} elsif ( /^\s*LINKTYPE?\s*=\s*([A-Za-z0-9_]+)\s*/ ) {
			$_linktype = "$1" ; 
		} elsif ( /^\s*THREADTYPE?\s*=\s*/ ) {
			$_threadtype = "$1" ; 
		} elsif ( /^\s*ARCHTYPE?\s*=\s*([A-Za-z0-9_]+)/ ) {
			$_archtype = "$1" ; 
		} elsif ( /^\s*DB_VERSION?\s*=\s*([A-Za-z0-9_]+)/ ) {
			$_dbversion = "$1" ; 
		} elsif ( /^\s*ENCODING?\s*=\s*([A-Za-z0-9_]+)/ ) {
			$_encoding = "$1" ; 
		} elsif ( /^\s*APPTARGET?\s*=\s*([A-Za-z0-9_]+)/ ) {
			$_apptarget = "$1" ; 			
		} elsif ( /^\s*NO_OPTIMIZE_FILE\s*=\s*(.+)\s*/ ) {
			$_no_optimize_list =  "$_no_optimize_list $1" ; 
		} elsif ( /^\s*THREADS?\s*=\s*([A-Za-z]+)/ ) {
			$D{'OPT'} .= " <THREADS>" if ($1 =~ /yes/i); 
		} elsif ( /^\s*SOCKETS?\s*=\s*([A-Za-z]+)/ ) {
			$D{'OPT'} .= " <SOCKETS>" if ($1 =~ /yes/i); 
		} elsif ( /^\s*MATHS?\s*=\s*([A-Za-z]+)/ ) {
			$D{'OPT'} .= " <MATH>" if ($1 =~ /yes/i); 
		} elsif ( /^\s*PURIFY\s*=\s*([A-Za-z]+)/ ) {
			$D{'PURIFY'} = "purify " if ($1 =~ /yes/i);
		} elsif ( /^\s*PURECOV\s*=\s*([A-Za-z]+)/ ) {
			$D{'PURECOV'} = "purecov " if ($1 =~ /yes/i);
		} elsif ( /^\s*QUANTIFY\s*=\s*([A-Za-z]+)/ ) {
			$D{'QUANTIFY'} = "quantify " if ($1 =~ /yes/i);
		} elsif ( /^\s*DEBUG\s*=\s*([A-Za-z]+)/ ) {
			$D{'OPT'} .= " <DEBUG>" if ($1 =~ /yes/i);
		} elsif ( /^\s*ODBC\s*=\s*([A-Za-z]+)/ ) {
			$D{'OPT'} .= " <ODBC>" if ($1 =~ /yes/i);
		} elsif ( /^\s*OSPACE\s*=\s*([A-Za-z]+)/ ) {
			$D{'OPT'} .= " <OSPACE>" if ($1 =~ /yes/i);
		} elsif ( /^\s*VERSION_FILE\s*=\s*([A-Za-z0-9.\-\/_]+)/ ) {
			$VERSION_FILE = $1;
		} elsif ( /^\s*STATIC\s*=\s*([A-Za-z]+)/ ) {
			$D{'OPT'} .= " <STATIC>" if ($1 =~ /yes/i);
		}
		elsif ( /^\s*OBJDIR\s*=\s*([A-Za-z0-9.\-\/_]+)/ ) {
			$D{'OBJDIR'} = $1;
		}
		elsif ( /^\s*BINDIR\s*=\s*([A-Za-z0-9.\-\/_]+)/ ) {
			$aux = $1;
            # Crea el directorio para binarios si no existe
            system "mkdir -m 775 -p $aux" if ! &Exist($aux);
			$D{'BINDIR'} = $aux;
		}
		elsif ( /^\s*CCFLAGS_OPT\s*=\s*(.*)$/ ) {
			$D{'CCFLAGS_OPT'} .= " $1";
			}
		elsif ( /^\s*CCFLAGS_NOOPT\s*=\s*(.*)$/ ) {
			$D{'CCFLAGS_NOOPT'} .= " $1";
			}
		elsif ( /^\s*CCFLAGS_EH\s*=\s*(.*)$/ ) {
			$D{'CCFLAGS_EH'} .= " $1";
			}
		elsif ( /^\s*CCFLAGS_NOEH\s*=\s*(.*)$/ ) {
			$D{'CCFLAGS_NOEH'} .= " $1";
			}
		elsif ( /^\s*CCFLAGS?\s*=\s*(.*)$/ ) {
			$D{'CCFLAGS'} .= " $1";

			if ( /^\s*.*\s*=\s*-ptr(.*)\s*.*$/ ) {
			    $D{'CC_TEMPLATES_DIR'} = $1;
			}
		} elsif ( /^\s*LINKFLAGS?\s*=\s*(.*)$/ ) {
			$D{'LINKFLAGS'} .= " $1";

			if ( /^\s*.*\s*=\s*-ptr(.*)\s*.*$/ ) {
			    $D{'LD_TEMPLATES_DIR'} = $1;
			}
		} elsif ( /^\s*PURIFY_OPTIONS?\s*=\s*(.*)$/ ) {
			$D{'PURIFY_OPTIONS'} .= " $1";
		} elsif ( /^\s*EH\s*=\s*(.*)$/ ) {
			$_eh = "$1";
		} else {
			&Error($line,"palabra clave desconocida: $_");   
		}
	}

    # Comprueba si los paths para el directorio Templates.DB puesto
    # en las variables CCFLAGS y LINKFLAGS son el mismo

    if ($D{CC_TEMPLATES_DIR} eq $D{LD_TEMPLATES_DIR}) {
    $D{TEMPLATES_DIR} = $D{CC_TEMPLATES_DIR};
    }
    else {
	&Error($line,"Path pasado con parametro ptr en CCFLAGS difiere del path en LINKFLAGS");   
    }

	# Poner OBJDIR = .$PLATFORM si no se le ha dado ningun valor en *.prj
	$D{'OBJDIR'} = "$_platform" if ! $D{'OBJDIR'};
	# Poner BINDIR = . si no se le ha dado ningun valor en *.prj
	$D{'BINDIR'} = "." if ! $D{'BINDIR'};
	return %D;
	&Error($line,"compilador no definido, no se encuentra linea COMPILER = ");
}


# Devuelve un par (compilador_de_C++,compilador_de_c) a partir del nombre generico del compilador (gcc,kcc,cc) 
# Los nombres devueltos no son los compiladores reales, (en ibm seria xlC), sino una forma normal intermedia.
sub NormalizeCompiler
{
	my( $cc ) = @_;
	if ($cc eq 'gcc' or $cc eq 'g++') {
		return ('g++','gcc');
	} elsif ( $cc =~ /^kcc$/i ) {
		return ('KCC','KCC');
    } elsif ( $cc eq 'CC' or $cc eq 'cc' ) {
		return ('CC','cc');	
	} else {
		return ($cc,$cc);	
	}
}


sub ProcessForPlatform
{
	# Aqui se efectua un primer procesamiento, comun a todas las
        # plataformas y se ejecutan las funciones dependientes de
        # plataforma-compilador.
	my( %D ) = @_; #data
	my( $complatf ) = "$D{'COMPILER'}-$_platform";
	$D{'COMPLATF'} = $complatf;

	# Common processing is made here
	if ( $D{'EXE'} =~ /(.*)\.\*/ ) 
	{ 
		my( $aux ) = $1;
		$D{'OPT'} .= " <SHLIB>";
		if ($_platform =~ /hp-ux/) 
		{
			$D{'BIN'} = "$aux.sl";
	    } 
		elsif ($_platform =~ /aix/) 
		{
			$D{'BIN'} = "$aux.a";
	    } 
		else 
		{
			$D{'BIN'} = "$aux.so";
		}
		$D{'BIN'} =~ s/\.s[lo]/.a/ if $D{'OPT'} =~ /<STATIC>/;
    } 
	else 
	{
		$D{'BIN'} = $D{'EXE'};
	}

	$D{'SRCS'} =~ s/^\s+//g;
	$D{'OBJS'} = $D{'SRCS'};

	# Arregla el problema de falta de directorio de objetos en el makefile
	# Este problema es del makefile de HP-UX 10.20 y aqui lo que se hace
	# es poner "./../" antes del directorio de los objetos (esto es una
	# solucion esoterica, no hay razon alguna para que funcione). Por
	# este error se abrio la consulta 30688 en HP, con la que se reporto
	# este bug a HP en USA, ya que no se tenia constancia de este bug
	# hasta el momento (esto sucedio antes del verano del 98). Quedaron
	# en que sacarian un parche que arreglara esto (hay que preguntar
	# por ese parche, por si lo han sacado ya).
	$D{'OBJS'} =~ s/^/\.\//g;
	$D{'OBJS'} =~ s/\s/ \.\//g;

	# Para arreglar el problema descrito en el comentario anterior
	# pero cuando se introduce un directorio para los objetos, distinto
	# del directorio por defecto (o sea "../<plataforma>..."

	$D{'OBJS'} =~ s/\.[cC]\b/.o/g;
	$D{'OBJS'} =~ s/\.cpp\b/.o/g;
	$D{'OBJS'} =~ s/[\S]+\//$D{'OBJDIR'}\//g;
	$D{'LINKER'} = $D{'COMPILER'};
	# La siguiente opcion determina el valor de la opcion para indicar
	# el nombre del fichero ejecutable en la linea de enlace del makefile.
	# Normalmente es '-o', excepto cuando el LINKER es 'ar', que debe ser
	# vacia.
	$D{'-O-FLAG'} = '-o';
	# if static compilation, replace SHLIB with STATLIB
	$D{'OPT'} =~ s/<SHLIB>/<STATLIB>/g  if $D{'OPT'} =~ /<STATIC>/;

	# Special processing is made here
	%D = &ProcessForAix( %D ) if ($_platform =~ /^aix/);
	%D = &ProcessForSun( %D ) if ($_platform =~ /^solaris/);
	return %D;
}


sub ProcessForAix
{
	my( %D ) = @_;

	# compile options
	$D{'CCFLAGS'} .= $D{'OPT'};
	$D{'CCFLAGS'} =~ s/<SHLIB>//g;
	$D{'CCFLAGS'} =~ s/<DEBUG>/-g/g; 
	$D{'CCFLAGS'} =~ s/<\w*>//g; # strip non-processed <> options

	# link options
	$D{'LINKFLAGS'} .= $D{'OPT'};
	$D{'LINKFLAGS'} =~ s/<SHLIB>/-G/g;
	$D{'LINKFLAGS'} =~ s/<ODBC>/-lodbc/g;
	$D{'LINKFLAGS'} =~ s/<DEBUG>/-g/g if $D{'OPT'} !~ /<STATIC>/; 
	$D{'LINKFLAGS'} =~ s/<\w*>//g; # strip non-processed <> options

	if ( $D{'OPT'} =~ /<STATLIB>/ ) {
		$D{'LINKER'} = 'ar';
		$D{'-O-FLAG'} = '';
		$D{'LINKFLAGS'} = ' -qv';
		$D{'LIBDIRS'} = '';
		$D{'LIBS'} = '';
	}

    return %D;
}


sub ProcessForSun
{
	my( %D ) = @_;

	# ObjectSpace includes
	if ( $D{'OPT'} =~ /<OSPACE>/ ) {
	    $D{'CCFLAGS'} =~ s/<OSPACE>/ /g;
            # "-Didet" is for not appear the sun information when is
	    # executed the command what with the executable file
	    $D{'CCFLAGS'} .= " ";
	    $D{'INCDIRS'} .= " ";
	}

	# compile options
	$D{'CCFLAGS'} .= $D{'OPT'};
	$D{'CCFLAGS'} .= " -D_REENTRANT";
	$D{'CCFLAGS'} =~ s/<SHLIB>/-KPIC/g;
	$D{'CCFLAGS'} =~ s/<DEBUG>/-g/g; 
	$D{'CCFLAGS'} =~ s/<THREADS>/-mt/g;

	$D{'CCFLAGS'} =~ s/<\w*>//g; # strip non-processed <> options

	# link options
	$D{'LINKFLAGS'} .= $D{'OPT'};
	$D{'LINKFLAGS'} =~ s/<SHLIB>/-G/g;
	$D{'LINKFLAGS'} =~ s/<THREADS>/-mt/g;
	$D{'LINKFLAGS'} =~ s/<SOCKETS>/-lsocket -lnsl/g;
	$D{'LINKFLAGS'} =~ s/<ODBC>/-lodbc/g;

# Comentado porque el linkador de Sun-Sparc se queda bloqueado cuando se le
# pasa esta opcion (que no es de verdad una opcion del linkador, segun la 
# pagina del manual on-line
#	$D{'LINKFLAGS'} =~ s/<DEBUG>/-g/g; 


	$D{'LINKFLAGS'} =~ s/<MATH>/-lm/g;

	# ObjectSpace Library
	if ( $D{'OPT'} =~ /<OSPACE>/ ) {
	    $D{'LINKFLAGS'} =~ s/<OSPACE>/ /g;
	    $D{'LIBDIRS'} .= " ";
	    $D{'LIBS'} .= " -ldynospace";
	}
	$D{'LINKFLAGS'} =~ s/<\w*>//g; # strip non-processed <> options

	if ( $D{'OPT'} =~ /<STATLIB>/ ) {
		$D{'LINKER'} = 'CC';
		$D{'-O-FLAG'} = '-o';
		$D{'LINKFLAGS'} .= ' -xar';
	}

	# makedepend options
	$D{'MKDFLAGS'} = '-I/opt/SUNWspro/SC4.2/include/CC';

    return %D;
}


# Imprime error $m en linea $l y aborta la ejecucion
sub Error
{
    my( $l,$m ) = @_;
	print STDERR "Error: (S): $0($l):$m\n";
	exit( -1 );
}


# Existe el fichero $file?
sub Exist
{
    my( $file ) = @_;
	return ( -d $file | -f $file );
}


sub WriteMakefile
{
	my( %P ) = @_; # data

	my( $mkd ) = $P{'OBJDIR'};
	my( $mkf ) = "$mkd/makefile";
	
	if ( ! -d $mkd ) 
	{
		if (! -d $mkd)
		{
			my $retCode = system ("mkdir -p $mkd");
			if ($retCode != 0)
			{
				return -1;
			}
		}
				
#---	      ( ! (system "mkdir -m 775 -p $mkd") ) or &Error(0,"No puedo crear dir <$mkd>");
#---	      ( ! (system "mkdir $mkd") ) or &Error(0,"No puedo crear dir <$mkd>");
	}

#	mkdir( $mkd,0755) or &Error(0,"No puedo crear dir <$mkd>") if ( ! -d $mkd );
	open( F,">$mkf" ) or &Error(0,"No puedo escribir <$mkf>");

	my( $objs ) = $P{'OBJS'};
	$objs =~ s/$P{'OBJDIR'}/\$(OBJDIR)/g;
	$objs =~ s/ / \\\n       /g;
	
	my( $incs ) = $P{'INCDIRS'};
	$incs =~ s/ / \\\n       /g;
		 

    # Write the file!!!
	print F <<EOF;
#############################################################################
# AVISO: Este makefile ha sido construido automaticamente a partir de un
#        fichero de proyecto, cualquier cambio que se
#		 haga en el se perdera al reconstruir el proyecto
#############################################################################
#OPT= ($P{'OPT'})
#COMPLATF = ($P{'COMPLATF'})

INCDIRS = $incs

CC = $P{'COMPILER'}
cc = $P{'C_COMPILER'}
CCFLAGS = $P{'CCFLAGS'}
CCFLAGS_OPT = $P{'CCFLAGS_OPT'}
CCFLAGS_NOOPT = $P{'CCFLAGS_NOOPT'}
CCFLAGS_EH = $P{'CCFLAGS_EH'}
CCFLAGS_NOEH = $P{'CCFLAGS_NOEH'}

LINKER = $P{'LINKER'}
LIBDIRS = $P{'LIBDIRS'} 
LIBS = $P{'LIBS'}
LINKFLAGS = $P{'LINKFLAGS'}

PURIFY = $P{'PURIFY'}
PURECOV = $P{'PURECOV'}
QUANTIFY = $P{'QUANTIFY'}

OBJDIR = $P{'OBJDIR'}
OBJS = $objs

BIN = $P{'BINDIR'}/$P{'BIN'}

######## COMMANDS ###########
build: \$(BIN)
EOF
    if ($_platform =~ /solaris/ && !($P{'BIN'} =~ /\.a\s*$/)){
	print F <<EOF;
	\@mcs -d \$(BIN)
EOF
}
	print F <<EOF;
	\@echo 
	\@echo SUCCESS: \$(BIN)
	\@echo 

clean: 
	-\@rm \$(OBJS) 2>/dev/null ; true
	-\@rm \$(BIN) 2>/dev/null ; true
	-\@rm \$(WHAT_OBJ) 2>/dev/null ; true
EOF
    if ($_platform =~ /aix/){
	print F <<EOF;
	-\@rm \$(OBJDIR)/*.rpo 2>/dev/null ; true
EOF
	}
    if ($_platform =~ /solaris/){
	print F <<EOF;
	-\@rm -r $P{'TEMPLATES_DIR'}/Templates.DB 2>/dev/null ; true
EOF
    }
	print F <<EOF;

	\@echo FICHEROS OBJETO Y EJECUTABLES BORRADOS ; echo

all: clean build

######### WHAT DEFINITIONS #######
EOF

    $BINARY = $P{'BIN'};
    $BINARY =~ s/\.*\///g;
		$RC_DIR = "../rc";
		
	if ( ! $VERSION_FILE ) {
		  $RC_FILE = $RC_DIR;
		  $RC_FILE .= "/";
	    $RC_FILE .= $BINARY;
	    $RC_FILE .= ".rc";
		$WIN_FILE_NAME = $BINARY;
		$WIN_FILE_NAME =~ s/\..*//;
	    } else {
	    $RC_FILE = $VERSION_FILE;
		$WIN_FILE_NAME = $VERSION_FILE;
		$WIN_FILE_NAME =~ s/.*\///;
		$WIN_FILE_NAME =~ s/\.rc//;
	    }


	print F <<EOF;
WHAT_SRC = $RC_DIR/$_nprjname.what.cpp
WHAT_OBJ = \$(OBJDIR)/$BINARY.what.o
MAKEWHAT = rccompiler.pl

######### TARGET #############
\$(BIN): \$(OBJS) \$(WHAT_OBJ)
	\$(QUANTIFY) \$(PURECOV) \$(PURIFY) \$(LINKER) \$(LINKFLAGS) \$(LIBDIRS)  $P{'-O-FLAG'} \$(BIN) \$(OBJS) \$(WHAT_OBJ) \$(LIBS)

######### OBJECT FILES #######
EOF
    my($f,$o,$cc);
	my($ccflagsopt, $ccflagseh);
    foreach $f ( split(/ /,$P{'SRCS'} ) ) {
		$o = $f;
		$cc = ( $o =~ /\.c$/ ) ? 'cc' : 'CC'; #choose cc or CC as compiler
	    $o =~ s/[\S]+\///g;  #keep basename only, stripping paths
		if ( $_linktype =~ /debug/ || $_no_optimize_list =~ /\b$o\b/  || $_no_optimize_list =~ /\bALL\b/ ) {
			$ccflagsopt= "\$(CCFLAGS_NOOPT)";
		} else {
			$ccflagsopt= "\$(CCFLAGS_OPT)";
	  	} ; 
		if ( $_eh =~ /yes/) {
			$ccflagseh= "\$(CCFLAGS_EH)";
		} else {
			$ccflagseh= "\$(CCFLAGS_NOEH)";
	  	} ; 
		$o =~ s/\.[cC]\b/.o/;#replace .c .C by .o
	    $o =~ s/\.cpp\b/.o/; #replace .cpp by .o
		print F <<EOF;
\$(OBJDIR)/$o: $f
	\$($cc) -c \$(INCDIRS) \$(CCFLAGS) $ccflagsopt $ccflagseh -o \$(OBJDIR)/$o $f
	\@echo -------------------- 
EOF
        }

		print F <<EOF;

\$(WHAT_OBJ): \$(WHAT_SRC)
	\$($cc) -c \$(INCDIRS) \$(CCFLAGS) -o \$(WHAT_OBJ) \$(WHAT_SRC)
	\@echo -------------------- 

EOF

	close F;
}


