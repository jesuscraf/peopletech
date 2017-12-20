#============================================================================
# @(#) FileVersion: [M4_FILE_VERSION]
# @(#) FileDescription: the Meta4 components configuration.
# @(#) CompanyName: Meta4 Spain, S.A.
# @(#) LegalCopyright: (c) [M4_YEAR]
# @(#) ProductName:  PeopleNet
#============================================================================

require 5.004;			# perl version required.

#============================================================================
# module definition.
#============================================================================
package	m4component;

#============================================================================
# dependencies
#============================================================================
use strict;
use m4trace;	
use m4system;
use	iniconf;

#============================================================================
# global variables for this module.
#============================================================================
my $g_oTrace = m4trace::GetTraceObject();
my $g_errorBase = m4trace::GetErrorBase(2);
my $g_oSystem = m4system::GetSystemObject();

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
	
	return undef if ($error);
	return $self;
}

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
	my $ai_componentName = shift;
	my $ai_IniObject = shift;
	$g_oTrace->Print("ai_componentName=$ai_componentName", $g_errorBase+2);
	
	# insert options from default section.
	my (@parameters, $parameter, $value);
	@parameters= $ai_IniObject->Parameters("default");
	foreach $parameter (@parameters)
	{
		$value = $ai_IniObject->val("default", $parameter);
		$self->{$parameter} = $value;
	}
	
	# insert options from this component section
	my (@parameters, $parameter, $value);
	$g_oTrace->Print("getting parameters for $ai_componentName", $g_errorBase+3);
	@parameters = $ai_IniObject->Parameters($ai_componentName);
	$g_oTrace->Print("$ai_componentName parameters=@parameters", $g_errorBase+3);
	foreach $parameter (@parameters)
	{
		$value = $ai_IniObject->val($ai_componentName, $parameter);
		$self->{$parameter} = $value;
	}
	
	# subst predefined macros.
	foreach $parameter (keys %$self)
	{
		$value = $self->{$parameter};
		$value =~ s/\@ComponentName\@/$ai_componentName/g;
		$self->{$parameter} = $value;
	}

	return 0;
}




#============================================================================
# module definition.
#============================================================================
package	m4configuration;

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
	$self->Initialize(@_);
	
	return $self;
}


