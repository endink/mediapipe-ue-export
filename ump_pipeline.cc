#include "ump_pipeline.h"
#include "ump_observer.h"
#include "ump_frame.h"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"

#include "mediapipe/framework/output_stream_poller.h"

#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/util/resource_util.h"

#include <chrono>
#include <thread>
#include <windows.h>

inline double get_timestamp_us() // microseconds
{
	return static_cast<double>(cv::getTickCount()) / (double)cv::getTickFrequency() * 1e6;
}

UmpPipeline::UmpPipeline()
{
	log_d("+UmpPipeline");
	_packet_api.reset(new PacketAPI());
}

absl::Status UmpPipeline::AddImageFrameIntoStream(const char* stream_name, IMediaPipeTexture* texture) const
{
	TRY
	
	 auto image_frame_out =absl::make_unique<mediapipe::ImageFrame>(
	 	static_cast<mediapipe::ImageFormat::Format>(static_cast<int>(texture->GetFormat())),
	 	texture->GetWidth(),
	 	texture->GetHeight(),
	 	texture->GetWidthStep(),
	 	static_cast<uint8*>(texture->GetData()),
	 	[texture](uint8*)
	 	{
	 		texture->Release();
	 	}
	 );
	const auto packet_in = Adopt(image_frame_out.release()).At(mediapipe::Timestamp(static_cast<size_t>(get_timestamp_us())));
	auto status = _graph->AddPacketToInputStream(stream_name, packet_in);
	//delete image_frame_out; //just delete semantics, its moved
	return status;
	CATCH_RETURN_STATUS
}

UmpPipeline::~UmpPipeline()
{
	log_d("~UmpPipeline");
	UmpPipeline::Stop();
	UmpPipeline::ClearObservers();
}

void UmpPipeline::SetGraphConfiguration(const char* filename)
{
	log_i(strf("SetGraphConfiguration: %s", filename));
	_config_filename = filename;
}

void UmpPipeline::SetCaptureFromFile(const char* filename)
{
	log_i(strf("SetCaptureFromFile: %s", filename));
	_input_filename = filename;
}

void UmpPipeline::SetCaptureFromCamera(int cam_id, int cam_api, int cam_resx, int cam_resy, int cam_fps)
{
	log_i(strf("SetCaptureParams: cam=%d api=%d w=%d h=%d fps=%d", cam_id, cam_api, cam_resx, cam_resy, cam_fps));
	_cam_id = cam_id;
	_cam_api = cam_api;
	_cam_resx = cam_resx;
	_cam_resy = cam_resy;
	_cam_fps = cam_fps;
}

void UmpPipeline::ShowVideoWindow(bool show)
{
	log_i(strf("ShowVideo: %d", (show ? 1 : 0)));
	_show_video_winow = show;
}


IUmpObserver* UmpPipeline::CreateObserver(const char* stream_name, long timeoutMillisecond)
{
	log_i(strf("CreateObserver: %s", stream_name));
	if (_run_flag)
	{
		log_e("Invalid state: pipeline running");
		return nullptr;
	}
	auto* observer = new UmpObserver(stream_name, _packet_api, timeoutMillisecond);
	observer->AddRef();
	_observers.emplace_back(observer);
	return observer;
}

void UmpPipeline::SetListener(IUmpPipelineListener* listener)
{
	_listener = listener;
}

void UmpPipeline::SetFrameCallback(class IUmpFrameCallback* callback)
{
	log_i(strf("SetFrameCallback: %p", callback));
	_frame_callback = callback;
}

bool UmpPipeline::Start(void* side_packet)
{
	Stop();
	try
	{
		log_i("UmpPipeline::Start");
		_frame_id = 0;
		_frame_ts = 0;
		_run_flag = true;
		SidePacket packet = side_packet != nullptr ?  *static_cast<SidePacket*>(side_packet) : SidePacket();
		if(side_packet == nullptr)
		{
			log_w("StartImageSource use null packet");
		}
		_worker = std::make_unique<std::thread>([this, packet]() { this->WorkerThread(packet, nullptr); });
		log_i("UmpPipeline::Start OK");
		return true;
	}
	catch (const std::exception& ex)
	{
		log_e(ex.what());
	}
	return false;
}

