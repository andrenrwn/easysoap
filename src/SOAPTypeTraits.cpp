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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPTypeTraits.cpp#1 $
 */

#include <easysoap/SOAP.h>
#include <easysoap/SOAPParameter.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_MATH_H
#include <math.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif

#ifdef HAVE_FLOAT_H
#include <float.h>
#endif

USING_EASYSOAP_NAMESPACE

static int
sp_strtol(const char *str)
{
    char *endptr = 0;
	const char *startptr = str;
	int ret;

    if (str == NULL)
    {
        throw SOAPException("Could not convert NULL to long");
    }
	errno = 0;
	ret	= strtol(startptr, &endptr, 10);

    if (endptr)
    {
        while (sp_isspace(*endptr))
            ++endptr;

        if (startptr == endptr || *endptr != 0)
            throw SOAPException("Could not convert string to integer: '%s'",
                    str);
    }

	if (errno == ERANGE)
		throw SOAPException("Integer %s: %s",
				(ret < 0) ? "underflow" : "overflow",
				str);

	return ret;
}

static unsigned long
sp_strtoul(const char *str)
{
    char *endptr = 0;
	const char *startptr = str;
	unsigned long ret;

    if (str == NULL)
    {
        throw SOAPException("Could not convert NULL to unsigned long");
    }
	errno = 0;
	ret	= strtoul(startptr, &endptr, 10);

    if (endptr)
    {
        while (sp_isspace(*endptr))
            ++endptr;

        if (startptr == endptr || *endptr != 0)
            throw SOAPException("Could not convert string to unsigned long: '%s'",
                    str);
    }

	if (errno == ERANGE)
		throw SOAPException("Integer %s: %s",
				(ret < 0) ? "underflow" : "overflow",
				str);

	return ret;
}


//
// TODO:  This is too tricky.  We need to find a
// portable and nice way to return NAN.
typedef union
{
	double d;
	struct
	{
		int i1;
		int i2;
	} s;
} fptricks;

static double
sp_strtod(const char *str)
{
    if (str == NULL)
    {
        throw SOAPException("Could not convert NULL to double");
    }
	if (sp_strcasecmp(str, "INF") == 0)
		return HUGE_VAL;
	else if (sp_strcasecmp(str, "-INF") == 0)
		return -HUGE_VAL;
	else if (sp_strcasecmp(str, "NaN") == 0)
	{
		fptricks t;
		t.s.i1 = 0xFFFFFFFF;
		t.s.i2 = 0xFFFFFFFF;
		return t.d;
	}

	char *endptr = 0;
	const char *startptr = str;
	double ret;

	errno = 0;
	ret = strtod(startptr, &endptr);

	if (endptr)
	{
		while (sp_isspace(*endptr))
			++endptr;

		if (startptr == endptr || *endptr != 0)
			throw SOAPException("Could not convert string to floating point: '%s'",
					startptr);
	}

	if (errno == ERANGE)
		throw SOAPException("Double floating-point %s: %s",
				(ret == 0.0) ? "underflow" : "overflow",
				(const char *)str);

	return ret;
}

//
//  Trait info for bool
void SOAPTypeTraits<bool>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::boolean;
}

SOAPParameter&
SOAPTypeTraits<bool>::Serialize(SOAPParameter& param, bool val)
{
	param.GetStringRef() = val ? SOAPParameter::m_true : SOAPParameter::m_false;
	return param;
}

SOAPParameter&
SOAPTypeTraits<bool>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<bool>::Deserialize(const SOAPParameter& param, bool& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to a boolean.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to a boolean.");

	if (str == "false" || str == "0")
		val = false;
	else if (str == "true" || str =="1")
		val = true;
	else
		throw SOAPException("Could not convert value to boolean: %s", (const char *)str);
	return param;
}

const SOAPParameter&
SOAPTypeTraits<bool>::Deserialize(const SOAPParameter& param, std::_Bit_reference val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to a boolean.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to a boolean.");

	if (str == "false" || str == "0")
		val = false;
	else if (str == "true" || str =="1")
		val = true;
	else
		throw SOAPException("Could not convert value to boolean: %s", (const char *)str);
	return param;
}

//
//  Trait info for char
void SOAPTypeTraits<char>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::byte;
}

