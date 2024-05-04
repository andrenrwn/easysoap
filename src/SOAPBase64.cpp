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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPBase64.cpp#1 $
 */

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPBase64.h>

USING_EASYSOAP_NAMESPACE

static char base64encode[64];
static int  base64decode[256];
static char base64pad = '=';

static int
initializeBase64Tables()
{
	int i;

	// initialize the encoding table
	for (i = 0; i < 26; ++i)
	{
		base64encode[i] = 'A' + i;
		base64encode[26 + i] = 'a' + i;
	}

	for (i = 0; i < 10; ++i)
		base64encode[52 + i] = '0' + i;

	base64encode[62] = '+';
	base64encode[63] = '/';


	// initialize the decoding table
	for (i = 0; i < 256; ++i)
		base64decode[i] = (char)0x80;
	for (i = 'A'; i <= 'Z'; ++i)
		base64decode[i] = i - 'A';
	for (i = 'a'; i <= 'z'; ++i)
		base64decode[i] = 26 + i - 'a';
	for (i = '0'; i <= '9'; ++i)
		base64decode[i] = 52 + i - '0';

	base64decode['+'] = 62;
	base64decode['/'] = 63;
	base64decode[(int)base64pad] = 0;

	return 1;
}

static __attribute__((unused)) int initialized = initializeBase64Tables();


inline int
nextChar(const char*& str)
{
	int c = 0;

	// skip over white space
	while (sp_isspace(*str))
		++str;

	// only increment pointer if
	// we're not at the end of string
	if ((c = *str))
		++str;

	return c;
}


size_t
SOAPBase64Base::EstimateSize(const SOAPString& str)
{
	return (str.Length() / 4) * 3 + 3;
}

void
SOAPBase64Base::Decode(const SOAPString& strx, char *bytes, size_t& byteslen)
{
	size_t outlen = 0;

	bool done = false;
	const char *str = strx;

    if (str == NULL) {
        byteslen = 0;
        return;
    }

	while (!done)
	{
		int		in[4];
		char	out[3];
		int		valid = 3;

		in[0] = nextChar(str);
		in[1] = nextChar(str);
		in[2] = nextChar(str);
		in[3] = nextChar(str);

		if (in[0] == 0)
			break;

		if (in[2] == base64pad)
			valid = 1;
		else if (in[3] == base64pad)
			valid = 2;

		in[0] = base64decode[in[0]];
		in[1] = base64decode[in[1]];
		in[2] = base64decode[in[2]];
		in[3] = base64decode[in[3]];

		if (in[0] == 0x80 ||
			in[1] == 0x80 ||
			in[2] == 0x80 ||
			in[3] == 0x80)
			throw SOAPException("Invalid character in base64 string.");

		out[0] = (in[0] << 2) | (in[1] >> 4);
		out[1] = (in[1] << 4) | (in[2] >> 2);
		out[2] = (in[2] << 6) |  in[3];

		if (outlen + valid > byteslen)
			throw SOAPException("Input array for base64 decoding not big enough.");

		outlen += valid;

		if (valid == 1)
		{
			*bytes++ = out[0];
			done = true;
		}
		else if (valid == 2)
		{
			*bytes++ = out[0];
			*bytes++ = out[1];
			done = true;
		}
		else // valid == 3
		{
			*bytes++ = out[0];
			*bytes++ = out[1];
			*bytes++ = out[2];
		}
	}

	byteslen = outlen;
}

void
SOAPBase64Base::Encode(const char *bytes, size_t size, SOAPString& str)
{
	char buf[4];
	size_t num64chars = (size / 3) * 4 + 4;
	const unsigned char *in = (const unsigned char *)bytes;

	str.Empty();
	str.Reserve(num64chars + 4);

	while (size >= 3)
	{
		buf[0] = base64encode[in[0] >> 2];
		buf[1] = base64encode[((in[0] & 3) << 4) | (in[1] >> 4)];
		buf[2] = base64encode[((in[1] & 0xF) << 2) | (in[2] >> 6)];
		buf[3] = base64encode[in[2] & 0x3F];
		str.Append(buf, 4);
		size -= 3;
		in += 3;
	}

	if (size == 2)
	{
		buf[0] = base64encode[in[0] >> 2];
		buf[1] = base64encode[((in[0] & 3) << 4) | (in[1] >> 4)];
		buf[2] = base64encode[(in[1] & 0xF) << 2];
		buf[3] = base64pad;
		str.Append(buf, 4);
	}
	else if (size == 1)
	{
		buf[0] = base64encode[in[0] >> 2];
		buf[1] = base64encode[(in[0] & 3) << 4];
		buf[2] = base64pad;
		buf[3] = base64pad;
		str.Append(buf, 4);
	}
}

