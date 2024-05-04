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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/SOAPTypeTraits.h#1 $
 */


#if !defined(AFX_SOAPTYPETRAITS_H__C5FEAF2C_BF9D_4B2A_BA32_516712F68E78__INCLUDED_)
#define AFX_SOAPTYPETRAITS_H__C5FEAF2C_BF9D_4B2A_BA32_516712F68E78__INCLUDED_

#include <vector>
#include <easysoap/SOAPBase64.h>

#include <string.h>

BEGIN_EASYSOAP_NAMESPACE

template<typename T>
class SOAPTypeTraits;

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<bool>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, bool val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, bool& val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, std::_Bit_reference val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<char>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, char val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, char& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<short>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, short val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, short& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<unsigned short>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, unsigned short val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, unsigned short& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<int>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, int val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, int& val);
	static const SOAPParameter& Deserialize(const SOAPParameter& param, long& val)
	{
		int tval;
		Deserialize(param, tval);
		val = tval;
		return param;
	}
};

//
// assume long and int are the same, 32 bit integers
/**
*
*/
/*
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<long> : public SOAPTypeTraits<int>
{
};
*/

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<float>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, float val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, float& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<double>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, double val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, double& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<long>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, long val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, long& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<unsigned long>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, unsigned long val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, unsigned long& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<int64_t>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, int64_t val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, int64_t& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<u_int64_t>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, u_int64_t val);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, u_int64_t& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<SOAPString>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, const SOAPString& val);
	static const SOAPParameter& Deserialize(const SOAPParameter&, SOAPString& val);
};

/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<const char *>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, const char *val);
	// Can't deserialize into const char *
	// and char * is just dangerous
};

#ifdef HAVE_WCHART
/**
*
*/
template <>
class EASYSOAP_EXPORT SOAPTypeTraits<const wchar_t *>
{
public:
	static void GetType(SOAPQName& type);
	static SOAPParameter& Serialize(SOAPParameter& param, const wchar_t *val);
	// Can't deserialize into const wchar_t *
	// and wchar_t * is just dangerous
};
#endif

/**
*
*/
class SOAPMapTypeTraits
{
public:
	static void GetType(SOAPQName& type)
	{
		type.Set("Map", "http://xml.apache.org/xml-soap");
	}

	template <typename V>
	static SOAPParameter& Serialize(SOAPParameter& param, const V& val)
	{
		param.SetIsStruct();
		for (TYPENAME(V::const_iterator) i = val.begin(); i != val.end(); ++i)
		{
			SOAPParameter& p = param.AddParameter();
			p.SetIsStruct();
			p.AddParameter("key") << i.Key();
			p.AddParameter("value") <<i.Item();
		}
		return param;
	}

	template <typename V>
	static const SOAPParameter& Deserialize(const SOAPParameter& param, V& val)
	{
		TYPENAME(V::key_type) key;
		const SOAPParameter::Params& arr = param.GetParams();
		for (SOAPParameter::Params::const_iterator iter = arr.begin();
               iter != arr.end(); ++iter)
		{
			(*iter).GetParameter("key") >> key;
			(*iter).GetParameter("value") >> val[key];
		}
		return param;
	}
};

#ifdef HAVE_PARTIAL_SPECIALIZATION
/**
*
*/
template <typename K, typename V>
class SOAPTypeTraits< SOAPHashMap<K, V> > : public SOAPMapTypeTraits
{
};
#endif // HAVE_PARTIAL_SPECIALIZATION

/**
*
*/
class SOAPArrayTypeTraits
{
private:
	//
	// returns false if the position is []
	// other wise it returns true and x
	// is set to the integer value.
	static bool parsepos(const SOAPQName& attr, const SOAPQName& val, size_t& x)
	{
		//
		// arrayType should be a value like "xsd:string[2]"
		// We use strrchr() in case it's something like
		// "xsd:string[][2]" which is certainly possible...
		const char *s = sp_strrchr(val.GetName(), '[');
		if (s && *s++ == '[')
		{
			if (*s == ']')
				return false;
			char *e = 0;
			x = strtol(s, &e, 10);
			if (*e == ']' && e[1] == 0)
				return true;
		}
		throw SOAPException("Invalid value for array encoding tag '%s': %s",
			(const char *)attr.GetName(), (const char *)val.GetName());
	}

public:
	static void GetType(SOAPQName& type)
	{
		type = SOAPEnc::Array;
	}

