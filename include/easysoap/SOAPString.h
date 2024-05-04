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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/SOAPString.h#1 $
 */


#ifndef __SOAPSTRING_H__
#define __SOAPSTRING_H__

#ifdef HAVE_WCHAR_H
#include <wchar.h>
#define HAVE_WCHART
#else
#ifdef STDDEF_HAS_WCHART
#include <stddef.h>
#define HAVE_WCHART
#endif
#endif


#include <string>

#include <easysoap/SOAP.h>
#include <easysoap/SOAPUtil.h>

BEGIN_EASYSOAP_NAMESPACE

/**
*
*/
class EASYSOAP_EXPORT SOAPString
{
private:
	std::string m_str;
    static const std::string &empty_string(void);
    static const std::string &null_string(void);

    bool isNull(void) const { return m_str == null_string(); }
    void unnullify(void)
    {
        if (isNull()) {
            m_str = empty_string();
        }
    }
    void nullify(void)
    {
        if (!isNull()) {
            m_str = null_string();
        }
    }

	void Assign(const char* str);
	void Assign(const SOAPString& str);
	void Assign(const std::string& str);
    
public:
	// Default constructor
	SOAPString(const char *str = NULL);
	SOAPString(const char *str, int len);
	SOAPString(const SOAPString& str);
	SOAPString(const std::string& str);

#ifdef HAVE_WCHART
	SOAPString(const wchar_t *str)
	{
		Assign(str);
	}
#endif // HAVE_WCHART

	// Destructor
	~SOAPString();

	// Assignment operator
	SOAPString& operator=(const SOAPString& str)
	{
		Assign(str);
		return *this;
	}

	SOAPString& operator=(const std::string& str)
	{
		Assign(str);
		return *this;
	}

	SOAPString& operator=(const char *str)
	{
		Assign(str);
		return *this;
	}
	
	SOAPString& Append(const char *str)
	{
		return Append(str, sp_strlen(str));
	}

	SOAPString& Append(const char *str, size_t len);

	SOAPString& operator+=(const char *str)
	{
		return Append(str);
	}

	void Reserve(size_t size);
	void Empty(void);
    void Clear(void);

	const char *Str() const
	{
		return isNull() ? NULL : m_str.c_str();
	}

    // This function returns a C++ string and is 
    // safer than the Str() alternative, since 
    // the other version can possibly return a 
    // NULL pointer.
    const std::string& cppStr() const
    {
        return m_str;
    }

	bool operator!() const
	{
		return isNull();
	}

	operator bool() const
	{
		return !isNull();
	}

	operator const char *() const
	{
		return Str();
	}

    operator const std::string &() const
    {
        return isNull() ? empty_string() : m_str;
    }

	size_t Length() const
	{
        return isNull() ? 0 : m_str.size();
	}

	void Length(int length);

	bool IsEmpty() const
	{
		return isNull() || m_str.empty();
	}

	int Compare(const char *str) const
	{
		return sp_strcmp(Str(), str);
	}

	bool operator==(const SOAPString& str) const
	{
		return Compare(str) == 0;
	}

	bool operator==(const char *str) const
	{
		return Compare(str) == 0;
	}

	bool operator!=(const SOAPString& str) const
	{
		return Compare(str) != 0;
	}

	bool operator!=(const char *str) const
	{
		return Compare(str) != 0;
	}

	bool operator<(const SOAPString& str) const
	{
		return Compare(str) < 0;
	}

	bool operator<(const char *str) const
	{
		return Compare(str) < 0;
	}

#ifdef HAVE_WCHART
	void Assign(const wchar_t *str)
	{
		Assign("");
		Append(str);
	}

	void Append(const wchar_t *str)
	{
		if (str)
		{
			char buffer[1024];
			char *b = buffer;
			const char *const end = b + 1000;
			while (*str)
			{
				int c = 0;
#ifdef _WIN32
				sp_UTF16_UCS(str, c);
#else
				c = *str++;
#endif
				sp_UCS_UTF8(c, b);

				if (b >= end)
				{
					*b = 0;
					Append(buffer);
					b = buffer;
				}
			}
			*b = 0;
			Append(buffer);
		}
	}

	SOAPString& operator=(const wchar_t *str)
	{
		Assign(str);
		return *this;
	}

	SOAPString& operator+=(const wchar_t *str)
	{
		Append(str);
		return *this;
	}
#endif // HAVE_WCHART
};

#ifdef __SOAPHASHMAP_H__

/**
*
*/
template <>
struct SOAPHashCodeFunctor<SOAPString>
{
	size_t operator()(const SOAPString& str) const
	{
		return sp_hashcode(str);
	}

	size_t operator()(const char *str) const
	{
		return sp_hashcode(str);
	}
};

/**
*
*/
template <>
struct SOAPHashCodeFunctorNoCase<SOAPString>
{
	size_t operator()(const SOAPString& str) const
	{
		return sp_hashcodecase(str);
	}

	size_t operator()(const char *str) const
	{
		return sp_hashcodecase(str);
	}
};

/**
*
*/
template <>
struct SOAPEqualsFunctor<SOAPString>
{
	bool operator()(const SOAPString& a, const SOAPString& b) const
	{
		return a == b;
	}

	bool operator()(const SOAPString& a, const char *b) const
	{
		return a == b;
	}
};

/**
*
*/
template <>
struct SOAPEqualsFunctorNoCase<SOAPString>
{
	bool operator()(const SOAPString& a, const SOAPString& b) const
	{
		return sp_strcasecmp(a, b) == 0;
	}

	bool operator()(const SOAPString& a, const char *b) const
	{
		return sp_strcasecmp(a, b) == 0;
	}
};

#endif // __SOAPHASHMAP_H__

END_EASYSOAP_NAMESPACE

#endif // __SOAPSTRING_H__

