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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/SOAPDebugger.h#1 $
 */


#if !defined(AFX_SOAPDEBUGGER_H__2ED6E429_A5CB_4FEB_B1B1_19E83A2F8046__INCLUDED_)
#define AFX_SOAPDEBUGGER_H__2ED6E429_A5CB_4FEB_B1B1_19E83A2F8046__INCLUDED_

#include <stdio.h>
#include <syslog.h>
#include <easysoap/SOAP.h>

BEGIN_EASYSOAP_NAMESPACE

/**
*
*/
class EASYSOAP_EXPORT SOAPDebugger
{
public:
	//
	// Returns true if we were able to open
	// the file for writing.
	static bool SetFile(const char *fname);
	static void SetLogComponent(const char *component);
	static void SetLogFacility(int log_facility = LOG_LOCAL0);

	static void SetMessageLevel(int level);
	static void Close();
	static void Write(int level, const char *bytes, size_t len);
	static void Print(int level, const char *str, ...);

	static void OpenLog(void);
	static void SysLog(const char *str, ...);
	static void CloseLog(void);

private:
	SOAPDebugger();
	//~SOAPDebugger();

	static FILE	*m_file;
	static int	m_messageLevel;
	static bool	m_syslog;
	static SOAPString m_logComponent;
	static int  m_logFacility;
};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPDEBUGGER_H__2ED6E429_A5CB_4FEB_B1B1_19E83A2F8046__INCLUDED_)

