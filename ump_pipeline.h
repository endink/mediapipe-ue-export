#pragma once

#include "ump_object.h"

using UmpPipelineBase = UmpObject<IUmpPipeline>;

class UmpPipeline : public UmpPipelineBase
{
protected:
	virtual ~UmpPipeline() override;

public:
	UmpPipeline();

	virtual void SetGraphConfiguration(const char* filename) override;
	virtual void SetCaptureFromFile(const char* filename) override;
	virtual void SetCaptureParams(int cam_id, int cam_api, int cam_resx, int cam_resy, int cam_fps) override;
	virtual void SetOverlay(bool overlay) override;
	virtual IUmpObserver* CreateObserver(const char* stream_name) override;
	virtual void SetFrameCallback(class IUmpFrameCallback* callback) override;
	virtual bool Start() override;
	virtual void Stop() override;

	virtual void LogProfilerStats() override;
	virtual uint64_t GetLastFrameId() override { return _frame_id; }
	virtual double GetLastFrameTimestamp() override { return _frame_ts; }

private:
	void WorkerThread();
	void ShutdownImpl();
	absl::Status RunImpl();
	absl::Status LoadGraphConfig(const std::string& filename, std::string& out_str);
	absl::Status LoadResourceFile(const std::string& filename, std::string& out_str);
	class UmpFrame* AllocFrame();
	void ReturnFrameToPool(class UmpFrame* frame);
	void ReleaseFramePool();

private:
	std::string resource_dir;
	std::string _config_filename;
	std::string _input_filename;
	int _cam_id = 0;
	int _cam_api = 0;
	int _cam_resx = 0;
	int _cam_resy = 0;
	int _cam_fps = 0;
	bool _use_camera = false;
	bool _show_overlay = false;

	using ObserverPtr = std::unique_ptr<class UmpObserver, IUmpObject::Dtor>;
	std::list<ObserverPtr> _observers;

	std::list<class UmpFrame*> _frame_pool;
	class IUmpFrameCallback* _frame_callback = nullptr;
	std::mutex _frame_mux;

	std::shared_ptr<mediapipe::CalculatorGraph> _graph;

	std::unique_ptr<std::thread> _worker;
	std::atomic<bool> _run_flag;

	uint64_t _frame_id = 0;
	double _frame_ts = 0;
};
