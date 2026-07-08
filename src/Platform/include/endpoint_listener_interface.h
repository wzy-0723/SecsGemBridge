
#pragma once

#include "standard.h"

namespace jca
{
	/**
	* @class IEndpointListener
	* @brief 通讯终端侦听器接口
	*/
	class IEndpointListener
	{
	public:
		/**
		* @brief 通讯链路建立
		* @return void
		*/
		virtual void OnConnected() = 0;

		/**
		* @brief 通讯链路断开
		* @return void
		*/
		virtual void OnDisconnected() = 0;

		/**
		* @brief 从通讯链路接收到数据
		* @param pData 保存接收到数据的缓冲的首地址
		* @param uDataLen 数据pData的长度(字节数)
		* @return void
		*/
		virtual void OnReceive(const uint8_t* pData, uint32_t uDataLen) = 0;

		/**
		* @brief 通讯空闲中
		* @return
		* - TRUE 监听器在通讯空闲中有操作
		* - FALSE 监听器在通讯空闲中无操作
		*/
		virtual bool OnIdle() = 0;
	};
}