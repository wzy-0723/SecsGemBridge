#pragma once

#include "standard.h"
#include "endpoint_interface.h"

#include <memory>
#include <functional>
#include <mutex>
#include <thread>


namespace jca
{
    class TcpClient : public IEndpoint
    {
    public:
        TcpClient();
        ~TcpClient();

        // IEnpoint interface
	    void SetEndpointListener(IEndpointListener* pListener) override;
		bool Connect(bool bAsynchronous) override;
		void Disconnect() override;
		uint32_t Recv(uint8_t* pData, uint32_t uDataLen) override;
		bool Send(const uint8_t* pData, uint32_t uDataLen) override;
		void Destroy() override;
        void Interrupt() override;
    public:
		void SetConfig(const TcpClientInfo& info);

	private:	
		typedef std::unique_ptr<std::thread> ThreadPrt;

	private:
		void RecvRun();

	private:
		IEndpointListener*			m_pListener{nullptr};					//!< 终端侦听器
		std::string					m_strHostAddress{ "127.0.0.1" };		//!< 主机地址
		uint32_t					m_uPort{ 80 };	            			//!< 端口号
        int32_t                     m_connectSocket{0};                     //!< 通讯套接字
		std::mutex					m_lock;						//!< 线程同步锁
		ThreadPrt					m_recvThreadPtr;			//!< 接收线程句柄
        bool						m_bWouldExit{false};		//!< 是否退出接收线程循环
    };
} // namespace jca