	template <typename V>
	static SOAPParameter& Serialize(SOAPParameter& param, const V& val)
	{
		//
		// Add SOAP-ENC:arrayType attribute
		char buffer[32];
		bool bIsArray = 0;

		SOAPQName& atype = param.AddAttribute(SOAPEnc::arrayType).GetValue();
		SOAPTypeTraits<TYPENAME(V::value_type)>::GetType(atype);
		if (atype.GetName() == "Array")
		{
			bIsArray = 1;
		}
		snprintf(buffer, sizeof(buffer), "[%d]", val.size());
		atype.GetName().Append(buffer);

		//
		// Serialize the array values
		int bFoundArray = 0;
		param.SetIsStruct();
		for (TYPENAME(V::const_iterator) i = val.begin(); i != val.end(); ++i)
		{
			//SOAPTypeTraits<TYPENAME(V::value_type)>::Serialize(param.AddParameter(), *i);
			SOAPParameter &rParam = param.AddParameter();
			SOAPTypeTraits<TYPENAME(V::value_type)>::Serialize(rParam, *i);

			if ( !bFoundArray )
			{
				if ( bIsArray )
				{
					SOAPAttribute* attr = rParam.FindAttribute(SOAPEnc::arrayType);
					if (attr != NULL)
					{
						SOAPQName& childType = attr->GetValue();
						char typebuf[128];
						strcpy(typebuf, childType.GetName());
						char *bracket = strrchr(typebuf, '[');
						if ( NULL != bracket )
						{
							bracket[0] = '[';
							bracket[1] = ']';
							bracket[2] = '\0';
						}
						strcat(typebuf, buffer);

						atype.Set((const char *)typebuf, childType.GetNamespace());

						bFoundArray = 1;
					}
				}
			}
		}

		return param;
	}

	template <typename V>
	static const SOAPParameter& Deserialize(const SOAPParameter& param, V& val)
	{
		const SOAPParameter::Params& arr = param.GetParams();
		//
		// check for xsi:nil
        SOAPAttribute* attr = param.FindAttribute(XMLSchema2001::nil);
		if ((attr != NULL) && arr.empty())
		{
			val.resize(0);
			return param;
		}
		else if (attr != NULL)
			throw SOAPException("Got xsi:nil attribute on array but parameter has values..");

		//
		// parse arrayType attribute
		size_t numvals;
		attr = param.FindAttribute(SOAPEnc::arrayType);
		if (attr == NULL || !parsepos(SOAPEnc::arrayType, attr->GetValue(), numvals))
			numvals = arr.size();
        else if (arr.size() > numvals)
				throw SOAPException("Error de-serializing array.  Too many values in array.  Array specified %u, found %u.", numvals, arr.size());

		size_t pos = 0;

		//
		// parse offset attribute if present
		attr = param.FindAttribute(SOAPEnc::offset);
		if (attr != NULL)
			parsepos(SOAPEnc::offset, attr->GetValue(), pos);

		val.resize(numvals);
		for (SOAPParameter::Params::const_iterator iter = arr.begin(); iter != arr.end(); ++iter)
		{
			//
			// parse position attribute if present
			attr = (*iter).FindAccessorAttribute(SOAPEnc::position);
			if (attr != NULL)
			{
				size_t newpos;
				parsepos(SOAPEnc::position, attr->GetValue(), newpos);
				if (newpos < pos)
					throw SOAPException("Error decoding array, position attribute cannot point to a previous element."
						" Current position: [%d], next position: [%d]",
						pos, newpos);
				if (newpos >=  numvals)
					throw SOAPException("Error decoding array, position out of range."
						" Current size: [%d], requested position: [%d]",
						numvals, newpos);

				pos = newpos;
			}
			SOAPTypeTraits<TYPENAME(V::value_type)>::Deserialize(*iter, val[pos++]);
		}
		return param;
	}
};

//
// Base class for Encoding and
// Decoding hex and base64 strings.
/**
*
*/
class SOAPByteArrayEncodingTraits
{
public:
	template<typename T>
	static SOAPParameter&
	Serialize(SOAPParameter& param, const T& val)
	{
		size_t size = (val.m_arr ? val.m_arr : val.m_carr)->size();
		const char *bytes = (val.m_arr ? val.m_arr : val.m_carr)->begin();
		val.Encode(bytes, size, param.GetStringRef());
		return param;
	}

