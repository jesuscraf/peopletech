#!/usr/bin/perl
#============================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) FileDescription: script to configure application server.
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
#============================================================================

require 5.004;			# perl version required.

#============================================================================
# script dependencies.
#============================================================================
use lib './modules';
use m4scriptbase;
use m4trace;
use m4getopt;
use m4obl;
use iniconf;
use strict;

#============================================================================
# global variables for this script.
#============================================================================
my $g_oTrace = m4trace::GetTraceObject();
my $g_errorBase = m4trace::GetErrorBase(6);
my $g_oSystem = m4system::GetSystemObject();

#============================================================================
# AutoExecute. This method create an object an execute it only when this
#	file is loaded from the command line.
#============================================================================
my $errorCode = &m4ScriptBase::AutoExecute("m4AppServerSetup", __FILE__);

#============================================================================
# module definition.
#============================================================================
package	m4AppServerSetup;
@m4AppServerSetup::ISA = qw(m4TraceGetOpt);

#============================================================================
# module dependencies.
#============================================================================
use Getopt::Long;
use English;
use File::Basename;
use Cwd;
use Cwd 'abs_path'; 

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
	my $self = new m4TraceGetOpt(@_);
	m4GetOpt::PassThrough(0);
	
	return undef if(!defined($self));
	bless $self, $class;
	
	# initialize.
	my $error = $self->_Initialize(@_);
	
    $error ? return undef : return $self;
}

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

	# set in _SetDirectories function
	$self->{InstallationRootDir}; 
	$self->{CacheDir};
	$self->{DebugDir};
	$self->{ReportsDir};

	# specific options for this object.
	# identification options.
	$self->{m4appserver_id} = "M4R&D";
	$self->{description} = "Meta4 Application Server";
	
	# directory options.
	$self->{configuration_root_dir} = "/m4serversite-local";
	
	# meta4AppServer user.
	$self->{m4user} = "TESTER";
	$self->{m4password} = "TESTER";

	# Communications options.
	$self->{base_port} = 2221;

	# database options.
	$self->{dsn} = "ORA32_INT_01";
	$self->{dbserver} = "ATLANTIS2TCPO1120";
	$self->{dbuser} = "PNET8CPTEVO";
	$self->{dbpassword} = "PNET8CPTEVO";
	$self->{maxnumconn} = "50";
	
	# reports options
	$self->{graphicspath} = "";
	$self->{graphicstathbw} = "";
	$self->{paramsdir} = "";

	# read options from the ini file.
	# first parameter is the current object.
	# second parameter is the section.
	# rest of parameters are the options
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "m4appserver_id");
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "description");

	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "configuration_root_dir");

	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "m4user");
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "m4password");
				
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "base_port");

	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "dsn");
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "dbserver");
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "dbuser");
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "dbpassword");
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "maxnumconn");

	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "graphicspath");
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "graphicstathbw");
	m4GetOpt::GetOptionsFromIni($self, "SetupOptions", "paramsdir");

	return 0;
}



#============================================================================
# function: ModuleOptions.
# description: get options from command line.
#============================================================================
sub ModuleOptions
{
	$g_oTrace->Print("ModuleOptions", $g_errorBase+1);
	my $errorCode = 0;

	# read input arguments.
	my $self = shift;

    my $error = &GetOptions(
    	$self,
    	"configuration_root_dir=s",
	);
	
    $errorCode ? return 0 : return $errorCode;
}



#============================================================================
# function: Usage.
# description: prints Usage options in the stdout.
#============================================================================
sub Usage
{
	# read input arguments.
	my $self = shift;
	
	$g_oTrace->Print("Usage", $g_errorBase+1);
	
	# specific options for this script.
	print STDERR "isetup options:\n";
	print STDERR "   --ex=<fullpath>/isetup.ini \n";
}



#============================================================================
# function: Exec
# description: execute ssample.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub Exec
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("Exec", $g_errorBase+1);
	
	# set installationroot and temp directories.
	$self->_SetDirectories();
	
	# create configuration directories.
	print "Creating Config Directories ...\n";
	$self->_CreateConfigDirectories();
		
	# server configuration.
	$retCode = $self->_ServerConfiguration();

	# dispatcher configuration.
	$retCode = $self->_DispatcherConfiguration();
	
	# dispatcher configuration.
	$retCode = $self->_AppctlConfiguration();
	
	return $retCode;
}


