#ifndef UMP_PIPELINE_H
#define UMP_PIPELINE_H

#include "ump_object.h"
#include "mediapipe/framework/calculator_graph.h"

using UmpPipelineBase = UmpObject<IUmpPipeline>;

class UmpPipeline : public UmpPipelineBase
{
	typedef std::map<std::string, mediapipe::Packet> SidePacket;
protected:
	virtual ~UmpPipeline() override;

public:
	UmpPipeline();

	int AddImageFrameIntoStream(const char* stream_name, MediaPipeImageFormat format, int width, int height, int width_step,  uint8* pixel_data);
	virtual void SetGraphConfiguration(const char* filename) override;
	virtual void SetCaptureFromFile(const char* filename) override;
	virtual void SetCaptureFromCamera(int cam_id, int cam_api, int cam_resx, int cam_resy, int cam_fps) override;
	virtual void ShowVideoWindow(bool show) override;
	inline virtual void EnableFrameCallback(bool enabled) override { _frame_callback_enabled = enabled; };
	inline virtual bool IsFrameCallbackEnabled() override { return _frame_callback_enabled; };
	virtual IUmpObserver* CreateObserver(const char* stream_name) override;
	virtual void SetFrameCallback(class IUmpFrameCallback* callback) override;
	virtual bool Start(void* side_packet) override;
	virtual bool StartImageSource(IImageSource* image_source, void* side_packet) override;
	virtual void Stop() override;
	virtual IPacketAPI* GetPacketAPI() override;

	virtual void LogProfilerStats() override;
	virtual uint64_t GetLastFrameId() override { return _frame_id; }
	virtual double GetLastFrameTimestamp() override { return _frame_ts; }

private:
	void WorkerThread(SidePacket side_packet, IImageSource* image_source);
	void ShutdownImpl();
	absl::Status RunImageImpl(SidePacket side_packet, IImageSource* image_source);
	absl::Status RunCaptureImpl(SidePacket side_packet);
	absl::Status LoadGraphConfig(const std::string& filename, std::string& out_str);
	absl::Status LoadResourceFile(const std::string& filename, std::string& out_str);
	class UmpFrame* AllocFrame();
	void ReturnFrameToPool(class UmpFrame* frame);
	void ReleaseFramePool();

private:
	std::string resource_dir;
	std::string _config_filename;
	std::string _input_filename;
	int _cam_id = -1;
	int _cam_api = 0;
	int _cam_resx = 0;
	int _cam_resy = 0;
	int _cam_fps = 0;
	bool _use_camera = false;
	bool _show_video_winow = false;
	bool _frame_callback_enabled = true;

	using ObserverPtr = std::unique_ptr<class UmpObserver, IUmpObject::Dtor>;
	std::list<ObserverPtr> _observers;

	std::list<class UmpFrame*> _frame_pool;
	class IUmpFrameCallback* _frame_callback = nullptr;
	std::mutex _frame_mux;

	std::shared_ptr<mediapipe::CalculatorGraph> _graph;
	std::shared_ptr<IPacketAPI> _packet_api;

	std::unique_ptr<std::thread> _worker;
	std::atomic<bool> _run_flag;

	uint64_t _frame_id = 0;
	double _frame_ts = 0;
};

#endif
