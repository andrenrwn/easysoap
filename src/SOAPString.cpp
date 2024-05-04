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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPString.cpp#1 $
 */

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPString.h>

#include <string.h>

USING_EASYSOAP_NAMESPACE

const std::string &SOAPString::empty_string(void)
{
    static const std::string s;
    return s;
}
const std::string &SOAPString::null_string(void)
{
    static const std::string s("\0");
    return s;
}

SOAPString::SOAPString(const char *str)
{
    if (str != NULL)
    {
        Assign(str);
    }
    else
    {
        m_str = null_string();
    }
}

SOAPString::SOAPString(const char *str, int len) :
    m_str(str, len)
{
}

SOAPString::SOAPString(const SOAPString& str)
{
    Assign(str);
}

SOAPString::SOAPString(const std::string& str)
{
    Assign(str);
}

SOAPString::~SOAPString()
{
}

void
SOAPString::Assign(const char* str)
{
    if (str != NULL)
    {
        unnullify();
        m_str = str;
    }
    else
    {
        nullify();
    }
}

void
SOAPString::Assign(const SOAPString& str)
{
    m_str = str.m_str;
}

void
SOAPString::Assign(const std::string& str)
{
    m_str = str;
}

SOAPString&
SOAPString::Append(const char *str, size_t len)
{
    unnullify();
    m_str.append(str, len);
    return *this;
}

void
SOAPString::Reserve(size_t size)
{
    unnullify();
    m_str.reserve(size);
}

void
SOAPString::Length(int length)
{
    unnullify();
    m_str.resize(length);
}

void
SOAPString::Empty(void)
{
    m_str = empty_string();
}

void
SOAPString::Clear(void)
{
    m_str = null_string(); 
}
