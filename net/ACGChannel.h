#pragma once

#include <functional>

#include "../base/acgNoCopyable.h"
#include "../base/acgTimestamp.h"

namespace acg
{
	namespace net
	{

		/*
		struct ACGOVERLAPPED
		{
			WSAOVERLAPPED m_wsaol;
			SOCKET        m_socket;        //投递操作的SOCKET句柄
		};
		*/

		class ACGEventLoop;

		class ACGChannel : public acg::base::CACGNoCopyable
		{
		public:
			typedef std::function<void()> EventCallback;
			typedef std::function<void(acg::base::CACGTimestamp)> ReadEventCallback;

			ACGChannel(ACGEventLoop* loop, HANDLE handle);
			~ACGChannel();

		private:

			static const SHORT sNoneEvent;
			static const SHORT sReadEvent;
			static const SHORT sWriteEvent;

			ACGEventLoop *m_loop;
			const HANDLE m_hanle;
			int32_t          m_index;
			//ACGOVERLAPPED m_ov = {};
			SHORT        m_event;
			SHORT        m_revent;

			ReadEventCallback m_readEventCallback;
			EventCallback     m_writeEventCallback;
			EventCallback     m_closeEventCallback;
			EventCallback     m_errorEventCallback;

		public:
			void HandleEvent(acg::base::CACGTimestamp receiveTime);

			//设置事件回调
			void SetReadCallback(ReadEventCallback cb){ m_readEventCallback = std::move(cb); }
			void SetWriteCallback(EventCallback cb){ m_writeEventCallback = std::move(cb);}
			void SetCloseCallback(EventCallback cb){ m_closeEventCallback = std::move(cb);}
			void SetErrorCallback(EventCallback cb){ m_errorEventCallback = std::move(cb);}

			void EnableReadCallback() { m_event |= sReadEvent; Update(); }
			void DisableReadCallback() { m_event &= ~sReadEvent; Update(); }
			void EnableWriteCallback() { m_event |= sWriteEvent; Update(); }
			void DisableWriteCallback() { m_event &= ~sWriteEvent; Update(); }
			void DisableAll() { m_event = sNoneEvent; Update(); }
			bool IsWriting() const { return m_event & sWriteEvent; }
			bool IsReading() const { return m_event & sReadEvent; }

			HANDLE GetHandle() const { return m_hanle; }
			void SetREvent(SHORT revent) { m_revent = revent; }
			void SetIndex(int32_t index) { m_index = index; }
			int32_t GetIndex() const { return m_index; }

		private:
			void Update();


		};
	}
}
