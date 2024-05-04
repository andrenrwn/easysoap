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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPFault.cpp#1 $
 */

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPFault.h>
#include <easysoap/SOAPNamespaces.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USING_EASYSOAP_NAMESPACE

const SOAPString SOAPFault::faultactor_attr = "faultactor";
const SOAPString SOAPFault::faultcode_attr = "faultcode";
const SOAPString SOAPFault::faultdetail_attr = "detail";
const SOAPString SOAPFault::faultstring_attr = "faultstring";

SOAPFault::SOAPFault()
{
	SetName(SOAPEnv::Fault);
}

SOAPFault::~SOAPFault()
{

}

const SOAPParameter*
SOAPFault::GetFaultCode(void) const
{
	return FindParameter(faultcode_attr);
}

const SOAPParameter*
SOAPFault::GetFaultString(void) const
{
	return FindParameter(faultstring_attr);
}

const SOAPParameter*
SOAPFault::GetFaultActor(void) const
{
	return FindParameter(faultactor_attr);
}

const SOAPParameter*
SOAPFault::GetDetail(void) const
{
	return FindParameter(faultdetail_attr);
}

bool
SOAPFault::WriteSOAPPacket(XMLComposer& packet) const
{
	const SOAPParameter *p;

	packet.StartTag(GetName());

	//
	// Enforce element order
	if ((p = GetFaultCode()))
		p->WriteSOAPPacket(packet);
	if ((p = GetFaultString()))
		p->WriteSOAPPacket(packet);
	if ((p = GetFaultActor()))
		p->WriteSOAPPacket(packet);
	if ((p = GetDetail()))
		p->WriteSOAPPacket(packet);

	for (SOAPParameter::Params::const_iterator iter = GetParams().begin();
            iter != GetParams().end(); ++iter)
	{
		const SOAPParameter& param = *iter;
		//
		// skip elements which have already been output
		const SOAPQName& name = param.GetName();
		if (name == faultcode_attr ||
			name == faultstring_attr ||
			name == faultactor_attr ||
			name == faultdetail_attr)
			continue;
		param.WriteSOAPPacket(packet);
	}

	packet.EndTag(GetName());

	return true;
}

SOAPFaultException::SOAPFaultException(const SOAPFault& fault)
: m_fault(fault)
{
		const SOAPParameter *p;
		m_what = "SOAP Fault";
		if ((p = fault.GetFaultString()))
		{
			m_what.Append(": ");
			m_what.Append(p->GetString());
		}
		if ((p = fault.GetFaultActor()))
		{
			m_what.Append(": ");
			m_what.Append(p->GetString());
		}
}

