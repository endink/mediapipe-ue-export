#include "ump_pipeline.h"
#include "mediapipe/framework/port/opencv_video_inc.h"

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
		pipe->SetCaptureParams(0, cv::CAP_DSHOW, 0, 0, 0); // CAP_DSHOW | CAP_MSMF
		pipe->SetOverlay(true);

		std::vector<UMP_UNIQ(IUmpObserver)> observers;

		#if 0
		pipe->SetGraphConfiguration("mediapipe/unreal/pose_landmarks.pbtxt");
		observers.emplace_back(pipe->CreateObserver("pose_landmarks"));
		#endif

		#if 0
		pipe->SetGraphConfiguration("mediapipe/unreal/holistic_landmarks.pbtxt");
		observers.emplace_back(pipe->CreateObserver("pose_landmarks"));
		observers.emplace_back(pipe->CreateObserver("face_landmarks"));
		observers.emplace_back(pipe->CreateObserver("left_hand_landmarks"));
		observers.emplace_back(pipe->CreateObserver("right_hand_landmarks"));
		#endif

		#if 0
		pipe->SetGraphConfiguration("mediapipe/unreal/face_landmarks_with_iris.pbtxt");
		observers.emplace_back(pipe->CreateObserver("multi_face_landmarks"));
		observers.emplace_back(pipe->CreateObserver("left_eye_contour_landmarks"));
		observers.emplace_back(pipe->CreateObserver("left_iris_landmarks"));
		observers.emplace_back(pipe->CreateObserver("left_eye_rect_from_landmarks"));
		observers.emplace_back(pipe->CreateObserver("right_eye_contour_landmarks"));
		observers.emplace_back(pipe->CreateObserver("right_iris_landmarks"));
		observers.emplace_back(pipe->CreateObserver("right_eye_rect_from_landmarks"));
		#endif

		#if 1
		pipe->SetGraphConfiguration("mediapipe/unreal/objectron_landmarks.pbtxt");
		observers.emplace_back(pipe->CreateObserver("objectron_landmarks"));
		#endif

		pipe->Start();
		getchar();
		pipe->Stop();
		pipe->LogProfilerStats();
	}

	std::cout << "press enter to exit" << std::endl;
	getchar();
	return 0;
}
