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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPBody.cpp#1 $
 */


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPBody.h>
#include <easysoap/XMLComposer.h>
#include <easysoap/SOAPNamespaces.h>

USING_EASYSOAP_NAMESPACE

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOAPBody::SOAPBody() :
    m_isfault(false)
{
}

SOAPBody::~SOAPBody()
{
	for (ParamList::iterator iter = m_params.begin();
            iter != m_params.end(); ++iter)
	{
		delete *iter;
	}
}

SOAPParameter&
SOAPBody::AddParameter()
{
    SOAPParameter* p = new SOAPParameter();
    m_params.push_back(p);
	return *p;
}

bool
SOAPBody::WriteSOAPPacket(XMLComposer& packet) const
{
	packet.StartTag(SOAPEnv::Body);

	if (m_isfault)
		m_fault.WriteSOAPPacket(packet);
	else
	{
		m_method.WriteSOAPPacket(packet);
		for (ParamList::const_iterator iter = m_params.begin();
                iter != m_params.end(); ++iter)
			(*iter)->WriteSOAPPacket(packet);
	}

	packet.EndTag(SOAPEnv::Body);

	return true;
}


