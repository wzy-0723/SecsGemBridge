#include "tcp_client.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include "TextUtil.h"
//#include "logger.h"

#pragma comment(lib, "ws2_32.lib")

namespace jca
{
	namespace	
	{
		const uint32_t g_uMaxPacketSize = 1024;
	}

	IEndpoint* CreateTcpClientEndpoint(const TcpInfo& info)
	{
    	TcpClient* pEndpoint = new TcpClient();
        
        if (!pEndpoint->Init())
        {
            delete pEndpoint;
            return nullptr;
        }

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

        // 清理 Winsock
        WSACleanup();
	}

	void TcpClient::SetConfig(const TcpInfo& info)
	{
		m_strHostAddress = info.m_strHostAddress;
        m_uPort = info.m_uPort;
	}

    bool TcpClient::Init()
    {    
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) 
		{
            //LOG_ERROR << "WSAStartup failed: " << iResult << "\n";
            return false; 
        }
		return true;
	}

    bool TcpClient::Connect(bool bAsynchronous)
	{
		tstring				strHostAddress;

        if (INVALID_SOCKET != m_connectSocket)
        {
            //LOG_INFO << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] is already open";
            return true;
        }

	#ifdef _UNICODE
		strHostAddress = UTF8Decode(m_strHostAddress);
	#else
		strHostAddress = m_strHostAddress;
	#endif
		
        m_connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_connectSocket)
		{
            //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] is already open" << ::GetLastError();
			return false;
		}

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons((uint16_t)m_uPort);
        if (inet_pton(AF_INET, m_strHostAddress.c_str(), &serverAddress.sin_addr) <= 0)
        {
            //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] connect failed" << ::GetLastError();
            return false;
        }

        if (connect(m_connectSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) 
        {
            //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] connect failed" << ::GetLastError();
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

        if (INVALID_SOCKET != m_connectSocket)
        {
            closesocket(m_connectSocket);
            m_connectSocket = INVALID_SOCKET;
        }
        
        //LOG_INFO << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] disconnected";
	}

	uint32_t TcpClient::Recv(uint8_t* pData, uint32_t uDataLen)
	{
		if (INVALID_SOCKET == m_connectSocket)
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

		if (INVALID_SOCKET == m_connectSocket)
		{
            //LOG_ERROR << "Tcp client is not open";
			return false;
		}

		while (nRetryCount--)
		{
			std::lock_guard<std::mutex>				lockGuard(m_lock);

            if (SOCKET_ERROR == (uWritten = ::send(m_connectSocket, (const char *)(pData + uTotalSend), uDataLen - uTotalSend, 0)))
            {
                //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] send failed:" << ::GetLastError();
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
		DWORD				dwRecvLen;
		BYTE				recvBuffer[g_uMaxPacketSize];
		DWORD				dwSleep;
	
		dwSleep = 5;
		while (!m_bWouldExit)
		{
			dwRecvLen = 0;
            if (SOCKET_ERROR == (dwRecvLen = ::recv(m_connectSocket, (char*)recvBuffer, g_uMaxPacketSize, 0)))
            {
                if (ERROR_IO_PENDING == ::GetLastError())
					continue;

                //LOG_ERROR << "tcp client[" << m_strHostAddress << ":" << m_uPort << "] recv failed:" << ::GetLastError();
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

	    if (INVALID_SOCKET != m_connectSocket)
        {
            closesocket(m_connectSocket);
            m_connectSocket = INVALID_SOCKET;
        }
	}
}