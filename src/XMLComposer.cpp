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
 * XMLComposer.cpp,v 1.1 2001/12/20 22:38:19 dcrowley Exp
 */

#include <string>
#include <easysoap/SOAP.h>
#include <easysoap/XMLComposer.h>

USING_EASYSOAP_NAMESPACE

bool XMLComposer::g_makePretty = false;
bool XMLComposer::g_bufferResults = true;

static const SOAPString ns_prefix("ns");

XMLComposer::XMLComposer(XMLComposerOutput* output)
: m_instart(false)
, m_output(output ? *output :
        (g_bufferResults ? (XMLComposerOutput&)m_buffer :
                                 (XMLComposerOutput&)m_stdout))
, m_gensym(0)
, m_level(0)
{
}

XMLComposer::~XMLComposer()
{
}

void
XMLComposer::SetAddWhiteSpace(bool ws)
{
	g_makePretty = ws;
}

void
XMLComposer::SetBufferResults(bool use_buffer)
{
	g_bufferResults = use_buffer;
}

bool
XMLComposer::GetBufferResults()
{
	return g_bufferResults;
}

void
XMLComposer::GetSymbol(SOAPString& symbol,  const SOAPString& prefix)
{
	char buf[12];
	sprintf(buf, "%d", ++m_gensym);
	symbol = prefix;
	symbol += buf;
}

void
XMLComposer::Reset(bool addDecl)
{
	m_gensym = 0;
	m_level = 0;
	m_instart = false;
    m_buffer.Reset();
	m_nsmap.clear();
	m_nsarray.clear();

	if (addDecl)
	{
		StartPI("xml");
		AddAttr("version", "1.0");
		AddAttr("encoding", "UTF-8");
		EndPI();
	}
}

const char *
XMLComposer::GetBytes()
{
	return m_buffer.GetBytes();
}

unsigned int
XMLComposer::GetLength()
{
	return m_buffer.GetLength();
}

void
XMLComposer::StartPI(const char *tag)
{
	EndStart();
	Write("<?");
	Write(tag);
	m_instart = true;
}

void
XMLComposer::EndPI()
{
	Write("?>");
	if (g_makePretty)
		Write("\r\n");
	m_instart = false;
}

void
XMLComposer::StartTag(const char *tag)
{
	PushLevel();

	EndStart();
	Write("<");
	Write(tag);
	m_instart = true;
}

void
XMLComposer::StartTag(const SOAPString& tag)
{
    StartTag(tag.Str());
}

void
XMLComposer::StartTag(const SOAPQName& tag, const char *prefix)
{
	StartTag(tag, SOAPString(prefix));
}

void
XMLComposer::StartTag(const SOAPQName& tag, const SOAPString& prefix)
{
	if (tag.GetNamespace().IsEmpty())
	{
		StartTag(tag.GetName());
		return;
	}

	PushLevel();

	EndStart();
	Write("<");

	SOAPString nsprefix;
	if (!tag.GetNamespace().IsEmpty())
	{
		NamespaceMap::const_iterator i = m_nsmap.find(tag.GetNamespace());
		if (i != m_nsmap.end())
		{
			Write((*i).second.prefix);
		}
        else
		{
			if (!prefix.IsEmpty())
				nsprefix = prefix;
			else
				GetSymbol(nsprefix, ns_prefix);

		    Write(nsprefix);
		}

		Write(":");
	}

	Write(tag.GetName());
	m_instart = true;

    if (!nsprefix.IsEmpty())
        AddXMLNS(nsprefix, tag.GetNamespace());
}

void
XMLComposer::AddAttr(const SOAPQName& tag, const char *value)
{
	if (!m_instart)
		throw SOAPException("XML serialization error.  Adding attribute when not in start tag.");

	if (g_makePretty)
		Write("\r\n\t");
	else
		Write(" ");

	SOAPString nsprefix;
	if (!tag.GetNamespace().IsEmpty())
	{
		NamespaceMap::const_iterator i = m_nsmap.find(tag.GetNamespace());
		if (i != m_nsmap.end())
		{
			Write((*i).second.prefix);
		}
        else
		{
			GetSymbol(nsprefix, ns_prefix);
		    Write(nsprefix);
		}

		Write(":");
	}

	Write(tag.GetName());
	Write("=\"");
	WriteEscaped(value);
	Write("\"");

	if (!nsprefix.IsEmpty())
        AddXMLNS(nsprefix, tag.GetNamespace());
}