#============================================================================
# function: _SetDirectories
# description: gets and set the installation root directory, and tmp 
#	directories.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _SetDirectories
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_SetDirectories", $g_errorBase+1);
		
	my $fullProgramName	= $PROGRAM_NAME;
	$g_oTrace->Print("fullProgramName = $fullProgramName", $g_errorBase+4);
	my $dirname = dirname($fullProgramName);
	$g_oTrace->Print("dirname = $dirname", $g_errorBase+4);
	
	$dirname = $g_oSystem->UnixPath($dirname);
	if ($g_oSystem->Platform() eq "win32")
	{
		# delete the last directory
		$dirname =~ /(.*)\/(.*)/; 
		$self->{InstallationRootDir} = $g_oSystem->NormalizePath($1);
	}
	else
	{
		$dirname .= "/..";
		$self->{InstallationRootDir} = abs_path($dirname);
	}  

	$self->{SetupDir} = $self->{InstallationRootDir} . "/setup";

	$self->{ServerConfigRootDir} = $self->{configuration_root_dir} . "/m4appserver-config";
	$self->{ServerInstallationRootDir} = $self->{InstallationRootDir} . "/m4server";
	
	if ($g_oSystem->Platform() eq "win32")
	{
		$self->{ReportsInstallationRootDir} = $self->{InstallationRootDir} . "/../m4reports";
	}
	else
	{
		$self->{ReportsInstallationRootDir} = $self->{ServerInstallationRootDir} . "/m4reports";
	}

	$self->{ServerTempRootDir} = $self->{ServerConfigRootDir} . "/temp";
	$self->{CacheDir} = $self->{ServerTempRootDir} . "/m4cache";
	$self->{DebugDir} = $self->{ServerTempRootDir} . "/m4debug";
	$self->{ReportsDir} = $self->{ServerTempRootDir} . "/m4reports";

	$self->{DispatcherConfigRootDir} = $self->{configuration_root_dir} . "/m4dispatcher";
	$self->{DispatcherInstallationRootDir} = $self->{InstallationRootDir} . "/m4dispatcher";
	$self->{DispatcherTempRootDir} = $self->{DispatcherConfigRootDir} . "/temp";

	$self->{AppctlConfigRootDir} = $self->{configuration_root_dir} . "/m4appctl";
	$self->{AppctlInstallationRootDir} = $self->{InstallationRootDir} . "/m4appctl";
	$self->{AppctlTempRootDir} = $self->{AppctlConfigRootDir} . "/temp";

	$g_oTrace->Print("InstallationRootDirectory = $self->{InstallationRootDir}", $g_errorBase+3);
	$g_oTrace->Print("SetupDir = $self->{SetupDir}", $g_errorBase+3);
	$g_oTrace->Print("ServerConfigRootDir = $self->{ServerConfigRootDir}", $g_errorBase+3);
		
	return $retCode;
}


#============================================================================
# function: _CreateConfigDirectories
# description: create all directories in the m4appserver configuration.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateConfigDirectories
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_CreateConfigDirectories", $g_errorBase+1);
		
	$g_oSystem->Mkdir("$self->{configuration_root_dir}/setup/conf");

	$g_oSystem->Mkdir($self->{ServerConfigRootDir});
	$g_oSystem->Mkdir($self->{ServerTempRootDir});
	$g_oSystem->Mkdir($self->{CacheDir});
	$g_oSystem->Mkdir($self->{DebugDir});
	$g_oSystem->Mkdir($self->{ReportsDir});

	$g_oSystem->Mkdir($self->{DispatcherConfigRootDir});
	$g_oSystem->Mkdir("$self->{DispatcherConfigRootDir}/bin");
	$g_oSystem->Mkdir("$self->{DispatcherConfigRootDir}/conf");
	$g_oSystem->Mkdir($self->{DispatcherTempRootDir});
				
	$g_oSystem->Mkdir($self->{AppctlConfigRootDir});
	$g_oSystem->Mkdir("$self->{AppctlConfigRootDir}/bin");
	$g_oSystem->Mkdir("$self->{AppctlConfigRootDir}/conf");
	$g_oSystem->Mkdir($self->{AppctlTempRootDir});
				
	return $retCode;
}


#============================================================================
#============================================================================
#
# Server configuration.
#
#============================================================================
#============================================================================

#============================================================================
# function: _ServerConfiguration
# description: It creates a configures a server instance.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _ServerConfiguration
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_ServerConfiguration", $g_errorBase+1);
	print "Configuring m4appserver instance ...\n";
	
	# create buildversion dll.
	$self->_CreateBuildVersion() if $g_oSystem->Platform() eq "win32";
		
	# create m4appserver password file.
	print "   Creating m4appserver password file ...\n";
	$self->_CreateAppServerPasswordFile();

	# create registry.
	print "   Creating Registry ...\n";
	$self->_CreateRegistryFile();
		
	# create log file.
	print "   Creating log file ...\n";
	$self->_CreateLogFile();
				
	# create oblfile.
	print "   Creating obl file ...\n";
	$self->_CreateOblFile();
				
	# create startup script.
	print "   Creating startup script ...\n";
	$self->_CreateScript();
				
	return $retCode;
}