void
SOAPBase64Base::Decode(const SOAPString& strx, std::vector<char>& bytes)
{
	size_t outlen = 0;

	bool done = false;
	const char *str = strx;
        
    if (str == NULL) return;
    
	while (!done)
	{
		int		in[4];
		char	out[3];
		int		valid = 3;

		in[0] = nextChar(str);
		in[1] = nextChar(str);
		in[2] = nextChar(str);
		in[3] = nextChar(str);

		if (in[0] == 0)
			break;

		if (in[2] == base64pad)
			valid = 1;
		else if (in[3] == base64pad)
			valid = 2;

		in[0] = base64decode[in[0]];
		in[1] = base64decode[in[1]];
		in[2] = base64decode[in[2]];
		in[3] = base64decode[in[3]];

		if (in[0] == 0x80 ||
			in[1] == 0x80 ||
			in[2] == 0x80 ||
			in[3] == 0x80)
			throw SOAPException("Invalid character in base64 string.");

		out[0] = (in[0] << 2) | (in[1] >> 4);
		out[1] = (in[1] << 4) | (in[2] >> 2);
		out[2] = (in[2] << 6) |  in[3];

		outlen += valid;
        
		if (valid == 1)
		{
			bytes.push_back(out[0]);
			done = true;
		}
		else if (valid == 2)
		{
			bytes.push_back(out[0]);
			bytes.push_back(out[1]);
			done = true;
		}
		else // valid == 3
		{
			bytes.push_back(out[0]);
			bytes.push_back(out[1]);
			bytes.push_back(out[2]);
		}
	}
}

void
SOAPBase64Base::Encode(const std::vector<char>& bytes, SOAPString& str)
{
    const unsigned char *in = (const unsigned char *)&bytes[0];
	char out[4];
    size_t size = bytes.size();
	size_t num64chars = (size / 3) * 4 + 4;

	str.Empty();
	str.Reserve(num64chars + 4);

	while (size >= 3)
	{
		out[0] = base64encode[in[0] >> 2];
		out[1] = base64encode[((in[0] & 3) << 4) | (in[1] >> 4)];
		out[2] = base64encode[((in[1] & 0xF) << 2) | (in[2] >> 6)];
		out[3] = base64encode[in[2] & 0x3F];
		str.Append(out, 4);
        in += 3;
		size -= 3;
	}

	if (size == 2)
	{
		out[0] = base64encode[in[0] >> 2];
		out[1] = base64encode[((in[0] & 3) << 4) | (in[1] >> 4)];
		out[2] = base64encode[(in[1] & 0xF) << 2];
		out[3] = base64pad;
		str.Append(out, 4);
	}
	else if (size == 1)
	{
		out[0] = base64encode[in[0] >> 2];
		out[1] = base64encode[(in[0] & 3) << 4];
		out[2] = base64pad;
		out[3] = base64pad;
		str.Append(out, 4);
	}
}
inline
int getHexValue(int c)
{
	switch (c)
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A': case 'a': return 10;
	case 'B': case 'b': return 11;
	case 'C': case 'c': return 12;
	case 'D': case 'd': return 13;
	case 'E': case 'e': return 14;
	case 'F': case 'f': return 15;
	default:
		throw SOAPException("Invalid hex character: %c(%d)", c, c);
	}
}

size_t
SOAPHexBase::EstimateSize(const SOAPString& str)
{
	return str.Length() / 2;
}

void
SOAPHexBase::Decode(const SOAPString& str, char *bytes, size_t& byteslen)
{
	size_t outlen = 0;
	const char *s = str.Str();
	int ub;

	while ((ub = nextChar(s)))
	{
		int lb = nextChar(s);
		if (!lb)
			throw SOAPException("Reached unexpected end of hex string, not an even number of characters.");
		if (outlen > byteslen)
			throw SOAPException("");

		*bytes++ = ((getHexValue(ub) << 4) + getHexValue(lb));
		++outlen;
	}

	byteslen = outlen;
}

void
SOAPHexBase::Encode(const char *b, size_t size, SOAPString& str)
{
	static const char *hexchars = "0123456789ABCDEF";

	str.Empty();
	str.Reserve((size * 2) + 1);
	const char *const bend = b + size;
	char buf[2];
	while (b != bend)
	{
		int c = *b++;
		buf[0] = hexchars[(c >> 4) & 0x0F];
		buf[1] = hexchars[c & 0x0F];
		str.Append(buf, 2);
	}
}

void
SOAPHexBase::Decode(const SOAPString& str, std::vector<char>& bytes)
{
	const char *s = str.Str();
	int ub;

	while ((ub = nextChar(s)))
	{
		int lb = nextChar(s);
		if (!lb)
			throw SOAPException("Reached unexpected end of hex string, not an even number of characters.");

		bytes.push_back((getHexValue(ub) << 4) + getHexValue(lb));
	}
}

void
SOAPHexBase::Encode(const std::vector<char>& bytes, SOAPString& str)
{
	static const char *hexchars = "0123456789ABCDEF";
    size_t size = bytes.size();
    std::vector<char>::const_iterator iter = bytes.begin();
    
	str.Empty();
	str.Reserve((size * 2) + 1);
	char buf[2];
	while (size--)
	{
		int c = *iter++;
		buf[0] = hexchars[(c >> 4) & 0x0F];
		buf[1] = hexchars[c & 0x0F];
		str.Append(buf, 2);
	}
}
