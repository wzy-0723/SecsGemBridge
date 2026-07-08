#include "tcp_single_server.h"

//#include "logger.h"

namespace jca
{
	IEndpoint* CreateTcpSingleServerEndpoint(const TcpInfo& info)
	{
		return new TCPSingleServer(info);
	}

	TCPSingleServer::TCPSingleServer(const TcpInfo& info)
		: m_info(info)
	{
	}

	TCPSingleServer::~TCPSingleServer()
	{
		Disconnect();
	}

	void TCPSingleServer::SetEndpointListener(IEndpointListener* pListener)
	{
		m_pListener = pListener;
	}

	bool TCPSingleServer::Connect(bool bAsynchronous)
	{
		if (m_bRunning || m_socket != INVALID_SOCKET)
		{
			return true;
		}

		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
            //LOG_ERROR << "WSAStartup failed: " << iResult << std::endl;
			return false;
		}

		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
		{
            //LOG_ERROR << "Failed to create socket: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return false;
		}

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons((uint16_t)m_info.m_uPort);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;

		if (bind(m_socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
            //LOG_ERROR << "Failed to bind socket: " << WSAGetLastError() << std::endl;
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			WSACleanup();
			return false;
		}

		// 创建一个线程来监听连接
		if (m_pThread && m_pThread->joinable())
		{
			m_pThread->join();
			m_pThread.reset();
		}

		m_bRunning = true;

		m_pThread.reset(new std::thread(&TCPSingleServer::Listen, this));

		return true;
	}

	void TCPSingleServer::Disconnect()
	{
		m_bRunning = false;

		if (m_socket != INVALID_SOCKET)
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

		if (m_clientSocket != INVALID_SOCKET)
		{
			closesocket(m_clientSocket);
			m_clientSocket = INVALID_SOCKET;
		}

		if (m_pThread && m_pThread->joinable())
		{
			m_pThread->join();
			m_pThread.reset();
		}

		WSACleanup();
	}

	void TCPSingleServer::Interrupt()
	{
		if (m_clientSocket != INVALID_SOCKET)
		{
			closesocket(m_clientSocket);
			m_clientSocket = INVALID_SOCKET;
		}
	}

	uint32_t TCPSingleServer::Recv(uint8_t* pData, uint32_t uDataLen)
	{
		if (m_clientSocket == INVALID_SOCKET)
		{
            //LOG_ERROR << "Socket is invalid" << std::endl;
			return 0;
		}

		int len = recv(m_clientSocket, (char*)pData, uDataLen, 0);

		if (len == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
                //LOG_ERROR << "Recv timeout" << std::endl;
				return 0;
			}
            //LOG_ERROR << "Failed to recv data: " << error << std::endl;
			return 0;
		}

		return len;
	}

	bool TCPSingleServer::Send(const uint8_t* pData, uint32_t uDataLen)
	{
		if (m_clientSocket == INVALID_SOCKET)
		{
            //LOG_ERROR << "Socket is invalid" << std::endl;
			return false;
		}

		int len = send(m_clientSocket, (const char*)pData, uDataLen, 0);
		if (len == SOCKET_ERROR)
		{
            //LOG_ERROR << "Failed to send data: " << WSAGetLastError() << std::endl;
			return false;
		}

		return true;
	}

	void TCPSingleServer::Destroy()
	{
		Disconnect();

		delete this;
	}

	void TCPSingleServer::Listen()
	{
		while (m_bRunning)
		{
			if (m_socket == INVALID_SOCKET)
				return;

			if (listen(m_socket, 5) == SOCKET_ERROR)
			{
                //LOG_ERROR << "Failed to listen: " << WSAGetLastError() << std::endl;
				return;
			}

			sockaddr_in addr;
			int addrLen = sizeof(addr);
			m_clientSocket = accept(m_socket, (sockaddr*)&addr, &addrLen);
			if (m_clientSocket == INVALID_SOCKET)
			{
                //LOG_ERROR << "Failed to accept: " << WSAGetLastError() << std::endl;
				return;
			}

			if (!m_bRunning)
				break;

            //LOG_ERROR << "Client connected" << std::endl;

			if (m_pListener)
				m_pListener->OnConnected();

			RecvRun();
		}
	}

	void TCPSingleServer::RecvRun()
	{
		while (m_bRunning)
		{
			uint8_t buffer[1024]{ 0 };
			int len = recv(m_clientSocket, (char*)buffer, sizeof(buffer), 0);

			if (!m_bRunning)
				break;

			if (len == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error == WSAEWOULDBLOCK)
				{
                    //LOG_ERROR << "Recv timeout" << std::endl;
					continue;
				}

                //LOG_ERROR << "Failed to recv data: " << error << std::endl;
				break;
			}

			if (len == 0)
			{
                //LOG_ERROR << "Connection closed" << std::endl;
				break;
			}

			if (m_pListener)
				m_pListener->OnReceive(buffer, len);

		}

		m_clientSocket = INVALID_SOCKET;

		// 断开连接
		if (m_pListener)
			m_pListener->OnDisconnected();
	}
}