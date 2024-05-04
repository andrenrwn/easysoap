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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPParameter.cpp#1 $
 */


#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif // _MSC_VER

#include <easysoap/SOAP.h>
#include <easysoap/SOAPParameter.h>
#include <easysoap/SOAPNamespaces.h>
#include <easysoap/XMLComposer.h>

USING_EASYSOAP_NAMESPACE


const SOAPString SOAPParameter::m_defaultName("item");
const SOAPString SOAPParameter::m_true("true");
const SOAPString SOAPParameter::m_false("false");


SOAPParameter::SOAPParameter(void)
: m_dataPtr(&m_x_data)
{
}

SOAPParameter::SOAPParameter(const SOAPString& name)
: m_name(name)
, m_dataPtr(&m_x_data)
{
}

SOAPParameter::SOAPParameter(const SOAPQName& name)
: m_name(name)
, m_dataPtr(&m_x_data)
{
}

SOAPParameter::SOAPParameter(const SOAPParameter& param)
: m_dataPtr(&m_x_data)
{
	Assign(param);
}

SOAPParameter::~SOAPParameter()
{
}

void
SOAPParameter::Clear(void)
{
    m_x_data.Clear();    
	m_dataPtr = &m_x_data;
}

void
SOAPParameter::Data::Clear(void)
{
    m_params.clear();
    m_attrs.clear();
    m_isstruct = false;
}

void
SOAPParameter::Assign(const SOAPParameter& param)
{
	m_name = param.m_name;
	m_x_data.Assign(*param.m_dataPtr);
	m_dataPtr = &m_x_data;
}


void
SOAPParameter::Data::Assign(const Data& d)
{
	m_strval = d.m_strval;
	m_isstruct = d.m_isstruct;
	m_attrs = d.m_attrs;
    m_params = d.m_params;
}


SOAPParameter&
SOAPParameter::operator=(const SOAPParameter& param)
{
	Assign(param);
	return *this;
}

SOAPParameter::Data::~Data()
{
}

void
SOAPParameter::SetName(const char *name, const char *ns)
{
	if (!ns)
		m_name = name;
	else
		m_name.Set(name, ns);
}

void
SOAPParameter::SetType(const char *name, const char *ns)
{
	AddAttribute(XMLSchema2001::type).GetValue().Set(name, ns);
}

void
SOAPParameter::SetType(const SOAPString& name, const SOAPString &ns)
{
	AddAttribute(XMLSchema2001::type).GetValue().Set(name, ns);
}

void
SOAPParameter::SetType(const SOAPQName& type)
{
	AddAttribute(XMLSchema2001::type).GetValue().Set(type);
}

bool
SOAPParameter::IsNull() const
{
	SOAPAttribute* null = FindAttribute(XMLSchema2001::nil);
	if (null == NULL)
	{
		null = FindAttribute(XMLSchema1999::null);
	}

	return ((null != NULL)) &&
			((null->GetValue() == m_true) || (null->GetValue() == "1"));
}

bool
SOAPParameter::IsStruct() const
{
	return m_dataPtr->m_isstruct;
}

void
SOAPParameter::SetNull(bool isnull)
{
    SOAPAttribute* null = FindAttribute(XMLSchema2001::nil);
	if (isnull)
    {
        if (null == NULL) {
            AddAttribute(XMLSchema2001::nil, m_true);
        } else
		    null->SetValue(m_true);
    }
	else
    {
        if (null != NULL)
        {
            for(Attrs::iterator iter = m_dataPtr->m_attrs.begin();
                    iter != m_dataPtr->m_attrs.end(); ++iter)
            {
                if ((*iter).GetName() == XMLSchema2001::nil)
                {
                    m_dataPtr->m_attrs.erase(iter);
                    break;
                }
            }
        }
    }
}

void
SOAPParameter::SetIsStruct()
{
	m_dataPtr->m_isstruct = true;
}

SOAPAttribute&
SOAPParameter::AddAttribute(const SOAPQName& name)
{
    SOAPAttribute attribute(name);
	m_dataPtr->m_attrs.push_back(attribute);
    return m_dataPtr->m_attrs.back();
}

SOAPAttribute&
SOAPParameter::AddAttribute(const SOAPQName& name, const SOAPQName& value)
{
    SOAPAttribute attribute(name, value);
	m_dataPtr->m_attrs.push_back(attribute);
    return m_dataPtr->m_attrs.back();
}

