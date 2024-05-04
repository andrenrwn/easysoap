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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPParse.cpp#1 $
 */

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPParse.h>
#include <easysoap/SOAPNamespaces.h>

#include "SOAPEnvelopeHandler.h"
#include "es_namespaces.h"

#include <string.h>

#define BUFF_SIZE 65536

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USING_EASYSOAP_NAMESPACE

SOAPParser::SOAPParser()
{
	m_envelopeHandler = new SOAPEnvelopeHandler();
    SOAPStrings::initSOAPStrings();
}

SOAPParser::~SOAPParser()
{
	delete m_envelopeHandler;
}


SOAPEnvelope&
SOAPParser::Parse(SOAPEnvelope& env, SOAPTransport& trans)
{
	m_envelopeHandler->SetEnvelope(env);
	m_handler = m_envelopeHandler;

	// make sure our stack is empty
    while (!m_handlerstack.empty())
    {
	    m_handlerstack.pop();
    }
	m_nsmap.clear();

	//
	// clear out any id/href info
	// we have hanging around...
	m_idmap.clear();
	m_hrefs.clear();

	const SOAPString &contentType = trans.GetContentType();
    if (!contentType.IsEmpty() && contentType != "text/xml")
		throw SOAPException("Unexpected content type, only support text/xml: %s", contentType.Str());

	InitParser(trans.GetCharset());
	while (1)
	{
		//
		// create a buffer to read the HTTP payload into
		//
		void *buffer = GetParseBuffer(BUFF_SIZE);
		if (!buffer)
			throw SOAPMemoryException();

		//
		// read the HTTP payload
		//
		int read = trans.Read((char *)buffer, BUFF_SIZE);
		if (!ParseBuffer(read))
		{
			throw SOAPException(
				"Error while parsing SOAP XML payload: %s",
				GetErrorMessage());
		}

		if (read == 0)
			break;
	}

	HandleHRefs();

	return env;
}

SOAPEnvelope&
SOAPParser::Parse(SOAPEnvelope& env, const char *message)
{
	m_envelopeHandler->SetEnvelope(env);
	m_handler = m_envelopeHandler;

	// make sure our stack is empty
    while (!m_handlerstack.empty())
    {
	    m_handlerstack.pop();
    }
	m_nsmap.clear();

	//
	// clear out any id/href info
	// we have hanging around...
	m_idmap.clear();
	m_hrefs.clear();

	const char *pMarker = message;
	int message_length = strlen(message);
	int read = 0;
	int left_to_read = message_length;

	//InitParser(trans.GetCharset());
	InitParser("UTF-8");
	while (1)
	{
		//
		// create a buffer to read the HTTP payload into
		//
		void *buffer = GetParseBuffer(BUFF_SIZE);
		if (!buffer)
			throw SOAPMemoryException();

		//
		// read in chunks limited by BUFF_SIZE
		//
		//int read = trans.Read((char *)buffer, BUFF_SIZE);
		if ( left_to_read > BUFF_SIZE )
		{
			read = BUFF_SIZE;
		}
		else
		{
			read = left_to_read;
		}
		memcpy(buffer, pMarker, read);

		if (!ParseBuffer(read))
		{
			throw SOAPException(
				"Error while parsing SOAP XML payload: %s",
				GetErrorMessage());
		}

		// jump marker up read bytes
		pMarker += read;
		// decrement left_to_read by read bytes
		left_to_read -= read;

		if (read == 0)
		{
			break;
		}
	}

	HandleHRefs();

	return env;
}

void
SOAPParser::startElement(const char *name, const char **attrs)
{
	SOAPParseEventHandler* handler = 0;
	if (m_handlerstack.empty())
	{
		if (sp_strcmp(name, SOAP_ENV PARSER_NS_SEP "Envelope") == 0)
		{
			handler = m_handler;
		}
		else
		{
			//
			// FIXME:
			// Probably what we should do instead of throw is set
			// a flag that says the response is invalid.  We usually
			// get in here when the HTTP response code is 500 and it
			// gives us back some HTML instead of a SOAP Fault.
			throw SOAPException("Unknown SOAP response tag: %s", name);
		}
	}
	else
	{
		handler = m_handlerstack.top();
	}

	if (handler)
	{
		m_handlerstack.push(handler->startElement(*this, name, attrs));
	}
	else
	{
		m_handlerstack.push(NULL);
	}
}

void
SOAPParser::characterData(const char *str, int len)
{
	SOAPParseEventHandler* handler = m_handlerstack.top();
	if (handler)
		handler->characterData(*this, str, len);
}

void
SOAPParser::endElement(const char *name)
{
	SOAPParseEventHandler* handler = m_handlerstack.top();
	if (handler)
		handler->endElement(*this, name);
	m_handlerstack.pop();
}

void
SOAPParser::startNamespace(const char *prefix, const char *uri)
{
	m_nsmap[prefix ? prefix : ""] = uri;
}

void
SOAPParser::endNamespace(const char *prefix)
{
	m_nsmap.erase(prefix ? prefix : "");
}

void
SOAPParser::SetHRefParam(SOAPParameter *param)
{
	m_hrefs.push_back(param);
}

void
SOAPParser::SetIdParam(const SOAPString &id, SOAPParameter *param)
{
	const IdMap::const_iterator iter = m_idmap.find(id);
	if (iter != m_idmap.end())
		throw SOAPException("Found parameter with duplicate id='%s'", id.Str());
	m_idmap[id] = param;
}

const SOAPString&
SOAPParser::ExpandNamespace(const char *ns, const char *nsend) const
{
    static SOAPString emptyNS("");
	m_work.Clear();
    m_work.Append(ns, nsend - ns);
	const NamespaceMap::const_iterator iter = m_nsmap.find(m_work);
	return (iter != m_nsmap.end()) ? (*iter).second : emptyNS;
}

const SOAPString&
SOAPParser::DeclareString(const char* string)
{
    StringMap::const_iterator iter = m_stringmap.find(string);
    if (iter != m_stringmap.end())
    {
        return (*iter).second;
    }
    else
    {
        SOAPString cacheString(string);
        m_stringmap[string] = cacheString;
        return m_stringmap[string];
    } 
}

const SOAPString&
SOAPParser::DeclareString(const SOAPString& string)
{
    StringMap::const_iterator iter = m_stringmap.find(string);
    if (iter != m_stringmap.end())
    {
        return (*iter).second;
    }
    else
    {
        m_stringmap[string] = string;
        return string;
    } 
}

void
SOAPParser::HandleHRefs()
{
	//
	// For all of the parameters with href's
	// which were registerd, link them to
	// the param with the corresponding id.
	for (HRefArray::const_iterator iter = m_hrefs.begin();
            iter != m_hrefs.end(); ++iter)
	{
		SOAPParameter *p = *iter;
		const SOAPAttribute* href = p->FindAttribute("href");
		if (href == NULL)
			throw SOAPException("Somehow a parameter without an href got in the href map...");
		const char *h = (*href).GetValue().GetName();
		if (*h == '#')
		{
			const IdMap::const_iterator id = m_idmap.find(++h);
			if (id == m_idmap.end())
				throw SOAPException("Could not find parameter for href='%s'", --h);
			SOAPParameter *pid = (*id).second;
			p->LinkTo(*pid);
		}
		else
			throw SOAPException("Could not resolve href='%s'", h);
	}
}