	template<typename T>
	static const SOAPParameter&
	Deserialize(const SOAPParameter& param, T& val)
	{
		size_t size = val.EstimateSize(param.GetStringRef());
		if (size > 0)
		{
			val.m_arr->resize(size);
			val.Decode(param.GetStringRef(), &(*val.m_arr)[0], size);
			val.m_arr->resize(size);
		}
		return param;
	}
};

//
// Traits for base64 encoded byte arrays
/**
*
*/
class SOAPBase64 : public SOAPBase64Base
{
public:
	const SOAPArray<char>*	m_carr;
	SOAPArray<char>*		m_arr;

	SOAPBase64(const SOAPArray<char>& carr)
		: m_carr(&carr), m_arr(0)
	{
	}

	SOAPBase64(SOAPArray<char>& arr)
		: m_carr(0), m_arr(&arr)
	{
	}
    
private:
	SOAPBase64();
	SOAPBase64(const SOAPBase64&);
	SOAPBase64& operator=(const SOAPBase64&);
};

/**
*
*/
class SOAPBase64Traits : public SOAPByteArrayEncodingTraits
{
public:
	static void GetType(SOAPQName& type)
	{
		type = XMLSchema2001::base64Binary;
	}
};

//
// Traits for hex encoded byte arrays
/**
*
*/
class SOAPHex : public SOAPHexBase
{
public:
	const SOAPArray<char>*	m_carr;
	SOAPArray<char>*		m_arr;

	SOAPHex(const SOAPArray<char>& carr)
		: m_carr(&carr), m_arr(0)
	{
	}

	SOAPHex(SOAPArray<char>& arr)
		: m_carr(0), m_arr(&arr)
	{
	}
private:
	SOAPHex();
	SOAPHex(const SOAPHex&);
	SOAPHex& operator=(const SOAPHex&);
};

/**
*
*/
class SOAPHexTraits : public SOAPByteArrayEncodingTraits
{
public:
	static void GetType(SOAPQName& type)
	{
		type = XMLSchema2001::hexBinary;
	}
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAPBase64 > : public SOAPBase64Traits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAPHex > : public SOAPHexTraits
{
};

#ifdef HAVE_PARTIAL_SPECIALIZATION
/**
*
*/
template <typename T>
class SOAPTypeTraits< SOAPArray<T> > : public SOAPArrayTypeTraits
{
};

#else // DONT HAVE_PARTIAL_SPECIALIZATION
/**
*
*/
template <>
class SOAPTypeTraits< SOAPArray<bool> > : public SOAPArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAPArray<short> > : public SOAPArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAPArray<int> > : public SOAPArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAPArray<long> > : public SOAPArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAPArray<float> > : public SOAPArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAPArray<double> > : public SOAPArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAPArray<SOAPString> > : public SOAPArrayTypeTraits
{
};
#endif // HAVE_PARTIAL_SPECIALIZATION

/**
*
*/
class SOAP2DArrayTypeTraits
{
private:
	static void parse2Dpos(const SOAPQName& attr, const SOAPQName& val, size_t& x, size_t& y)
	{
		//
		// arrayType should be a value like "xsd:string[2,3]"
		// We use strrchr() in case it's something like
		// "xsd:string[][2,3]" which is certainly possible...
		const char *s = sp_strrchr(val.GetName(), '[');
		if (s && *s++ == '[')
		{
			char *e = 0;
			x = strtol(s, &e, 10);
			if (*e++ == ',')
			{
				s = e;
				y = strtol(s, &e, 10);
				if (*e == ']' && e[1] == 0)
					return;
			}
		}
		throw SOAPException("Invalid 2D value for array encoding tag '%s': %s",
			(const char *)attr.GetName(), (const char *)val.GetName());
	}

public:
	static void GetType(SOAPQName& type)
	{
		type = SOAPEnc::Array;
	}

	template<typename V>
	static SOAPParameter& Serialize(SOAPParameter& param, const V& val)
	{
		//
		// Add SOAP-ENC:arrayType attribute
		char buffer[64];

		SOAPQName& atype = param.AddAttribute(SOAPEnc::arrayType);
		SOAPTypeTraits<TYPENAME(V::value_type)>::GetType(atype);
		snprintf(buffer, sizeof(buffer), "[%d,%d]", val.GetNumRows(), val.GetNumCols());
		atype.GetName().Append(buffer);

		//
		// Serialize the array values
		param.SetIsStruct();
		for (size_t r = 0; r < val.GetNumRows(); ++r)
			for (size_t c = 0; c < val.GetNumCols(); ++c)
				SOAPTypeTraits<TYPENAME(V::value_type)>::Serialize(param.AddParameter(), val[r][c]);
		return param;
	}

