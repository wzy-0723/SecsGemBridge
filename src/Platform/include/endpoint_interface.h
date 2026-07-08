
#pragma once

#include "endpoint_def.h"
#include "endpoint_listener_interface.h"

namespace jca
{
	/**
	* @brief 通讯终端的接口
	*/
	class IEndpoint
	{
	public:
		/**
		* @brief 设置端口侦听器
		* @param pListener 通讯侦听器接口
		* @return 出错代码
		*/
		virtual void SetEndpointListener(IEndpointListener* pListener) = 0;

		/*!
		* 建立连接
		* @param bAsynchronous
		* - true 启动异步接收线程，一般用于全双工模式，收到数据会通过IEndpointListener接口的OnReceive方法返回
		* - false 不启动异步接受线程，需要主动调用Recv方法来获取数据
		* @return
		* - TRUE 连接建立成功
		* - FALSE 连接建立失败
		*/
		virtual bool Connect(bool bAsynchronous) = 0;

		/*!
		* 断开连接
		* @return
		*/
		virtual void Disconnect() = 0;

		/**
		* 中断连接
		*/

		virtual void Interrupt() = 0;

		/**
		* @brief 接受数据
		* @param pData 用于保存接受到数据的缓冲区
		* @param uDataLen 指示缓冲区pData的大小
		* @return 
		* - -1 接收出错
		* - 其他 接收到的数据的长度
		*/
		virtual uint32_t Recv(uint8_t* pData, uint32_t uDataLen) = 0;

		/**
		* @brief 发送数据
		* @param pData 待发送数据缓冲的首地址
		* @param uDataLen 待发送数据lpBuffer的字节数
		* @return
		* - true 数据发送成功
		* - false 数据发送失败
		*/
		virtual bool Send(const uint8_t* pData, uint32_t uDataLen) = 0;

		/**
		* @brief 销毁通讯终端
		* @return void
		*/
		virtual void Destroy() = 0;
	};
 
	/**
	* @brief 创建串口通讯终端
	* @param info 待创建的窗口的配置信息
	* @param nReadIntervalTimeout 读取间隔超时
	* @return 返回串口通讯终端的接口
	*/
    //IEndpoint* CreaateSerialEndpoint(const SerialInfo &info, int64_t nReadIntervalTimeout = 50);


	/**
	* @brief 创建TCPClient通讯终端
	* @param info 待创建的窗口的配置信息
	* @return 返回TCPClient终端的接口
	*/
	IEndpoint* CreateTcpClientEndpoint(const TcpInfo& info);

	/**
	* @brief 创建TCPServer(1-1)通讯终端
	* @param info 待创建的窗口的配置信息
	* @return 返回TcpSingleServer终端的接口
	*/
	IEndpoint* CreateTcpSingleServerEndpoint(const TcpInfo& info);
}