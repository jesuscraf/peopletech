#============================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
# @(#) FileDescription: functions to send messages to standard output.
#	TraceLevel
#	TraceFile
#	TraceHeader
#	Print
#	Warning
#	Error
#	GetTraceObject
#============================================================================

require 5.004;			# perl version required.

#============================================================================
# module definition.
#============================================================================
package	m4trace;
$VERSION = 1.00;	# version for this module.

#============================================================================
# dependencies.
#============================================================================
use Exporter;
@ISA = qw(Exporter);
use strict;

#============================================================================
# private methods.
# all private methods has an leading underscore. This is a normal convention.
#============================================================================
#============================================================================
# function: _InitializeLogFile
# description: initialize the log file.
#============================================================================
sub _InitializeLogFile()
{
	# read input arguments.
	my $self = shift;
	my $ai_filename = shift;
	
	my($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdist) = localtime(time); 
	my $currentDate = sprintf("%02d/%02d/%02d %02d:%02d:%02d", $year, $mon+1, $mday, $hour, $min, $sec);
	
	open (OUT, ">$ai_filename") || return 1;
	print OUT "-----------------------------------------------------------\n";
	print OUT "--- filename   : $ai_filename\n";
	print OUT "--- description: Meta4 Perl Trace Log File\n";
	print OUT "--- started at : $currentDate \n";
	print OUT "-----------------------------------------------------------\n\n";
	close (OUT);
	
	return 0;
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
	
	# initialize properties.
	my ($m4_trace_level, $m4_trace_file, $m4_log_file, $m4_trace_header);
	$m4_trace_level = $ENV{"M4_PL_TRACE_LEVEL"};
	if ($m4_trace_level)
	{
		$self->{m_TraceLevel} = $m4_trace_level;
	}
	else
	{
		$self->{m_TraceLevel} = 0;
	}
	
	$m4_trace_file = $ENV{"M4_PL_TRACE_FILE"};
	if ($m4_trace_file)
	{
		$self->{m_TraceFile} = $m4_trace_file;
		$self->_InitializeLogFile($self->{m_TraceFile});
	}
	else
	{
		$self->{m_TraceFile} = "";
	}
	
	$m4_log_file = $ENV{"M4_PL_LOG_FILE"};
	if ($m4_log_file)
	{
		$self->{m_LogFile} = $m4_log_file;
	}
	else
	{
		$self->{m_LogFile} = "m4perl.log";
	}
	$self->_InitializeLogFile($self->{m_LogFile});
		
	$m4_trace_header = $ENV{"M4_PL_TRACE_HEADER"};
	if ($m4_trace_header)
	{
		$self->{m_TraceHeader} = $m4_trace_header;
	}
	else
	{
		$self->{m_TraceHeader} = 0;
	}
		
}

#============================================================================
# function: _Enabled
# description: return if trace is enable for the input tracelevel.
# input parameters:
#	ai_tracelevel : the tracelevel to check.
# return:
#	1	if trace is enable for the input tracelevel.
#	0	otherwise
#============================================================================
sub _Enabled
{
	# read input arguments.
	my $self = shift;
	my ($level) = @_;
	$level = $level || 0;
	
	if ($level <= $self->{m_TraceLevel})
	{
		return 1;
	}
	else
	{	
		return 0;
	}
}


#============================================================================
# function: _WhiteSpaces
# description: return a string with ai_length, and filled with whitespaces.
# input_parameters:
#	ai_length, string length
#============================================================================
sub _WhiteSpaces
{
	# read input arguments.
	my $self = shift;
	my ($ai_length) = @_;
	
	my $returnString = " " x $ai_length;
	return $returnString;
}


#============================================================================
# constructor.
#============================================================================
sub new
{
	# input arguments.
	my $class = shift;
	
	# create object.
	my $self = {};
	bless $self, $class;
	
	# initialize.
	$self->_Initialize();
	
	return $self;
}



