#pragma once

#include "standard.h"

#include <mutex>

#include "endpoint_interface.h"
#include "endpoint_listener_interface.h"
#include <thread>
namespace jca
{
	class TCPSingleServer : public IEndpoint
	{
	public:
		TCPSingleServer(const TcpInfo& info);
		~TCPSingleServer();

	public:
		void SetEndpointListener(IEndpointListener* pListener) override;
		bool Connect(bool bAsynchronous) override;
		void Disconnect() override;
		void Interrupt() override;
		uint32_t Recv(uint8_t* pData, uint32_t uDataLen) override;
		bool Send(const uint8_t* pData, uint32_t uDataLen) override;
		void Destroy() override;

	private:
		void Listen();

		void RecvRun();

	private:
		typedef std::unique_ptr<std::thread> ThreadPtr;

	private:
		TcpInfo					m_info;
		IEndpointListener*		m_pListener{nullptr};

		ThreadPtr				m_pThread;
		SOCKET					m_socket{ INVALID_SOCKET };
		SOCKET					m_clientSocket{ INVALID_SOCKET };

		bool		m_bRunning{ false };
	};
}