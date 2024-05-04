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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPEnvelope.cpp#1 $
 */


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPEnvelope.h>
#include <easysoap/SOAPNamespaces.h>
#include <easysoap/XMLComposer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USING_EASYSOAP_NAMESPACE

static const SOAPString A_prefix("A");
static const SOAPString s_prefix("s");
static const SOAPString y_prefix("y");
static const SOAPString iControl_prefix("iControl");
static const SOAPString iControl_namespace("urn:iControl");
static const SOAPString envelope_prefix("E");

SOAPEnvelope::SOAPEnvelope()
{

}

SOAPEnvelope::~SOAPEnvelope()
{

}

bool
SOAPEnvelope::WriteSOAPPacket(XMLComposer& packet) const
{
	packet.Reset();
	packet.StartTag(SOAPEnv::Envelope, envelope_prefix);

	// TODO: automagically add only the tags we need...
	packet.AddXMLNS(A_prefix, SOAPEnc::base());
	packet.AddXMLNS(s_prefix, XMLSchema2001::xsi());
	packet.AddXMLNS(y_prefix, XMLSchema2001::xsd());
	packet.AddXMLNS(iControl_prefix, iControl_namespace);

	// TODO: allow user to set custom encoding style
	packet.AddAttr(SOAPEnv::encodingStyle, SOAPEnc::base());

	m_header.WriteSOAPPacket(packet);
	m_body.WriteSOAPPacket(packet);

	packet.EndTag(SOAPEnv::Envelope);

	return true;
}




