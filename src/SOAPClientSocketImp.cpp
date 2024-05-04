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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPClientSocketImp.cpp#1 $
 */


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <fcntl.h>
#include <easysoap/SOAPDebugger.h>

#include "SOAPClientSocketImp.h"

#if defined (_WIN32)
#if !defined (__MWERKS__)
#include <winsock2.h>
#endif

//
// Initialize Windows socket library
//
static class WinSockInit
{
private:
	bool didinit;
public:
	WinSockInit() : didinit(false)
	{
	}

	void Init()
	{
		if (!didinit)
		{
			WSADATA wsaData;
			WSAStartup( MAKEWORD(2,0), &wsaData );
			didinit = true;
		}
	}

	~WinSockInit()
	{
		if (didinit)
			WSACleanup();
	}
} __winsockinit;

#else // not _WIN32

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#endif

#ifdef HAVE_RESOLV_H
#include <resolv.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#define HAVE_POLL_H
#ifdef HAVE_POLL_H
#include <sys/poll.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#else
#ifdef HAVE_SYS_UNISTD_H
#include <sys/unistd.h>
#endif
#endif

#define INVALID_SOCKET 0xFFFFFFFF
#define SOCKET_ERROR -1
#define closesocket close

#endif // _WIN32

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USING_EASYSOAP_NAMESPACE

SOAPClientSocketImp::SOAPClientSocketImp()
: m_socket(INVALID_SOCKET), m_isNonBlocking(false)
{
#if defined (_WIN32)
	__winsockinit.Init();
#endif
	SetTimeout(60);
}

SOAPClientSocketImp::~SOAPClientSocketImp()
{
	Close();
}

void
SOAPClientSocketImp::Close()
{
	SOAPDebugger::Print(5, "SOAPClientSocketImp::Close()\r\n");
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
	}

	m_socket = INVALID_SOCKET;
}


void
SOAPClientSocketImp::Wait(int sec, int usec)
{
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	select(0, 0, 0, 0, sec == -1 ? 0 : &tv);
}

bool
SOAPClientSocketImp::WaitRead(int sec, int usec)
{
#ifndef HAVE_POLL_H
	struct timeval tv;
	fd_set rset, eset;

	FD_ZERO(&rset);
	FD_SET(m_socket, &rset);
	FD_ZERO(&eset);
	FD_SET(m_socket, &eset);

	tv.tv_sec = sec;
	tv.tv_usec = usec;

	int ret = select(m_socket+1, &rset, 0, &eset, sec == -1 ? 0 : &tv);
	int rsetresult = FD_ISSET(m_socket, &rset);
	int esetresult = FD_ISSET(m_socket, &eset);
	SOAPDebugger::Print(3, "read select() return: %d\n", ret);
	SOAPDebugger::Print(4, "write select() wset: %d\n", rsetresult);
	SOAPDebugger::Print(4, "write select() eset: %d\n", esetresult);
	if (ret == (int)SOCKET_ERROR)
		throw SOAPException("WaitRead select error");

	return rsetresult != 0;
#else
	struct pollfd pfd = {0};
	pfd.fd = m_socket;
	pfd.events = POLLIN;
                                                                                                                 
	int ret = poll(&pfd, 1, sec * 1000 + (usec / 1000));
                                                                                                                 
	if (ret == (int)SOCKET_ERROR)
		throw SOAPException("WaitRead poll error");
                                                                                                                 
	return (pfd.revents & POLLIN);
#endif

}

bool
SOAPClientSocketImp::WaitWrite(int sec, int usec)
{
#ifndef HAVE_POLL_H
	struct timeval tv;
	fd_set wset, eset;

	FD_ZERO(&eset);
	FD_SET(m_socket, &eset);
	FD_ZERO(&wset);
	FD_SET(m_socket, &wset);

	tv.tv_sec = sec;
	tv.tv_usec = usec;

	int ret = select(m_socket+1, 0, &wset, &eset, sec == -1 ? 0 : &tv);
	int wsetresult = FD_ISSET(m_socket, &wset);
	int esetresult = FD_ISSET(m_socket, &eset);
	SOAPDebugger::Print(3, "write select() return: %d\n", ret);
	SOAPDebugger::Print(4, "write select() wset: %d\n", wsetresult);
	SOAPDebugger::Print(4, "write select() eset: %d\n", esetresult);
	if (ret == (int)SOCKET_ERROR)
		throw SOAPException("WaitWrite select error");

	return wsetresult != 0;
#else
	struct pollfd pfd = {0};
	pfd.fd = m_socket;
	pfd.events = POLLOUT;
                                                                                                                 
	int ret = poll(&pfd, 1, sec * 1000 + (usec / 1000));
                                                                                                                 
	if (ret == (int)SOCKET_ERROR)
		throw SOAPException("WaitWrite poll error");
                                                                                                                 
	return (pfd.revents & POLLOUT);
#endif
}

