#============================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) FileDescription: class to manage obl files.
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
#============================================================================

require 5.004;			# perl version required.

#============================================================================
# module definition.
#============================================================================
package	m4obl;
$VERSION = 1.00;	# version for this module.

#============================================================================
# script dependencies.
#============================================================================
use lib '../modules';
use m4trace;
use m4system;
use strict;

#============================================================================
# global variables for this script.
#============================================================================
my $g_oTrace = m4trace::GetTraceObject();
my $g_errorBase = m4trace::GetErrorBase(1);

my $g_oSystem = m4system::GetSystemObject();

#============================================================================
# constructor.
#============================================================================
sub new
{
	# input arguments.
	my $class = shift;
	my $ai_oblfile = shift;
	
	$g_oTrace->Print("New object $class", $g_errorBase+1); 
	
	# create object.
	my $self = {};
	bless $self, $class;
	
	# initialize.
	return undef if $self->_Initialize($ai_oblfile);
	
	return $self;
}

#============================================================================
# function: _Initialize
# description: initialize the object. This method is called only from 
#	the constructor.
#============================================================================
sub _Initialize()
{
	# read input arguments.
	my $self = shift;
	my $ai_oblfile = shift;
	
	# properties.
	$self->{m_oblfilename} = ""; 
	@{$self->{m_obl}} = (); 


	# members used in search functions.
	$self->{m_blockpath};
	$self->{m_currentBlockPath};
	$self->{m_key};
	$self->{m_value};
	$self->{m_index};
	$self->{m_size};
	
	 
	my $retCode = 0;
	$retCode = $self->Open($ai_oblfile) if (defined($ai_oblfile));
	
	return $retCode;
}


#============================================================================
# function: Open
# description: open and load the oblfile.
#============================================================================
sub Open()
{
	# read input arguments.
	my $self = shift;
	my $ai_oblfile = shift;
	
	if (!defined($ai_oblfile))
	{
		$g_oTrace->Error("obl Open: input file not defined."); 
		return 1;
	}
	
	my $retCode = open(FH, $ai_oblfile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open file $ai_oblfile."); 
		return 1;
	}
	
	$self->{m_oblfilename} = $ai_oblfile;
	my @aobl = <FH>;
	@{$self->{m_obl}} = @aobl; 
	$self->{m_size} = @aobl;
	#-- $g_oTrace->Print("OBL: @{$self->{m_obl}}", $g_errorBase+4); 
	#-- $g_oTrace->Print("SIZE: $self->{m_size}", $g_errorBase+4); 
	close (FH);
	
	return 0;
}


#============================================================================
# function: SaveAs
# description: sve the obl in the specified file.
#============================================================================
sub SaveAs()
{
	# read input arguments.
	my $self = shift;
	my $ai_oblfile = shift;
	
	if (!defined($ai_oblfile))
	{
		$g_oTrace->Error("obl SaveAs: input file not defined."); 
		return 1;
	}
	
	my $retCode = open(FH, ">$ai_oblfile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open file $ai_oblfile."); 
		return 1;
	}
	
	print FH @{$self->{m_obl}};
	
	close (FH);

	return 0;
}


#============================================================================
# function: Edit
# description: edit the obl property.
# parameters:
#	ai_fullBlockPath
#	ai_key
#	ai_value
# return:
#	0 on success
#	-1 otherwise
#============================================================================
sub Edit()
{
	# read input arguments.
	my $self = shift;
	
	$self->{m_blockpath} = shift;
	$self->{m_currentBlockPath} = $self->{m_blockpath};
	$self->{m_key} = shift;
	$self->{m_value} = shift;
	$self->{m_index} = 0;
	
	$g_oTrace->Print("OBL Edit: $self->{m_blockpath}, $self->{m_key}, $self->{m_value}", $g_errorBase+4); 
	my $line = $self->_SearchBlock();	
	return -1 if ($line == -1);
	
	$line = $self->_SearchLine();	
	return -1 if ($line == -1);
	
	# edit the line.
	$line = $self->{m_obl}[$self->{m_index}];
	$g_oTrace->Print("Old line Edit: $line", $g_errorBase+3); 
	$line =~ /(\s*)$self->{m_key}(\s*)=(\s*).*/;
	$self->{m_obl}[$self->{m_index}] = $1.$self->{m_key}.$2."=".$3.$self->{m_value}."\n";
	$self->{m_obl}[$self->{m_index}] = $g_oSystem->NormalizePath($self->{m_obl}[$self->{m_index}]);

	$g_oTrace->Print("New line Edit: $self->{m_obl}[$self->{m_index}]", $g_errorBase+3); 
	
	return 0;
}


