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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPUrl.cpp#1 $
 */


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPUrl.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USING_EASYSOAP_NAMESPACE

static const unsigned int ftp_default_port = 21;
static const unsigned int http_default_port = 80;
static const unsigned int https_default_port = 443;

void
SOAPUrl::Reset()
{
    m_proto = no_proto;

    m_user = "";
    m_password = "";
    m_hostname = "";

    m_port = 0;

    m_path = "";
}

void
SOAPUrl::Set(const char *url)
{
    //
    // urls are of the form:
    // proto://[user[:password]@]hostname[:port]/[path]
    //

    const char * const keepurl = url;

    Reset();
    if (url && *url)
    {
        char buffer[1024];
        char *work;

        const char * const workend = buffer + 1023;
        const char * const urlend = url + sp_strlen(url);

        // parse out the protocol name
        work = buffer;
        while (url != urlend && *url != ':' && *url != '/' && work != workend)
            *work++ = *url++;
        *work = 0;

        // found protocol name
        if (*url++ == ':')
        {
            if (!sp_strcasecmp(buffer, "http"))
            {
                m_proto = http_proto;
                m_port = http_default_port;
            }
            else if (!sp_strcasecmp(buffer, "https"))
            {
                m_proto = https_proto;
                m_port = https_default_port;
            }
            else if (!sp_strcasecmp(buffer, "ftp"))
            {
                m_proto = ftp_proto;
                m_port = ftp_default_port;
            }
            else if (!sp_strcasecmp(buffer, "file"))
            {
                m_proto = file_proto;
                m_port = 0;
            }
            else
            {
                m_proto = unknown_proto;
                throw SOAPException("Unknown protocol: %s", buffer);
            }
            m_base = buffer;
            m_base.Append("://");
        }
        else
        {
            throw SOAPException("Cannot parse url, expecting ':' after protocol name: %s", keepurl);
        }

        // pass over the '//' part
        if (*url++ != '/' || *url++ != '/')
            throw SOAPException("Cannot parse url, expecting '://' after protocol name: %s", keepurl);

        // parse out the [user[:password]@]host[:port] part

        // the last ampersand is the one separating the user[:password] part,
        // and the host[:port] part
        char *lastAmpersand = sp_strrchr (url, '@');
        char *colon = 0;
        work = buffer;
        if (lastAmpersand)
        {
            while (url != urlend && url < lastAmpersand && work != workend)
            {
                if (*url == ':' && !colon)
                    colon = work;

                *work++ = *url++;
            }
            *work = 0;
        }

        if (*url == '@')
        {
            // we have 'user:password' part
            m_base.Append(buffer);
            m_base.Append("@");
            ++url;

            if (colon)
            {
                // we have both a user and a password;
                *colon++ = 0;
                m_password = colon;
            }
            m_user = buffer;
        }

        // parse through the rest of the url, the 'hostname:port' part

        //
        // RFC2732 specifies that IPv6 addresses within URLs must be
        // contained within brackets.  We attempt to handle that here.
        // We also attempt to deal with the legacy formats that EasySoap
        // has previously been supporting.
        //
        
        colon = 0;
        work = buffer;

        bool isIpv6Addr = false;
        bool foundEndingBracket = false;

        if ((url != urlend) && (*url == '[')) {
            isIpv6Addr = true;
            url++;
        }

        while (url != urlend && *url != '/' && work != workend)
        {
                    // as we find more colons, as is the case in IPv6 addresses,
                    // adjust the colon pointer since we only want to track the
                    // last colon before the port number. This is due to the fact
                    // that the format of the hostname:port could be:
                    // - hostname:port
                    // - 10.10.10.1:port
                    // - ::ffff:10.10.10.1:port
                    // - 2001:db8::10:0:1:235:port
            //
            // RFC2732 specifies that IPv6 addresses within URLs must be
            // contained within brackets.  We reset the colon pointer if
            // we see a closing bracket so that we can handle this format
            // properly.  This should allow us to handle the previous "broken"
            // format as well (as long as a port has been specified).
            //
            if (*url == ']') {
                if ((!isIpv6Addr) || foundEndingBracket) {
                    throw SOAPException("Unexpected character in url: %s",
                                        keepurl);
                }
                colon = 0;
                foundEndingBracket = true;
                url++;
            }
            else {
                if (*url == ':') {
                    colon = work;
                }
                
                *work++ = *url++;
            }
        }
        *work = 0;

        if (*url == '/' || *url == 0)
        {
            if (isIpv6Addr && (!foundEndingBracket)) {
                throw SOAPException("Invalid IPv6 address in url: %s", keepurl);
            }

            // we have 'hostname:port' part
            m_base.Append(buffer);
            m_base.Append("/");
            if (colon)
            {
                // we have both a hostname and a port;
                *colon++ = 0;
                m_port = atoi(colon);
            }
            m_hostname = buffer;
        }
        else
        {
            throw SOAPException("Could not parse url: %s", keepurl);
        }

        if (*url)
            m_path = url;
        else
            m_path = "/";

        m_url = keepurl;

    }
}

bool
SOAPUrl::PortIsDefault() const
{
    switch (m_proto)
    {
    case ftp_proto: return m_port == ftp_default_port;
    case http_proto: return m_port == http_default_port;
    case https_proto: return m_port == https_default_port;
    default: return false;
    }
}

