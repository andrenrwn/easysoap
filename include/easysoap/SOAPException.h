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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/SOAPException.h#1 $
 */


#if !defined(AFX_SOAPEXCEPTION_H__18828772_7674_41DA_A39A_14D5CCB77E75__INCLUDED_)
#define AFX_SOAPEXCEPTION_H__18828772_7674_41DA_A39A_14D5CCB77E75__INCLUDED_

#if defined(_MSC_VER)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif // defined(_MSC_VER)

#include <stdio.h>
#include <stdarg.h>

#include <easysoap/SOAPString.h>

BEGIN_EASYSOAP_NAMESPACE

/**
* SOAPExceptions have a design flaw in that they take a formatted string and
* variadic inputs to create the class. When creating a SOAPException with a
* a possibly user inputted string, the caller MUST call the constructor like
* this: SOAPException("%s", <string with potential user input>)
* This applies to subclasses of SOAPExceptions as well.
*/
class EASYSOAP_EXPORT SOAPException  
{
protected:
	SOAPString	 m_what;

	SOAPException() {}

	void FormattedMessage(const char *fmt, va_list args)
	{
		char buffer[2048];
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		m_what = buffer;
	}

public:
	SOAPException(const SOAPString& what)
		: m_what(what) {}
	SOAPException(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		FormattedMessage(fmt, args);
		va_end(args);
	}

	virtual ~SOAPException() {}

	const SOAPString& What() const {return m_what;}
};

/**
*
*/
class EASYSOAP_EXPORT SOAPMemoryException : public SOAPException
{
public:
	SOAPMemoryException() {}
	~SOAPMemoryException() {}
};

/**
*
*/
class EASYSOAP_EXPORT SOAPSocketException : public SOAPException
{
public:
    enum Reason
    {
        UNKNOWN,
        BAD_ADDRESS,
        BAD_PROTOCOL,
        NO_SOCKET,
        BIND_FAILED,
        CONNECT_TIMED_OUT,
        CONNECT_FAILED,
        PEER_UNREACHABLE,
        CONNECTION_REFUSED,
        READ_FAILED,
        WRITE_FAILED,
        PROTOCOL_ERROR
    };
    
	SOAPSocketException(Reason reason, const SOAPString& what)
		: SOAPException(what), m_reason(reason) {}
	SOAPSocketException(Reason reason, const char *fmt, ...)
        : m_reason(reason)
	{
		va_list args;
		va_start(args, fmt);
		FormattedMessage(fmt, args);
		va_end(args);
	}
	~SOAPSocketException() {}

    Reason getReason(void) { return m_reason; }

    private:
        Reason m_reason;
};

class EASYSOAP_EXPORT SOAPSSLException : public SOAPException
{
public:
	SOAPSSLException(const SOAPString& what)
		: SOAPException(what) {}
	SOAPSSLException(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		FormattedMessage(fmt, args);
		va_end(args);
	}
	~SOAPSSLException() {}
};



END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPEXCEPTION_H__18828772_7674_41DA_A39A_14D5CCB77E75__INCLUDED_)

