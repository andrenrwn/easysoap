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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPDebugger.cpp#1 $
 */


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <string>
#include <easysoap/SOAPDebugger.h>

USING_EASYSOAP_NAMESPACE

FILE *SOAPDebugger::m_file = 0;
int SOAPDebugger::m_messageLevel = 1;
bool SOAPDebugger::m_syslog = 0;
int SOAPDebugger::m_logFacility = LOG_LOCAL0;
SOAPString SOAPDebugger::m_logComponent = "icontrol_portal";

void
SOAPDebugger::SetMessageLevel(int level)
{
	m_messageLevel = level;
}

void
SOAPDebugger::Write(int level, const char *bytes, size_t len)
{
	if (level <= m_messageLevel )
	{
		if ( m_file )
		{
			fwrite((void *)bytes, 1, len, m_file);
			fflush(m_file);
		}
		else if ( m_syslog )
		{
			SOAPString logMessage;
			if ( !m_logComponent.IsEmpty() )
			{
				logMessage = m_logComponent;
				logMessage += ":";
			}
			logMessage.Append(bytes, len);

			syslog(LOG_DEBUG, (const char *)logMessage, logMessage.Length());
		}
	}
}

void
SOAPDebugger::Print(int level, const char *str, ...)
{
	if (level <= m_messageLevel )
	{
		if ( m_file )
		{
			va_list ap;
			va_start(ap, str);
			vfprintf(m_file, str, ap);
			va_end(ap);
			fflush(m_file);
		}
		else if ( m_syslog )
		{
			SOAPString logMessage;
			if ( !m_logComponent.IsEmpty() )
			{
				logMessage = m_logComponent;
				logMessage += ":";
			}
			logMessage += str;

			va_list ap;
			va_start(ap, (const char *)str);
			vsyslog(LOG_DEBUG, (const char *)logMessage, (char *)ap);
			va_end(ap);
		}
	}
}

void
SOAPDebugger::Close()
{
	if (m_file)
	{
		fwrite((void *)"\r\n", 1, 2, m_file);
		fclose(m_file);
		m_file = 0;
	}
	else if ( m_syslog )
	{
		CloseLog();
	}
}

bool
SOAPDebugger::SetFile(const char *name)
{
	Close();
	m_file = fopen(name, "ab");
	return (m_file != 0);
}

void
SOAPDebugger::SetLogComponent(const char *component)
{
	if ( (NULL != component) && ('\0' != *component) )
	{
		m_logComponent = component;
	}
	else
	{
		m_logComponent.Empty();
	}
}

void
SOAPDebugger::SetLogFacility(int log_facility /* = LOG_LOCAL0 */)
{
	m_logFacility = log_facility;
	OpenLog();
}

void
SOAPDebugger::OpenLog(void)
{
	CloseLog();
	openlog(NULL, LOG_NDELAY|LOG_PID, m_logFacility);
	m_syslog = 1;
}

void
SOAPDebugger::SysLog(const char *str, ...)
{
	SOAPString logMessage;
	va_list lst;
	if ( !m_logComponent.IsEmpty() )
	{
		logMessage = m_logComponent;
		logMessage += ":";
	}
	logMessage += str;

	va_start(lst, (const char *)str);
	vsyslog(LOG_DEBUG, (const char *)logMessage, (char *)lst);
	va_end(lst);
}

void
SOAPDebugger::CloseLog(void)
{
	closelog();
	m_syslog = 0;
}
