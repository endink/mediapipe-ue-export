#pragma once

#include "ump_api.h"

class DummyPacketCallback : public IUmpPacketCallback
{
public:
	DummyPacketCallback();
	virtual void OnUmpPresence(class IUmpObserver* observer, bool present) override {  }
	virtual bool OnUmpPacket(class IUmpObserver* observer, void* packet) override;
private:

};


