//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              M4Log
// File:                basiclog_manip.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                19-11-1998
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    template definitions/declarations for parameterized manipulators
//
//
//==============================================================================

#ifndef _BASICLOG_MANIP_HPP_
#define _BASICLOG_MANIP_HPP_

// Template for class-manipulators with 2 args
template <class C, class A1, class A2> 
class BasicLogManip2 {
	public:
		BasicLogManip2 (C& (*f)(C&,A1,A2), A1 a1, A2 a2) : _fp(f), _pa1(a1), _pa2(a2) {}
		friend C& operator<<(C& s, const BasicLogManip2<C,A1,A2>& sm) // Template friend Declaration
		{
			(*sm._fp)(s, sm._pa1, sm._pa2);	// Call function
			return s;						// return object
		}
	private:
		C& (*_fp)(C&,A1,A2);	// pointer to manipulator-function
		A1 _pa1;				// Arg1 for manipulator-function
		A2 _pa2;				// Arg2 for manipulator-function
};



// Template for class-manipulators with 1 arg
template <class C, class A1> 
class BasicLogManip1 {
	public:
		BasicLogManip1 (C& (*f)(C&,A1), A1 a1) : _fp(f), _pa1(a1) {}
		friend C& operator<<(C& s, const BasicLogManip1<C,A1>& sm) // Template friend Declaration
		{
			(*sm._fp)(s, sm._pa1);	// Call function
			return s;				// return object
		}
	private:
		C& (*_fp)(C&,A1);		// pointer to manipulator-function
		A1 _pa1;				// Arg1 for manipulator-function
};



// Template for class-manipulators with 1 arg. by reference, with return especial type
template <class C, class A1, class ET>
class BasicLogManipRefEsp {
	public:
		BasicLogManipRefEsp (ET (*f)(C&,A1), A1 a1) : _fp(f), _pa1(a1) {}
		friend ET operator<<(C& s, const BasicLogManipRefEsp<C,A1,ET>& sm) // Template friend Declaration
		{
			return (*sm._fp)(s, sm._pa1);	// Call function			
		}
	private:
		ET (*_fp)(C&,A1);		// pointer to manipulator-function
		A1 _pa1;				// Arg1 for manipulator-function
};



// Template for class-manipulators with 1 generic arg
template <class C, class A1> 
class BasicLogManipGen {
	public:
		BasicLogManipGen (C& (*f)(C&,A1), A1 a1) : _fp(f), _pa1(a1) {}
		friend C& operator<<(C& s, const BasicLogManipGen<C,A1>& sm) // Template friend Declaration
		{
			(*sm._fp)(s, sm._pa1);	// Call function
			return s;				// return object
		}
	private:
		C& (*_fp)(C&,A1);		// pointer to manipulator-function
		A1 _pa1;				// Arg1 for manipulator-function
};



/*
 Usage:
 - You must define a struct that is a wrapper for arguments (contains all arguments in class member's)
 - Make a typedef for your class and arguments (instantiate a template)
 - Make a Manipulator call function
 - Make a Manipulator function, that (normally) is a friend of your class. This is the functional manipulator
		This is an example:
	
	struct MyArgs_t {		// Struct or Class that contains arguments
		m4int32_t	m_Arg1;
		m4date32_t	m_Arg2;
		m4double_t	m_Arg3;
	}
	typedef BasicLogManipGen<MyClass,MyArgs_t>				BasicLogManip_MyClass_MyArgs_t;	// Template Class Definition
	inline BasicLogManip_MyClass_MyArgs_t		MyManipulator	( MyArgs_t ai_oSt ) { return  BasicLogManip_MyClass_MyArgs_t( MyManipulatorf, ai_oSt ); }	// Direct Function
	inline MyClass&						MyManipulatorf	(MyClass& aio_myclass, MyArgs_t ai_oSt)	// maybe friend of MyClass. Indirect Manipulator funcion
	{ 
		// Get args
		m4int32_t	Arg1 = ai_oSt.m_Arg1;
		m4date32_t	Arg2 = ai_oSt.m_Arg2;
		m4double_t	Arg3 = ai_oSt.m_Arg3;

		// Make the functionality
		...	(if you want access to private data of "aio_myclass" this function must be a friend)

		// Return an object reference
		return aio_myclass;
	}

	// The Class for that you want define the manipulator
  	class MyClass {
		public:
		friend MyClass& MyManipulatorf	(MyClass& aio_myclass, MyArgs_t ai_oSt)	// friend of MyClass (only if necessary)
	};

  - Then you can use :
		oMyClass << MyManipulator(oMyArgs) ;

*/

#endif	// _BASICLOG_MANIP_HPP_


