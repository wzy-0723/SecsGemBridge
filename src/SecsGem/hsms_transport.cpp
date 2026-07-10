#include "hsms_transport.h"

namespace why {

HsmsTransport::HsmsTransport()
{

}

HsmsTransport::~HsmsTransport()
{

}

void HsmsTransport::Send(DataBuffer& data, uint8_t uStream, uint8_t uFunction, bool bWBit)
{

}

void HsmsTransport::Send(HsmsMessage& message)
{

}

void HsmsTransport::Send(DataBuffer& data, uint8_t uStream, uint8_t uFunction, uint32_t m_systemByte)
{

}

void HsmsTransport::Connect()
{
    TcpInfo info;
    memcpy(&info.m_strHostAddress, m_config.m_acIpAddress, sizeof(info.m_strHostAddress));
    info.m_uPort = m_config.m_uPort;

    if (m_config.m_eConnctMode == HsmsConfig::Passive)
        m_endpointPtr.reset(CreateTcpSingleServerEndpoint(info));
    else
        m_endpointPtr.reset(CreateTcpClientEndpoint(info));

    if (m_endpointPtr)
    {
        m_endpointPtr->SetEndpointListener(this);
        m_endpointPtr->Connect(true);
    }
}

void HsmsTransport::Disconnect()
{

}

bool HsmsTransport::IsConnected() const
{

    return true;
}

bool HsmsTransport::IsDisconnected() const
{
    return true;
}

void HsmsTransport::OnConnected()
{
    SetState(HsmsState::Connected_Deselected);

    // if (m_config.m_eConnctMode == HsmsConfig::Active)
    //     Select();

    // TODO: 添加T7超时处理逻辑
}

void HsmsTransport::OnDisconnected()
{
    SetState(HsmsState::Disconnected);
    m_recvBuff.clear();
}

namespace
{
const uint32_t g_uHsmsLengthSize = 4;
}

void HsmsTransport::OnReceive(const uint8_t* pData, uint32_t uDataLen)
{
    m_recvBuff.insert(m_recvBuff.end(), pData, pData + uDataLen);

    while (m_recvBuff.size() >= HsmsHead::uLength + g_uHsmsLengthSize)
    {
        HsmsMessage message;

        //@todo
        // if (!ParseMessage(m_recvBuff, message))
        // {
        //     break;
        // }

        // m_recvBuff.erase(m_recvBuff.begin(), m_recvBuff.begin() + message.uLength + g_uHsmsLengthSize);

        // ProcessMessage(message);
    }
}

bool HsmsTransport::OnIdle()
{
    return true;
}

void HsmsTransport::SetState(HsmsState state)
{

}

}