bool
SOAPClientSocketImp::WaitReadWrite(int sec, int usec)
{
#ifndef HAVE_POLL_H
	struct timeval tv;
	fd_set rset, wset, eset;

	FD_ZERO(&eset);
	FD_SET(m_socket, &eset);
	FD_ZERO(&rset);
	FD_SET(m_socket, &rset);
	FD_ZERO(&wset);
	FD_SET(m_socket, &wset);

	tv.tv_sec = sec;
	tv.tv_usec = usec;

	int ret = select(m_socket+1, &rset, &wset, &eset, sec == -1 ? 0 : &tv);
	int rsetresult = FD_ISSET(m_socket, &rset);
	int wsetresult = FD_ISSET(m_socket, &wset);
	int esetresult = FD_ISSET(m_socket, &eset);
	SOAPDebugger::Print(3, "write select() return: %d\n", ret);
	SOAPDebugger::Print(4, "write select() rset: %d\n", rsetresult);
	SOAPDebugger::Print(4, "write select() wset: %d\n", wsetresult);
	SOAPDebugger::Print(4, "write select() eset: %d\n", esetresult);
	if (ret == (int)SOCKET_ERROR)
		throw SOAPException("WaitReadWrite select error");

	return ((wsetresult != 0) || (rsetresult != 0));
#else
	struct pollfd pfd = {0};
	pfd.fd = m_socket;
	pfd.events = POLLIN | POLLOUT | POLLERR;

	int ret = poll(&pfd, 1, sec * 1000 + (usec / 1000));
                                                                                                                 
	if (ret == (int)SOCKET_ERROR)
		throw SOAPException("WaitReadWrite poll error");
                                                                                                                 
	return ((pfd.revents & POLLIN)
                 || (pfd.revents & POLLOUT)
                 || (pfd.revents & POLLERR));
#endif
}

bool
SOAPClientSocketImp::IsOpen()
{
	return m_socket != INVALID_SOCKET;
}


