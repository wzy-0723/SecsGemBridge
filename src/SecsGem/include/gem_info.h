
#pragma once

#include "standard.h"

namespace why
{
	typedef std::pair<uint8_t, uint8_t> StreamFunction;

	typedef uint16_t EventID;

	enum class GemType : uint8_t
	{
		EQP = 0,
		HOST = 1,
	};

	enum class GemProtocolType
	{
		HSMS = 0,
		SECS_I = 1,
	};

	enum class GemCommunicationState : uint8_t
	{
		NOT_CONNECTED = 0,
		WAIT_CRA,
		WAIT_DELAY,
		WAIT_CR_FROM_HOST,
		COMMUNICATING,
	};

	/**
	 * OFF-LINE
	 * 此状态下设备基本不接受主机任何控制，提供有限的信息范围。此状态是控件状态的最低级别。
	 * 当 OFF-LINE 状态处于活动状态时，Host发送的控制执行Equ将使用 xF0/S9F0 响应来自主机的任何主要消息
	 * 但 S1F13（建立通信请求）和 S1F17（在线请求）除外。
	 * 
	 * EQUIPMENT_OFF_LINE
	 * 当此状态处于活动状态时，系统将保持 OFF-LINE 状态。它等待操作员指令尝试 ON-LINE。
	 * 
	 * ATTEMPT_ON_LINE
	 * 当 ATTEMPT ON-LINE 状态处于活动状态时，设备已响应作员指示，尝试进入 ON-LINE 状态。
	 * 激活此状态后，Equ会尝试将 S1F1 发送到Host。
	 * 注意）当此状态处于活动状态时，系统不会响应作员对 ON-LINE 或 OFF-LINE 开关的驱动。.
	 * 
	 * HOST_OFF_LINE
	 * 此状态为Host不同意Equ尝试联机。
	 * 之所以达到此状态，是因为尝试转到 ON-LINE 失败，或者Host请求Equ从 ON-LINE 脱机。
	 * 当此状态处于活动状态时，Equ应积极响应任何Host的 ON-LINE 请求 （S1F17）。
	 * 当 HOST OFF-LINE 状态未处于活动状态时，此类请求将被拒绝（S1F17）
	 * 
	 * ON-LINE
	 * 当 ON-LINE 状态处于活动状态时，可以交换 SECS-II 消息，并基于该消息执行操作。
	 * 
	 * ON_LINE_LOCAL
	 * 中等级别 允许主机完全访问信息，但有一些限制，具体取决于主机如何影响设备的运行。
	 * 
	 * ON_LINE_REMOTE
	 * 最高级别 允许主机通过 SECS-II 通信接口全面控制设备。可以使用任何流/函数。
	 */
	enum class GemControlState : uint8_t
	{
		OFF_LINE = 0,
		ON_LINE// = 128,
	};

	enum class GemControlSubState : uint8_t
	{
		EQUIPMENT_OFF_LINE = 1,// = 0,
		ATTEMPT_ON_LINE,
		HOST_OFF_LINE,
		ON_LINE_LOCAL,// = 128,
		ON_LINE_REMOTE,
	};

	enum class GemTimeFormat : uint8_t
	{
		YYMMDDHHmmss = 0,
		YYYYMMDDHHmmsscc,
	};

	struct HsmsConfig
	{
		uint16_t m_uDeviceID = 0;
		uint32_t m_uT3 = 0;
		uint32_t m_uT5 = 0;
		uint32_t m_uT6 = 0;
		uint32_t m_uT7 = 0;
		uint32_t m_uT8 = 0;
		uint32_t m_uLinkTestInterval = 0;

		enum {
			Passive = 0,
			Active = 1,
		} m_eConnctMode;

		char		m_acIpAddress[64]{ "127.0.0.1" };
		uint32_t	m_uPort{ 5000 };
	};

	struct GemConfig
	{
		GemType eType{ GemType::EQP };
		GemProtocolType eProtocolType{ GemProtocolType::HSMS };
		bool bEnable{ false };
		GemControlState eInitialState{ GemControlState::OFF_LINE };
		GemControlSubState eOnlineSubState{ GemControlSubState::ON_LINE_REMOTE };
		GemControlSubState eOfflineSubState{ GemControlSubState::ATTEMPT_ON_LINE };
		GemControlSubState eOnlineFailureState{ GemControlSubState::HOST_OFF_LINE };
		GemTimeFormat eTimeFormat{ GemTimeFormat::YYMMDDHHmmss };
		bool bTimeSet{ false };	// 链接成功后是否执行时间匹配
	};


}