#============================================================================
# function: _CreateRegistryFile
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateRegistryFile
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_CreateRegistryFile", $g_errorBase+1);
		
	my $inputRegistryFile = $self->{SetupDir} . "/templates/regmeta4.reg";
	my $outputRegistryFile = $self->{ServerConfigRootDir} . "/regmeta4.reg";

	# normalize paths.
	$inputRegistryFile = $g_oSystem->NormalizePath($inputRegistryFile);
	$outputRegistryFile = $g_oSystem->NormalizePath($outputRegistryFile);
						
	$g_oTrace->Print("inputRegistryFile = $inputRegistryFile", $g_errorBase+3);
	$g_oTrace->Print("outputRegistryFile = $outputRegistryFile", $g_errorBase+3);


	# copy (and edit) the registry file to a tmp file.
	# open the file.
	$retCode = open (FH, $inputRegistryFile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $inputRegistryFile", $g_errorBase+2);
		return -1;
	}	

	# read the file in an array.
	my @aregistry = <FH>;
	#-- $g_oTrace->Print("Registry file: @aregistry", $g_errorBase+5);
	close (FH);
	
	# delete the line REGEDIT4
	while (@aregistry)
	{
		my $line = shift(@aregistry);
		$g_oTrace->Print("Checking registry line: $line", $g_errorBase+4);
		if ($line =~ /REGEDIT4/)
		{
			$g_oTrace->Print("Deleting registry line: $line", $g_errorBase+4);
			last;
		}
	}
	
	# save the registry in a tmp file.
	my $tmpfile = $self->{ServerTempRootDir} . "/regmeta4.reg.tmp";
	$retCode = open (FH, ">$tmpfile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open output tmpfile: $tmpfile", $g_errorBase+2);
		return -1;
	}
	print FH @aregistry;
	close (FH);	
		
	# open the input registry file. It has INI format.
	my $registry = new IniConf(-file => $tmpfile);
	if (!defined($registry))
	{
		$g_oTrace->Error("Can't load INI file: $tmpfile", $g_errorBase+2);
		my $nerrors = @IniConf::errors;
		my $serrors = join (';', @IniConf::errors);
		$g_oTrace->Error("INI module errors ($nerrors): $serrors");
		return -1;
	}
	
	# Edit the registry. Each line edit a line in the regmeta4.reg file.
	$self->_EditRegistryObject($registry);
		
	# save the tmp registry.
	$registry->RewriteConfig();
	
	# open the new registry file.
	$retCode = open (IFH, $tmpfile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $tmpfile");
		return -1;
	}	
	
	# save the final registry.
	$retCode = open (OFH, ">$outputRegistryFile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open output tmpfile: $outputRegistryFile");
		return -1;
	}
	
	print OFH "REGEDIT4\n\n";
	my $line;
	while (defined($line = <IFH>))
	{
		chomp($line);
		print OFH "$line\n";
	}
	close (IFH);	
	close (OFH);	

	# delete the tmp file.
	unlink($tmpfile);	
	return $retCode;
}


#============================================================================
# function: _EditRegistryObject
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _EditRegistryObject
{
	# read input arguments.
	my $self = shift;
	my $registry = shift;
	
	# local variables 
	my $retCode = 0;
	my $tmpValue;
	$g_oTrace->Print("_EditRegistryObject", $g_errorBase+1);
		
	# cvm values.
	$tmpValue = $self->_RegistryPath("$self->{ServerInstallationRootDir}/workdir");
	$self->_EditRegistryLine($registry, "APPServer\\CVM", "WorkDir", "$tmpValue");
	
	$tmpValue = $self->_RegistryPath("$self->{ServerTempRootDir}");
	$self->_EditRegistryLine($registry, "APPServer\\CVM", "TempDir", "$tmpValue");
	
	# cache values.
	$tmpValue = $self->_RegistryPath("$self->{CacheDir}");
	$self->_EditRegistryLine($registry, "APPServer\\CVM\\Cache", "CacheDir", "$tmpValue");

	# ldb values.
	$tmpValue = "DSN=$self->{dsn};SRVR=$self->{dbserver};UID=$self->{dbuser};PWD=\%s";
	$self->_EditRegistryLine($registry, "APPServer\\CVM\\LDB", "ConnectionString", "$tmpValue");
	$tmpValue = $self->_CreateDataBasePassword();
	$self->_EditRegistryLine($registry, "APPServer\\CVM\\LDB", "DBPassword", "$tmpValue");

	$tmpValue = $self->{maxnumconn};
	$self->_EditRegistryLine($registry, "APPServer\\CVM\\LDB", "MaxNumConn", "$tmpValue");

	# ldb-log values.
	$tmpValue = $self->_RegistryPath($self->{DebugDir});
	$self->_EditRegistryLine($registry, "APPServer\\CVM\\LOG", "DebugDir", "$tmpValue");
	
	# logon values.
	$tmpValue = $self->_RegistryPath($self->{temp_root_dir});
	$self->_EditRegistryLine($registry, "Machine", "LogonPath", "$tmpValue");
	
	# reports values.
	$tmpValue = $self->_RegistryPath("$self->{ReportsDir}/trace.txt");
	$self->_EditRegistryLine($registry, "APPServer\\TOOLS\\REPORTS", "TraceFile", "$tmpValue");
	
	$tmpValue = $self->_RegistryPath("$self->{ReportsDir}");
	$self->_EditRegistryLine($registry, "APPServer\\TOOLS\\REPORTS", "WorkDir", "$tmpValue");
	
	$tmpValue = $self->_RegistryPath("$self->{ReportsInstallationRootDir}/bmps/color");
	$self->_EditRegistryLine($registry, "APPServer\\TOOLS\\REPORTS", "GraphicsPath", "$tmpValue");	
	
	$tmpValue = $self->_RegistryPath("$self->{ReportsInstallationRootDir}/bmps/b_w");
	$self->_EditRegistryLine($registry, "APPServer\\TOOLS\\REPORTS", "GraphicsPathBW", "$tmpValue");	
	
	$tmpValue = $self->_RegistryPath("$self->{ReportsInstallationRootDir}/params");
	$self->_EditRegistryLine($registry, "APPServer\\TOOLS\\REPORTS", "ParamsDir", "$tmpValue");	

	$tmpValue = $self->_RegistryPath("$self->{ReportsInstallationRootDir}/rfnfiles");
	$self->_EditRegistryLine($registry, "APPServer\\TOOLS\\REPORTS", "RFNPath", "$tmpValue");
		
	$tmpValue = $self->_RegistryPath("$self->{ReportsInstallationRootDir}/htmlfiles");
	$self->_EditRegistryLine($registry, "APPServer\\TOOLS\\REPORTS", "GraphicsPathHTML", "$tmpValue");
	
	return $retCode;
}


