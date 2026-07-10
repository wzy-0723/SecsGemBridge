#pragma once

#include "hsms_def.h"
#include "secs_transport_interface.h"
#include "gem_info.h"

#include <functional>
#include "endpoint_listener_interface.h"
#include "endpoint_interface.h"
namespace why {

class HsmsTransport : public ISecsTransport, public IEndpointListener
{
public:
    HsmsTransport();
    ~HsmsTransport();

public:
    // 通过 ITransport 继承
    void Send(DataBuffer& data, uint8_t uStream, uint8_t uFunction, bool bWBit) override;
    void Send(HsmsMessage& message) override;
    void Send(DataBuffer& data, uint8_t uStream, uint8_t uFunction, uint32_t m_systemByte) override;
    void Connect() override;										//
    void Disconnect() override;
    bool IsConnected() const override;
    bool IsDisconnected() const override;

private:
    // @SecsGemBridge:网络状态变化后设置Secs的状态
    // 通过 IEndpointListener 继承
    void OnConnected() override;
    void OnDisconnected() override;
    void OnReceive(const uint8_t* pData, uint32_t uDataLen) override;
    bool OnIdle() override;

private:
    void SetState(HsmsState state);
private:
    typedef std::function<void(IEndpoint*)> EndpointDeleter;
    typedef std::unique_ptr<IEndpoint, EndpointDeleter> IEndpointPtr;

private:
    HsmsConfig				m_config;
    IEndpointPtr			m_endpointPtr;

    DataBuffer				m_recvBuff;

};
}
