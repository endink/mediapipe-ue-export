#pragma once

#include "ump_object.h"
#include "packet_api.h"
#include "mediapipe/framework/calculator_graph.h"

using UmpObserverBase = UmpObject<IUmpObserver>;

class UmpObserver : public UmpObserverBase
{
protected:
	virtual ~UmpObserver() override { 
		if (_packetAPI != nullptr)
		{
			delete _packetAPI;
		}
		log_d(strf("~UmpObserver %s", *_stream_name)); 
	}

public:
	UmpObserver(const char* in_stream_name) : _stream_name(in_stream_name) { log_d(strf("+UmpObserver %s", *_stream_name)); }


	absl::Status ObserveOutput(mediapipe::CalculatorGraph* graph)
	{
		std::string presence_name(_stream_name);
		presence_name.append("_presence");

		/*graph->ObserveOutputStream(*presence_name, [this](const mediapipe::Packet& pk)
		{
			_presence = pk.Get<bool>();

			if (_callback)
				_callback->OnUmpPresence(this, _presence);

			return absl::OkStatus();
		}, true);*/

		RET_CHECK_OK(graph->ObserveOutputStream(*_stream_name, [this](const mediapipe::Packet& pk)
		{
			if (_callback)
			{
				PROF_NAMED("observer_callback");
				if (!pk.IsEmpty())
				{
					const void* p = &pk;
					auto s = _callback->OnUmpPacket(this, const_cast<void*>(p));
					if (!s)
					{
						return absl::AbortedError("OnUmpPacket error.");
					}
				}
			}

			return absl::OkStatus();
		}, true));

		return absl::OkStatus();
	}

	virtual void SetPacketCallback(IUmpPacketCallback* in_callback) override { _callback = in_callback; }

	virtual class IPacketAPI* GetPacketAPI() override 
	{ 
		if (_packetAPI == nullptr) {
			_packetAPI = new PacketAPI();
		}
		return _packetAPI;
	}

protected:
	std::string _stream_name;
	IUmpPacketCallback* _callback = nullptr;
	bool _presence = false;
	IPacketAPI* _packetAPI = nullptr;
};