#============================================================================
# function: TraceLevel
# description: Gets/Sets the trace level.
#============================================================================
sub TraceLevel
{
	# read input arguments.
	my $self = shift;
		
	if (defined $_[0])
	{
		$self->{m_TraceLevel} = $_[0];
	}
	
	return $self->{m_TraceLevel};
}


#============================================================================
# function: TraceFile
# description: Gets/Sets the trace file.
#============================================================================
sub TraceFile
{
	# read input arguments.
	my $self = shift;
		
	if (defined $_[0])
	{
		$self->{m_TraceFile} = $_[0];
		$self->_InitializeLogFile($self->{m_TraceFile});
	}
	
	return $self->{m_TraceFile};
}

   
#============================================================================
# function: LogFile
# description: Gets/Sets the log file.
#============================================================================
sub LogFile
{
	# read input arguments.
	my $self = shift;
		
	if (defined $_[0])
	{
		$self->{m_LogFile} = $_[0];
		$self->_InitializeLogFile($_[0]);
	}
	
	return $self->{m_LogFile};
}

   
#============================================================================
# function: TraceHeader
# description: Sets if traces are printed with headers o no. The header is 
# 	the filename and the linenumber where this method is called.
# input parameters:
#	0 - disable headers.
#	1 - enable headers.
#============================================================================
sub TraceHeader
{
	# read input arguments.
	my $self = shift;
		
	if (defined $_[0])
	{
		$self->{m_TraceHeader} = $_[0];
	}
	
	return $self->{m_TraceHeader};
}


#============================================================================
# function: Print
# description: prints the message if the level specified is less than or equal 
# 	to m_TraceLevel.
# input parameters:
#	ai_message : message to print.
#	ai_tracelevel : the level for this message.
#============================================================================
sub Print
{
	# read input arguments.
	my $self = shift;
	my ($message, $level) = @_;
	
	# set default level if level it's not specified.
	$level = $level || 1;
	
	if ($self->_Enabled($level))
	{
		my $fullMessage = $self->_WhiteSpaces($level);
		
		if ($self->TraceHeader())
		{
			my ($foo, $file, $line, $subname);
			
			# get file and line in the first stack entry.
			($foo, $file, $line, $foo) = caller(0);
			
			# get subroutine name in the second stack entry.
			($foo, $foo, $foo, $subname) = caller(1);
			if (! $subname)
			{
				$subname = "main";
			}
			$fullMessage .= "[#$level, $subname, $file($line)] ";
		}
		else
		{
			$fullMessage .= "[#$level] ";
		}
		$fullMessage .= $message;
		
		
		# print the message.
		my $traceFile = $self->TraceFile();
		if ($traceFile ne "")
		{
			open (OUT, ">>$traceFile") || return 1;
			print OUT "$fullMessage\n";
			close (OUT);
		}
		print STDERR "$fullMessage\n";
	}
	
	return 1;
}



#============================================================================
# function: fPrint
# description: prints the message if the level specified is less than or equal 
# 	to m_TraceLevel. Only prints in the tracefile.
# input parameters:
#	ai_message : message to print.
#	ai_tracelevel : the level for this message.
#============================================================================
sub fPrint
{
	# read input arguments.
	my $self = shift;
	my ($message, $level) = @_;
	
	# set default level if level it's not specified.
	$level = $level || 1;
	
	if ($self->_Enabled($level))
	{
		my $fullMessage = $self->_WhiteSpaces($level);
		
		if ($self->TraceHeader())
		{
			my ($foo, $file, $line, $subname);
			
			# get file and line in the first stack entry.
			($foo, $file, $line, $foo) = caller(0);
			
			# get subroutine name in the second stack entry.
			($foo, $foo, $foo, $subname) = caller(1);
			if (! $subname)
			{
				$subname = "main";
			}
			$fullMessage .= "[#$level, $subname, $file($line)] ";
		}
		else
		{
			$fullMessage .= "[#$level] ";
		}
		$fullMessage .= $message;
		
		
		# print the message.
		my $traceFile = $self->TraceFile();
		if ($traceFile ne "")
		{
			open (OUT, ">>$traceFile") || return 1;
			print OUT "$fullMessage\n";
			close (OUT);
		}
	}
	
	return 1;
}