#============================================================================
# function: _CreateLogFile
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateLogFile
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_CreateLogFile", $g_errorBase+1);
		
	my $inputFile = $self->{SetupDir} . "/templates/logsysserver.ini";
	my $outputFile = $self->{ServerConfigRootDir} . "/logsysserver.ini";

	# open the input log file. It has INI format.
	my $logfile = new IniConf(-file => $inputFile);
	if (!defined($logfile))
	{
		$g_oTrace->Error("Can't load INI file: $inputFile", $g_errorBase+2);
		my $nerrors = @IniConf::errors;
		my $serrors = join (';', @IniConf::errors);
		$g_oTrace->Error("INI module errors ($nerrors): $serrors");
		return -1;
	}
	
	$logfile->WriteConfig($outputFile);
	return $retCode;
}


#============================================================================
# function: _CreateOblFile
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateOblFile
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_CreateOblFile", $g_errorBase+1);
	
	my $inputOblFile = $self->{SetupDir} . "/templates/startup.obl";
	my $outputOblFile = $self->{ServerConfigRootDir} . "/startup.obl";
	
	my $obl = new m4obl($inputOblFile);
	
	$obl->Edit("SERVER.STARTUP", "DESCRIPTION", "\"$self->{description}\"");
	$obl->Edit("SERVER.STARTUP", "SERVER_NAME", "\"$self->{m4appserver_id}\"");
	$obl->Edit("SERVER.STARTUP", "FILE_MANAGER_PATH", "\"$self->{ServerTempRootDir}\"");
	$obl->EditPath("SERVER.STARTUP", "LOG_FILE", "$self->{ServerConfigRootDir}");
		
	$obl->EditPath("SERVER.STARTUP", "PROPERTIES_FILE", "$self->{ServerInstallationRootDir}/workdir");
	$obl->EditPath("SERVER.STARTUP", "TABLES_FILE", "$self->{ServerInstallationRootDir}/workdir");
	$obl->EditPath("SERVER.STARTUP", "LITERALS_FILE", "$self->{ServerInstallationRootDir}/workdir");

    $obl->Edit("SERVER.STARTUP.SNMP.SUBSYSTEM", "SNMP_PORT", "$self->{base_port}");
    $obl->EditPath("SERVER.STARTUP.SNMP.SUBSYSTEM", "SNMP_CONFIG_FILE", "$self->{ServerInstallationRootDir}/workdir");

	$obl->EditPath("SERVER.STARTUP.DISPATCHER_SUBSYSTEM.DISPATCHER.METADATA_SUBSYSTEM.BLOCK3", "DUMP_FILE", "$self->{ServerTempRootDir}");
	$obl->EditPath("SERVER.STARTUP.DISPATCHER_SUBSYSTEM.DISPATCHER.METADATA_SUBSYSTEM.BLOCK3.EXECUTOR.BLOCK", "DUMP_FILE", "$self->{ServerTempRootDir}");

	$obl->EditPath("SERVER.STARTUP.USERS.BLOCK4.USERS_SUBSYSTEM_SERVICE.BLOCK1.EXECUTOR.BLOCK", "DUMP_FILE", "$self->{ServerTempRootDir}");
	
	$obl->EditPath("SERVER.STARTUP.SESSION_CHANNEL_SERVICE.BLOCK3.EXECUTOR.BLOCK", "DUMP_FILE", "$self->{ServerTempRootDir}");
	
	$obl->EditPath("SERVER.STARTUP.PROXY_SUBSYSTEM.BLOCK5.EXECUTOR.BLOCK", "DUMP_FILE", "$self->{ServerTempRootDir}");	
	$obl->EditPath("SERVER.STARTUP.OLTP_SUBSYSTEM.BLOCK6.EXECUTOR.BLOCK", "DUMP_FILE", "$self->{ServerTempRootDir}");	
	$obl->EditPath("SERVER.STARTUP.XML_SUBSYSTEM.BLOCK7.EXECUTOR.BLOCK", "DUMP_FILE", "$self->{ServerTempRootDir}");
	
	$obl->Edit("SERVER.STARTUP.XML_SUBSYSTEM.BLOCK7", "STATE_CACHE_DIR", "\"$self->{ServerTempRootDir}\"");


	$obl->EditPath("SERVER.STARTUP.JOB_SCHEDULER_SUBSYSTEM.BLOCK9", "DUMP_FILE", "$self->{ServerTempRootDir}");
	
	$self->{base_port}++;
	$obl->Edit("SERVER.STARTUP.COMMUNICATION_SERVER.BLOCK10", "BASE_PORT", $self->{base_port}++);
	$obl->EditPath("SERVER.STARTUP.COMMUNICATION_SERVER.BLOCK10", "REQUEST_OUTPUT_FILE", "$self->{ServerTempRootDir}");
	#--- $obl->Edit("SERVER.STARTUP.COMMUNICATION_SERVER.BLOCK10", "ACCEPT_PDU_PORT", $self->{base_port}++);
	#--- $obl->Edit("SERVER.STARTUP.COMMUNICATION_SERVER.BLOCK10", "ACCEPT_ADMIN_PORT", $self->{base_port}++);
	#--- $obl->Edit("SERVER.STARTUP.COMMUNICATION_SERVER.BLOCK10.ADMIN_ACCEPTOR.BLOCK7.ADMIN_ACCEPTOR1.BLOCK", "ADMIN_ACCEPTOR_PORT", $self->{base_port}++);
	$obl->Edit("SERVER.STARTUP.COMMUNICATION_SERVER.BLOCK10", "CERTIFICATE_PATH", $g_oSystem->NormalizePath("\"$self->{ServerInstallationRootDir}/workdir/\""));
	$obl->Edit("SERVER.STARTUP.COMMUNICATION_SERVER.BLOCK10", "TRANSPORT_DUMP_DIRECTORY", "\"$self->{ServerTempRootDir}\"");
	
	$obl->SaveAs($outputOblFile);
	
	return 0;
}


