#============================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) FileDescription: modules to parse input options.
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
#============================================================================

require 5.004;			# perl version required.

#============================================================================
# module definition.
#============================================================================
package	m4GetOpt;
$VERSION = 1.00;	# version for this module.

#============================================================================
# dependencies.
#============================================================================
use strict;
use m4trace;
use Getopt::Long;
use English;
use m4configuration;
use m4system;

#============================================================================
# global variables for this file.
#============================================================================
my $g_oTrace = m4trace::GetTraceObject();
my $g_errorBase = m4trace::GetErrorBase(2);
my $g_oConfiguration = m4configuration::GetConfigurationObject();
my $g_oSystem = m4system::GetSystemObject();

# this variable saves how many times passtrough function was called.
my $_PassTroughCounter = 0;

#============================================================================
# function: _Initialize
# description: initialize the object. This method is called only from 
#	the constructor.
# return:
#	0 on success.
#	1 on error.
#============================================================================
sub _Initialize()
{
	$g_oTrace->Print("_Initialize", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	# the executable file.
	#---$self->{ex} = "../conf/m4execute.ini";
	$self->{ex} = undef;
	$self->{exeIni} = undef;
	
	# fisrt check for exe file.
	my $error = &GetOptions(
        $self,
        "ex:s",
	);
	
	if ($error == 0)
	{
		$g_oTrace->Print("error in GetOptions()", $g_errorBase+2);
		return 1;
	}
	
	# check if option --ex has a value.
	my $value;
	if (defined($self->{ex}) && ($self->{ex} eq ""))
	{
		$self->{ex} = "../conf/m4execute.ini";
		$value = $self->{ex};
		$g_oTrace->Print("--ex defined with value = #$value#", $g_errorBase+2);
	}
	else
	{
		$value = $self->{ex};
		$g_oTrace->Print("--ex not defined with  value = #$value#", $g_errorBase+2);
	}


	if (defined($self->{ex}))
	{
		my $exefile = $self->{ex};
		$g_oTrace->Print("Loading INI executable file=$exefile", $g_errorBase+2);
		if (! -r $exefile)
		{
			$g_oTrace->Error("Can't open file: $exefile", $g_errorBase+2);
		}
		else
		{
			$self->{exeIni} = new IniConf(-file => $exefile);
			if (!defined($self->{exeIni}))
			{
				$g_oTrace->Error("Can't load file: $exefile", $g_errorBase+2);
				my $nerrors = @IniConf::errors;
				my $serrors = join (';', @IniConf::errors);
				$g_oTrace->Error("INI module errors ($nerrors): $serrors", $g_errorBase+2);
			}
		}
	}
	return 0;
}


#============================================================================
# constructor.
#============================================================================
sub new
{
	# input arguments.
	my $class = shift;
	
 	$g_oTrace->Print("new $class", $g_errorBase+1);
	
	# create object.
	my $self = {};
	bless $self, $class;
	
	# initialize.
	my $error = $self->_Initialize(@_);
	
    $error ? return undef : return $self;
}


#============================================================================
# function: PassThrough
# description: pass over undefined options.
# input parameters:
#	1 enable passthrough
#	0 disable
#============================================================================
sub PassThrough
{	
	# input arguments. 
	my $ai_enable = shift;
 	$g_oTrace->Print("PassThrough($ai_enable)", $g_errorBase+1);
		
	if ($ai_enable)
	{
		$_PassTroughCounter++;
		if ($_PassTroughCounter == 1)
		{
 			$g_oTrace->Print("config(pass_through)", $g_errorBase+2);
			Getopt::Long::config("pass_through");
		}
	}
	else
	{
		$_PassTroughCounter--;
		if ($_PassTroughCounter == 0)
		{
 			$g_oTrace->Print("config(no_pass_through)", $g_errorBase+2);
			Getopt::Long::config("no_pass_through");
		}
	}
}


#============================================================================
# Usage.
#============================================================================
sub Usage
{
	$g_oTrace->Print("Usage", $g_errorBase+1);

	print STDERR "Base options:\n";
	print STDERR "   --ex= \n";
}


#============================================================================
# function: GetOptionsFromIni.
# description: Read object options from the --ex INI file.
#============================================================================
sub GetOptionsFromIni
{
	$g_oTrace->Print("GetOptionsFromIni", $g_errorBase+1);

	# read input arguments.
	my $object = shift;
	my $iniSection = shift;
	my @properties = @_;

    # fisrt check for exe file.
	if (defined($object->{exeIni}))
	{
		my $exeFile = $object->{ex};
		my $exeIni = $object->{exeIni};
		my $property;
		my $value = undef;
		$g_oTrace->Print("Reading object options from $exeFile::$iniSection...", $g_errorBase+2);
		
		foreach $property (@properties)
		{
			$value = $exeIni->val($iniSection, $property);
			if (defined($value))
			{
				$g_oTrace->Print("Property $property = #$value#.", $g_errorBase+4);
				$object->{$property} = $value  if ($value ne "");
			}
			else
			{
				$g_oTrace->Print("Property $property not defined.", $g_errorBase+4);
			}
		}
		return 0;
	}
	
	return 1;
}


#============================================================================
#============================================================================
# module definition.
#============================================================================
package	m4ConfigFilesGetOpt;
@m4ConfigFilesGetOpt::ISA = qw(m4GetOpt);

#============================================================================
# dependencies.
#============================================================================
use Getopt::Long;

#============================================================================
# function: _Initialize
# description: initialize the object. This method is called only from 
#	the constructor.
# return:
#	0 on success.
#	1 on error.
#============================================================================
sub _Initialize()
{
	$g_oTrace->Print("_Initialize", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	# insert members for trace system..
	$self->{IniComponent} = undef;
	$self->{IniCompilation} = undef;
	
    # fisrt check for exe file.
	if (defined($self->{exeIni}))
	{
		my $exeFile = $self->{ex};
		my $exeIni = $self->{exeIni};
		my $value = undef;
		$g_oTrace->Print("Reading configfiles options from $exeFile...", $g_errorBase+2);
		
		$value = $exeIni->val("ConfigFiles", "IniComponent");
		$self->{IniComponent} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("ConfigFiles", "IniCompilation");
		$self->{IniCompilation} = $value if (defined($value) && ($value ne ""));
	}
	
	# now read other command line options.
	my $error = &GetOptions(
        $self,
        "IniComponent=s",
        "IniCompilation=s",
	);
	
	if ($error == 0)
	{
		$g_oTrace->Print("error in GetOptions()", $g_errorBase+2);
		return 1;
	}

	
	# set m4configuration options.
	my $normalizedPath;
	if (defined($self->{IniComponent}))
	{
		$normalizedPath = $g_oSystem->NormalizePath($self->{IniComponent});
		$g_oConfiguration->IniComponent($normalizedPath);
	}
	if (defined($self->{IniCompilation}))
	{
		$normalizedPath = $g_oSystem->NormalizePath($self->{IniCompilation});
		$g_oConfiguration->IniCompilation($normalizedPath);
	}

    $error ? return 0 : return 1;
}


#============================================================================
# constructor.
#============================================================================
sub new
{
	# input arguments.
	my $class = shift;
	
 	$g_oTrace->Print("new $class", $g_errorBase+1);
	
	# create object.
	m4GetOpt::PassThrough(1);
	$g_oTrace->Print("calling new base class m4GetOpt", $g_errorBase+2);
	my $self = new m4GetOpt(@_);
	m4GetOpt::PassThrough(0);
	
	return undef if(!defined($self));
	bless $self, $class;
	
	# initialize.
	my $error = $self->_Initialize(@_);
	
    $error ? return undef : return $self;
}


#============================================================================
# Usage.
#============================================================================
sub Usage
{
	$g_oTrace->Print("Usage", $g_errorBase+1);

	m4GetOpt::Usage();
	print STDERR "Configuration files options:\n";
	print STDERR "   --IniComponent= \n";
	print STDERR "   --IniCompilation= \n";
}




#============================================================================
#============================================================================
# module definition.
#============================================================================
package	m4TraceGetOpt;
@m4TraceGetOpt::ISA = qw(m4ConfigFilesGetOpt);

#============================================================================
# dependencies.
#============================================================================
use Getopt::Long;

#============================================================================
# function: _Initialize
# description: initialize the object. This method is called only from 
#	the constructor.
# return:
#	0 on success.
#	1 on error.
#============================================================================
sub _Initialize()
{
	$g_oTrace->Print("_Initialize", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	# insert members for trace system..
	$self->{tracelevel} = undef;
	$self->{tracefile} = undef;
	$self->{logfile} = undef;
	$self->{printheader} = undef;
	
    # fisrt check for exe file.
	if (defined($self->{exeIni}))
	{
		my $exeFile = $self->{ex};
		my $exeIni = $self->{exeIni};
		my $value = undef;
		$g_oTrace->Print("Reading trace options from $exeFile...", $g_errorBase+2);
		
		$value = $exeIni->val("TraceOptions", "tracelevel");
		$self->{tracelevel} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("TraceOptions", "tracefile");
		$self->{tracefile} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("TraceOptions", "logfile");
		$self->{logfile} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("TraceOptions", "printheader");
		$self->{printheader} = $value if (defined($value) && ($value ne ""));
	}
	
	# now read other command line options.
	my $error = &GetOptions(
        $self,
        "tracelevel=i",
        "tracefile=s",
        "logfile=s",
        "printheader=i"
	);
	
	if ($error == 0)
	{
		$g_oTrace->Print("error in GetOptions()", $g_errorBase+2);
		return 1;
	}

	# set trace options.
	$g_oTrace->TraceLevel($self->{tracelevel}) if (defined($self->{tracelevel}));
	$g_oTrace->TraceFile($self->{tracefile}) if (defined($self->{tracefile}));
	$g_oTrace->LogFile($self->{logfile}) if (defined($self->{logfile}));
	$g_oTrace->PrintHeader($self->{printheader}) if (defined($self->{printheader}));

    $error ? return 0 : return 1;
}


#============================================================================
# constructor.
#============================================================================
sub new
{
	# input arguments.
	my $class = shift;
 	$g_oTrace->Print("new $class", $g_errorBase+1);
		
	# create object.
	m4GetOpt::PassThrough(1);
	$g_oTrace->Print("calling new base class m4ConfigFilesGetOpt", $g_errorBase+2);
	my $self = new m4ConfigFilesGetOpt(@_);
	m4GetOpt::PassThrough(0);
	
	return undef if(!defined($self));
	bless $self, $class;
	
	# initialize.
	my $error = $self->_Initialize(@_);
	
    $error ? return undef : return $self;
}


#============================================================================
# Usage.
#============================================================================
sub Usage
{
	$g_oTrace->Print("Usage", $g_errorBase+1);

	m4ConfigFilesGetOpt::Usage();
	print STDERR "Trace options:\n";
	print STDERR "   --tracelevel= \n";
	print STDERR "   --tracefile= \n";
	print STDERR "   --logfile= \n";
	print STDERR "   --printheader= \n";
}



#============================================================================
#============================================================================
# module definition.
#============================================================================
package	m4SccsGetOpt;
@m4SccsGetOpt::ISA = qw(m4TraceGetOpt);

#============================================================================
# dependencies.
#============================================================================
use Getopt::Long;

#============================================================================
# function: _Initialize
# description: initialize the object. This method is called only from 
#	the constructor.
# return:
#	0 on success.
#	1 on error.
#============================================================================
sub _Initialize()
{
	$g_oTrace->Print("_Initialize", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	# insert members for SCCS system.
	$self->{sccsdatabase} = undef;
	$self->{sccsrootdirectory} = undef;
	$self->{sccspseudorootdirectory} = undef;
	$self->{sccslocalrootdirectory} = undef;
	$self->{sccsuser} = undef;
	$self->{sccspassword} = undef;
	$self->{sccslabel} = undef;
	$self->{platform} = "win32";

    # fisrt check for exe file.
	if (defined($self->{exeIni}))
	{
		my $exeFile = $self->{ex};
		my $exeIni = $self->{exeIni};
		my $value = undef;
		$g_oTrace->Print("Reading SCCS options from $exeFile...", $g_errorBase+2);
		
		$value = $exeIni->val("SccsOptions", "sccsdatabase");
		$self->{sccsdatabase} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("SccsOptions", "sccsrootdirectory");
		$self->{sccsrootdirectory} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("SccsOptions", "sccspseudorootdirectory");
		$self->{sccspseudorootdirectory} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("SccsOptions", "sccslocalrootdirectory");
		$self->{sccslocalrootdirectory} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("SccsOptions", "sccsuser");
		$self->{sccsuser} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("SccsOptions", "sccspassword");
		$self->{sccspassword} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("SccsOptions", "sccslabel");
		$self->{sccslabel} = $value if (defined($value) && ($value ne ""));

		$value = $exeIni->val("SccsOptions", "platform");
		$self->{platform} = $value if (defined($value) && ($value ne ""));		
	}
	
	# now read other command line options.
	my $error = &GetOptions(
        $self,
        "sccsdatabase:s",
        "sccsrootdirectory:s",
        "sccspseudorootdirectory:s",
        "sccslocalrootdirectory:s",
        "sccsuser:s",
        "sccspassword:s",
        "sccslabel:s",
        "platform:s",
	);
	
	if ($error == 0)
	{
		$g_oTrace->Print("error in GetOptions()", $g_errorBase+2);
		return 1;
	}

    $error ? return 0 : return 1;
}


#============================================================================
# constructor.
#============================================================================
sub new
{
	# input arguments.
	my $class = shift;
 	$g_oTrace->Print("new $class", $g_errorBase+1);
	
	# create object.
	m4GetOpt::PassThrough(1);
	$g_oTrace->Print("calling new base class m4TraceGetOpt", $g_errorBase+2);
	my $self = new m4TraceGetOpt(@_);
	m4GetOpt::PassThrough(0);
	
	return undef if(!defined($self));
	bless $self, $class;
	
	# initialize.
	my $error = $self->_Initialize(@_);
	
    $error ? return undef : return $self;
}


#============================================================================
# Usage.
#============================================================================
sub Usage
{
	$g_oTrace->Print("Usage", $g_errorBase+1);

	m4TraceGetOpt::Usage();
	print STDERR "SCCS options:\n";
	print STDERR "   --sccsrootdirectory= \n";
	print STDERR "   --sccspseudorootdirectory= \n";
	print STDERR "   --sccslocalrootdirectory= \n";
	print STDERR "   --sccsuser= \n";
	print STDERR "   --sccspassword= \n";
	print STDERR "   --sccslabel= \n";
	print STDERR "   --platform= (win32 | sol_sparc6 | ...)\n";
}



#============================================================================
#============================================================================
# module definition.
#============================================================================
package	m4CompilationGetOpt;
@m4CompilationGetOpt::ISA = qw(m4SccsGetOpt);

#============================================================================
# dependencies.
#============================================================================
use Getopt::Long;

#============================================================================
# function: _Initialize
# description: initialize the object. This method is called only from 
#	the constructor.
#============================================================================
sub _Initialize()
{
	$g_oTrace->Print("_Initialize", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	# insert members for compilation system.
	$self->{component} = undef;
	$self->{compilation_mode} = "debug";
	$self->{root_directory} = "./";
	$self->{build_mode} = "build";
	$self->{log_directory} = "./";
	$self->{comp_log_file} = "default-comp-log.txt";
		
    # fisrt check for exe file.
	if (defined($self->{exeIni}))
	{
		my $exeFile = $self->{ex};
		my $exeIni = $self->{exeIni};
		my $value = undef;
		$g_oTrace->Print("Reading Compilation options from $exeFile...", $g_errorBase+2);
		
		$value = $exeIni->val("CompilationOptions", "component");
		$self->{component} = $value if (defined($value) && ($value ne ""));
				
		$value = $exeIni->val("CompilationOptions", "root_directory");
		$self->{root_directory} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("CompilationOptions", "compilation_mode");
		$self->{compilation_mode} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("CompilationOptions", "build_mode");
		$self->{build_mode} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("CompilationOptions", "log_directory");
		$self->{log_directory} = $value if (defined($value) && ($value ne ""));
		
		$value = $exeIni->val("CompilationOptions", "comp_log_file");
		$self->{comp_log_file} = $value if (defined($value) && ($value ne ""));
	}
	
    my $error = &GetOptions(
        $self,
        "component=s",
        "root_directory=s",
		"compilation_mode=s",
		"build_mode=s",
		"log_directory=s",
		"comp_log_file=s",
	);
	
    return 1 if (!$error);
	
	# check if component was set.
	my ($ai_component, $component, @componentList, $componentGroupList);
	$ai_component = $self->{component};
	$componentGroupList = join(" ", $g_oConfiguration->GetComponentGroupList());

	$g_oTrace->Print("ai_component=$ai_component", $g_errorBase+2);
	foreach $component (split(/[\s,]+/, $ai_component))
	{
		if(index($componentGroupList, $component) == -1)
		{
			# the input component is a real component
			push(@componentList, $component);
		}
		else
		{
			# the input component is a group of components.
			my @cgroup = $g_oConfiguration->GetComponentGroup($component);
			push(@componentList, @cgroup);
		}
	}
	$g_oTrace->Print("componentList=@componentList", $g_errorBase+2);
	$self->{component} = join(",", @componentList);
	
	# set compilation modes.
	my (@modeList, $mode);
	$mode = $self->{compilation_mode};
	if ($mode eq "both")
	{
		push(@modeList, "debug");
		push(@modeList, "release");
	}
	else
	{
		push(@modeList, $mode);
	}
	$self->{compilation_mode} = join(",", @modeList);
		
	return 0;
}


#============================================================================
# constructor.
#============================================================================
sub new
{
	# input arguments.
	my $class = shift;
 	$g_oTrace->Print("new $class", $g_errorBase+1);
	
	# create object.
	m4GetOpt::PassThrough(1);
	$g_oTrace->Print("calling new base class m4SccsGetOpt", $g_errorBase+2);
	my $self = new m4SccsGetOpt(@_);
	m4GetOpt::PassThrough(0);
	
	return undef if(!defined($self));
	bless $self, $class;
	
	# initialize.
	my $error = $self->_Initialize(@_);
	
    $error ? return undef : return $self;
}


#============================================================================
# Usage.
#============================================================================
sub Usage
{
	$g_oTrace->Print("Usage", $g_errorBase+1);

	# input arguments. 
	m4SccsGetOpt::Usage();
	print STDERR "Compilation options:\n";
	print STDERR "   --component= \n";
	print STDERR "   --root_directory= \n";
	print STDERR "   --compilation_mode= (debug | release)\n";
	print STDERR "   --build_mode= (build | rebuild)\n";
	print STDERR "   --log_directory= \n";
	print STDERR "   --comp_log_file= \n";
}


#============================================================================
# file return
#============================================================================
1;