bool UmpPipeline::StartImageSource(IImageSource* image_source, void* side_packet)
{
	Stop();
	try
	{
		log_i("UmpPipeline::Start");
		_frame_id = 0;
		_frame_ts = 0;
		_run_flag = true;
		SidePacket packet = side_packet != nullptr ?  *static_cast<SidePacket*>(side_packet) : SidePacket();
		if(side_packet == nullptr)
		{
			log_w("StartImageSource use null packet");
		}
		_worker = std::make_unique<std::thread>([this, packet, image_source]() { this->WorkerThread(packet, image_source); });
		log_i("UmpPipeline::Start OK");
		return true;
	}
	catch (const std::exception& ex)
	{
		log_e(ex.what());
	}
	return false;
}

void UmpPipeline::Stop()
{
	try
	{
		_run_flag = false;
		if (_worker)
		{
			log_i("UmpPipeline::Stop");
			_worker->join();
			_worker.reset();
			_frame_id = 0;
			log_i("UmpPipeline::Stop OK");
		}
	}
	catch (const std::exception& ex)
	{
		log_e(ex.what());
	}
}

IPacketAPI* UmpPipeline::GetPacketAPI()
{
	return _packet_api.get();
}

void UmpPipeline::ClearObservers()
{
	_observers.clear();
}

void UmpPipeline::WorkerThread(SidePacket side_packet, IImageSource* image_source)
{
	_image_size_known = false;
	log_i("Enter WorkerThread");
	// RUN
	if(_listener)
	{
		_listener->OnEnterPipelineWorkThread();
	}
	TRY
		auto status = image_source != nullptr ? this->RunImageImpl(side_packet, image_source) : this->RunCaptureImpl(side_packet); 
		if (!status.ok())
		{
			std::string msg(status.message());
			log_e(msg);
			if(_listener)
			{
				_listener->OnPipelineWorkThreadFault();
			}
		}
	CATCH_ONLY
	// SHUTDOWN
	
	TRY
		ShutdownImpl();
	CATCH_ONLY
	log_i("Leave WorkerThread");
	if(_listener)
	{
		_listener->OnExitPipelineWorkThread();;
	}
}

void UmpPipeline::OptimizeGraphConfig(SidePacket& side_packet, mediapipe::CalculatorGraphConfig& config)
{
	//fix: https://github.com/google/mediapipe/issues/3003
	google::protobuf::RepeatedPtrField<mediapipe::CalculatorGraphConfig_Node> nodes = config.node();
	for (int i = 0; i < config.node_size(); i ++)
	{
		mediapipe::CalculatorGraphConfig_Node& node = nodes[i];
		auto findIndex = static_cast<int>(node.calculator().find("FaceGeometry"));
		if(findIndex >= 0 && side_packet.find("refine_face_landmarks") != side_packet.end())
		{
			side_packet["refine_face_landmarks"] = mediapipe::MakePacket<bool>(false);
			log_w("FaceGeometry is enabled, auto disable refine_face_landmarks options.");
			break;
		}
	}
}

absl::Status UmpPipeline::ShutdownImpl()
{
	_frame_id = 0;
	absl::Status status;
	_run_flag = false;
	log_i(strf("CalculatorGraph::CloseInputStream: %d", status.raw_code()));
	status = _graph->CloseAllPacketSources();
	log_i(strf("CalculatorGraph::CloseAllPacketSources: %d", status.raw_code()));
	status = _graph->WaitUntilDone();
	log_i(strf("CalculatorGraph::WaitUntilDone: %d", status.raw_code()));
	_graph.reset();
	if (_show_video_winow)
	{
		cv::destroyAllWindows();
	}
	ReleaseFramePool();
	log_i("UmpPipeline::Shutdown OK");
	
	return absl::OkStatus();
}