SOAPParameter&
SOAPTypeTraits<char>::Serialize(SOAPParameter& param, char val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", (signed char)val);
	param.GetStringRef() = buffer;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<char>::Deserialize(const SOAPParameter& param, char& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to a byte.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to byte.");

	int v;
	val = v = sp_strtol(str);
	if (v != val)
		throw SOAPException("Value out of range for byte: %d", v);
	return param;
}


//
//  Trait info for short
void SOAPTypeTraits<short>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::short_;
}

SOAPParameter&
SOAPTypeTraits<short>::Serialize(SOAPParameter& param, short val)
{
	char buffer[64];
	sp_itoa(val, buffer);
	param.GetStringRef() = buffer;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<short>::Deserialize(const SOAPParameter& param, short& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to a short.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to short.");

	int v;
	val = v = sp_strtol(str);
	if (v != val)
		throw SOAPException("Value out of range for short: %d", v);
	return param;
}

//
//  Trait info for short
void SOAPTypeTraits<unsigned short>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::int_;
}

SOAPParameter&
SOAPTypeTraits<unsigned short>::Serialize(SOAPParameter& param, unsigned short val)
{
	char buffer[64];
	sp_itoa(val, buffer);
	param.GetStringRef() = buffer;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<unsigned short>::Deserialize(const SOAPParameter& param, unsigned short& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to a unsigned short.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to unsigned short.");

	int v;
	val = v = sp_strtol(str);
	if (v != val)
		throw SOAPException("Value out of range for unsigned short: %d", v);
	return param;
}

//
//  Trait info for int
void SOAPTypeTraits<int>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::int_;
}

SOAPParameter&
SOAPTypeTraits<int>::Serialize(SOAPParameter& param, int val)
{
	char buffer[64];
	sp_itoa(val, buffer);
	Serialize(param, buffer);
	return param;
}

SOAPParameter&
SOAPTypeTraits<int>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<int>::Deserialize(const SOAPParameter& param, int& val)
{
	const SOAPString& str = param.GetString();
    if (param.IsStruct())
        throw SOAPException("Cannot convert a struct to an integer.");

    if (param.IsNull() || str.IsEmpty())
        throw SOAPException("Cannot convert null value to integer.");

	val = sp_strtol(str);
	return param;
}


//
//  Trait info for float
void SOAPTypeTraits<float>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::float_;
}

SOAPParameter&
SOAPTypeTraits<float>::Serialize(SOAPParameter& param, float val)
{
    double dval = val;
    if (finite(dval))
    {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%.9G", dval);
        Serialize(param, buffer);
    }
    else
    {
        if (isnan(dval))
            Serialize(param, "NaN");
        else if (dval > 0.0)
            Serialize(param, "INF");
        else
            Serialize(param, "-INF");
    }
	return param;
}

SOAPParameter&
SOAPTypeTraits<float>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<float>::Deserialize(const SOAPParameter& param, float& val)
{
	if (param.IsStruct())
		throw SOAPException("Cannot convert a struct to a float.");

	if (param.IsNull())
		throw SOAPException("Cannot convert null value to float.");

	double dret = sp_strtod(param.GetStringRef());
	float ret = (float)dret;

	if (ret == HUGE_VAL && dret != HUGE_VAL ||
        ret == -HUGE_VAL && dret != -HUGE_VAL)
        throw SOAPException("Floating-point overflow: %s", (const char *)param.GetStringRef());
    if (ret == 0.0 && dret != 0.0)
        throw SOAPException("Floating-point underflow: %s", (const char *)param.GetStringRef());

	val = ret;
	return param;
}

//
//  Trait info for double

void SOAPTypeTraits<double>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::double_;
}

SOAPParameter&
SOAPTypeTraits<double>::Serialize(SOAPParameter& param, double val)
{
    if (finite(val))
    {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%.18G", val);
        Serialize(param, buffer);
    }
    else
    {
        if (isnan(val))
            Serialize(param, "NaN");
        else if (val > 0.0)
            Serialize(param, "+INF");
        else
            Serialize(param, "-INF");
    }
	return param;
}

SOAPParameter&
SOAPTypeTraits<double>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<double>::Deserialize(const SOAPParameter& param, double& val)
{
	if (param.IsStruct())
		throw SOAPException("Cannot convert a struct to a double.");

	if (param.IsNull())
		throw SOAPException("Cannot convert null value to double.");

	val = sp_strtod(param.GetStringRef());
	return param;
}

//
//  Trait info for long

void SOAPTypeTraits<long>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::long_;
}

