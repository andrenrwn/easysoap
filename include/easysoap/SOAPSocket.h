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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/SOAPSocket.h#1 $
 */


#if !defined(AFX_SIMPLESOCKET_H__10CE9068_BA2E_4BE4_8AE2_D48D2FCA40AF__INCLUDED_)
#define AFX_SIMPLESOCKET_H__10CE9068_BA2E_4BE4_8AE2_D48D2FCA40AF__INCLUDED_

#include <easysoap/SOAP.h>
#include <easysoap/SOAPUrl.h>
#include <easysoap/SOAPException.h>

BEGIN_EASYSOAP_NAMESPACE

/**
*
*/
class SOAPSocketInterface
{
public:
	virtual ~SOAPSocketInterface() {}

    //
    // F5 Note: 
    //
    // At this level (and only at this level), a 0 value is
    // interpreted as a poll operation, returning immediately.  In
    // addition, a -1 value is interpreted as a blocking operation.
    //
    // At higher levels, 0 is interpreted as a block operation and -1
    // is interpreted as "almost" block (waiting for UINT_MAX
    // seconds).  Before calling the low level Wait methods, the
    // caller must convert between the two interfaces.  In general,
    // this will involve either skipping the Wait function and
    // immediately calling a blocking Read or Write or converting the
    // 0 to -1 before calling the Wait functions.
    //
	virtual bool WaitRead(int sec = -1, int usec = 0) = 0;
	virtual bool WaitWrite(int sec = -1, int usec = 0) = 0;
	virtual bool IsOpen() = 0;
	virtual void Close() = 0;
	virtual bool Connect(const char *host, unsigned int port) = 0;
	virtual size_t Read(char *buffer, size_t len) = 0;
	virtual size_t Write(const char *buffer, size_t len) = 0;

        virtual void  SetTimeout(size_t timeout) { m_timeout = timeout; }
        virtual size_t  GetTimeout() { return m_timeout; }

protected:
	SOAPSocketInterface() {}
	size_t		m_timeout;
};

/**
*
*/
class EASYSOAP_EXPORT SOAPProtocolBase
{
private:

	char		m_buffer[1024];
	char		m_wbuff[1460];

	SOAPSocketInterface	*m_socket;
	const char	*m_buff;
	const char	*m_buffend;
	char		*m_wpos;
	const char  *m_wend;

	bool	Readbuff();

	SOAPProtocolBase(const SOAPProtocolBase&);
	SOAPProtocolBase& operator=(const SOAPProtocolBase&);

protected:
	SOAPProtocolBase();
	void	Flush();
	size_t		m_timeout;

public:
	virtual ~SOAPProtocolBase();

	void SetSocket(SOAPSocketInterface *socket);

	virtual bool CanRead() {return IsOpen() && (m_buff != m_buffend || m_socket->WaitRead(0));}
	virtual bool IsOpen() {return m_socket && m_socket->IsOpen();}
	virtual void Close();
	virtual bool Connect(const char *host, unsigned int port);
	virtual void SetTimeout(size_t secs) {m_timeout = secs;}
	virtual size_t GetTimeout(size_t secs) {return m_timeout;}

	// read up to len chars
	virtual size_t Read(char *buffer, size_t len);

	// reads up to \r\n but doesn't add it to string
	virtual size_t ReadLine(char *buff, size_t bufflen);

	// write up to terminating null
	virtual size_t Write(const char *buffer);

	// write len chars
	virtual size_t Write(const char *buffer, size_t len);

	// write string plus \r\n
	virtual size_t WriteLine(const char *str = 0);
};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SIMPLESOCKET_H__10CE9068_BA2E_4BE4_8AE2_D48D2FCA40AF__INCLUDED_)

