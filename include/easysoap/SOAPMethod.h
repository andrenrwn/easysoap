/* 
 * EasySoap++ - A C++ library for SOAP (Simple Object Access Protocol)
 * Copyright (C) 2001 David Crowley; SciTegic, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/SOAPMethod.h#1 $
 */


#if !defined(AFX_SOAPMETHOD_H__B3726AD6_5844_4059_8ECD_36A553A918AD__INCLUDED_)
#define AFX_SOAPMETHOD_H__B3726AD6_5844_4059_8ECD_36A553A918AD__INCLUDED_

#include <easysoap/SOAP.h>
#include <easysoap/SOAPParameter.h>

BEGIN_EASYSOAP_NAMESPACE

/**
*
*/
class EASYSOAP_EXPORT SOAPMethod : public SOAPParameter
{
public:
	SOAPMethod() {}
	SOAPMethod(const char *name, const char *ns);
	SOAPMethod(const char *name, const char *ns, const char *soapAction);
    SOAPMethod(const SOAPString& name, const SOAPString& ns, const SOAPString& action);
    virtual ~SOAPMethod();

	size_t GetNumParameters() const
	{
		return GetParams().size();
	}

	const SOAPString& GetNamespace() const
	{
		return GetName().GetNamespace();
	}

	void SetSoapAction(const char *sa);
	void SetSoapAction(const SOAPString& sa);
	const SOAPString& GetSoapAction() const
	{
		return m_action;
	}

	bool WriteSOAPPacket(XMLComposer& packet) const;

private:
	SOAPString				m_action;
};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPMETHOD_H__B3726AD6_5844_4059_8ECD_36A553A918AD__INCLUDED_)

