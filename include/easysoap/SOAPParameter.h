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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/SOAPParameter.h#1 $
 */


#if !defined(AFX_SOAPPARAMETER_H__30811BAD_D6A1_4535_B256_9EEB56A84026__INCLUDED_)
#define AFX_SOAPPARAMETER_H__30811BAD_D6A1_4535_B256_9EEB56A84026__INCLUDED_

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <list>
#include <map>
#include <easysoap/SOAP.h>
#include <easysoap/SOAPNamespaces.h>

BEGIN_EASYSOAP_NAMESPACE

class SOAPParameterHandler;

class EASYSOAP_EXPORT SOAPAttribute
{
public:
    SOAPAttribute(void)
    {
    }
    
    SOAPAttribute(const SOAPQName& name) :
        m_name(name)
    {
    }
    
    SOAPAttribute(const SOAPQName& name, const SOAPQName& value) :
        m_name(name), m_value(value)
    {
    }
    
	SOAPAttribute& operator=(const SOAPAttribute& attribute)
    {
        m_name = attribute.m_name;
        m_value = attribute.m_value;
        return *this;
    }

    const SOAPQName&
    GetName(void) const
    {
        return m_name;
    }

    void
    SetValue(const SOAPQName& value)
    {
        m_value = value;
    }

    SOAPQName&
    GetValue(void)
    {
        return m_value;
    }

    const SOAPQName&
    GetValue(void) const
    {
        return m_value;
    }

private:
    SOAPQName m_name;
    SOAPQName m_value;    
};

/**
*
*/
class EASYSOAP_EXPORT SOAPParameter
{
public:

	typedef std::list<SOAPParameter> Params;
	typedef std::list<SOAPAttribute> Attrs;

	SOAPParameter(void);
    SOAPParameter(const SOAPString& name);
    SOAPParameter(const SOAPQName& name);
	SOAPParameter(const SOAPParameter& param);
	virtual ~SOAPParameter();

    void Clear(void);

	SOAPParameter& operator=(const SOAPParameter& param);

	SOAPQName& GetName()					{return m_name;}
	const SOAPQName& GetName() const		{return m_name;}

	void SetName(const SOAPQName& name)		{m_name = name;}
	void SetName(const SOAPString& name)	{m_name = name;}
	void SetName(const char *name, const char *ns = 0);

	void SetType(const char *name, const char *ns = 0);
	void SetType(const SOAPString& name, const SOAPString& ns);
	void SetType(const SOAPQName& type);

	void SetValue(bool val);
	void SetBoolean(const char *val);
	void SetValue(int val);
	void SetInt(const char *val);
	void SetValue(float val);
	void SetFloat(const char *val);
	void SetValue(double val);
	void SetDouble(const char *val);

	void SetValue(long val);
	void SetLong(const char *val);
	void SetValue(unsigned long val);
	void SetULong(const char *val);
	void SetValue(int64_t val); // 64 bit support
	void SetInt64(const char *val);
	void SetValue(u_int64_t val);
	void SetUInt64(const char *val);

	void SetValue(const char *val);
#ifdef HAVE_WCHART
	void SetValue(const wchar_t *val);
#endif // HAVE_WCHART
	void SetValue(const SOAPString& val);


	bool GetBoolean() const;

	int GetInt() const;
	operator int() const					{return GetInt();}

	float GetFloat() const;
	operator float() const					{return GetFloat();}

	double GetDouble() const;
	operator double() const					{return GetDouble();}

	const SOAPString& GetString() const;
	operator const SOAPString&() const		{return GetString();}

	long GetLong() const;
	operator long() const           {return GetLong();}

	unsigned long GetULong() const;
	operator unsigned long() const          {return GetULong();}

	int64_t GetInt64() const;
	operator int64_t() const                {return GetInt64();}

	u_int64_t GetUInt64() const;
	operator u_int64_t() const              {return GetUInt64();}

	Params& GetParams()
	{
		return m_dataPtr->m_params;
	}

	const Params& GetParams() const
	{
		return m_dataPtr->m_params;
	}