	template<typename V>
	static const SOAPParameter& Deserialize(const SOAPParameter& param, V& val)
	{
		const SOAPParameter::Params& arr = param.GetParams();
		SOAPAttribute* attr;
		//
		// check xsi:nil attribute
		attr = param.FindAttribute(XMLSchema2001::nil);
		if ((attr != NULL) && arr.empty())
		{
			val.resize(0, 0);
			return param;
		}
		else if (attr != NULL)
			throw SOAPException("Got xsi:nil attribute on array but parameter has values..");

		//
		// parse arrayType attribute
		attr = param.FindAttribute(SOAPEnc::arrayType);
		if (attr == NULL)
			throw SOAPException("Cannot de-serialize 2D-array without arrayType attribute.");

		size_t numrows;
		size_t numcols;
		parse2Dpos(SOAPEnc::arrayType, attr->GetValue(), numrows, numcols);

		size_t row = 0;
		size_t col = 0;

		//
		// parse offset attribute if present
		attr = param.FindAttribute(SOAPEnc::offset);
		if (attr != NULL)
			parse2Dpos(SOAPEnc::offset, attr->GetValue(), row, col);

		if (arr.size() > numrows * numcols)
				throw SOAPException("Error de-serializing 2D array.  Too many values.");

		val.resize(numrows, numcols);
		for (SOAPParameter::Params::const_iterator iter = arr.begin(); iter != arr.end(); ++iter)
		{
			//
			// parse position attribute if present
			attr = (*iter).FindAccessorAttribute(SOAPEnc::position);
			if (attr != NULL)
			{
				size_t newrow, newcol;
				parse2Dpos(SOAPEnc::position, attr->GetValue(), newrow, newcol);
				if (newrow < row || newcol < col)
					throw SOAPException("Error decoding array, position attribute cannot point to a previous element."
						" Current position: [%d,%d], next position: [%d,%d]",
						row, col, newrow, newcol);
				if (newcol >= numcols || newrow >= numrows)
					throw SOAPException("Error decoding array, position out of range."
						" Current size: [%d,%d], requested position: [%d,%d]",
						numrows, numcols, newrow, newcol);

				row = newrow;
				col = newcol;
			}
			SOAPTypeTraits<TYPENAME(V::value_type)>::Deserialize(*iter, val[row][col]);
			if (++col == numcols)
			{
				++row;
				col = 0;
			}
		}
		return param;
	}
};

#ifdef HAVE_PARTIAL_SPECIALIZATION
/**
*
*/
template <typename T>
class SOAPTypeTraits< SOAP2DArray<T> > : public SOAP2DArrayTypeTraits
{
};
#else // DONT HAVE_PARTIAL_SPECIALIZATION
/**
*
*/
template <>
class SOAPTypeTraits< SOAP2DArray<bool> > : public SOAP2DArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAP2DArray<char> > : public SOAP2DArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAP2DArray<int> > : public SOAP2DArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAP2DArray<long> > : public SOAP2DArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAP2DArray<float> > : public SOAP2DArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAP2DArray<double> > : public SOAP2DArrayTypeTraits
{
};

/**
*
*/
template <>
class SOAPTypeTraits< SOAP2DArray<SOAPString> > : public SOAP2DArrayTypeTraits
{
};
#endif // HAVE_PARTIAL_SPECIALIZATION

/**
  *
  */
template <typename T>
class SOAPTypeTraits< SOAPArrayOfArray<T> > : public SOAPArrayTypeTraits
{
	public:
	template <typename V>
	static SOAPParameter& Serialize(SOAPParameter& param, const V& val)
	{
		//
		// Add SOAP-ENC:arrayType attribute
		char buffer[32];
	
		SOAPQName& atype = param.AddAttribute(SOAPEnc::arrayType);
		SOAPTypeTraits<T>::GetType(atype);
		snprintf(buffer, sizeof(buffer), "[][%d]", val.size());
		atype.GetName().Append(buffer);
	
		//
		// Serialize the array values
		param.SetIsStruct();
		for (TYPENAME(V::const_iterator) i = val.begin(); i != val.end(); ++i)
		{
			SOAPParameter &rParam = param.AddParameter();
			SOAPTypeTraits<TYPENAME(V::value_type)>::Serialize(rParam, *i);
		}
		return param;
	}
};

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPTYPETRAITS_H__C5FEAF2C_BF9D_4B2A_BA32_516712F68E78__INCLUDED_)

