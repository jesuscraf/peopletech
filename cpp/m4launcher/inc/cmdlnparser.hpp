/****************************************************************************
* File:			clparser.h
* Description:	Header file for the classes cBaseOption, template cOption
*				and cCommandLineParser.
****************************************************************************/

#ifndef _clparser_h_
#define _clparser_h_


#include <stdio.h>
#include "m4types.hpp"

class cBaseOption;

typedef m4pchar_t ClXMLDoc ;



//===========================================================================
// DESCRIPTION  Constants used in the cCommandLineParser class
//===========================================================================
const m4uint32_t MAX_OPTIONS	= 64;
const m4uint32_t MAX_OPT_LENGH	= 128;


//===========================================================================
// DESCRIPTION  Flags to use in the cCommandLineParser class constructor.
//				See sample program at bottom of this file.
//===========================================================================
const m4uint32_t IgnoreUnknownOptions	= 0x0001;
const m4uint32_t NeedArguments			= 0x0010;


//===========================================================================
// CLASS        cCommandLineParser
// DESCRIPTION  A base class to parse a command line. This class has all
//				methods needed to parse a command line. The user must
//				derived his own class from this and add members of class
//				cOption type.
//
//				In the command line an option always begin with a '-'.
//				If a program has two option, like 'r' and 's', you
//				can write:
//					program -r -s
//				or
//					program -rs
//				If an option has a value you can write:
//					program -r value
//				or
//					program -rvalue
//				When an option with no '-' is found it, it's is considered
//				like an argument an the parse is stopped.
//
//				After you parse your command line, the variables
//				cCommandLineParser::argc and cCommandLineParser::argv are
//				like the main(argc, argv) without the options, i.e., only
//				when input arguments to the program.
//
//				See a sample program at botton of this file.
//===========================================================================
class cCommandLineParser
{
public:
	static int argc;
    static char argv[MAX_OPTIONS][MAX_OPT_LENGH];
	char m_programName[1024];

private:
	static m4bool_t m_ignoreUnknownOptions;
    static m4bool_t m_needArguments;
    static int m_optionsCounter;
	static cBaseOption* m_options[MAX_OPTIONS];

protected:
	static cCommandLineParser* m_instance;

public:

    //-----------------------------------------------------------------------
    // Method name: Parser
    // Description: this method parse the input arguments an check all
    //		options declared in the derived class.
    // Parameters:	The input arguments to main function.
    // Return:		
	//	0 on success
	//	OnError return code on error.
    //-----------------------------------------------------------------------
	int Parser(int argc, char** argv);

    //-----------------------------------------------------------------------
    // Method name: AddOption
    // Description:	This methods is called each time that an cOption object
    //		is declared. The cOption object is save in an internal list.
    // Parameters:
    //		ai_opt		- A pointer to the cOption object
    // Return:		None.
    //-----------------------------------------------------------------------
	static void AddOption(cBaseOption *ai_opt);

    //-----------------------------------------------------------------------
    // Method name: OnError
    // Description:	This method is called when the command line options are
    //		wrong. The user can be rewrite this method in this
    //		derived class.
    // Return:		None.
    //-----------------------------------------------------------------------
    virtual int OnError();

    //-----------------------------------------------------------------------
    // Method name: Usage
    // Description:	This method print a help program usage in the screen
    //		and exit. It's is called from the onError method.
	//		The user can be rewrite this method in this derived class.
    // Parameters:
    //		ai_programName	- The executable name of the program.
    // Return:		None.
    //-----------------------------------------------------------------------
    virtual void Usage(const char *ai_programName);

protected:
    //-----------------------------------------------------------------------
    // Method name: cCommandLineParser
    // Description: constructor
    // Parameters:	Any of the flags above indicated. This flags can be
    //				ORED between them.
    // Return:		None.
    //-----------------------------------------------------------------------
    cCommandLineParser(int flags = 0);

    //-----------------------------------------------------------------------
    // Method name: CheckMandatoryOptions
    // Description: check that all mandatory options were introduced
    // Return:		0 on success.
	//				-1 on error
    //-----------------------------------------------------------------------
    int CheckMandatoryOptions();
};


//===========================================================================
// CLASS		cBaseOption
// DESCRIPTION  A base class to save an option in a command line.
//				This class has all its members protected, because only
//				the cCommandLineParser clase use them.
//===========================================================================
class cBaseOption
{
public:
    //-----------------------------------------------------------------------
    // Method name: name
    // Description: Return the name for this option.
    // Parameters:	None.
    // Return:		The name for this option
    //-----------------------------------------------------------------------
    char * name() { return m_name; }

	//
    // I do cCommandLineParser class friend, so it can access to all members
    // of this class.
	friend class cCommandLineParser;


	//-----------------------------------------------------------------------
    // Method name:	getValue
    // Description: This method get the value for an option in the command
    //				line.
    // Parameters:
    //      newValue	- A void pointer for the new value for this object.
    // Return:		None.
    //-----------------------------------------------------------------------
    virtual void getValue(void * & newValue)=0;