absl::Status UmpPipeline::RunImageImpl(SidePacket& side_packet, IImageSource* image_source)
{
	constexpr char kInputStream[] = "input_video";

	log_i("UmpPipeline::Run");

	// init mediapipe

	std::string config_str;
	RET_CHECK_OK(LoadGraphConfig(_config_filename, config_str));

	log_i("Parse Graph Proto");
	mediapipe::CalculatorGraphConfig config{};
	RET_CHECK(mediapipe::ParseTextProto<mediapipe::CalculatorGraphConfig>(config_str, &config));

	OptimizeGraphConfig(side_packet, config);

	log_i("CalculatorGraph::Initialize");
	_graph.reset(new mediapipe::CalculatorGraph());
	RET_CHECK_OK(_graph->Initialize(config));

	for (auto& iter : _observers)
	{
		RET_CHECK_OK(iter->ObserveOutput(_graph.get()));
	}


	std::string str = "CalculatorGraph::StartRun\n";
	if (side_packet.size() > 0)
	{
		for (auto& value : side_packet) {
			str += strf("%s : %s\n", value.first.c_str(), value.second.DebugString().c_str());
		}
	}
	else
	{
		str += "Empty size package used. ";
	}
	log_i(str);

	if(image_source->IsStatic())
	{
		std::string key("static_image_mode");
		side_packet[key] = mediapipe::MakePacket<bool>(true);
		log_i("Static mode used.");
	}
	RET_CHECK_OK(_graph->StartRun(side_packet));

	log_i("------------> Start Loop Work Thread <------------");
	bool first_loop = true;
	bool is_static = image_source->IsStatic();
	auto mills = !is_static ? 33 : 1000;
	while (_run_flag)
	{
		IMediaPipeTexture* image = nullptr;
		if(!image_source->GetTexture(image))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(mills));
			continue;
		}
		if(_listener && !_image_size_known)
		{
			_listener->OnImageSizeReceived(image->GetWidth(),image->GetHeight());
			_image_size_known = true;
		}
		auto status = AddImageFrameIntoStream(kInputStream, image);
		if(!status.ok())
		{
			log_e(strf("AddImageFrameIntoStream failed: %.*s", static_cast<int>(status.message().size()), status.message().data()));
			//image->Release();
			std::this_thread::sleep_for(std::chrono::milliseconds(mills));
			continue;
		}
		if(first_loop)
		{
			log_i("UmpPipeline::AddImageFrameIntoStream (in loop) OK.");
		}
		_frame_id++;
		if (first_loop)
		{
			first_loop = false;
		}
		if(is_static)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(mills));
		}
	}
	return absl::OkStatus();
}