SOAPAttribute*
SOAPParameter::FindAttribute(const char* name) const
{
    SOAPAttribute* attr = NULL;
    for(Attrs::iterator iter = m_dataPtr->m_attrs.begin();
            iter != m_dataPtr->m_attrs.end(); ++iter)
    {
        if ((*iter).GetName() == name)
        {
            attr = &*iter;
            break;
        }
    }

    return attr;
}

SOAPAttribute*
SOAPParameter::FindAttribute(const SOAPQName& name) const
{
    SOAPAttribute* attr = NULL;
    for(Attrs::iterator iter = m_dataPtr->m_attrs.begin();
            iter != m_dataPtr->m_attrs.end(); ++iter)
    {
        if ((*iter).GetName() == name)
        {
            attr = &*iter;
            break;
        }
    }

    return attr;
}

SOAPAttribute*
SOAPParameter::FindAccessorAttribute(const char* name) const
{
    SOAPAttribute* attr = NULL;
    for(Attrs::const_iterator iter = m_x_data.m_attrs.begin();
            iter != m_x_data.m_attrs.end(); ++iter)
    {
        if ((*iter).GetName() == name)
        {
            attr = const_cast<SOAPAttribute *>(&*iter);
            break;
        }
    }

    return attr;
}

SOAPAttribute*
SOAPParameter::FindAccessorAttribute(const SOAPQName& name) const
{
    SOAPAttribute* attr = NULL;
    for(Attrs::const_iterator iter = m_x_data.m_attrs.begin();
            iter != m_x_data.m_attrs.end(); ++iter)
    {
        if ((*iter).GetName() == name)
        {
            attr = const_cast<SOAPAttribute *>(&*iter);
            break;
        }
    }

    return attr;
}

void
SOAPParameter::SetValue(const char *val)
{
	SOAPTypeTraits<const char *>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<const char *>::Serialize(*this, val);
}

#ifdef HAVE_WCHART
void
SOAPParameter::SetValue(const wchar_t *val)
{
	SOAPTypeTraits<const wchar_t *>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<const wchar_t *>::Serialize(*this, val);
}
#endif

void
SOAPParameter::SetValue(const SOAPString &val)
{
	SOAPTypeTraits<SOAPString>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<SOAPString>::Serialize(*this, val);
}

void
SOAPParameter::SetInt(const char *val)
{
	SOAPTypeTraits<int>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<int>::Serialize(*this, val);
}

void
SOAPParameter::SetFloat(const char *val)
{
	SOAPTypeTraits<float>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<float>::Serialize(*this, val);
}

void
SOAPParameter::SetDouble(const char *val)
{
	SOAPTypeTraits<double>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<double>::Serialize(*this, val);
}

void
SOAPParameter::SetValue(int val)
{
	SOAPTypeTraits<int>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<int>::Serialize(*this, val);
}

void
SOAPParameter::SetValue(float val)
{
	SOAPTypeTraits<float>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<float>::Serialize(*this, val);
}

void
SOAPParameter::SetValue(double val)
{
	SOAPTypeTraits<double>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<double>::Serialize(*this, val);
}

void
SOAPParameter::SetBoolean(const char *val)
{
	SOAPTypeTraits<bool>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<bool>::Serialize(*this, val);
}

void
SOAPParameter::SetValue(bool val)
{
	SOAPTypeTraits<bool>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<bool>::Serialize(*this, val);
}

void
SOAPParameter::SetLong(const char *val)
{
	SOAPTypeTraits<long>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<long>::Serialize(*this, val);
}

void
SOAPParameter::SetValue(long val)
{
	SOAPTypeTraits<long>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<long>::Serialize(*this, val);
}

void
SOAPParameter::SetULong(const char *val)
{
	SOAPTypeTraits<unsigned long>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<unsigned long>::Serialize(*this, val);
}

void
SOAPParameter::SetValue(unsigned long val)
{
	SOAPTypeTraits<unsigned long>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<unsigned long>::Serialize(*this, val);
}

void
SOAPParameter::SetInt64(const char *val)
{
	SOAPTypeTraits<int64_t>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<int64_t>::Serialize(*this, val);
}

void
SOAPParameter::SetValue(int64_t val)
{
	SOAPTypeTraits<int64_t>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<int64_t>::Serialize(*this, val);
}

void
SOAPParameter::SetUInt64(const char *val)
{
	SOAPTypeTraits<u_int64_t>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<u_int64_t>::Serialize(*this, val);
}