#============================================================================
# function: _CreateBuildVersion
# description: create the personalized m4buildversion.dll library.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateBuildVersion
{
	# read input arguments.
	my $self = shift;
		
	my $inputlibrary = $g_oSystem->NormalizePath("$self->{SetupDir}/templates/m4buildversion.dll");
	my $outputlibrary = $g_oSystem->NormalizePath("$self->{ServerConfigRootDir}/m4buildversion.dll");
	
	system("copy $inputlibrary $outputlibrary");
	return 0;
}


#============================================================================
# function: _CreateAppServerPasswordFile
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateAppServerPasswordFile
{
	# read input arguments.
	my $self = shift;
		
	my $exename;
	my $executable;
	
	if ($g_oSystem->Platform() eq "win32")
	{
		$exename = "m4cryptfile.exe"; 
		$executable = $g_oSystem->NormalizePath("$exename");
	}
	else
	{
		$exename = "m4cryptfile";
		$executable = $exename;
	}
	 
	my $file = $g_oSystem->NormalizePath("$self->{ServerConfigRootDir}/ssc_appuser");
	$g_oSystem->Exec("$executable $self->{m4user} $self->{m4password} $file");
	
	return 0;
}


#============================================================================
# function: _CreateScript
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateScript
{
	# read input arguments.
	my $self = shift;
	
	if ($g_oSystem->Platform() eq "win32")
	{
		$self->_CreateWinDumpDebScripts();
		return $self->_CreateNTScript();
	}
	else
	{
		$self->_CreateUxDumpDebScripts();
		return $self->_CreateUxScript();
	}
		
}


#============================================================================
# function: _CreateNTScript
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateNTScript
{
	# read input arguments.
	my $self = shift;
		
	my $retCode = 0;
	my $inputScriptFile = $g_oSystem->UnixPath($self->{InstallationRootDir} . "/setup/templates/m4server.bat");
	my $outputScriptFile = $g_oSystem->UnixPath($self->{ServerConfigRootDir} . "/m4server.bat");

	# open the new script file.
	$retCode = open (IFH, $inputScriptFile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $inputScriptFile");
		return -1;
	}	
	
	# save the final script.
	$retCode = open (OFH, ">$outputScriptFile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open output tmpfile: $outputScriptFile");
		return -1;
	}
	
	my $line;
	my $installation_root = $self->{InstallationRootDir};
	$installation_root .= "/..";
	$installation_root = $g_oSystem->NormalizePath($self->{ServerInstallationRootDir});
	my $configuration_root = $g_oSystem->NormalizePath($self->{ServerConfigRootDir});
	
	$g_oTrace->Print("installation_root=$installation_root", $g_errorBase+1);

	while (defined($line = <IFH>))
	{
		chomp($line);
		$line = "set M4_SERVER_CONFIGURATION_ROOT=$configuration_root" if ($line =~ /^set M4_SERVER_CONFIGURATION_ROOT=/);
		$line = "set M4_SERVER_INSTALLATION_ROOT=$installation_root" if ($line =~ /^set M4_SERVER_INSTALLATION_ROOT=/);
		#---$line = "set M4_LOGBASE_PATH=$installation_root\\workdir\\logbase.ini" if ($line =~ /^set M4_LOGBASE_PATH/);
		$line = "set M4_USER=$self->{m4user}" if ($line =~ /^set M4_USER=/);
		$line = "set M4_PASSWORD=$self->{m4password}" if ($line =~ /^set M4_PASSWORD=/);
		$line = "set M4_RUN_MODE=" if ($line =~ /^set M4_RUN_MODE=/);
		print OFH "$line\n";
	}
	close (IFH);	
	close (OFH);	
	
	return 0;
}


#============================================================================
# function: _CreateUxScript
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateUxScript
{
	# read input arguments.
	my $self = shift;
		
	my $retCode = 0;
	my $inputScriptFile = $g_oSystem->UnixPath($self->{SetupDir} . "/templates/m4kshserver");
	my $outputScriptFile = $g_oSystem->UnixPath($self->{ServerConfigRootDir} . "/m4kshserver");

	# open the new script file.
	$retCode = open (IFH, $inputScriptFile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $inputScriptFile");
		return -1;
	}	
	
	# save the final script.
	$retCode = open (OFH, ">$outputScriptFile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open output tmpfile: $outputScriptFile");
		return -1;
	}
	
	my $line;
	my $installation_root = $g_oSystem->NormalizePath($self->{ServerInstallationRootDir});
	my $configuration_root = $g_oSystem->NormalizePath($self->{ServerConfigRootDir});
	while (defined($line = <IFH>))
	{
		chomp($line);
		$line = "export M4_SERVER_CONFIGURATION_ROOT=$configuration_root" if ($line =~ /^export M4_SERVER_CONFIGURATION_ROOT/);
		$line = "export M4_SERVER_INSTALLATION_ROOT=$installation_root" if ($line =~ /^export M4_SERVER_INSTALLATION_ROOT/);
		#---$line = "export M4_LOGBASE_PATH=$installation_root\\workdir\\logbase.ini" if ($line =~ /^export M4_LOGBASE_PATH/);
		$line = "export M4_USER=$self->{m4user}" if ($line =~ /^export M4_USER/);
		$line = "export M4_PASSWORD=$self->{m4password}" if ($line =~ /^export M4_PASSWORD/);
		$line = "export M4_RUN_MODE=" if ($line =~ /^export M4_RUN_MODE/);
		print OFH "$line\n";
	}
	close (IFH);	
	close (OFH);	
	
	$g_oSystem->Exec("chmod 755 $outputScriptFile");
	
	return 0;
}