#============================================================================
# function: _LogMessage
# description: Sends the input message to stdout and log file.
# input parameters:
#	ai_header : header message.
#	ai_message : message to print.
#============================================================================
sub _LogMessage
{
	# read input arguments.
	my $self = shift;
	my $header = shift;
	my $message = shift;
	
	my($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdist) = localtime(time); 
	my $currentDate = sprintf("%02d/%02d/%02d %02d:%02d:%02d", $year, $mon+1, $mday, $hour, $min, $sec);
	
	my $fullMessage = $currentDate . "  " . $header . ": ";
		
	if ($self->TraceHeader())
	{
		my ($foo, $file, $line, $subname);
			
		# get file and line in the first stack entry.
		($foo, $file, $line, $foo) = caller(1);
			
		# get subroutine name in the second stack entry.
		($foo, $foo, $foo, $subname) = caller(2);
		if (! $subname)
		{
			$subname = "main";
		}
		$fullMessage .= "[$subname, $file($line)] ";
	}
	$fullMessage .= $message;
		
		
	# print the message.
	print STDERR "$fullMessage\n";
	
	my $logFile = $self->LogFile();
	open (OUT, ">>$logFile") || return 1;
	print OUT "$fullMessage\n";
	close (OUT);

	return 0;
}


#============================================================================
# function: Success
# description: Sends the input message to stdout and log file.
# input parameters:
#	ai_message : message to print.
#============================================================================
sub Success
{
	# read input arguments.
	my $self = shift;
	my $ai_message = shift;
	
	my $errorCode = $self->_LogMessage("SUCCESS", $ai_message);
			
	return $errorCode;
}


#============================================================================
# function: Warning
# description: Sends the input message to stdout and log file..
# input parameters:
#	ai_message : message to print.
#============================================================================
sub Warning
{
	# read input arguments.
	my $self = shift;
	my $ai_message = shift;
	
	my $errorCode = $self->_LogMessage("WARNING", $ai_message);
			
	return $errorCode;
}


#============================================================================
# function: Error
# description: Sends the input message to stdout and log file.
# input parameters:
#	ai_message : message to print.
#============================================================================
sub Error
{
	# read input arguments.
	my $self = shift;
	my $ai_message = shift;
	
	my $errorCode = $self->_LogMessage("ERROR", $ai_message);
			
	return $errorCode;
}


#============================================================================
# function: FatalError
# description: Sends the input message to stdout and log file and exit.
# input parameters:
#	ai_message : message to print.
#============================================================================
sub FatalError
{
	# read input arguments.
	my $self = shift;
	my $ai_message = shift;
	
	$self->_LogMessage("ERROR", $ai_message);

	die "";
}


#============================================================================
# function: GetTraceObject
# description: return a reference to the global trace object defined in this
#	module.
#============================================================================
my $_PrivateTraceObject = new m4trace;
sub GetTraceObject
{
	return $_PrivateTraceObject;
}



#============================================================================
# function: GetErrorBase
# description: return the error level base for a specific module.
#============================================================================
sub GetErrorBase
{
	# read input arguments.
	my $moduleLevel = shift;
	
	my $NumberOfLevelsOfModules = 6;
	my $NumberOfTraceLevelsByModules = 5;
	my $MaxTraceLevel = $NumberOfLevelsOfModules * $NumberOfTraceLevelsByModules;
		
	return $MaxTraceLevel - $moduleLevel*$NumberOfTraceLevelsByModules;
}



#============================================================================
# file return
#============================================================================
1;