SOAPParameter&
SOAPTypeTraits<long>::Serialize(SOAPParameter& param, long val)
{
	char buffer[64];
	sp_itoa(val, buffer);
	Serialize(param, buffer);
	return param;
}

SOAPParameter&
SOAPTypeTraits<long>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<long>::Deserialize(const SOAPParameter& param, long& val)
{
	if (param.IsStruct())
		throw SOAPException("Cannot convert a struct to a long.");

	if (param.IsNull())
		throw SOAPException("Cannot convert null value to long.");

	val = sp_strtol(param.GetStringRef());
	return param;
}

//
//  Trait info for unsigned long

void SOAPTypeTraits<unsigned long>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::unsignedLong;
}

SOAPParameter&
SOAPTypeTraits<unsigned long>::Serialize(SOAPParameter& param, unsigned long val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%lu", val);
	Serialize(param, buffer);
	return param;
}

SOAPParameter&
SOAPTypeTraits<unsigned long>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<unsigned long>::Deserialize(const SOAPParameter& param, unsigned long& val)
{
	if (param.IsStruct())
		throw SOAPException("Cannot convert a struct to a unsigned long.");

	if (param.IsNull())
		throw SOAPException("Cannot convert null value to unsigned long.");

	val = sp_strtoul(param.GetStringRef());
	return param;
}

//
//  Trait info for int64_t

void SOAPTypeTraits<int64_t>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::long_;
}

SOAPParameter&
SOAPTypeTraits<int64_t>::Serialize(SOAPParameter& param, int64_t val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%qd", val);
	Serialize(param, buffer);
	return param;
}

SOAPParameter&
SOAPTypeTraits<int64_t>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<int64_t>::Deserialize(const SOAPParameter& param, int64_t& val)
{
	if (param.IsStruct())
		throw SOAPException("Cannot convert a struct to a 64 bit integer.");

	if (param.IsNull())
		throw SOAPException("Cannot convert null value to 64 bit integer.");

	int numFields = sscanf(param.GetStringRef(), "%lld", &val);
	if ( 0 == numFields )
	{
		throw SOAPException("Could not convert string to 64 bit integer: '%s'", (const char *)param.GetStringRef());
	}
	return param;
}

//
//  Trait info for u_int64_t

void SOAPTypeTraits<u_int64_t>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::unsignedLong;
}

SOAPParameter&
SOAPTypeTraits<u_int64_t>::Serialize(SOAPParameter& param, u_int64_t val)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%qu", val);
	Serialize(param, buffer);
	return param;
}

SOAPParameter&
SOAPTypeTraits<u_int64_t>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<u_int64_t>::Deserialize(const SOAPParameter& param, u_int64_t& val)
{
	if (param.IsStruct())
		throw SOAPException("Cannot convert a struct to a 64 bit unsigned integer.");

	if (param.IsNull())
		throw SOAPException("Cannot convert null value to 64 bit unsigned integer.");

	int numFields = sscanf(param.GetStringRef(), "%llu", &val);
	if ( 0 == numFields )
	{
		throw SOAPException("Could not convert string to 64 bit unsigned integer: '%s'", (const char *)param.GetStringRef());
	}
	return param;
}

//
//  Trait info for const char *
void SOAPTypeTraits<const char *>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::string;
}

SOAPParameter&
SOAPTypeTraits<const char *>::Serialize(SOAPParameter& param, const char *val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}


#ifdef HAVE_WCHART
//
//  Trait info for const wchar_t *
void SOAPTypeTraits<const wchar_t *>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::string;
}

SOAPParameter&
SOAPTypeTraits<const wchar_t *>::Serialize(SOAPParameter& param, const wchar_t * val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}
#endif // HAVE_WCHART


//
//  Trait info for SOAPString
void SOAPTypeTraits<SOAPString>::GetType(SOAPQName& type)
{
	type = XMLSchema2001::string;
}

SOAPParameter&
SOAPTypeTraits<SOAPString>::Serialize(SOAPParameter& param, const SOAPString& val)
{
	if (!val)
		param.AddAttribute(XMLSchema2001::nil, SOAPParameter::m_true);
	param.GetStringRef() = val;
	return param;
}

const SOAPParameter&
SOAPTypeTraits<SOAPString>::Deserialize(const SOAPParameter& param, SOAPString& val)
{
	if (param.IsStruct())
		throw SOAPException("Can't de-serialize a complex type into a string.");

	if (param.IsNull())
		val = (const char *)0;
	else
		val = param.GetStringRef();
	return param;
}