#============================================================================
# function: Initialize
# description: initialize the object. This method is called only from 
#	the constructor.
#============================================================================
sub Initialize()
{
	$g_oTrace->Print("Initialize", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	# create members.
	my $IniComponent = shift;
	my $IniCompilation = shift;
	$g_oTrace->Print("IniComponent = $IniComponent", $g_errorBase+2);
	$g_oTrace->Print("IniCompilation = $IniCompilation", $g_errorBase+2);

	my $retCode = 0;
	
	$retCode += $self->IniComponent($IniComponent) if (-r $IniComponent);
	$retCode += $self->IniCompilation($IniCompilation) if (-r $IniCompilation);
			
	return $retCode;
}


#============================================================================
# function: $IniCompilation
# description: initialize the object m_IniCompilation
#============================================================================
sub IniCompilation()
{
	$g_oTrace->Print("IniCompilation", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	# create members.
	my $IniCompilationFile = shift;
	$g_oTrace->Print("IniCompilation = $IniCompilationFile", $g_errorBase+2);
	$self->{m_IniCompilation} = new IniConf(-file => "$IniCompilationFile");
	
	if (!defined ($self->{m_IniCompilation}))
	{
		$g_oTrace->Error("$IniCompilationFile load failed");
		return 1;
	}
		
	return 0;
}



#============================================================================
# function: IniComponent
# description: initialize the object m_IniConfiguration
#============================================================================
sub IniComponent()
{
	$g_oTrace->Print("IniComponent", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;

	# create members.
	my $IniComponentFile = shift;
	$g_oTrace->Print("IniComponent = $IniComponentFile", $g_errorBase+2);
	
	$self->{m_IniConfiguration} = new IniConf(-file => "$IniComponentFile");
	
	if (!defined ($self->{m_IniConfiguration}))
	{
		$g_oTrace->Error("$IniComponentFile load failed");
		return 1;
	}
		
	return 0;
}



#============================================================================
# function: GetComponentObject
# description: Return a hash object with all component options.
# input parameters:
#	ai_componentName
#============================================================================
sub GetComponentObject()
{
	$g_oTrace->Print("GetComponentObject", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my $ai_componentName = shift;

	# create component object.
	my $oComponent = new m4component($ai_componentName, $self->{m_IniConfiguration});

	# trace component object.
	foreach my $key (keys %$oComponent)
	{
		$g_oTrace->Print("key=$key, value=$oComponent->{$key}", $g_errorBase+3);
	}

	return $oComponent;
	
}


#============================================================================
# function: GetComponentGroup
# description: Return an array with the list of components of the input group.
# input parameters:
#	ai_componentGroup
#============================================================================
sub GetComponentGroup()
{
	$g_oTrace->Print("GetComponentGroup", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	my $ai_componentGroupName = shift;
	
	my @totalcomponents;
	my $component = $self->{m_IniCompilation}->val("ComponentGroup", $ai_componentGroupName);
	my @components = split(/[\s,]+/, $component);
	$g_oTrace->Print("ComponentGroup=$ai_componentGroupName", $g_errorBase+2);
	$g_oTrace->Print("components=$component", $g_errorBase+2);
	foreach $component (@components)
	{
		if ($component =~ /@(.+)@/)
		{
			# is a component group.
			my $realname = $1;
			my @subcomponents = $self->GetComponentGroup($realname);
			push (@totalcomponents, @subcomponents);
		}
		else
		{
			push (@totalcomponents, $component);
		}
	}
	
	return @totalcomponents;
}


#============================================================================
# function: GetComponentGroupList
# description: Return an array with the list of components groups.
#============================================================================
sub GetComponentGroupList()
{
	$g_oTrace->Print("GetComponentGroupList", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	
	my @componentList= $self->{m_IniCompilation}->Parameters("ComponentGroup");
	$g_oTrace->Print("componentList=@componentList", $g_errorBase+2);

	return @componentList;
}


#============================================================================
# function: GetNtCompilerTools
# description: Return a hash with the list of nt compilation tools.
#============================================================================
sub GetNtCompilerTools()
{
	$g_oTrace->Print("GetNtCompilerTools", $g_errorBase+1);
	
	# read input arguments.
	my $self = shift;
	
	my @CompilerTools = $self->{m_IniCompilation}->Parameters("NtCompilerTools");
	my $CompilerToolsName = {};
	
	foreach my $tool (@CompilerTools)
	{
		my $toolName = $self->{m_IniCompilation}->val("NtCompilerTools", $tool);
		if (defined($toolName))
		{
			$CompilerToolsName->{$tool} = $toolName;
		}
		else
		{
			$CompilerToolsName->{$tool} = "";
		}
		$g_oTrace->Print("GetNtCompilerTools - $tool = $toolName", $g_errorBase+2);
	}

	return $CompilerToolsName;
}



#============================================================================
# function: GetPlatformInfo
# description: Return a object with the properties of the input platform.
#============================================================================
sub GetPlatformInfo()
{
	
	# read input arguments.
	my $self = shift;
	my $platform = shift;
	$g_oTrace->Print("GetPlatformInfo [$platform]", $g_errorBase+1);
	
	my @PlatformProperties = $self->{m_IniCompilation}->Parameters($platform);
	my $oPlatform = {};
	
	foreach my $property (@PlatformProperties)
	{
		my $propertyValue = $self->{m_IniCompilation}->val($platform, $property);
		if (defined($propertyValue))
		{
			$oPlatform->{$property} = $propertyValue;
		}
		else
		{
			$oPlatform->{$property} = "";
		}
		$g_oTrace->Print("GetPlatformInfo - $property = $propertyValue", $g_errorBase+2);
	}

	return $oPlatform;
}



#============================================================================
# function: GetConfigurationObject
# description: return a reference to the global configuration object defined 
#	in this module.
#============================================================================
my $_defaultComponentFile = "../conf/m4component.ini";
my $_defaultCompilationFile = "../conf/m4compilation.ini";
my $_PrivateConfigurationObject = new m4configuration($_defaultComponentFile, $_defaultCompilationFile);
sub GetConfigurationObject
{
	$g_oTrace->Print("GetConfigurationObject", $g_errorBase+1);
	
	return $_PrivateConfigurationObject;
}


#============================================================================
# file return
#============================================================================
1;