#============================================================================
# function: EditPath
# description: edit the path in the specified property. It assumes that
#	last word if the filename.
# parameters:
#	ai_fullBlockPath
#	ai_key
#	ai_value
# return:
#	0 on success
#	-1 otherwise
#============================================================================
sub EditPath()
{
	# read input arguments.
	my $self = shift;
	
	$self->{m_blockpath} = shift;
	$self->{m_currentBlockPath} = $self->{m_blockpath};
	$self->{m_key} = shift;
	$self->{m_value} = shift;
	$self->{m_index} = 0;
	
	$g_oTrace->Print("OBL Edit: $self->{m_blockpath}, $self->{m_key}, $self->{m_value}", $g_errorBase+4); 
	my $line = $self->_SearchBlock();	
	return -1 if ($line == -1);
	
	$line = $self->_SearchLine();	
	return -1 if ($line == -1);
	
	# get the line.
	$line = $self->{m_obl}[$self->{m_index}];
	
	# get the full key.
	$line =~ /(\s*\w+\s*=\s*)"(.*)"/;
	my $key = $1;
	
	# get the file fullname.
	my $fullname = $2;
	$fullname = $g_oSystem->UnixPath($fullname);
	$g_oTrace->Print("Old fullpath: $fullname", $g_errorBase+4); 
		
	# get the filename
	my $filename = $fullname;
	if ($fullname =~ /.*\/(.*)/)
	{
		$filename = $1;
	}
	$g_oTrace->Print("filename: $filename", $g_errorBase+4); 
	
	# set the new full name.
	my $newfullpath = $self->{m_value} . "/" . $filename;
	$newfullpath = $g_oSystem->NormalizePath($newfullpath);
	$g_oTrace->Print("New fullpath: $newfullpath", $g_errorBase+4); 
	
	# update obl line.
	$self->{m_obl}[$self->{m_index}] = $key."\"".$newfullpath."\"\n";
	
	return 0;
}


#============================================================================
# function: _SearchBlock
# description: search the block in the obl with path and key specified.
# parameters:
#	ai_fullBlockPath
#	ai_key
#	ai_value
# return:
#	-1 if line not found
#	the line number otherwise
#============================================================================
sub _SearchBlock()
{
	# read input arguments.
	my $self = shift;
	
	my @subpaths = split(/\./, $self->{m_currentBlockPath});
	my $subpath1 = shift(@subpaths);
	my $subpath2 = shift(@subpaths);
	$self->{m_currentBlockPath} = join(".", @subpaths);

	#-- $g_oTrace->Print("subpaths: @subpaths", $g_errorBase+3);
	#-- $g_oTrace->Print("new m_currentBlockPath: $self->{m_currentBlockPath}", $g_errorBase+3);
	$g_oTrace->Print("Searching block: $subpath1 $subpath2", $g_errorBase+2);
	
	while ($self->{m_index} < $self->{m_size})
	{
		my $line = $self->{m_obl}[$self->{m_index}];
		$g_oTrace->Print("obl line: $line", $g_errorBase+4);
		
		if ($line =~ /^\s*begin\s+$subpath1\s+$subpath2/i)  # if block name matches
		{
			$g_oTrace->Print("path founded: $subpath1 $subpath2, next: $self->{m_currentBlockPath}", $g_errorBase+2);
			if ($self->{m_currentBlockPath} ne "")
			{
				# there is more block names.
				return $self->_SearchBlock();
			}
			else
			{
				return $self->{m_index};
			}
		}
		#--- else
		#--- {
		#--- 	print "$line not found. begin $subpath1 $subpath2\n";
		#--- }
		$self->{m_index}++; 
	}
	
	$g_oTrace->Error("Path $self->{m_blockpath} not found."); 
	return -1;
}


#============================================================================
# function: _SearchLine
# description: search the line in the obl with path and key specified.
# parameters:
#	ai_fullBlockPath
#	ai_key
#	ai_value
# return:
#	-1 if line not found
#	the line number otherwise
#============================================================================
sub _SearchLine()
{
	# read input arguments.
	my $self = shift;
		
	$g_oTrace->Print("Searching line: $self->{m_key}", $g_errorBase+2);
	my $line = $self->{m_obl}[$self->{m_index}];
	$g_oTrace->Print("Matching in line: $line, $self->{m_key} ($self->{m_index},$self->{m_size})", $g_errorBase+3);
	while (!($line =~ /^\s*END\s*$/i) and ($self->{m_index} < $self->{m_size}))
	{
		$g_oTrace->Print("Matching in line: $line, $self->{m_key} ($self->{m_index},$self->{m_size})", $g_errorBase+3);
		if ($line =~ /^\s*$self->{m_key}.*/)
		{
			return $self->{m_index};
		}
		$self->{m_index}++; 
		$line = $self->{m_obl}[$self->{m_index}];
	}
	
	$g_oTrace->Error("Line $self->{m_key} not found."); 
	return -1;
}


#============================================================================
# file return
#============================================================================
1;