void
XMLComposer::AddAttr(const SOAPQName& tag, const SOAPString& value)
{
	if (!m_instart)
		throw SOAPException("XML serialization error.  Adding attribute when not in start tag.");

	if (g_makePretty)
		Write("\r\n\t");
	else
		Write(" ");

	SOAPString nsprefix;
	if (!tag.GetNamespace().IsEmpty())
	{
		NamespaceMap::const_iterator i = m_nsmap.find(tag.GetNamespace());
		if (i != m_nsmap.end())
		{
			Write((*i).second.prefix);
		}
        else
		{
			GetSymbol(nsprefix, ns_prefix);
		    Write(nsprefix);
		}

		Write(":");
	}

	Write(tag.GetName());
	Write("=\"");
	WriteEscaped(value);
	Write("\"");

	if (!nsprefix.IsEmpty())
        AddXMLNS(nsprefix, tag.GetNamespace());
}
void
XMLComposer::AddAttr(const SOAPQName& tag, const SOAPQName& value)
{
	if (!m_instart)
		throw SOAPException("XML serialization error.  Adding attribute when not in start tag.");

	if (g_makePretty)
		Write("\r\n\t");
	else
		Write(" ");

    SOAPString tnsprefix;
	if (!tag.GetNamespace().IsEmpty())
	{
		NamespaceMap::const_iterator i = m_nsmap.find(tag.GetNamespace());
		if (i != m_nsmap.end())
		{
			Write((*i).second.prefix);
		}
        else
		{
			GetSymbol(tnsprefix, ns_prefix);
		    Write(tnsprefix);
		}

		Write(":");
	}

	Write(tag.GetName());
	Write("=\"");

    SOAPString vnsprefix;
	if (value.GetNamespace().IsEmpty())
	{
		Write(value.GetName());
	}
	else
	{
		NamespaceMap::const_iterator i = m_nsmap.find(value.GetNamespace());
		if (i != m_nsmap.end())
		{
			Write((*i).second.prefix);
		}
		else
		{
			GetSymbol(vnsprefix, ns_prefix);
            Write(vnsprefix);
		}

		Write(":");
		WriteEscaped(value.GetName());

	}

	Write("\"");

    if (!tnsprefix.IsEmpty())
        AddXMLNS(tnsprefix, tag.GetNamespace());
    if (!vnsprefix.IsEmpty())
        AddXMLNS(vnsprefix, value.GetNamespace());
}

void
XMLComposer::AddAttr(const char *attr, const char *value)
{
	if (!m_instart)
		throw SOAPException("XML serialization error.  Adding attribute when not in start tag.");

	if (g_makePretty)
		Write("\r\n\t");
	else
		Write(" ");

	Write(attr);
	Write("=\"");
	WriteEscaped(value);
	Write("\"");
}

void
XMLComposer::AddXMLNS(const char *prefix, const char *ns)
{
    AddXMLNS(SOAPString(prefix), SOAPString(ns));
}

void
XMLComposer::AddXMLNS(const SOAPString &prefix, const SOAPString &ns)
{
	NamespaceMap::const_iterator i = m_nsmap.find(ns);
	if (i == m_nsmap.end())
	{
		NamespaceInfo& ni = m_nsmap[ns];
		ni.prefix = prefix;
		ni.level = m_level;
		ni.value = ns;

		m_nsarray.push_back(&ni);

		if (g_makePretty)
			Write("\r\n\t");
		else
			Write(" ");

		Write("xmlns");
		if (prefix)
		{
			Write(":");
			Write(prefix);
		}
		Write("=\"");
		WriteEscaped(ns);
		Write("\"");
	}
}
void
XMLComposer::EndTag(const char *tag)
{
	bool bUseShortcut = 0;
	if (m_instart)
	{
		if ( bUseShortcut )
		{
			Write("/>");
		}
		else
		{
			Write("></");
			Write(tag);
			Write(">");
		}

		if (g_makePretty)
			Write("\r\n");
		m_instart = false;
	}
	else
	{
		Write("</");
		Write(tag);
		Write(">");
		if (g_makePretty)
			Write("\r\n");
	}

	PopLevel();
}

void
XMLComposer::EndTag(const SOAPString &tag)
{
    EndTag(tag.Str());
}

