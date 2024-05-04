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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPHeader.cpp#1 $
 */


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPHeader.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USING_EASYSOAP_NAMESPACE

const char *SOAPHeader::actorNext = "http://schemas.xmlsoap.org/soap/actor/next";

SOAPHeader::SOAPHeader(void)
: m_outtasync(false)
{
}

SOAPHeader::~SOAPHeader(void)
{
	for (Headers::iterator i = m_headers.begin(); i != m_headers.end(); ++i)
	{
		delete *i;
	}
}

SOAPParameter&
SOAPHeader::AddHeader(void)
{
	m_outtasync = true;
    SOAPParameter *ret = new SOAPParameter();
    m_headers.push_back(ret);
	return *ret;
}

SOAPParameter&
SOAPHeader::AddHeader(const SOAPQName& name)
{
	SOAPParameter *p = new SOAPParameter(name);
	m_headermap[name] = p;
    m_headers.push_back(p);
	return *p;
}

const SOAPParameter&
SOAPHeader::GetHeader(const SOAPQName& name) const
{
	if (m_outtasync)
		Sync();

	HeaderMap::Iterator found = m_headermap.Find(name);
	if (!found)
		throw SOAPException("Could not find header {%s}:%s",
			(const char *)name.GetNamespace(),
			(const char *)name.GetName());
	return **found;
}

SOAPParameter&
SOAPHeader::GetHeader(const SOAPQName& name)
{
	if (m_outtasync)
		Sync();

	HeaderMap::Iterator found = m_headermap.Find(name);
	if (!found)
		throw SOAPException("Could not find header {%s}:%s",
			(const char *)name.GetNamespace(),
			(const char *)name.GetName());
	return **found;
}

void
SOAPHeader::Sync() const
{
	m_headermap.Clear();
	for (Headers::const_iterator i = m_headers.begin();
            i != m_headers.end(); ++i)
    {
		m_headermap[(*i)->GetName()] = (*i);
    }
	m_outtasync = false;
}

bool
SOAPHeader::WriteSOAPPacket(XMLComposer& packet) const
{
	if (!m_headers.empty())
	{
		packet.StartTag(SOAPEnv::Header);

		for (Headers::const_iterator i = m_headers.begin(); i != m_headers.end(); ++i)
			(*i)->WriteSOAPPacket(packet);

		packet.EndTag(SOAPEnv::Header);
	}
	return true;
}