bool
SOAPClientSocketImp::Connect(const char *server, unsigned int port)
{
	SOAPDebugger::Print(5, "SOAPClientSocketImp::Connect()\r\n");
	Close();

	int gaiStatus = 0;
	bool isIPv6 = false;

	struct addrinfo *addrInfo = NULL;
	if ((gaiStatus = getaddrinfo (server, NULL, NULL, &addrInfo)) == 0 &&
		addrInfo != NULL)
	{
		if (addrInfo->ai_family == PF_INET6)
			isIPv6 = true;
		else
			isIPv6 = false;
	}
	else
	{
#ifdef HAVE_STRERROR
		throw SOAPSocketException(SOAPSocketException::BAD_ADDRESS,
                "Error calling getaddrinfo for %s (%s)", server, gai_strerror(gaiStatus));
#else
		throw SOAPSocketException(SOAPSocketException::BAD_ADDRESS,
                "Error calling getaddrinfo, status = %d", gaiStatus);
#endif
	}

	//
	// TODO: Abstract this away into other class/methods
	//
	m_socket = 0;
	m_socket = socket(addrInfo->ai_family, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		freeaddrinfo (addrInfo);
#ifdef HAVE_STRERROR_R
		char errBuff[256];
		throw SOAPSocketException(SOAPSocketException::NO_SOCKET,
                "Error creating socket: %s", 
                strerror_r(errno, errBuff, sizeof(errBuff)));
#else
#ifdef HAVE_STRERROR
		throw SOAPSocketException(SOAPSocketException::NO_SOCKET,
                "Error creating socket: %s", strerror(errno));
#else
		throw SOAPSocketException(SOAPSocketException::NO_SOCKET,
                "Error creating socket, errno = %d", errno);
#endif
#endif
	}

	//
	// We need to set a socket timeout here because connections may
	// hang forever (the raw socket, by default, will not timeout).
	//

	if (m_timeout != 0) {
	    SetTimeoutSockOpts();
	}

	struct sockaddr_in sockAddr;
	struct sockaddr_in6 sockAddr6;

	sp_memset(&sockAddr, 0, sizeof(sockAddr));
	sp_memset(&sockAddr6, 0, sizeof(sockAddr6));

	if (isIPv6)
	{
		sockAddr6.sin6_family = AF_INET6;
	}
	else
	{
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

        /* F5 Note: There was a bind() call here, but we deemed it
           unecessary and harmful, so we removed it.  It was using
           INADDR_ANY and 0 at any rate, which should be the default.
           The bind should not be here between the socket() and
           connect(); we're a client and not caring whether we look
           like we are coming from a particular address and port.  The
           bind was getting an "address already in use" error in
           scenarios in which we have huge numbers of TIME_WAIT
           connections, for a customer, causing 3 pages in the GUI to
           fail with cryptic errors. */

	if (isIPv6)
	{
		sp_memset(&sockAddr6, 0, sizeof(sockAddr6));
		sockAddr6.sin6_family = AF_INET6;
		memcpy (&sockAddr6.sin6_addr, 
				&((struct sockaddr_in6 *)addrInfo->ai_addr)->sin6_addr,
				sizeof (struct in6_addr));
		sockAddr6.sin6_port = htons((u_short)port);
	}
	else
	{
		sp_memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = ((struct sockaddr_in *) addrInfo->ai_addr)->sin_addr.s_addr;
		sockAddr.sin_port = htons((u_short)port);
	}

	if (m_isNonBlocking) {
		int flags=fcntl(m_socket, F_GETFL, 0);
		fcntl(m_socket, F_SETFL, flags | O_NONBLOCK );
	}

	if (connect(m_socket,(isIPv6 ? (struct sockaddr*)&sockAddr6 : (struct sockaddr*)&sockAddr), 
						 (isIPv6 ? sizeof(sockAddr6) : sizeof(sockAddr))) == (int)SOCKET_ERROR)
	{

		int connecterror = SOCKET_ERROR;

		// If the socket is non blocking, then wait up
		// to the timeout period for socket activity.
		// If the socket is ready for action, try the connect once again.
             
		if (m_isNonBlocking && (errno == EINPROGRESS))
		{
			if (m_timeout > 0) {
				WaitReadWrite((int)m_timeout);
			}
			else {
				WaitReadWrite();
			}
			connecterror = connect(m_socket,
				(isIPv6 ? (struct sockaddr*)&sockAddr6 : (struct sockaddr*)&sockAddr), 
				(isIPv6 ? sizeof(sockAddr6) : sizeof(sockAddr)));
		}
		if (connecterror == SOCKET_ERROR) {

			SOAPSocketException::Reason reason;
			switch(errno)
			{
				case ECONNREFUSED:
					reason = SOAPSocketException::CONNECTION_REFUSED;
					break;
					 
				case ETIMEDOUT:
					reason = SOAPSocketException::CONNECT_TIMED_OUT;
					break;

				case ENETUNREACH:
					reason = SOAPSocketException::PEER_UNREACHABLE;
					break;

				default:
					reason = SOAPSocketException::CONNECT_FAILED;
					break;
			}
 
			Close();
			freeaddrinfo (addrInfo);

#ifdef HAVE_STRERROR_R
            char errBuff[256];
			throw SOAPSocketException(reason, "Failed to connect to host %s, port %d: %s", server, port, strerror_r(errno, errBuff, sizeof(errBuff)));
#else
#ifdef HAVE_STRERROR
			throw SOAPSocketException(reason, "Failed to connect to host %s, port %d: %s", server, port, strerror(errno));
#else
			throw SOAPSocketException(reason, "Failed to connect to host %s, port %d", server, port);
#endif
#endif
        }
	}

	int nodelay = 1;
	struct protoent *tcpproto = getprotobyname("tcp");
	if (!tcpproto)
	{
		//
		// Couldn't get the struct by name (/etc/protocols missing?)
		// So lets try by number.  TCP should always be 6, we hope...
		tcpproto = getprotobynumber(6);
		if (!tcpproto)
		{
			Close();
			freeaddrinfo (addrInfo);
			throw SOAPSocketException(SOAPSocketException::UNKNOWN, 
                    "Could not get TCP protocol struct.");
		}
	}

	if (setsockopt(m_socket, tcpproto->p_proto, TCP_NODELAY, (const char *)&nodelay, sizeof(nodelay)) == -1)
	{
		Close();
		freeaddrinfo (addrInfo);

#ifdef HAVE_STRERROR_R
		char errBuff[256];
		throw SOAPSocketException(SOAPSocketException::UNKNOWN,
                "Error setting TCP_NODELAY: %s",
                strerror_r(errno, errBuff, sizeof(errBuff)));
#else
#ifdef HAVE_STRERROR
		throw SOAPSocketException(SOAPSocketException::UNKNOWN,
                "Error setting TCP_NODELAY: %s", strerror(errno));
#else
		throw SOAPSocketException(SOAPSocketException::UNKNOWN,
                "Error setting TCP_NODELAY");
#endif
#endif
	}

    int keepalive = 1; /* Enable TCP keepalive probes */
    int keepidle  = 5; /* Start probes after 5 seconds */
    int keepintvl = 30; /* Probe every 30 seconds */
    int keepcnt   = 5; /* Close connection after 5 probes fail */
    if ((setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE,
                (char *)&keepalive, sizeof(keepalive)) < 0) ||
            (setsockopt(m_socket, IPPROTO_TCP, TCP_KEEPIDLE,
                (char *)&keepidle, sizeof(keepidle)) < 0) ||
            (setsockopt(m_socket, IPPROTO_TCP, TCP_KEEPINTVL,
                (char *)&keepintvl, sizeof(keepintvl)) < 0) ||
            (setsockopt(m_socket, IPPROTO_TCP, TCP_KEEPCNT,
                (char *)&keepcnt, sizeof(keepcnt)) < 0)) {
#ifdef HAVE_STRERROR
		throw SOAPSocketException(SOAPSocketException::UNKNOWN,
                "Error setting TCP keepalives: %s", strerror(errno));
#else
		throw SOAPSocketException(SOAPSocketException::UNKNOWN,
                "Error setting TCP keepalives");
#endif
    }

	freeaddrinfo (addrInfo);

	SOAPDebugger::Print(5, "SOAPClientSocketImp::Connect() successful\r\n");
	return true;
}

