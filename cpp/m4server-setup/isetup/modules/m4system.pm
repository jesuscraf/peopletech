#============================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) FileDescription: wrapper to OS commands.
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
#============================================================================

require 5.004;			# perl version required.

#============================================================================
# module definition.
#============================================================================
package	m4system;
$VERSION = 1.00;	# version for this module.

#============================================================================
# dependencies.
#============================================================================
#---require Exporter;
#---@ISA = qw(Exporter);
use strict;
use m4trace;

#============================================================================
# global variables for this module.
#============================================================================
my $g_oTrace = m4trace::GetTraceObject();
my $g_errorBase = m4trace::GetErrorBase(0);

#============================================================================
# private methods.
# all private methods has an leading underscore. This is a normal convention.
#============================================================================
sub _Initialize()
{
	$g_oTrace->Print("_Initialize", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	
	# initialize platform.
	my $os = $^O;
	$os =~ tr/A-Z/a-z/;
	$os =~ s/ /-/g;

	$self->{m_Os} = $os;
	$self->{m_Platform} = $self->_Platform();
	$self->{m_Alias} = $self->_Alias();
}


#============================================================================
# function: _Alias
# description: generate the Meta4 Platform name.
#============================================================================
sub _Alias()
{
	$g_oTrace->Print("_Alias", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	my %aliasHash = (
		"sunos-5.5.1-i386",		"sol_sparc",
		"sunos-5.6-sparc",		"sol_sparc6",
		"aix-2-4",				"aix",
		"hp-ux-b.11.00-a",		"hp_ux11",
		"hp-ux-b.10.20-a",		"hp_ux",
		"win32",				"win32",
	);
		
	my $platform = $self->{m_Platform};
	my $alias = $aliasHash{$platform};
	if (!defined($alias) || ($alias eq ""))
	{
		$alias = "unknown";
	}
	
	$g_oTrace->Print("alias = $alias", $g_errorBase+2);
	return $alias;
}


#============================================================================
# function: _Platform
# description: generate the Meta4 Platform from uname command.
#============================================================================
sub _Platform()
{
	$g_oTrace->Print("_Platform", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	my $uname = "unknown";
		
	# initialize platform.
	$_ = $self->{m_Os};
	
	SWITCH: {
		$uname = `uname -prs`, last SWITCH 			if /^solaris/;
		$uname = `uname -s` . " " . `uname -v` . "\." . `uname -r`, last SWITCH if /^aix/;
		$uname = `uname -sr`, last SWITCH 			if /^hpux/;
		$uname = "win32", last SWITCH 				if /^mswin32/;
		
		# default
		$uname = `uname -a`, last SWITCH;
	}
	
	$uname =~ s/[\r\n]*//g;
	$uname =~ tr/A-Z/a-z/;
	$uname =~ s/ /-/g;
	$g_oTrace->Print("-uname = $uname-", $g_errorBase+2);
	
	return $uname;
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
	$self->_Initialize();
	
	return $self;
}


#============================================================================
# function: Os
# description: .
#============================================================================
sub Os
{
	$g_oTrace->Print("Os", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
			
	return $self->{m_Os};
}


#============================================================================
# function: Platform
# description: .
#============================================================================
sub Platform
{
	$g_oTrace->Print("Platform", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
			
	return $self->{m_Platform};
}


#============================================================================
# function: Alias
# description: .
#============================================================================
sub Alias
{
	$g_oTrace->Print("Alias", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
			
	return $self->{m_Alias};
}


#============================================================================
# function: NormalizePath
# description: Translate a path to the current OS path style
#============================================================================
sub NormalizePath
{
	$g_oTrace->Print("NormalizePath", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my ($path) = @_;
	
	# initialize OS.
	$_ = $self->{m_Os};
	
	SWITCH: {
		$path = $self->DosPath($path), last SWITCH 				if /^mswin32/;
		
		# default
		$path =  $self->UnixPath($path), last SWITCH;
	}
		
	return $path;
}


#============================================================================
# function: UnixPath
# description: Translate DOS path to UNIX path
#============================================================================
sub UnixPath
{
	$g_oTrace->Print("UnixPath", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my ($path) = @_;
	
	# search character '\' and replace by '/'
	$path =~ s/\\+/\//g;
	
	return $path;
}


#============================================================================
# function: DosPath
# description: Translate UNIX path to DOS path
#============================================================================
sub DosPath
{
	$g_oTrace->Print("DosPath", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my ($path) = @_;
	
	# search character '/' and replace by '\'
	$path =~ s/\/+/\\/g;
	
	return $path;
}


#============================================================================
# function: Copy
# description: copy a file or directory calling system() function.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub Copy
{
	$g_oTrace->Print("Copy", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my ($source, $target, $recursive) = @_;
	my $platf = $self->Platform();
	my $errorCode = 0;
	
	# check for recursive.
	if (!defined($recursive))
	{
		$recursive = 0;
	}
	
	SWITCH: {
		if ($platf eq "win32")
		{
			$source = $self->DosPath($source);
			$target = $self->DosPath($target);
			$g_oTrace->Print("source: $source", $g_errorBase+3);
			$g_oTrace->Print("target: $target", $g_errorBase+3);
			
			if (-d $source)
			{
				# source is a directory
				if ($recursive)
				{
					$recursive = "/E";
				}
				else
				{
					$recursive = " ";
				}
	
				my $command = "xcopy $source $target /I /C /H /Q /K /R $recursive > nul";
				$g_oTrace->Print("command: $command", $g_errorBase+3);
				$errorCode = $self->Exec($command);
			}
			else
			{
				# source is a file
				my $command = "copy $source $target > nul";
				$g_oTrace->Print("command: $command", $g_errorBase+3);
				$errorCode = $self->Exec($command);
				$g_oTrace->Error($command) if ($errorCode);
				
			}
			last SWITCH;
		}
		
		# default UNIX
		#--if ($platf eq "solaris")
		#--{
			$source = $self->UnixPath($source);
			$target = $self->UnixPath($target);
			
			if (-d $source)
			{				
				# create target directory.
				$self->Mkdir($target);
				
				# copy files.
				if ($recursive)
				{
					$errorCode = system ("cp -r $source $target");
				}
				else
				{
					my @files = <$source/*>;
					my $file;
					$errorCode = 0;
					foreach $file (<$source/*>)
					{
						if (-f $file)
						{	
							$errorCode += system ("cp $file $target");
						}
					}
				}
			}
			else
			{
				# source is a file
				$errorCode = system ("cp $source $target");
			}
			last SWITCH;
		#--}
	}
	
	return $errorCode;
}


#============================================================================
# function: CopyTxt
# description: copy a txt file translating eol character.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub CopyTxt
{
	$g_oTrace->Print("Copy", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my ($source, $target) = @_;
	my $platf = $self->Platform();
	my $retCode = 0;
	
	$retCode = open(FH, "$source");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open source file $source");
		return -1;
	}
	my @sourceFile = <FH>;
	close FH;

	$retCode = open(FH, ">$target");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open target file $target");
		close FH;
		return -1;
	}

	foreach my $line (@sourceFile)
	{
		# normalize the eol character.
		$line =~ s/[\r\n]*//g;
		
		print FH "$line\n";
	}

	return 0;
}


	
#============================================================================
# function: Mkdir
# description: creates a directory creating all the non-existing parent 
#	directories first.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub Mkdir
{
	
	# read input arguments.
	my $self = shift;
	my ($newdir) = @_;
	my $platf = $self->Platform();
	my $errorCode = 0;

	$g_oTrace->Print("Mkdir: $newdir", $g_errorBase+1);
	

	SWITCH: {
		if ($platf eq "win32")
		{
			$newdir = $self->DosPath($newdir);
		
			# On Unix shared directories, the mkdir command there is not recursive.
			#if (! -d $newdir)
			#{
			#	$g_oTrace->Print("new dir = $newdir", $g_errorBase+2);
			#	$errorCode = system ("mkdir $newdir");
			#}
		
			my @files = split(/\\/, $newdir);
			my $lastDir = "";
			my $firstDir = 1;
			foreach my $file (@files)
			{
				$lastDir .= $file;
				if (($firstDir) && ($file =~ /.:/))
				{
					$g_oTrace->Print("firstDir = $lastDir", $g_errorBase+3);
					$firstDir = 0;
				}
				elsif (! -d $lastDir)
				{
					$g_oTrace->Print("new dir = $lastDir", $g_errorBase+3);
					$self->Exec("mkdir $lastDir");
				}
			
				$lastDir .= "\\";
			}
			last SWITCH;
		}
		
		# default UNIX
		#--if ($platf eq "solaris")
		#--{
			$newdir = $self->UnixPath($newdir);
			$errorCode = $self->Exec("mkdir -p $newdir");
			last SWITCH;
		#--}
	}
	
	return $errorCode;
}

#============================================================================
# function: m4link
# description: crea un link entre el origen y el destino.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub m4link
{
	$g_oTrace->Print("m4link", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my $origen = shift;
	my $destino = shift;
	my $platf = $self->Platform();
	my $errorCode = 0;
	

	if ($platf eq "win32")
	{
		$g_oTrace->Error("Sobre la plataforma win32 no se pueden hacer link");				
		return 1;
	}
	else
	{
		my $command = "ln -s $origen $destino";		
		$errorCode = $self->Exec($command);
	}
	
	return $errorCode;
}


#============================================================================
# function: Exec.
# description: execute the input command with system().
#============================================================================
sub Exec
{
	$g_oTrace->Print("Exec", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my $command = shift; 
	my $errorCode = 0;
	
	$g_oTrace->Print("command = $command", $g_errorBase+2);
	$errorCode = system($command);
    
	return $errorCode;
}



#============================================================================
# function: Date.
# description: gets the current date.
#============================================================================
sub Date
{
	$g_oTrace->Print("Date", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	
	my($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdist) = localtime(time); 
	my $currentDate = sprintf("%02d/%02d/%02d %02d:%02d:%02d", $year, $mon+1, $mday, $hour, $min, $sec);    

	return $currentDate;
}



#============================================================================
# function: GetSystemObject
# description: return a reference to the global system object defined in this
#	module.
#============================================================================
my $_PrivateSystemObject = new m4system;
sub GetSystemObject
{
	$g_oTrace->Print("GetSystemObject", $g_errorBase+1);
	
	return $_PrivateSystemObject;
}



#============================================================================
# file return
#============================================================================
1;
