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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPParameterHandler.cpp#1 $
 */

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include "SOAPParameterHandler.h"
#include "SOAPStructHandler.h"

#include <easysoap/SOAPNamespaces.h>
#include <easysoap/SOAPParameter.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USING_EASYSOAP_NAMESPACE

SOAPParameterHandler::SOAPParameterHandler()
: m_param(0)
, m_structHandler(0)
, m_setvalue(false)
{
    m_str.reserve(1024);
}

SOAPParameterHandler::~SOAPParameterHandler()
{
	delete m_structHandler;
}

SOAPParseEventHandler *
SOAPParameterHandler::start(SOAPParser& parser, const char *name, const char **attrs)
{
	m_param->SetNull(false);

	const char *ns = sp_strchr(name, PARSER_NS_SEP[0]);
	if (ns)
	{
        SOAPString nsstring(name, ns - name); 
		m_param->GetName().GetNamespace() = parser.DeclareString(nsstring);
        m_param->GetName().GetName() = parser.DeclareString(++ns);
	}
	else
	{
	    m_param->SetName(parser.DeclareString(name));
	}

	m_setvalue = true;
	m_str.clear();

	const char **cattrs = attrs;
	while (*cattrs)
	{
		const char *tag = *cattrs++;
		const char *val = *cattrs++;

		const char *tsep = sp_strchr(tag, PARSER_NS_SEP[0]);
		if (tsep)
		{
                SOAPString attrns(tag, tsep - tag);
				m_attrName.GetNamespace() = parser.DeclareString(attrns);
				m_attrName.GetName() = parser.DeclareString(++tsep);

				SOAPQName& attr = m_param->AddAttribute(m_attrName).GetValue();

				const char *vsep = sp_strchr(val, ':');
				if (vsep)
				{
					const SOAPString& vns = parser.ExpandNamespace(val, vsep);
					if (!vns.IsEmpty())
						attr.Set(parser.DeclareString(++vsep), vns);
					else
						attr = parser.DeclareString(val);
				}
				else
				{
					attr = parser.DeclareString(val);
				}
		}
		else
		{
			tsep = sp_strchr(tag, ':');
			if (tsep)
			{
				throw SOAPException("Could not expand attribute namespace: %s", tag);
			}
			else
			{
				m_attrName = parser.DeclareString(tag);
				m_param->AddAttribute(m_attrName).GetValue() = val;
			}
		}
	}

	return this;
}

SOAPParseEventHandler *
SOAPParameterHandler::startElement(SOAPParser& parser, const char *name, const char **attrs)
{
	//
	// If a parameter has an element, then it must
	// be a struct!
	m_setvalue = false;
	if (!m_structHandler)
		m_structHandler = new SOAPStructHandler();
	m_structHandler->SetParameter(m_param);
	return m_structHandler->start(parser, 0, 0)->startElement(parser, name, attrs);
}

void
SOAPParameterHandler::characterData(const char *str, int len)
{
	if (m_setvalue)
		m_str.append(str, len);
}

void
SOAPParameterHandler::endElement(const char *)
{
	if (m_setvalue)
	{
		m_param->GetStringRef() = m_str.c_str();
	}
}