	SOAPParameter& AddParameter(const char *name,  const char *ns = 0);
    SOAPParameter& AddParameter(const SOAPString& name = m_defaultName);
	SOAPParameter& AddParameter(const SOAPString& name, const SOAPString& ns);
	SOAPParameter& AddParameter(const SOAPQName& name);
	SOAPParameter& AddParameter(const SOAPParameter& p);

	const SOAPParameter& GetParameter(const char *name) const;
	const SOAPParameter& GetParameter(const SOAPString &name) const;
    SOAPParameter* FindParameter(const char* name) const;

	void SetNull(bool isnull = true);
	void SetIsStruct();

	bool IsNull() const;
	bool IsStruct() const;

	Attrs& GetAttributes() {return m_dataPtr->m_attrs;}
	const Attrs& GetAttributes() const {return m_dataPtr->m_attrs;}
	const Attrs& GetAccessorAttributes() const {return m_x_data.m_attrs;}
	SOAPAttribute& AddAttribute(const SOAPQName& name);
	SOAPAttribute& AddAttribute(const SOAPQName& name, const SOAPQName& value);
    SOAPAttribute* FindAttribute(const char* name) const;
    SOAPAttribute* FindAttribute(const SOAPQName& name) const;
    SOAPAttribute* FindAccessorAttribute(const char* name) const;
    SOAPAttribute* FindAccessorAttribute(const SOAPQName& name) const;

	//
	// Use this to get access to the underlying string.
	SOAPString& GetStringRef()					{return m_dataPtr->m_strval;}
	const SOAPString& GetStringRef() const		{return m_dataPtr->m_strval;}

	bool WriteSOAPPacket(XMLComposer& packet) const;

	void LinkTo(SOAPParameter& param)
	{
		m_dataPtr = param.m_dataPtr;
	}

private:
	void Assign(const SOAPParameter&);

	class Data
	{
	public:
		Data() : m_isstruct(false) {}
		~Data();

        void Clear(void);

		void Assign(const Data&);

		bool			m_isstruct;	// true for array, struct types
		SOAPString		m_strval;	// value legal only if m_isstruct == false

	 	Params  		m_params;
		Attrs			m_attrs;
	private:
		Data& operator=(const Data&);
		Data(const Data&);
	};

	friend class Data;

	SOAPQName		m_name;
	Data			m_x_data;	// Don't use this directly!
    Data*           m_dataPtr;

public:
    static const SOAPString m_defaultName;
    static const SOAPString m_true;
    static const SOAPString m_false;
};

END_EASYSOAP_NAMESPACE

#include <easysoap/SOAPTypeTraits.h>

BEGIN_EASYSOAP_NAMESPACE

template<typename T>
inline SOAPParameter&
operator<<(SOAPParameter& param, const T& val)
{
	//
	// If SOAPTypeTraits<> is undefined for your
	// class then you will have to implement it.
	// Look in SOAPTypeTraits.h for examples.
	//
	// Add xsi:type attribute
	SOAPTypeTraits<T>::GetType(param.AddAttribute(XMLSchema2001::type).GetValue());
	// serialize
	return SOAPTypeTraits<T>::Serialize(param, val);
}

template<typename T>
inline const SOAPParameter&
operator>>(const SOAPParameter& param, T& val)
{
	//
	// If SOAPTypeTraits<> is undefined for your
	// class then you will have to implement it.
	// Look in SOAPTypeTraits.h for examples.
	//
	return SOAPTypeTraits<T>::Deserialize(param, val);
}

//
// specialize const char * and const wchar_t * so the
// compiler doesn't get confused thinking it's "char[5]"
// or something like that...
inline SOAPParameter&
operator<<(SOAPParameter& param, const char *val)
{
	SOAPTypeTraits<const char *>::GetType(param.AddAttribute(XMLSchema2001::type).GetValue());
	return SOAPTypeTraits<const char *>::Serialize(param, val);
}

#ifdef HAVE_WCHART
inline SOAPParameter&
operator<<(SOAPParameter& param, const wchar_t *val)
{
	SOAPTypeTraits<const wchar_t *>::GetType(param.AddAttribute(XMLSchema2001::type).GetValue());
	return SOAPTypeTraits<const wchar_t *>::Serialize(param, val);
}
#endif

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPPARAMETER_H__30811BAD_D6A1_4535_B256_9EEB56A84026__INCLUDED_)

