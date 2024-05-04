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
 * $Id: //depot/maint/bigip17.1.1.3/iControl/soap/EasySoap++-0.6.2/src/SOAPServerDispatch.cpp#1 $
 */

#include <easysoap/SOAPServerDispatch.h>
#include <easysoap/SOAPDispatchHandler.h>


BEGIN_EASYSOAP_NAMESPACE

static const SOAPString env_prefix("SOAP-ENV");
static const SOAPString xsi_prefix("xsi");
static const SOAPString xsd_prefix("xsd");
static const SOAPString unspecified("Unspecified");
static const char *serverfault = "SOAP-ENV:Server";
static const char *clientfault = "SOAP-ENV:Client";
END_EASYSOAP_NAMESPACE


//
// Special exception for mustUnderstand faults
// so we can return the correct faultstring

BEGIN_EASYSOAP_NAMESPACE

class SOAPMustUnderstandException : public SOAPException
{
public:
	SOAPMustUnderstandException(const SOAPString& what)
		: SOAPException(what) {}
	SOAPMustUnderstandException(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		FormattedMessage(fmt, args);
		va_end(args);
	}
	~SOAPMustUnderstandException() {}
};

END_EASYSOAP_NAMESPACE

USING_EASYSOAP_NAMESPACE

SOAPServerDispatch::SOAPServerDispatch()
: m_composer(new XMLComposer), m_preparsed(false)
{
}


SOAPServerDispatch::~SOAPServerDispatch()
{
}


SOAPServerDispatch&
SOAPServerDispatch::DispatchTo(SOAPDispatchHandlerInterface *disp)
{
	m_handlers.Add(disp);
	return *this;
}

SOAPServerDispatch&
SOAPServerDispatch::DispatchTo(SOAPHeaderHandlerInterface *disp)
{
	m_headerHandlers.Add(disp);
	return *this;
}


//
//
// Quick method for writing a SOAP Fault
//
// TODO:  Fix things so we can write a Fault
// out the correct way....
//
void
SOAPServerDispatch::WriteFault(const SOAPFault& fault,
    SOAPServerTransport& transport, XMLComposerPtr writer)
{
	bool bufferedResults = XMLComposer::GetBufferResults();
	if ( !bufferedResults )
	{
		transport.SetError();
		transport.WriteHeaders();
	}

	writer->Reset();
	writer->StartTag("SOAP-ENV:Envelope");
	writer->AddXMLNS(env_prefix, SOAPEnv::base());
	writer->AddXMLNS(xsi_prefix, XMLSchema2001::xsi());
	writer->AddXMLNS(xsd_prefix, XMLSchema2001::xsd());
	writer->StartTag("SOAP-ENV:Body");
	fault.WriteSOAPPacket(*writer);
	writer->EndTag("SOAP-ENV:Body");
	writer->EndTag("SOAP-ENV:Envelope");

	if ( bufferedResults )
	{
		transport.SetError();
		transport.Write(m_response->GetBody().GetMethod(),
                writer->GetBytes(), writer->GetLength());
	}
}


void
SOAPServerDispatch::WriteFault(const SOAPFault& fault)
{
    WriteFault(fault, *m_transport, m_writer);
}


void
SOAPServerDispatch::parse(SOAPEnvelopePtr request, SOAPServerTransportPtr trans) {
    m_parser.Parse(*request, *trans);
    m_request = request;
    m_preparsed = true;
}

