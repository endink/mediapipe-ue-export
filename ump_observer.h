#pragma once

#include "ump_object.h"

using UmpObserverBase = UmpObject<IUmpObserver>;

class UmpObserver : public UmpObserverBase
{
protected:
	virtual ~UmpObserver() override { log_d(strf("~UmpObserver %s", *_stream_name)); }

public:
	UmpObserver(const char* in_stream_name) : _stream_name(in_stream_name) { log_d(strf("+UmpObserver %s", *_stream_name)); }

	absl::Status ObserveOutputStream(mediapipe::CalculatorGraph* graph)
	{
		log_i(strf("ObserveOutputStream: %s", *_stream_name));

		std::string presence_name(_stream_name);
		presence_name.append("_presence");

		graph->ObserveOutputStream(*presence_name, [this](const mediapipe::Packet& pk)
		{
			_presence = pk.Get<bool>();

			if (_callback)
				_callback->OnUmpPresence(this, _presence);

			return absl::OkStatus();
		});

		RET_CHECK_OK(graph->ObserveOutputStream(*_stream_name, [this](const mediapipe::Packet& pk)
		{
			if (_callback)
			{
				PROF_NAMED("observer_callback");

				//_raw_data = pk.Get()
				_raw_data = pk.GetRaw(); // requires patched mediapipe\framework\packet.h
				_message_type = pk.GetTypeId().hash_code();

				_callback->OnUmpPacket(this);

				_raw_data = nullptr;
				_message_type = 0;
			}

			return absl::OkStatus();
		}));

		return absl::OkStatus();
	}

	virtual void SetPacketCallback(IUmpPacketCallback* in_callback) override { _callback = in_callback; }
	virtual size_t GetMessageType() override { return _message_type; }
	virtual const void* const GetData() override { return _raw_data; }

protected:
	std::string _stream_name;
	IUmpPacketCallback* _callback = nullptr;
	const void* _raw_data = nullptr;
	size_t _message_type = 0;
	bool _presence = false;
};
