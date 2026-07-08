#include "tcp_client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace jca
{
	namespace	
	{
		const uint32_t g_uMaxPacketSize = 1024;
	}

    IEndpoint* CreateTcpClientEndpoint(const TcpClientInfo& info)
	{
        TcpClient* pEndpoint = new TcpClient();

		pEndpoint->SetConfig(info);
		return pEndpoint;
	}

	TcpClient::TcpClient()
	{
	}

	TcpClient::~TcpClient()
	{
		if (m_recvThreadPtr.get())
		{
			m_bWouldExit = true;
			if (m_recvThreadPtr->joinable())
				m_recvThreadPtr->join();
			m_recvThreadPtr = nullptr;
		}
	}

    void TcpClient::SetConfig(const TcpClientInfo& info)
	{
		m_strHostAddress = info.m_strHostAddress;
        m_uPort = info.m_uPort;
	}

    void TcpClient::Interrupt()
    {
        //@todo
    }

    void TcpClient::SetEndpointListener(IEndpointListener* pListener)
    {
        	std::lock_guard<std::mutex>			lockGuard(m_lock);
			m_pListener = pListener; 
    }

    bool TcpClient::Connect(bool bAsynchronous)
	{
        if (0 != m_connectSocket)
        {            
            //LOG_INFO << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] is already open";
            return true;
        }

        m_connectSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == m_connectSocket) 
        {
            //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] open failed";
            m_connectSocket = 0;
            return false;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(m_uPort);
        serverAddr.sin_addr.s_addr = inet_addr(m_strHostAddress.c_str());

        //@todo
        int client_socket;
        void* server_addr;

        if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        {
            //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] connect failed";
            return false;
        }

        // 异步接收
		if (bAsynchronous)
		{
			if (nullptr == m_recvThreadPtr.get())
			{
				m_bWouldExit = false;
				m_recvThreadPtr.reset(new std::thread(&TcpClient::RecvRun, this));
			}
		}

		if (m_pListener)
			m_pListener->OnConnected();

        //LOG_INFO << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] open succeed";
		return true;
    }

    void TcpClient::Disconnect() 
	{
        //LOG_INFO << "tcp client["  << m_strHostAddress << ":" << m_uPort << "] disconnecting";

		if (m_recvThreadPtr.get())
		{
			m_bWouldExit = true;
			if (m_recvThreadPtr->joinable())
				m_recvThreadPtr->join();
			m_recvThreadPtr = nullptr;
		}

        if (0 != m_connectSocket)
        {
            close(m_connectSocket);
            m_connectSocket = 0;
        }
                
        //LOG_INFO << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] disconnected";
	}

    uint32_t TcpClient::Recv(uint8_t* pData, uint32_t uDataLen)
	{
		if (0 == m_connectSocket)
		{
            //LOG_ERROR << "Tcp client is not open";
			return (uint32_t)-1;
		}

		if ((nullptr == pData) || (0 == uDataLen))
		{
            //LOG_ERROR << "invalid param";
			return (uint32_t)-1;
		}

		return ::recv(m_connectSocket, (char*)pData, uDataLen, 0);
	}

    bool TcpClient::Send(const uint8_t* pData, uint32_t uDataLen)
	{
		int32_t			nRetryCount = 2;
		unsigned long	uWritten = 0;
		uint32_t		uTotalSend = 0;

		if (0 == m_connectSocket)
		{
            //LOG_ERROR << "Tcp client is not open";
			return false;
		}

		while (nRetryCount--)
		{
			std::lock_guard<std::mutex>				lockGuard(m_lock);

            if (-1 == (uWritten = ::send(m_connectSocket, (const char *)(pData + uTotalSend), uDataLen - uTotalSend, 0)))
            {
                //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] send failed.";
                return false;
            }

			uTotalSend += uWritten;
			if (uTotalSend >= uDataLen)
				break;
		}

		if (uTotalSend == uDataLen)
			return true;

        //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] send failed:" << uTotalSend << "/" << uDataLen;
		return false;
	}


    void TcpClient::RecvRun()
	{
        ssize_t				dwRecvLen;
        uint8_t             recvBuffer[g_uMaxPacketSize];
        int                 dwSleep;
	
		dwSleep = 5;
		while (!m_bWouldExit)
		{
			dwRecvLen = 0;
            if (-1 == (dwRecvLen = ::recv(m_connectSocket, (char*)recvBuffer, g_uMaxPacketSize, 0)))
            {
                //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] recv failed.";
				if (m_pListener)
					m_pListener->OnDisconnected();
				break;
            }

			if (dwRecvLen > 0)
			{		
				dwSleep = 5;
				if (m_pListener)
					m_pListener->OnReceive(recvBuffer, dwRecvLen);
			}
			else
			{	
				if (m_pListener)
					m_pListener->OnIdle();

				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
		}

	    if (0 != m_connectSocket)
        {
            close(m_connectSocket);
            m_connectSocket = 0;
        }
	}

}