bool
SOAPServerDispatch::Handle(SOAPServerTransportPtr trans,
	XMLComposerPtr composer)
{
	bool retval = false;
	const char *faultcode = serverfault;

	m_transport = trans;
	m_response.reset(new SOAPResponse);
	m_writer = composer.get() != NULL ? composer : m_composer;

	try
	{
		// Parse the SOAP packet
		faultcode = clientfault;
		// if transport hasn't been read then
		// allocate new request and response
		if (!m_preparsed) {
			m_request.reset(new SOAPEnvelope);
			parse(m_request, trans);
		}
		faultcode = serverfault;

		// Setup the SOAP action based on the method name
		SOAPMethod& requestMethod = m_request->GetBody().GetMethod();
		SOAPQName methodName = requestMethod.GetName();
		requestMethod.SetSoapAction(trans->GetSoapAction());

		// Set up the "suggested" method return name.  Actual
		// method can change it.  In future we set it up with
		// WSDL.
		SOAPString respname = requestMethod.GetName().GetName();
		respname.Append("Response");
		SOAPMethod& responseMethod = m_response->GetBody().GetMethod();
		responseMethod.SetName(respname, requestMethod.GetName().GetNamespace());

		// Handle any headers we have...
		HandleHeaders(*m_request, *m_response);

		// Now handle the request
        // 
		if (!HandleRequest(*m_request, *m_response))
		{ 
			faultcode = clientfault;
			throw SOAPException("Unknown method \"{%s}:%s\"", 
                    methodName.GetNamespace().IsEmpty() ? unspecified.Str() : methodName.GetNamespace().Str(),
                    methodName.GetName().IsEmpty() ? unspecified.Str() : methodName.GetName().Str());
		}

		bool bufferedResults = XMLComposer::GetBufferResults();
		if ( !bufferedResults )
		{
			m_transport->WriteHeaders();
		}

		// Compose our SOAP packet response
		m_response->WriteSOAPPacket(*m_writer);

		if ( bufferedResults )
		{
			// Send back the repsonse.
			m_transport->Write(m_response->GetBody().GetMethod(),
				m_writer->GetBytes(),
				m_writer->GetLength());
		}

		retval = true;
	}
	catch(SOAPFault& fault)
	{
		HandleFault(fault);
		WriteFault(fault);
	}
	catch(SOAPMustUnderstandException& mux)
	{
		SOAPFault fault;
		fault.SetFaultString(mux.What());
		fault.SetFaultCode("SOAP-ENV:MustUnderstand");

		HandleFault(fault);
		WriteFault(fault);
	}
	catch(SOAPException& sex)
	{
		SOAPFault fault;
		fault.SetFaultString(sex.What());
		fault.SetFaultCode(faultcode);

		HandleFault(fault);
		WriteFault(fault);
	}
	catch (...)
	{
		SOAPFault fault;
		fault.SetFaultString("Serious error occured.");
		fault.SetFaultCode(serverfault);

		HandleFault(fault);
		WriteFault(fault);
	}

    m_request.reset();
    m_response.reset();
    m_writer.reset();
    m_preparsed = false;

	return retval;
}

bool
SOAPServerDispatch::HandleRequest(SOAPEnvelope& request, SOAPResponse& response)
{
	//
	// TODO:  This is an O(n) lookup... but n is (hopefully!) small
	bool handled = false;
	for (Handlers::Iterator i = m_handlers.Begin(); i != m_handlers.End(); ++i)
	{
		//
		// We found a handler.  Now dispatch the method
		if ((*i)->ExecuteMethod(request, response.GetBody().GetMethod()))
		{
			handled = true;
			break;
		}
	}

	return handled;
}

void
SOAPServerDispatch::HandleHeaders(SOAPEnvelope& request, SOAPResponse& response)
{
	const SOAPHeader::Headers& headers = request.GetHeader().GetAllHeaders();
	for (SOAPHeader::Headers::const_iterator h = headers.begin();
           h != headers.end(); ++h)
	{
		const SOAPParameter& header = **h;
		const SOAPAttribute* actor = header.FindAttribute(SOAPEnv::actor);
		const SOAPAttribute* mu = header.FindAttribute(SOAPEnv::mustUnderstand);

		if ((mu == NULL) || (mu->GetValue() == "0") || (mu->GetValue() == "1"))
		{
			//
			// TODO: Be able to specify/check for a custom QName for this endpoint
			if ((actor == NULL) || (actor->GetValue() == SOAPHeader::actorNext))
			{
				//
				// TODO:  This is an O(n) lookup... but n is (hopefully!) small
				HeaderHandlers::Iterator i;
				for (i = m_headerHandlers.Begin();
					i != m_headerHandlers.End(); ++i)
				{
					//
					// We found a handler.  Now dispatch the method
					if ((*i)->HandleHeader(header, request, response))
						break;
				}

				//
				// check for mustUnderstand == 1
				if (i == m_headerHandlers.End() && (mu != NULL) && (mu->GetValue() == "1"))
				{
					// TODO:  Special MustUnderstand exception so the
					// actor(?) in the SOAPFault can be set correctly.
					throw SOAPMustUnderstandException("Failed to understand header \"{%s}:%s\"",
						(const char *)header.GetName().GetNamespace(),
						(const char *)header.GetName().GetName());
				}
			}
		}
		else
		{
			throw SOAPMustUnderstandException("Invalid value for mustUnderstand attribute on header {%s}:%s: %s",
				(const char *)header.GetName().GetNamespace(),
				(const char *)header.GetName().GetName(),
				(const char *)mu->GetName().GetName());
		}
	}
}