void
SOAPParameter::SetValue(u_int64_t val)
{
	SOAPTypeTraits<u_int64_t>::GetType(AddAttribute(XMLSchema2001::type).GetValue());
	SOAPTypeTraits<u_int64_t>::Serialize(*this, val);
}

const SOAPString&
SOAPParameter::GetString() const
{
	if (IsStruct())
		throw SOAPException("Cannot convert a struct to a string.");

	return m_dataPtr->m_strval;
}

int
SOAPParameter::GetInt() const
{
	int ret;
	SOAPTypeTraits<int>::Deserialize(*this, ret);
	return ret;
}

bool
SOAPParameter::GetBoolean() const
{
	bool ret;
	SOAPTypeTraits<bool>::Deserialize(*this, ret);
	return ret;
}

float
SOAPParameter::GetFloat() const
{
	float ret;
	SOAPTypeTraits<float>::Deserialize(*this, ret);
	return ret;
}

double
SOAPParameter::GetDouble() const
{
	double ret;
	SOAPTypeTraits<double>::Deserialize(*this, ret);
	return ret;
}

long
SOAPParameter::GetLong() const
{
	long ret;
	SOAPTypeTraits<long>::Deserialize(*this, ret);
	return ret;
}

unsigned long
SOAPParameter::GetULong() const
{
	unsigned long ret;
	SOAPTypeTraits<unsigned long>::Deserialize(*this, ret);
	return ret;
}

int64_t
SOAPParameter::GetInt64() const
{
	int64_t ret;
	SOAPTypeTraits<int64_t>::Deserialize(*this, ret);
	return ret;
}

u_int64_t
SOAPParameter::GetUInt64() const
{
	u_int64_t ret;
	SOAPTypeTraits<u_int64_t>::Deserialize(*this, ret);
	return ret;
}

const SOAPParameter&
SOAPParameter::GetParameter(const char *name) const
{
	SOAPParameter* param = FindParameter(name);
	if (param == NULL)
		throw SOAPException("Could not find element by name: %s", name);
	return *param;
}

const SOAPParameter&
SOAPParameter::GetParameter(const SOAPString &name) const
{
	return GetParameter(name.Str());
}

SOAPParameter*
SOAPParameter::FindParameter(const char* name) const
{
    SOAPParameter* param = NULL;
    for(Params::iterator iter = m_dataPtr->m_params.begin();
            iter != m_dataPtr->m_params.end(); ++iter)
    {
        if ((*iter).m_name.GetName() == name)
        {
            param = &*iter;
            break;
        }
    }

    return param;
}


SOAPParameter&
SOAPParameter::AddParameter(const char *name, const char *ns)
{
	SOAPParameter param(SOAPQName(name, ns));
	m_dataPtr->m_params.push_back(param);
	SetIsStruct();

	return m_dataPtr->m_params.back();
}

SOAPParameter&
SOAPParameter::AddParameter(const SOAPString& name)
{
	SOAPParameter param(name);
	m_dataPtr->m_params.push_back(param);
	SetIsStruct();

	return m_dataPtr->m_params.back();
}

SOAPParameter&
SOAPParameter::AddParameter(const SOAPString& name, const SOAPString& ns)
{
	SOAPParameter param(SOAPQName(name, ns));
	m_dataPtr->m_params.push_back(param);
	SetIsStruct();

	return m_dataPtr->m_params.back();
}

SOAPParameter&
SOAPParameter::AddParameter(const SOAPQName& name)
{
	SOAPParameter param(name);
	m_dataPtr->m_params.push_back(param);
	SetIsStruct();

	return m_dataPtr->m_params.back();
}

SOAPParameter&
SOAPParameter::AddParameter(const SOAPParameter& p)
{
	SOAPParameter param(p);
	m_dataPtr->m_params.push_back(param);
	SetIsStruct();

	return m_dataPtr->m_params.back();
}

bool
SOAPParameter::WriteSOAPPacket(XMLComposer& packet) const
{
	packet.StartTag(m_name);

    const Attrs &attrs = GetAttributes();
	for (Attrs::const_iterator iter = attrs.begin(); iter != attrs.end(); ++iter)
    {
		packet.AddAttr((*iter).GetName(), (*iter).GetValue());
    }

	if (IsStruct())
	{
		const Params &params = GetParams();
		for (Params::const_iterator iter = params.begin(); iter != params.end(); ++iter)
			(*iter).WriteSOAPPacket(packet);
	}
	else
	{
		packet.WriteValue(m_dataPtr->m_strval);
	}

	packet.EndTag(m_name);

	return true;
}

