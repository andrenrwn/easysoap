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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/XMLComposer.h#1 $
 */


#if !defined(AFX_XMLCOMPOSER_H__DB61E902_B4A0_4AA3_A0F7_479D6295BD06__INCLUDED_)
#define AFX_XMLCOMPOSER_H__DB61E902_B4A0_4AA3_A0F7_479D6295BD06__INCLUDED_

#include <easysoap/SOAPHashMap.h>
#include <easysoap/SOAPString.h>
#include <boost/shared_ptr.hpp>

BEGIN_EASYSOAP_NAMESPACE

class EASYSOAP_EXPORT XMLComposerOutput
{
    public:
        virtual ~XMLComposerOutput() {}
        virtual void Write(char c) = 0;
        virtual void Write(const char* s) = 0; 
};

class EASYSOAP_EXPORT XMLComposerBuffer : public XMLComposerOutput
{
    public:
        XMLComposerBuffer(void);
        virtual ~XMLComposerBuffer();
        
        void Write(char c);
        void Write(const char* s);

        const char* GetBytes();
        unsigned int GetLength();

        void Reset(void);

    private:
        void Resize(void);

    private:
        char			*m_buffer;
        char			*m_buffptr;
        const char		*m_buffend;
        unsigned int	m_buffsize;
};

class EASYSOAP_EXPORT XMLComposerStdout : public XMLComposerOutput
{
    public:
        void Write(char c);
        void Write(const char* s); 
};

/**
*
*/
class EASYSOAP_EXPORT XMLComposer  
{
public:
	XMLComposer(XMLComposerOutput* output = NULL);
	virtual ~XMLComposer();
	void Reset(bool addDecl = false);

	const char* GetBytes();
	unsigned int GetLength();
    const char* DetachBytes(unsigned int &length);

	void StartPI(const char *name);
	void EndPI();

	void StartTag(const char *tag);
	void StartTag(const SOAPString &tag);
	void StartTag(const SOAPQName& tag, const char *prefix = 0);
	void StartTag(const SOAPQName& tag, const SOAPString& prefix);

	void AddAttr(const char *attr, const char *value);
	void AddAttr(const SOAPQName& attr, const char *value);
	void AddAttr(const SOAPQName& attr, const SOAPString &value);
	void AddAttr(const SOAPQName& attr, const SOAPQName& value);

	void AddXMLNS(const char *prefix, const char *ns);
	void AddXMLNS(const SOAPString& prefix, const SOAPString& ns);

	void EndTag(const char *tag);
	void EndTag(const SOAPString& tag);
	void EndTag(const SOAPQName& tag);

	void WriteValue(const char *val);

	static void SetAddWhiteSpace(bool ws = true);
	static void SetBufferResults(bool use_buffer = true);
	static bool GetBufferResults();

private:
	XMLComposer(const XMLComposer&);
	XMLComposer& operator=(const XMLComposer&);

	void GetSymbol(SOAPString& symbol, const SOAPString& prefix);
	void EndStart();
	void Resize(void);
	void Write(const char *str);
	void WriteEscaped(const char *str);
	void Write(const char *str, unsigned int len);

	void PushLevel();
	void PopLevel();

	class NamespaceInfo {
	public:
		NamespaceInfo() : level(0) {}
		SOAPString		prefix;
		SOAPString		value;
		unsigned int	level;
	};

	typedef std::map<SOAPString, NamespaceInfo> NamespaceMap;
	typedef std::list<NamespaceInfo *> NamespaceArray;

	bool			m_instart;
    XMLComposerOutput& m_output;
    XMLComposerBuffer m_buffer;
    XMLComposerStdout m_stdout;
	unsigned int	m_gensym;
	unsigned int	m_level;
	NamespaceMap	m_nsmap;
	NamespaceArray	m_nsarray;

	static bool		g_makePretty;
	static bool		g_bufferResults;
};
typedef boost::shared_ptr<XMLComposer> XMLComposerPtr;

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_XMLCOMPOSER_H__DB61E902_B4A0_4AA3_A0F7_479D6295BD06__INCLUDED_)