absl::Status UmpPipeline::RunCaptureImpl(SidePacket& side_packet)
{
	constexpr char kInputStream[] = "input_video";
	constexpr char kOutputStream[] = "output_video";
	constexpr char kWindowName[] = "MediaPipe";

	log_i("UmpPipeline::Run");

	// init mediapipe

	std::string config_str;
	RET_CHECK_OK(LoadGraphConfig(_config_filename, config_str));

	log_i("Parse Graph Proto");
	mediapipe::CalculatorGraphConfig config;
	RET_CHECK(mediapipe::ParseTextProto<mediapipe::CalculatorGraphConfig>(config_str, &config));
	OptimizeGraphConfig(side_packet, config);

	log_i("CalculatorGraph::Initialize");
	_graph.reset(new mediapipe::CalculatorGraph());
	RET_CHECK_OK(_graph->Initialize(config));

	for (auto& iter : _observers)
	{
		RET_CHECK_OK(iter->ObserveOutput(_graph.get()));
	}

	std::unique_ptr<mediapipe::OutputStreamPoller> output_poller;
	if (_show_video_winow || (_frame_callback && _frame_callback_enabled))
	{
		//ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller, graph->AddOutputStreamPoller(kOutputStream));
		auto output_poller_sop = _graph->AddOutputStreamPoller(kOutputStream);
		RET_CHECK(output_poller_sop.ok());
		output_poller = std::make_unique<mediapipe::OutputStreamPoller>(std::move(output_poller_sop.value()));
	}

	// init opencv

	log_i("VideoCapture::open");
	cv::VideoCapture capture;
	_use_camera = _input_filename.empty();

	if (_use_camera)
	{
		#if defined(_WIN32)
		if (_cam_api == cv::CAP_ANY)
		{
			// CAP_MSMF is broken on windows! use CAP_DSHOW by default, also see: https://github.com/opencv/opencv/issues/17687
			_cam_api = cv::CAP_DSHOW;
		}
		#endif

		capture.open(_cam_id, _cam_api);
	}
	else
	{
		capture.open(*_input_filename);
	}

	RET_CHECK(capture.isOpened());

	if (_use_camera)
	{
		if (_cam_resx > 0 && _cam_resy > 0)
		{
			capture.set(cv::CAP_PROP_FRAME_WIDTH, _cam_resx);
			capture.set(cv::CAP_PROP_FRAME_HEIGHT, _cam_resy);
		}

		if (_cam_fps > 0)
			capture.set(cv::CAP_PROP_FPS, _cam_fps);
	}

	const int cap_resx = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
	const int cap_resy = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);
	const double cap_fps =capture.get(cv::CAP_PROP_FPS);
	log_i(strf("capture: w=%d h=%d fps=%f, overlay: %s", cap_resx, cap_resy, cap_fps, _show_video_winow ? "true" : "false"));
	if (_show_video_winow)
	{
		cv::namedWindow(kWindowName, cv::WINDOW_AUTOSIZE);
	}

	// start

	cv::Mat cvmat_bgr;
	cv::Mat cvmat_rgb;

	auto frame_dtor = [](UmpFrame* frame) {  };

	RET_CHECK_OK(_graph->StartRun(side_packet));

	std::string str = "CalculatorGraph::StartRun\n";
	if (side_packet.size() > 0)
	{
		for (auto& value : side_packet) {
			str += strf("%s : %s\n", value.first.c_str(), value.second.DebugString().c_str());
		}
	}
	log_i(str);

	double t0 = get_timestamp_us();

	log_i("------------> Start Loop Work Thread <------------");
	bool firstLoop = true;
	int maxEmptyCount = 60;
	while (_run_flag)
	{
		double t1 = get_timestamp_us();
		double dt = t1 - t0;
		t0 = t1;

		PROF_NAMED("pipeline_tick");

		{
			PROF_NAMED("capture_frame");
			capture >> cvmat_bgr;
		}

		if (!_use_camera && cvmat_bgr.empty())
		{
			maxEmptyCount--;
			if (maxEmptyCount > 0)
			{
				continue;
			}
			else
			{
				log_e("VideoCapture: frame is empty !");
				break;
			}
		}
		const double frame_timestamp_us = get_timestamp_us();
		_frame_ts = frame_timestamp_us;

		{
			PROF_NAMED("enque_frame");

			cv::cvtColor(cvmat_bgr, cvmat_rgb, cv::COLOR_BGR2RGB);
			/*if (_use_camera)
				cv::flip(cvmat_rgb, cvmat_rgb, 1);*/

			auto input_mif = absl::make_unique<mediapipe::ImageFrame>(
				mediapipe::ImageFormat::SRGB, cvmat_rgb.cols, cvmat_rgb.rows,
				mediapipe::ImageFrame::kDefaultAlignmentBoundary);

			// TODO: zero copy
			cv::Mat input_mif_view = mediapipe::formats::MatView(input_mif.get());
			cvmat_rgb.copyTo(input_mif_view);
			
			
			RET_CHECK_OK(_graph->AddPacketToInputStream(
				kInputStream,
				mediapipe::Adopt(input_mif.release())
				.At(mediapipe::Timestamp((size_t)frame_timestamp_us))));

			
			if(_listener && !_image_size_known)
			{
				_listener->OnImageSizeReceived(cvmat_rgb.cols,cvmat_rgb.rows);
				_image_size_known = true;
			}

			if (firstLoop)
			{
				log_i("CalculatorGraph::AddPacketToInputStream OK");
			}
		}

		if (output_poller)
		{
			PROF_NAMED("poll_output");

			mediapipe::Packet packet;
			if (!output_poller->Next(&packet))
			{
				log_w("OutputStreamPoller::Next failed");
				continue;
			}

			// TODO: zero copy
			auto& output_mif = packet.Get<mediapipe::ImageFrame>();
			cv::Mat output_mif_view = mediapipe::formats::MatView(&output_mif);

			if (_frame_callback)
			{
				UmpFrame* frame = AllocFrame();
				auto& dst_mat = frame->GetMatrixRef();
				cv::cvtColor(output_mif_view, dst_mat, cv::COLOR_RGB2BGRA); // unreal requires BGRA8 or RGBA8
				frame->_format = EUmpPixelFormat::B8G8R8A8;
				_frame_callback->OnUmpFrame(frame); // unreal should call frame->Release()
			}

			if (_show_video_winow)
			{
				auto stat = strf("%.0f | %.4f | %" PRIu64 "", _frame_ts, dt * 0.001, _frame_id);
				cv::putText(output_mif_view, *stat, cv::Point(10, 20), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0));

				cv::cvtColor(output_mif_view, output_mif_view, cv::COLOR_RGB2BGR);
				cv::imshow(kWindowName, output_mif_view);
				cv::waitKey(1); // required for cv::imshow
			}
		}

		// wait for next frame (when playing from file)
		if (!_use_camera && cap_fps > 0.0)
		{
			PROF_NAMED("wait_next_frame");

			const double frame_us = (1.0 / cap_fps) * 1e6;
			for (;;)
			{
				const double cur_timestamp_us = get_timestamp_us();
				const double delta = fabs(cur_timestamp_us - frame_timestamp_us);
				if (delta >= frame_us)
					break;
				std::this_thread::sleep_for(std::chrono::microseconds((size_t)(frame_us - delta)));
			}
		}

		_frame_id++;
		if (firstLoop)
		{
			firstLoop = false;
		}
	}
	_frame_id = 0;
	firstLoop = true;
	absl::Status status;
	_run_flag = false;
	status = _graph->CloseInputStream(kInputStream);
	log_i(strf("CalculatorGraph::CloseInputStream: %d", status.raw_code()));
	status = _graph->CloseAllPacketSources();
	log_i(strf("CalculatorGraph::CloseAllPacketSources: %d", status.raw_code()));
	status = _graph->WaitUntilDone();
	log_i(strf("CalculatorGraph::WaitUntilDone: %d", status.raw_code()));
	return absl::OkStatus();
}

