#pragma once

#include "standard.h"
#include <vector>
namespace why
{
	typedef std::vector<uint8_t> DataBuffer;

	enum class PType : uint8_t
	{
		PTYPE_SECSII = 0,
	};

	enum class SType : uint8_t
	{
		STYPE_DATA_MESSAGE = 0,
		STYPE_SELECT_REQ = 1,
		STYPE_SELECT_RSP = 2,
		STYPE_DESELECT_REQ = 3,
		STYPE_DESELECT_RSP = 4,
		STYPE_LINKTEST_REQ = 5,
		STYPE_LINKTEST_RSP = 6,
		STYPE_REJECT_REQ = 7,
		STYPE_SEPARATE_REQ = 9,
	};

	enum class SelectStatus
	{
		SUCCESSFULLY = 0,
		ALREADY_HAS_SELECTED = 1,
		CONNECTION_NOT_READY = 2,
		CONNECT_EXHAUST = 3,
	};

	enum class DeselectStatus
	{
		SUCCESSFULLY = 0,
		COMMUNICATION_NOT_ESTABLISHED = 1,
		COMMUNICATION_BUSY = 2,
	};

	enum class RejectReasonCode
	{
		STYPE_NOT_SUPPORT = 1,
		PTYPE_NOT_SUPPORT = 2,
		TRANSACTION_NOT_OPEN = 3,
		ENTITY_NOT_SELECTED = 4,
	};

	enum class HsmsState
	{
		Disconnected,
		Connected_Deselected,
		Connected_Selected
	};

	
	// 设置对齐规则
#pragma pack(push, 1)
	struct HsmsHead
	{
		static const uint32_t uLength = 10;

		uint16_t		uDeviceID{ 0 };
		uint8_t			uStream : 7;
		bool			bWBit : 1;
		uint8_t			uFunction{ 0 };
		uint8_t			uPType{ 0 };
		SType 			eSType{ SType::STYPE_DATA_MESSAGE };
		uint32_t		uSystemByte;
	};
#pragma pack(pop)

	struct HsmsMessage
	{
		uint32_t					uLength = 0;
		HsmsHead					head;
		DataBuffer 					data;
	};
}