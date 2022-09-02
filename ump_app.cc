#include "ump_pipeline.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "dummy_packet_callback.h"

#define UMP_UNIQ(_type) std::unique_ptr<_type, IUmpObject::Dtor>

class UmpStdoutLog : public IUmpLog
{
public:
	void Println(EUmpVerbosity verbosity, const char* msg) const override { std::cout << msg << std::endl; }
};

int main(int argc, char* argv[])
{
	std::cout << "== INIT ==" << std::endl;

	google::InitGoogleLogging(argv[0]);
	absl::ParseCommandLine(argc, argv);

	{
		UmpStdoutLog log;
		UMP_UNIQ(IUmpContext) context(UmpCreateContext());
		context->SetLog(&log);
		context->SetResourceDir("");

		UMP_UNIQ(IUmpPipeline) pipe(context->CreatePipeline());
		pipe->SetCaptureFromCamera(0, cv::CAP_DSHOW, 0, 0, 0); // CAP_DSHOW | CAP_MSMF
		pipe->ShowVideoWindow(true);

		std::vector<UMP_UNIQ(IUmpObserver)> observers;

		pipe->SetGraphConfiguration("mediapipe/graphs/holistic_tracking_cpu.pbtxt");
		auto ob = pipe->CreateObserver("pose_landmarks");
		observers.emplace_back(ob);
		observers.emplace_back(pipe->CreateObserver("face_landmarks"));
		observers.emplace_back(pipe->CreateObserver("left_hand_landmarks"));
		observers.emplace_back(pipe->CreateObserver("right_hand_landmarks"));

		auto callback = new DummyPacketCallback();
		ob->SetPacketCallback(callback);

		pipe->Start();
		getchar();
		pipe->Stop();
		delete callback;
		pipe->LogProfilerStats();
	}

	std::cout << "press enter to exit" << std::endl;
	getchar();
	return 0;
}