	//-----------------------------------------------------------------------
    // Method name:	getValue
    // Description: This method get the value for an option in the command
    //				line.
    // Parameters:
    //      newValue	- A void pointer for the new value for this object.
    // Return:		None.
    //-----------------------------------------------------------------------
    
//    virtual void getTranslatedValue(void * & newValue)=0;


protected:
    char * m_name;			// the character that identifies this option
//	char * m_translatedname;
    char *m_helpMessage;
    m4bool_t m_mandatory;
    m4bool_t m_checkMandatory;	// saves if a mandatory option was in the command
    						// line.
    char m_syntax[16];

    //-----------------------------------------------------------------------
    // Method name:	type
    // Description:	This method return a local string with the type of an
    //				specific object of this class.
    // Parameters:	None.
    // Return:		An string with the type.
    //-----------------------------------------------------------------------
    virtual char *type()=0;

    //-----------------------------------------------------------------------
    // Method name:	setValue
    // Description: This method set the value for an option in the command
    //				line.
    // Parameters:
    //      newValue	- A void pointer to the new value for this object.
    // Return:		None.
    //-----------------------------------------------------------------------
    virtual void setValue(void *newValue)=0;


	//-----------------------------------------------------------------------
    // Method name:	syntax
    // Description:	Return a string with the syntax for this option. This
    //				method is used by cCommandLineParser::printUsage()
    //				method.
    // Parameters:	None.
    // Return:		The syntax string.
    //-----------------------------------------------------------------------
    virtual char *syntax()=0;


    //-----------------------------------------------------------------------
    // Method name:	isMandatory
    // Description:	Return if this option is mandatory or not.
    // Parameters:	None.
    // Return:
    //		M4_TRUE	- if this option is mandatory
    //		M4_FALSE	- otherwise.
    //-----------------------------------------------------------------------
    m4bool_t isMandatory() { return m_mandatory; }

    //-----------------------------------------------------------------------
    // Method name:	checkMandatory
    // Description:	Return if a mandatory option was introduced in the
    //				command line options or not.
    // Parameters:	None.
    // Return:
    //		M4_TRUE	- if the mandatory option was not introduced
    //		M4_FALSE	- otherwise.
    //-----------------------------------------------------------------------
    m4bool_t checkMandatory() { return m_checkMandatory; }

    //-----------------------------------------------------------------------
    // Method name: setMandatory
    // Description: save a new state for m_checkMandatory member.
    // Parameters:
    //		mandatory	- the new value for m_checkMandatory
    // Return:		None.
    //-----------------------------------------------------------------------
    void setMandatory(m4bool_t mandatory) { m_checkMandatory = mandatory; }

    //-----------------------------------------------------------------------
    // Method name: helpMessage
    // Description:	Return a string with a description of this option.  This
    //				method is used by cCommandLineParser::printUsage()
    //				method.
    // Parameters:	None.
    // Return:		A string with the description.
    //-----------------------------------------------------------------------
    char *helpMessage();
};


//===========================================================================
// CLASS		cOption
// DESCRIPTION  A template class to evaluate each kind option in a command
//				line. This class has members templarized for types:
//					m4bool_t, char *, int and double
//===========================================================================
template <class T>
class cOption:public cBaseOption
{
	//
    // I do cCommandLineParser class friend, so it can access to all members
    // of this class.
	friend class cCommandLineParser;

protected:
	T m_value;		// this variable save the value for this option.

public:
    //-----------------------------------------------------------------------
    // Method name: cOption
    // Description: constructor. Create a new object of cOption class and
    //		insert it in the cCommandLineParser list.
    // Parameters:
    //		name		- the name for this option.
    //		value		-	it's default value.
    //		helpMessage	- a string with the description of this option.
    //		mandatory	- say if this option is mandatory or not.
    //					  Default value is M4_FALSE.
    // Return:
    //-----------------------------------------------------------------------
    cOption(char * name, T value,/* char * translatedname ,*/ char *helpMessage, m4bool_t mandatory=M4_FALSE)
    {
		//
		// init class members
		m_name = name;
//		m_translatedname = translatedname;
        m_value = value;
        m_helpMessage = helpMessage;
        m_checkMandatory = m_mandatory = mandatory;

        //
        // add this object to cCommandLineParser list.
        cCommandLineParser::AddOption(this);
    }

    //-----------------------------------------------------------------------
    // Method name:	value
    // Description:	Return the value for this option.
    // Parameters:	None.
    // Return:		the value for this option.
    //-----------------------------------------------------------------------
    T value() { return m_value; }

	//-----------------------------------------------------------------------
    // Method name: getValue
    // Description: See description in cBaseOption class.
    //-----------------------------------------------------------------------
    void getValue (void * & value);

	//-----------------------------------------------------------------------
    // Method name: getTranslatedValue
    // Description: See description in cBaseOption class.
    //-----------------------------------------------------------------------

//    void getTranslatedValue(void * & newValue);



protected:
    //-----------------------------------------------------------------------
    // Method name:	type
    // Description: See description in cBaseOption class.
    //-----------------------------------------------------------------------
    char *type();

    //-----------------------------------------------------------------------
    // Method name: setValue
    // Description: See description in cBaseOption class.
    //-----------------------------------------------------------------------
    void setValue (void *newValue);

    //-----------------------------------------------------------------------
    // Method name: syntax
    // Description: See description in cBaseOption class.
    //-----------------------------------------------------------------------
    char *syntax();
};


#endif // _clparser_h_


