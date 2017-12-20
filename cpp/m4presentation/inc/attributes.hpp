#ifndef __ATTRIBUTES_HPP
#define __ATTRIBUTES_HPP

#include "m4stl.hpp"

//Para la lista de includes que tiene una presentacion Main.
typedef vector<string>				VcIdInclude;
typedef VcIdInclude::iterator		itVcIdInclude;

typedef vector<m4date_t>			VcVersionInclude;
typedef VcVersionInclude::iterator	itVcVersionInclude;

//Para controlar la recursion infinita.
//Lo implementamos como vector para tener el path!.
typedef vector<string>				SetIdInclude;
typedef SetIdInclude::iterator		itSetIdInclude;

//En esta clase metemos los atributos que estabamos pasando como argumentos.
class ClPresentationAttributes {
public:
	m4pcchar_t m_pIdPresentation;
	m4pcchar_t m_pOrganization;
	m4pcchar_t m_pRole;
	m4uint8_t m_iLanguage;

	ClPresentationAttributes( m4pcchar_t ai_pIdPresentation, m4pcchar_t ai_pOrganization, m4pcchar_t ai_pRole, m4uint8_t ai_iLanguage )
	{
		m_pIdPresentation = ai_pIdPresentation;
		m_pOrganization = ai_pOrganization;
		m_pRole = ai_pRole;
		m_iLanguage = ai_iLanguage;
	}

	~ClPresentationAttributes()
	{
		m_pIdPresentation = NULL;
		m_pOrganization = NULL;
		m_pRole = NULL;
	}
};

#endif // __ATTRIBUTES_HPP
