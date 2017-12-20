#==============================================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) FileDescription: defines the base class for any script used in the m4builder environment.
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
#==============================================================================================

require 5.004;			# perl version required.

#============================================================================
# module definition.
#============================================================================
package	m4ScriptBase;

#============================================================================
# dependencies.
#============================================================================
use lib '../modules';
use strict;
use m4trace;
use English;
use File::Basename;


#============================================================================
# global variables for this script.
#============================================================================
my $g_oTrace = m4trace::GetTraceObject();
my $g_errorBase = m4trace::GetErrorBase(1);


#============================================================================
# main.
#============================================================================
sub Main
{
	# input arguments.
	my $class = shift;
	my @_argv = @_;
	my $_argc = @_argv;
		
	$g_oTrace->Print("$class main...", $g_errorBase+1);
	my $errorCode = 0;
	
	# create a object for the input class.
	my $object = new $class;

	# check input arguments.
	$g_oTrace->Print("ARGV=@_argv, ARGC=$_argc", $g_errorBase+2);
	if (! $_argc)
	{
		m4ScriptBase::Usage($object);
		return 1;
	}
	
	# parse input arguments.
	$errorCode = $object->ModuleOptions();
	if ($errorCode)
	{
		$g_oTrace->Error("Parsing input arguments in class $class");
		return $errorCode;
	}

	# exec.
	$errorCode = $object->Exec();

	return $errorCode;
}



#============================================================================
# Check if this script was called from command-line or with the require 
# clausule from another script.
#============================================================================
sub AutoExecute()
{
	# input parameters.
	my $class = shift;
	my $filename = shift;
	
	# extract the basenames.
	my $programName = basename($PROGRAM_NAME);
	my $filename = basename($filename);
	$g_oTrace->Print("AutoExecute: class=$class, filename=$filename, programName=$programName", $g_errorBase+1);
	
	my $errorCode = 1;	# default value is 1 to return true when the module is loaded.
	$g_oTrace->Print("filename=$filename, programName=$programName", $g_errorBase+2);
	
	if ($programName eq $filename)
	{
		$errorCode = m4ScriptBase::Main($class, @ARGV);
	}
	return $errorCode;
}



#============================================================================
# function: Usage.
# description: print a header for the Usage method.
#============================================================================
sub Usage
{
	# read input arguments.
	my $object = shift;

	$g_oTrace->Print("_Usage", $g_errorBase+1);

	print STDERR "$PROGRAM_NAME usage:\n";
	print STDERR "   $PROGRAM_NAME [options]\n";
	print STDERR "\n";

	$object->Usage();
}