#============================================================================
# function: _CreateUxDumpDebScripts
# description: .
# return:
#	0 on success.
#============================================================================
sub _CreateUxDumpDebScripts
{
	my $self = shift;

	my $inputScriptFile = $g_oSystem->UnixPath($self->{SetupDir} . "/templates/m4kshdump");
	my $outputScriptFile = $g_oSystem->UnixPath($self->{ServerConfigRootDir} . "/m4kshdump");

	$g_oSystem->Exec("cp $inputScriptFile $outputScriptFile");
	$g_oSystem->Exec("chmod 755 $outputScriptFile");

	my $inputScriptFile = $g_oSystem->UnixPath($self->{SetupDir} . "/templates/m4kshdumpext");
	my $outputScriptFile = $g_oSystem->UnixPath($self->{ServerConfigRootDir} . "/m4kshdumpext");

	$g_oSystem->Exec("cp $inputScriptFile $outputScriptFile");
	$g_oSystem->Exec("chmod 755 $outputScriptFile");

	return 0;
}

#============================================================================
# function: _CreateUxDumpDebScripts
# description: .
# return:
#	0 on success.
#============================================================================
sub _CreateWinDumpDebScripts
{
	my $self = shift;

	my $inputScriptFile = $g_oSystem->NormalizePath($self->{SetupDir} . "/templates/m4dump.bat");
	my $outputScriptFile = $g_oSystem->NormalizePath($self->{ServerConfigRootDir} . "/m4dump.bat");

	$g_oSystem->Exec("copy $inputScriptFile $outputScriptFile");

	return 0;
}

#============================================================================
# function: _CreateDataBasePassword
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateDataBasePassword
{
	# read input arguments.
	my $self = shift;
		
	my $password = "e4654b3bd710e464";
	
	my $exename = "m4cryptpwd";
	$exename = "cryptpwd.exe" if $g_oSystem->Platform() eq "win32";
	 
	#---my $executable = $g_oSystem->NormalizePath("$self->{SetupDir}/$exename");
	my $executable = $exename;
	my $tmpfile = $g_oSystem->NormalizePath("$self->{ServerTempRootDir}/dbpassword.reg");

	# create the dbpassord registry entry.
	my $nullFile = "/dev/null";
	$nullFile = "NUL" if $g_oSystem->Platform() eq "win32";
	$g_oSystem->Exec("$executable -s $self->{dbserver} -u $self->{dbuser} -p $self->{dbpassword} -o $tmpfile > $nullFile");
	
	# get dbpassword from tmp file.
	# get the resources. 
	my $retCode = open (FH, $tmpfile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $tmpfile");
		return $password;
	}
	
	my $line;
	while (defined($line = <FH>))
	{
		# searching line : "DBPassword"="e4654b3bd710e464"
		if ($line =~ /^"DBPassword"="(.*)"/)
		{
			 $password = $1;
		}
	}
	close (FH);
	
	# remove the tmp file.
	unlink $tmpfile;

	return $password;
}



#============================================================================
#============================================================================
#
# Dispatcher configuration.
#
#============================================================================
#============================================================================

#============================================================================
# function: _DispatcherConfiguration
# description: It creates a configures a dispatcher instance.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _DispatcherConfiguration
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_DispatcherConfiguration", $g_errorBase+1);
	print "Configuring Dispatcher instance ...\n";
				
	# create startup script.
	print "   Creating startup script ...\n";
	if ($g_oSystem->Platform() eq "win32")
	{
		$retCode = $self->_CreateNtDispatcherScript();
	}
	else
	{
		$retCode =  $self->_CreateUxDispatcherScript();
	}

	# create log file.
	print "   Creating log file ...\n";
	$self->_CreateDispatcherLogFile();
								
	return $retCode;
}


#============================================================================
# function: _CreateNtDispatcherScript
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateNtDispatcherScript
{
	# read input arguments.
	my $self = shift;
		
	my $retCode = 0;
	my $inputScriptFile = $g_oSystem->UnixPath($self->{DispatcherInstallationRootDir} . "/bin/m4dispatcher.bat");
	my $outputScriptFile = $g_oSystem->UnixPath($self->{DispatcherConfigRootDir} . "/bin/m4dispatcher.bat");

	# open the new script file.
	$retCode = open (IFH, $inputScriptFile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $inputScriptFile");
		return -1;
	}	
	
	# save the final script.
	$retCode = open (OFH, ">$outputScriptFile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open output tmpfile: $outputScriptFile");
		return -1;
	}
	
	my $line;
	my $installation_root = $g_oSystem->NormalizePath($self->{DispatcherInstallationRootDir});
	my $configuration_root = $g_oSystem->NormalizePath($self->{DispatcherConfigRootDir});
	
	$g_oTrace->Print("installation_root=$installation_root", $g_errorBase+1);

	while (defined($line = <IFH>))
	{
		chomp($line);
		$line = "set M4_APP_CONFIGURATION_ROOT=$configuration_root" if ($line =~ /^set M4_APP_CONFIGURATION_ROOT/);
		$line = "set M4_APP_INSTALLATION_ROOT=$installation_root" if ($line =~ /^set M4_APP_INSTALLATION_ROOT/);
		$line = "set M4_RUN_MODE=" if ($line =~ /^set M4_RUN_MODE/);
		print OFH "$line\n";
	}
	close (IFH);	
	close (OFH);	

	return $retCode;
}


