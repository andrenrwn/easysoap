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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPNamespaces.cpp#1 $
 */


#include <easysoap/SOAP.h>
#include <easysoap/SOAPNamespaces.h>

#include "es_namespaces.h"

USING_EASYSOAP_NAMESPACE

SOAPStrings::SOAPStringMap SOAPStrings::soapStrings;

const SOAPString&
SOAPEnc::base(void)
{
	static const SOAPString s(SOAP_ENC);
	return s;
}

const SOAPQName SOAPEnc::root("root", SOAPEnc::base());
const SOAPQName SOAPEnc::Array("Array", SOAPEnc::base());
const SOAPQName SOAPEnc::arrayType("arrayType", SOAPEnc::base());
const SOAPQName SOAPEnc::offset("offset", SOAPEnc::base());
const SOAPQName SOAPEnc::position("position", SOAPEnc::base());
const SOAPQName SOAPEnc::base64("base64", SOAPEnc::base());

const SOAPString&
SOAPEnv::base(void)
{
	static const SOAPString s(SOAP_ENV);
	return s;
}

const SOAPQName SOAPEnv::Body("Body", SOAPEnv::base());
const SOAPQName SOAPEnv::Header("Header", SOAPEnv::base());
const SOAPQName SOAPEnv::Envelope("Envelope", SOAPEnv::base());
const SOAPQName SOAPEnv::Fault("Fault", SOAPEnv::base());
const SOAPQName SOAPEnv::encodingStyle("encodingStyle", SOAPEnv::base());
const SOAPQName SOAPEnv::mustUnderstand("mustUnderstand", SOAPEnv::base());
const SOAPQName SOAPEnv::actor("actor", SOAPEnv::base());

const SOAPString&
XMLSchema1999::xsi(void)
{
	static const SOAPString s(SOAP_XSI_1999);
	return s;
}
const SOAPString&
XMLSchema1999::xsd(void)
{
	static const SOAPString s(SOAP_XSD_1999);
	return s;
}

const SOAPQName XMLSchema1999::null("null", XMLSchema1999::xsi());
const SOAPQName XMLSchema1999::type("type", XMLSchema1999::xsi());

const SOAPString&
XMLSchema2001::xsi(void)
{
	static const SOAPString s(SOAP_XSI_2001);
	return s;
}
const SOAPString&
XMLSchema2001::xsd(void)
{
	static const SOAPString s(SOAP_XSD_2001);
	return s;
}

const SOAPQName XMLSchema2001::nil("nil", XMLSchema2001::xsi());
const SOAPQName XMLSchema2001::type("type", XMLSchema2001::xsi());

const SOAPQName XMLSchema2001::string("string", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::boolean("boolean", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::decimal("decimal", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::float_("float", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::double_("double", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::duration("duration", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::dateTime("dateTime", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::time("time", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::date("date", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::base64Binary("base64Binary", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::hexBinary("hexBinary", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::integer("integer", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::nonPositiveInteger("nonPositiveInteger", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::long_("long", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::int_("int", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::short_("short", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::byte("byte", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::nonNegativeInteger("nonNegativeInteger", XMLSchema2001::xsd());
//const SOAPQName XMLSchema2001::unsignedLong("unsignedLong", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::unsignedLong("long", XMLSchema2001::xsd());
//const SOAPQName XMLSchema2001::unsignedInt("unsignedInt", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::unsignedInt("int", XMLSchema2001::xsd());
//const SOAPQName XMLSchema2001::unsignedShort("unsignedShort", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::unsignedShort("short", XMLSchema2001::xsd());
//const SOAPQName XMLSchema2001::unsignedByte("unsignedByte", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::unsignedByte("byte", XMLSchema2001::xsd());
const SOAPQName XMLSchema2001::positiveInteger("positiveInteger", XMLSchema2001::xsd());

const SOAPQName *schemaStringTable[] =
{
    &SOAPEnc::root,
    &SOAPEnc::Array,
    &SOAPEnc::arrayType,
    &SOAPEnc::offset,
    &SOAPEnc::position,
    &SOAPEnc::base64,
    &SOAPEnv::Body,
    &SOAPEnv::Header,
    &SOAPEnv::Envelope,
    &SOAPEnv::Fault,
    &SOAPEnv::encodingStyle,
    &SOAPEnv::mustUnderstand,
    &SOAPEnv::actor,
    &XMLSchema1999::null,
    &XMLSchema1999::type,
    &XMLSchema2001::nil,
    &XMLSchema2001::type,
    &XMLSchema2001::string,
    &XMLSchema2001::boolean,
    &XMLSchema2001::decimal,
    &XMLSchema2001::float_,
    &XMLSchema2001::double_,
    &XMLSchema2001::duration,
    &XMLSchema2001::dateTime,
    &XMLSchema2001::time,
    &XMLSchema2001::date,
    &XMLSchema2001::base64Binary,
    &XMLSchema2001::hexBinary,
    &XMLSchema2001::integer,
    &XMLSchema2001::nonPositiveInteger,
    &XMLSchema2001::long_,
    &XMLSchema2001::int_,
    &XMLSchema2001::short_,
    &XMLSchema2001::byte,
    &XMLSchema2001::nonNegativeInteger,
    &XMLSchema2001::unsignedLong,
    &XMLSchema2001::unsignedInt,
    &XMLSchema2001::unsignedShort,
    &XMLSchema2001::unsignedByte,
    &XMLSchema2001::positiveInteger
};

void
SOAPStrings::initSOAPStrings(void)
{
    if (soapStrings.empty())
    {
        soapStrings[SOAPEnc::base().Str()] = &SOAPEnc::base();
        soapStrings[SOAPEnv::base().Str()] = &SOAPEnv::base();
        soapStrings[XMLSchema2001::xsi().Str()] = &XMLSchema2001::xsi();
        soapStrings[XMLSchema2001::xsd().Str()] = &XMLSchema2001::xsd();
        for (unsigned int i = 0; i < sizeof(schemaStringTable) / sizeof(schemaStringTable[0]); i++)
        {
            soapStrings[schemaStringTable[i]->GetName().Str()] = &schemaStringTable[i]->GetName();
        }
    }
}