size_t
SOAPClientSocketImp::Read(char *buff, size_t bufflen)
{
	int bytes = 0;
	if (bufflen > 0)
	{
		*buff = 0;
		bytes = recv(m_socket, buff, bufflen, 0);
		SOAPDebugger::Print(2, "RECV: %d bytes\n", bytes);
		if (bytes == 0)
		{
			Close(); // other side dropped the connection
		}
		else if (bytes == (int)SOCKET_ERROR)
		{
			Close();
			throw SOAPSocketException(SOAPSocketException::READ_FAILED,
                    "Error reading socket");
		}
		SOAPDebugger::Write(1, buff, bytes);
	}
	return bytes;
}

size_t
SOAPClientSocketImp::Write(const char *buff, size_t bufflen)
{
	int bytes = 0;
	if (bufflen > 0)
	{
		bytes = send(m_socket, buff, bufflen, 0);
		SOAPDebugger::Print(2, "SEND: %d bytes\n", bytes);
		if (bytes == (int)SOCKET_ERROR)
		{
			Close();
			throw SOAPSocketException(SOAPSocketException::WRITE_FAILED,
                    "Error writing to socket");
		}
		else if (bytes != (int)bufflen)
		{
			Close();
			throw SOAPSocketException(SOAPSocketException::WRITE_FAILED,
                    "Error writing to socket, "
					"tried to write %d bytes, wrote %d",
					bufflen, bytes);
		}
		SOAPDebugger::Write(1, buff, bytes);
	}
	return bytes;
}

void
SOAPClientSocketImp::SetTimeout(size_t timeout)
{
    
    const size_t oldTimeout = SOAPSocketInterface::GetTimeout();

    SOAPSocketInterface::SetTimeout(timeout);

    //
    // We need to set a socket timeout here because connections may
    // hang forever (the raw socket, by default, will not timeout).
    //

    if (oldTimeout != timeout) {
        SetTimeoutSockOpts();
    }

} // SOAPClientSocketImp::SetTimeout

//
// F5 Note:
//
// The socket timeout is necessary to deal with calls to SSL_connect without
// hanging indefinitely.
//

void
SOAPClientSocketImp::SetTimeoutSockOpts()
{

    if (m_socket == INVALID_SOCKET) {
        return;
    }

    struct timeval tv;
    tv.tv_sec = m_timeout;
    tv.tv_usec = 0;
    
    if (::setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO,
                     (char*) &tv, sizeof(tv)) == -1) {
        std::string errorMessage = "Error setting SO_RCVTIMEO socket option";
#ifdef HAVE_STRERROR_R
        char errBuff[256];
        errorMessage += ": ";
        errorMessage += ::strerror_r(errno, errBuff, sizeof(errBuff));
#else
#ifdef HAVE_STRERROR
        errorMessage += ": ";
        errorMessage += ::strerror(errno);
#endif
#endif
        Close();
        throw SOAPSocketException(SOAPSocketException::UNKNOWN, errorMessage);
    }
 
    if (::setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO,
                     (char*) &tv, sizeof(tv)) == -1) {
        std::string errorMessage = "Error setting SO_SNDTIMEO socket option";
#ifdef HAVE_STRERROR_R
        char errBuff[256];
        errorMessage += ": ";
        errorMessage += ::strerror_r(errno, errBuff, sizeof(errBuff));
#else
#ifdef HAVE_STRERROR
        errorMessage += ": ";
        errorMessage += ::strerror(errno);
#endif
#endif
        Close();
        throw SOAPSocketException(SOAPSocketException::UNKNOWN, errorMessage);
    }

} // SOAPClientSockImp::SetTimeoutSockOpts


