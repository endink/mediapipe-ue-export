#pragma once

#include <cstdint>
#include <queue>

#include "ump_commons.h"
#include "ump_packet.h"

//
// Log
//

enum class EUmpVerbosity
{
	Error = 0,
	Warning,
	Info,
	Debug
};

class IUmpLog
{
protected:
	virtual ~IUmpLog() {}

public:
	virtual void Println(EUmpVerbosity verbosity, const char* msg) const { (void)verbosity; (void)msg; }
};



//
// Object
//

class IMediaPipeTexture
{
public:
	virtual long GetImageId() const = 0;
	//get uint8*
	virtual void* GetData() const = 0;
	virtual MediaPipeImageFormat GetFormat() const = 0;
	virtual int GetWidthStep() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual void Release() const = 0;
};

class IImageSource
{
public:
	virtual  IMediaPipeTexture* GetTexture() = 0;
};

class IUmpObject
{
protected:
	virtual ~IUmpObject() {}

public:
	struct Dtor { void operator()(IUmpObject* obj) { obj->Release(); } };
	virtual void Release() = 0;
	virtual void AddRef() = 0;
};

//
// Context
//

class IUmpContext : public IUmpObject
{
public:
	virtual void SetLog(IUmpLog* log) = 0;
	virtual void SetResourceDir(const char* resource_dir) = 0;
	virtual class IUmpPipeline* CreatePipeline() = 0;
};

extern "C"
{
	MP_API IUmpContext* UmpCreateContext();
	typedef IUmpContext* UmpCreateContext_Proto();
}
//
// Pipeline
//

class IUmpPipeline : public IUmpObject
{
public:
	virtual void SetGraphConfiguration(const char* filename) = 0;
	virtual void SetCaptureFromFile(const char* filename) = 0;
	virtual void SetCaptureFromCamera(int cam_id, int cam_api, int cam_resx, int cam_resy, int cam_fps) = 0;
	virtual void ShowVideoWindow(bool show) = 0;
	virtual void EnableFrameCallback(bool enabled) = 0;
	virtual bool IsFrameCallbackEnabled() = 0;
	virtual class IUmpObserver* CreateObserver(const char* stream_name) = 0;
	virtual void SetFrameCallback(class IUmpFrameCallback* callback) = 0;
	virtual bool Start(void* side_packet = nullptr) = 0;
	virtual bool StartImageSource(IImageSource* image_source, void* side_packet = nullptr) =0;
	virtual void Stop() = 0;
	virtual IPacketAPI* GetPacketAPI() = 0;

	// debug
	virtual void LogProfilerStats() = 0;
	virtual uint64_t GetLastFrameId() = 0;
	virtual double GetLastFrameTimestamp() = 0;
};

//
// Observer
//

class IUmpPacketCallback
{
public:
	virtual void OnUmpPresence(class IUmpObserver* observer, bool present) = 0;
	virtual bool OnUmpPacket(class IUmpObserver* observer, void* packet) = 0;
};

class IUmpObserver : public IUmpObject
{
public:
	virtual void SetPacketCallback(IUmpPacketCallback* callback) = 0;
	virtual class IPacketAPI* GetPacketAPI() = 0;
	virtual IUmpPacketCallback* GetPacketCallback() = 0;
};


//
// Frame callback
//


enum class EUmpPixelFormat
{
	Unknown = 0,
	B8G8R8A8,
	R8G8B8A8,
	NUM_ITEMS // last
};

class IUmpFrame : public IUmpObject
{
public:
	virtual const void* GetData() const = 0;
	virtual EUmpPixelFormat GetFormat() const = 0;
	virtual int GetPitch() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
};

class IUmpFrameCallback
{
public:
	virtual void OnUmpFrame(IUmpFrame* frame) = 0;
};




