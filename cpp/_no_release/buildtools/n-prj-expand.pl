#! /usr/bin/perl
#################################
# Este programa lee un fichero *.prj pasado como unico argumento y lo vuelca a la salida estandar,
# sustituyendo las lineas de tipo
# READFILE = file 
# por el contenido del fichero 'file'.
# Si dicho fichero no puede ser leido, este programa aborta.
#################################

$_f_ = 'f';			 #file handle
@_track = ();        #variable usada para mantener la cadena de READFILE anidadas y poder informar si se produce un error.

use Cwd;
use Cwd 'abs_path'; 


&PrintFile( $ARGV[0] );

# Imprime un fichero recursivamente
sub PrintFile
{
	my( $filename ) = @_;
	my( $handle,  $line );
	
	push( @_track, $filename );
	$handle = &NewHandle;
    open( $handle,"<$filename" ) or &Error( "file $filename not found" );
    
	while( $line = <$handle> )
	{
		if( $line =~ /^\s*READFILE\s*=\s*(.*)$/ )
		{
			$filename = $1;
			next if ! $filename;
			&PrintFile( $filename );
		}
		else
		{
			print $line;
		}
	}
	close $handle;
	pop( @_track );
}

# Devuelve un manejador de fichero unico cada vez que se la llama
sub NewHandle
{
	$_f_ .= 'x';
	return $_f_;
}

# Imprime un error y aborta la ejecucion
sub Error
{
	my( $m ) = @_;
	print STDERR "Error: $m\n";
	exit( -1 );
}