#============================================================================
# function: _CreateUxDispatcherScript
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateUxDispatcherScript
{
	# read input arguments.
	my $self = shift;
		
	my $retCode = 0;
	my $inputScriptFile = $g_oSystem->UnixPath($self->{DispatcherInstallationRootDir} . "/bin/m4kshdispatcher");
	my $outputScriptFile = $g_oSystem->UnixPath($self->{DispatcherConfigRootDir} . "/bin/m4kshdispatcher");

	# open the new script file.
	$retCode = open (IFH, $inputScriptFile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $inputScriptFile");
		return -1;
	}	
	
	# save the final script.
	$retCode = open (OFH, ">$outputScriptFile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open output tmpfile: $outputScriptFile");
		return -1;
	}
	
	my $line;
	my $app_installation_root = $g_oSystem->NormalizePath($self->{DispatcherInstallationRootDir});
	my $app_configuration_root = $g_oSystem->NormalizePath($self->{DispatcherConfigRootDir});
	while (defined($line = <IFH>))
	{
		chomp($line);
		$line = "export M4_APP_CONFIG_ROOTDIR=$app_configuration_root" if ($line =~ /^export M4_APP_CONFIG_ROOTDIR/);
		$line = "export M4_APP_INSTALL_ROOTDIR=$app_installation_root" if ($line =~ /^export M4_APP_INSTALL_ROOTDIR/);
		$line = "export M4_RUN_MODE=" if ($line =~ /^export M4_RUN_MODE/);
		print OFH "$line\n";
	}
	close (IFH);	
	close (OFH);	
	
	$g_oSystem->Exec("chmod 755 $outputScriptFile");
	
	return 0;
}


#============================================================================
# function: _CreateDispatcherLogFile
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateDispatcherLogFile
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_CreateDispatcherLogFile", $g_errorBase+1);
		
	my $inputFile = $self->{DispatcherInstallationRootDir} . "/conf/logsysdpch.ini";
	my $outputFile = $self->{DispatcherConfigRootDir} . "/conf/logsysdpch.ini";

	$g_oTrace->Print("inputFile: $inputFile", $g_errorBase+4);
	$g_oTrace->Print("outputFile: $outputFile", $g_errorBase+4);

	# open the input log file. It has INI format.
	my $logfile = new IniConf(-file => $inputFile);
	if (!defined($logfile))
	{
		$g_oTrace->Error("Can't load INI file: $inputFile", $g_errorBase+2);
		my $nerrors = @IniConf::errors;
		my $serrors = join (';', @IniConf::errors);
		$g_oTrace->Error("INI module errors ($nerrors): $serrors");
		return -1;
	}

	$logfile->WriteConfig($outputFile);
	return $retCode;
}


#============================================================================
#============================================================================
#
# Appctl configuration.
#
#============================================================================
#============================================================================

#============================================================================
# function: _AppctlConfiguration
# description: It creates a configures a Appctl instance.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _AppctlConfiguration
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_AppctlConfiguration", $g_errorBase+1);
	print "Configuring Appctl instance ...\n";
				
	# create startup script.
	print "   Creating startup script ...\n";
	if ($g_oSystem->Platform() eq "win32")
	{
		$retCode = $self->_CreateNtAppctlScript();
	}
	else
	{
		$retCode =  $self->_CreateUxAppctlScript();
	}

	#create log file.
	print "   Creating log file ...\n";
	$retCode =  $self->_CreateAppctlLogFile();
				
	# create Ini file.
	#---print "   Creating m4dspini.xml file ...\n";
	#---$retCode =  $self->_CreateAppctlIniFile();
				
	return $retCode;
}


#============================================================================
# function: _CreateNtAppctlScript
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateNtAppctlScript
{
	# read input arguments.
	my $self = shift;
		
	my $retCode = 0;
	my $inputScriptFile = $g_oSystem->UnixPath($self->{AppctlInstallationRootDir} . "/bin/m4appctl.bat");
	my $outputScriptFile = $g_oSystem->UnixPath($self->{AppctlConfigRootDir} . "/bin/m4appctl.bat");

	# open the new script file.
	$retCode = open (IFH, $inputScriptFile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $inputScriptFile");
		return -1;
	}	
	
	# save the final script.
	$retCode = open (OFH, ">$outputScriptFile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open output tmpfile: $outputScriptFile");
		return -1;
	}
	
	my $line;
	my $installation_root = $g_oSystem->NormalizePath($self->{AppctlInstallationRootDir});
	my $configuration_root = $g_oSystem->NormalizePath($self->{AppctlConfigRootDir});
	
	$g_oTrace->Print("installation_root=$installation_root", $g_errorBase+1);

	while (defined($line = <IFH>))
	{
		chomp($line);
		$line = "set M4_APP_CONFIGURATION_ROOT=$configuration_root" if ($line =~ /^set M4_APP_CONFIGURATION_ROOT/);
		$line = "set M4_APP_INSTALLATION_ROOT=$installation_root" if ($line =~ /^set M4_APP_INSTALLATION_ROOT/);
		$line = "set M4_RUN_MODE=" if ($line =~ /^set M4_RUN_MODE/);
		print OFH "$line\n";
	}
	close (IFH);	
	close (OFH);	

	return $retCode;
}


