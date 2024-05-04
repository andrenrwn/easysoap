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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/include/easysoap/SOAPResponse.h#1 $
 */


#if !defined(AFX_SOAPRESPONSE_H__B8BDA9D2_F6F6_4312_BFAD_B34A813A3B55__INCLUDED_)
#define AFX_SOAPRESPONSE_H__B8BDA9D2_F6F6_4312_BFAD_B34A813A3B55__INCLUDED_

#include <easysoap/SOAP.h>
#include <easysoap/SOAPEnvelope.h>
#include <boost/shared_ptr.hpp>

BEGIN_EASYSOAP_NAMESPACE

/**
*
*/
class EASYSOAP_EXPORT SOAPResponse : public SOAPEnvelope
{
public:
	SOAPResponse();
	virtual ~SOAPResponse();

	void SetMethod(const SOAPMethod& meth);

	bool IsFault() const
	{
		return GetBody().IsFault();
	}

	const SOAPParameter& GetReturnValue(int i = 0) const
	{
        const SOAPParameter::Params &values = GetBody().GetMethod().GetParams();
        for (SOAPParameter::Params::const_iterator iter = values.begin();
               iter != values.end(); ++iter)
        {
           if (i-- == 0)
           { 
		        return *iter;
           }
        }
        throw SOAPException("Index out of range getting parameter");
	}

	const SOAPParameter& GetReturnValue(const char *name) const
	{
		return GetBody().GetMethod().GetParameter(name);
	}

private:
};
typedef boost::shared_ptr<SOAPResponse> SOAPResponsePtr;

END_EASYSOAP_NAMESPACE

#endif // !defined(AFX_SOAPRESPONSE_H__B8BDA9D2_F6F6_4312_BFAD_B34A813A3B55__INCLUDED_)

