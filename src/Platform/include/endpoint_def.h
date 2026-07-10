
#pragma once

#include "standard.h"

namespace why
{
    // /**
    // * @brief 定义串口的配置信息
    // */
    // struct SerialInfo
    // {
    // 	char		m_acPortName[64]{ "COM1:" };	//!< 串口名称
    // 	uint32_t	m_uBaudRate{ CBR_9600 };		//!< 串口通讯的波特率
    // 	uint8_t		m_uByteSize{ 0 };				//!< 数据位
    // 	uint8_t		m_uParity{ NOPARITY };			//!< 奇偶校验方法
    // 	uint8_t		m_uStopBits{ ONESTOPBIT };		//!< 停止位标记
    // };

	/**
	 * @brief 定义TCP的配置信息
	 */
	struct TcpInfo
	{
		char		m_strHostAddress[64]{ "127.0.0.1" };	//!< 主机地址
		uint32_t	m_uPort{ 80 };	            			//!< 端口号
	};


    struct TcpClientInfo
    {
        char		m_strHostAddress[64]{ "127.0.0.1" };	//!< 主机地址
        uint32_t	m_uPort{ 80 };	            			//!< 端口号
    };
}