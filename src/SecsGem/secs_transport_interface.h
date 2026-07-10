#pragma once

#include "standard.h"

#include "hsms_def.h"
//#include "gem_info.h"

namespace why
{
	class ISecsTransportObserve
	{
	public:
		virtual void NewMessage() = 0;
		virtual void StateChange(HsmsState stage) = 0;
	};

	class ISecsTransport
	{
	public:
		virtual void Send(DataBuffer& data, uint8_t uStream, uint8_t uFunction, bool bWBit) = 0;
		virtual void Send(HsmsMessage& message) = 0;
		virtual void Send(DataBuffer& data, uint8_t uStream, uint8_t uFunction, uint32_t m_systemByte) = 0;
		virtual void Connect() = 0;
		virtual void Disconnect() = 0;

        //virtual void LinkTest() = 0;

		virtual bool IsConnected() const = 0;
		virtual bool IsDisconnected() const = 0;

        //virtual void Select() = 0;
        //virtual void Deselect() = 0;
        //virtual bool IsSelected() const = 0;
        //virtual bool IsDeselected() const = 0;

        //virtual void SetObserve(ISecsTransportObserve* pObserve) = 0;
        //virtual HsmsMessage GetNextMessage() = 0;
        //virtual void Destroy() = 0;
	};

    //ISecsTransport* CreateHsmsTransport(HsmsConfig& config);
}