#pragma once

#include "standard.h"
#include <functional>

namespace jca
{
	/**
	 * @class IProcedureCall
	 * @brief 过程调用接口
	 */
	class IProcedureCall
	{
	public:
		/**
		 * @brief 当前线程和过程调用初始化的线程是否为同一个线程
		 * @return
		 * - true 同一个线程
		 * - false 非同一个线程
		 */
		virtual bool IsSameThread() = 0;

		/**
		 * @brief 使用指定参数同步调用一次函数过程
		 * @param uParam 将被传给过程的实参
		 * @return void
		 * @warning 所谓同步，即当本函数调用返回的时候，创建过程调用接口传入的函数过程一定已经被执行
		 */
		virtual void CallSynchronously(uint32_t uParam) = 0;

		/**
		 * @brief 使用指定参数异步调用一次函数过程
		 * @param uParam 将被传给过程的实参
		 * @return void
		  * @warning 所谓异步，即当本函数调用返回的时候，创建过程调用接口传入的函数过程不一定已经被执行
		 */
		virtual void CallAsynchronous(uint32_t uParam) = 0;

		/**
		 * @brief 销毁过程调用接口
		 * @return void
		 */
		virtual void Destory() = 0;
	};

	/**
	* @brief 获取窗口发送消息的过程调用接口实例，将执行主体从当前线程转移到窗口线程。可能是为了处理UI变化？
	* @param callProce 过程调用接口将调用用于处理真实任务的函数对象
	* @return 过程调用接口实例
	*/
	IProcedureCall* GetProcedureCallInstance(const std::function<void(uint32_t)>& callProce);
}