UmpFrame* UmpPipeline::AllocFrame()
{
	PROF_NAMED("alloc_frame");
	UmpFrame* frame = nullptr;

	if (!_frame_pool.empty())
	{
		std::lock_guard<std::mutex> lock(_frame_mux);
		if (!_frame_pool.empty())
		{
			frame = _frame_pool.back();
			_frame_pool.pop_back();

			//log_d(strf("reuse UmpFrame %p", frame));
			return frame;
		}
	}

	auto* context = this;
	UmpCustomDtor dtor = [context](IUmpObject* obj) { context->ReturnFrameToPool(static_cast<UmpFrame*>(obj)); };
	frame = new UmpFrame(dtor); // frame->Release() triggers custom dtor

	log_d(strf("new UmpFrame %p", frame));
	return frame;
}

void UmpPipeline::ReturnFrameToPool(UmpFrame* frame)
{
	//log_d(strf("pool UmpFrame %p", frame));
	frame->AddRef(); // keep ref counter alive
	std::lock_guard<std::mutex> lock(_frame_mux);
	_frame_pool.push_back(frame);
}

void UmpPipeline::ReleaseFramePool()
{
	// manual delete because frame->Release() triggers ReturnFrameToPool() 
	for (auto* frame : _frame_pool)
	{
		log_d(strf("delete UmpFrame %p", frame));
		delete frame;
	}
	_frame_pool.clear();
}

// allows multiple files separated by ';'
absl::Status UmpPipeline::LoadGraphConfig(const std::string& filename, std::string& out_str)
{
	log_i(strf("LoadGraphConfig: %s", filename.c_str()));

	out_str.clear();
	out_str.reserve(4096);

	std::string sub_str;
	sub_str.reserve(1024);

	std::stringstream filename_ss(filename);
	std::string sub_name;

	while(std::getline(filename_ss, sub_name, ';'))
	{
		sub_str.clear();
		RET_CHECK_OK(LoadResourceFile(sub_name, sub_str));
		out_str.append(sub_str);
	}

	return absl::OkStatus();
}

absl::Status UmpPipeline::LoadResourceFile(const std::string& filename, std::string& out_str)
{
	out_str.clear();

	std::string path;
	ASSIGN_OR_RETURN(path, mediapipe::PathToResourceAsFile(filename));

	RET_CHECK_OK(mediapipe::file::GetContents(path, &out_str));

	return absl::OkStatus();
}

void UmpPipeline::LogProfilerStats() {
	#if defined(PROF_ENABLE)
		log_i(std::string(PROF_SUMMARY));
	#endif
}