#============================================================================
# function: _CreateUxAppctlScript
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateUxAppctlScript
{
	# read input arguments.
	my $self = shift;
		
	my $retCode = 0;
	my $inputScriptFile = $g_oSystem->UnixPath($self->{AppctlInstallationRootDir} . "/bin/m4kshappctl");
	my $outputScriptFile = $g_oSystem->UnixPath($self->{AppctlConfigRootDir} . "/bin/m4kshappctl");

	# open the new script file.
	$retCode = open (IFH, $inputScriptFile);
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't load file: $inputScriptFile");
		return -1;
	}	
	
	# save the final script.
	$retCode = open (OFH, ">$outputScriptFile");
	if ($retCode == 0)
	{
		$g_oTrace->Error("Can't open output tmpfile: $outputScriptFile");
		return -1;
	}
	
	my $line;
	my $app_installation_root = $g_oSystem->NormalizePath($self->{AppctlInstallationRootDir});
	my $app_configuration_root = $g_oSystem->NormalizePath($self->{AppctlConfigRootDir});
	while (defined($line = <IFH>))
	{
		chomp($line);
		$line = "export M4_APP_CONFIG_ROOTDIR=$app_configuration_root" if ($line =~ /^export M4_APP_CONFIG_ROOTDIR/);
		$line = "export M4_APP_INSTALL_ROOTDIR=$app_installation_root" if ($line =~ /^export M4_APP_INSTALL_ROOTDIR/);
		$line = "export M4_RUN_MODE=" if ($line =~ /^export M4_RUN_MODE/);
		print OFH "$line\n";
	}
	close (IFH);	
	close (OFH);	
	
	$g_oSystem->Exec("chmod 755 $outputScriptFile");
	
	return 0;
}




#============================================================================
# function: _CreateAppctlLogFile
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _CreateAppctlLogFile
{
	# read input arguments.
	my $self = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_CreateAppctlLogFile", $g_errorBase+1);
		
	my $inputFile = $self->{AppctlInstallationRootDir} . "/conf/logsys_appctl.ini";
	my $outputFile = $self->{AppctlConfigRootDir} . "/conf/logsys_appctl.ini";

	$g_oTrace->Print("inputFile: $inputFile", $g_errorBase+4);
	$g_oTrace->Print("outputFile: $outputFile", $g_errorBase+4);

	# open the input log file. It has INI format.
	my $logfile = new IniConf(-file => $inputFile);
	if (!defined($logfile))
	{
		$g_oTrace->Error("Can't load INI file: $inputFile", $g_errorBase+2);
		my $nerrors = @IniConf::errors;
		my $serrors = join (';', @IniConf::errors);
		$g_oTrace->Error("INI module errors ($nerrors): $serrors");
		return -1;
	}

	$logfile->WriteConfig($outputFile);
	return $retCode;
}




#============================================================================
#============================================================================
#
# Utilities.
#
#============================================================================
#============================================================================

#============================================================================
# function: _EditIniLine
# description: edit a line in an INI file.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _EditIniLine
{
	# read input arguments.
	my $self = shift;
	
	# input parameters.
	my $ai_iniobject = shift;
	my $ai_section = shift;
	my $ai_key = shift;
	my $ai_value = shift;
	
	my $retCode = 0;
	$g_oTrace->Print("_EditIniLine", $g_errorBase+3);
	
	if ($ai_iniobject->setval($ai_section, $ai_key, $ai_value) == undef) 
	{
		$g_oTrace->Error("Can't set registry value: $ai_section, $ai_key, $ai_value");
		return -1;
	}
	
	return 0;
}


#============================================================================
# function: _RegistryPath
# description: return the input path with the registry style path.
#	A double \\.
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _RegistryPath
{
	# read input arguments.
	my $self = shift;
	my $ai_path = shift;
	
	my $newpath = $g_oSystem->NormalizePath($ai_path);
	$newpath =~ s/\\/\\\\/g;
	
	$g_oTrace->Print("_RegistryPath: $ai_path - $newpath", $g_errorBase+4);
	
	return $newpath;
}


#============================================================================
# function: _EditRegistryLine
# description: .
# return:
#	0 on success.
#	!0 otherwise.
#============================================================================
sub _EditRegistryLine
{
	# read input arguments.
	my $self = shift;
	my $ai_registry = shift;
	my $ai_section = shift;
	my $ai_key = shift;
	my $ai_value = shift;

	my $section = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Meta4\\PeopleNet\\" . "$ai_section";
	my $key = "\"$ai_key\"";
	my $value = "\"$ai_value\"";
	
	if ($ai_registry->setval($section, $key, $value) == undef)
	{
		$section = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Meta4\\PeopleNet\\" . "$ai_section";
		if ($ai_registry->setval($section, $key, $value) == undef)
		{
			$g_oTrace->Error("Can't set registry value: $section, $key, $value");
			return -1;
		}
	}

	return 0;
}