void
XMLComposer::EndTag(const SOAPQName& tag)
{
	bool bUseShortcut = 0;
	if (tag.GetNamespace().IsEmpty())
	{
		EndTag(tag.GetName());
		return;
	}

	if (m_instart)
	{
		if ( bUseShortcut )
		{
			Write("/>");
		}
		else
		{
			Write("></");

			if (!tag.GetNamespace().IsEmpty())
			{
				NamespaceMap::const_iterator i = m_nsmap.find(tag.GetNamespace());
				if (i == m_nsmap.end())
					throw SOAPException("EndTag: Could not find tag for namespace: %s",
						(const char *)tag.GetNamespace());

				Write((*i).second.prefix);
				Write(":");
			}
	
			Write(tag.GetName());
			Write(">");
		}

		if (g_makePretty)
			Write("\r\n");
		m_instart = false;
	}
	else
	{
		Write("</");

		if (!tag.GetNamespace().IsEmpty())
		{
			NamespaceMap::const_iterator i = m_nsmap.find(tag.GetNamespace());
			if (i == m_nsmap.end())
				throw SOAPException("EndTag: Could not find tag for namespace: %s",
					(const char *)tag.GetNamespace());

			Write((*i).second.prefix);
			Write(":");
		}

		Write(tag.GetName());
		Write(">");
		if (g_makePretty)
			Write("\r\n");
	}

	PopLevel();
}

void
XMLComposer::WriteValue(const char *val)
{
	if (m_instart)
	{
		Write(">");
		m_instart = false;
	}
	WriteEscaped(val);
}

void
XMLComposer::EndStart()
{
	if (m_instart)
	{
		Write(">");
		if (g_makePretty)
			Write("\r\n");
		m_instart = false;
	}
}

void
XMLComposer::Write(const char *str)
{
    m_output.Write(str);
}

void
XMLComposer::WriteEscaped(const char *str)
{
	if (str)
	{
		while (*str)
		{
            char c = *str++;
            switch (c)
            {
            case '&':	Write("&amp;");		break;
            case '<':	Write("&lt;");		break;
            case '>':	Write("&gt;");		break;
            case '\'':	Write("&apos;");	break;
            case '\"':	Write("&quot;");	break;
            case '\r':	Write("&#xd;");		break;
            default:    m_output.Write(c);    break;
            }
		}
	}
}

void
XMLComposer::Write(const char *str, unsigned int len)
{
	while (len--)
	{
        m_output.Write(*str++);
	}
}

void
XMLComposer::PopLevel()
{
	for (NamespaceArray::iterator i = m_nsarray.end(); i-- != m_nsarray.begin();)
	{
		if ((*i)->level != m_level)
			break;
		m_nsmap.erase((*i)->value);
                i = m_nsarray.erase(i);
	}
	--m_level;
}

void
XMLComposer::PushLevel()
{
	++m_level;
}

XMLComposerBuffer::XMLComposerBuffer(void) :
    m_buffer(0), m_buffptr(0), m_buffend(0), m_buffsize(0)
{
}

XMLComposerBuffer::~XMLComposerBuffer()
{
    delete [] m_buffer;
    m_buffer = NULL;
}

const char *
XMLComposerBuffer::GetBytes(void)
{
	if (m_buffptr) *m_buffptr = 0;
	return m_buffer;
}

unsigned int
XMLComposerBuffer::GetLength()
{
	return m_buffptr - m_buffer;
}

void
XMLComposerBuffer::Write(char c)
{
    if (m_buffptr == m_buffend)
    {
        Resize();
    }
    
    *m_buffptr++ = c;
}

void
XMLComposerBuffer::Write(const char* s)
{
    while (*s)
    {
        if (m_buffptr == m_buffend)
        {
            Resize();
        }
        *m_buffptr++ = *s++;
    }
}

void
XMLComposerBuffer::Resize(void)
{
	m_buffsize *= 2;
	if (m_buffsize == 0)
		m_buffsize = 4096;

	char *newbuff = new char[m_buffsize];
    int length = GetLength();
    memcpy(newbuff, m_buffer, length);

	delete [] m_buffer;
	m_buffer = newbuff;
	m_buffptr = newbuff + length;
	m_buffend = newbuff + m_buffsize;
}

void
XMLComposerBuffer::Reset(void)
{
	m_buffptr = m_buffer;
}

void
XMLComposerStdout::Write(char c)
{
    fputc(c, stdout);
}

void
XMLComposerStdout::Write(const char* s)
{
    while(*s)
    {
        fputc(*s++, stdout);
    }
}
