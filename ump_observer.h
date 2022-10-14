#pragma once

#include "ump_object.h"
#include "packet_api.h"
#include "mediapipe/framework/calculator_graph.h"

using UmpObserverBase = UmpObject<IUmpObserver>;

class UmpObserver : public UmpObserverBase
{
protected:
	virtual ~UmpObserver() override 
	{ 
		log_d(strf("~UmpObserver %s", *_stream_name)); 
	}

public:
	UmpObserver(const char* in_stream_name, const std::shared_ptr<IPacketAPI>& packet_api, long timeoutMillisecond) : 
		_stream_name(in_stream_name), 
		_packet_api{ packet_api }
	{ 
		log_d(strf("+UmpObserver %s", *_stream_name));
		_timeout = timeoutMillisecond * 1000;
	}


	absl::Status ObserveOutput(mediapipe::CalculatorGraph* graph)
	{

		/*graph->ObserveOutputStream(*presence_name, [this](const mediapipe::Packet& pk)
		{
			_presence = pk.Get<bool>();

			if (_callback)
				_callback->OnUmpPresence(this, _presence);

			return absl::OkStatus();
		}, true);*/

		RET_CHECK_OK(graph->ObserveOutputStream(*_stream_name, [this](const mediapipe::Packet& pk)
		{
			if(!_callback)
			{
				log_w(strf("IUmpObserver::OnUmpPacket enter but call back ptr is null. (out stream : %s)", _stream_name.c_str()));
			}
			if (!pk.IsEmpty())
			{
				auto current = pk.Timestamp().Microseconds();
				if(_lastTimestampMicrosec <= 0)
				{
					_lastTimestampMicrosec = current;
				}
				bool hasTimedOut = (static_cast<long>(current) - _lastTimestampMicrosec) >= _timeout;
				if(!hasTimedOut)
				{
					const void* p = &pk;
					const auto succeed = _callback->OnUmpPacket(this, const_cast<void*>(p));
					if (!succeed)
					{
						return absl::AbortedError(strf("IUmpObserver::OnUmpPacket return false (out stream : %s), pipeline will be stopped.", _stream_name.c_str()));
					}
				}
				else
				{
					log_w(strf("IUmpObserver::OnUmpPacket packet timeout. (out stream : %s, current: %d, last: %d, timeout: %d)", _stream_name.c_str(), current, _lastTimestampMicrosec, _timeout));
				}
				_lastTimestampMicrosec = current;
			}
			else
			{
				_lastTimestampMicrosec = -1;
			}
			return absl::OkStatus();
		}, true));

		return absl::OkStatus();
	}

	virtual void SetPacketCallback(IUmpPacketCallback* in_callback) override { _callback = in_callback; }

	virtual IUmpPacketCallback* GetPacketCallback() override { return _callback; }

	virtual class IPacketAPI* GetPacketAPI() override 
	{ 
		return _packet_api.get();
	}



protected:
	std::string _stream_name;
	IUmpPacketCallback* _callback = nullptr;
	bool _presence = false;
	std::shared_ptr<IPacketAPI> _packet_api;
	long _timeout = -1;
	long _lastTimestampMicrosec = -1;
};
