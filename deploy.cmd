:: TODO => USE BAZEL TO DEPLOY?

@echo off

set "PYTHON_EXE=C:\\Users\\endink\\AppData\\Local\\Programs\\Python\\Python310\\python.exe"
set "UNREAL_PLUGIN_DIR=D:\3D_Works\UE\MediaPipe4U"
set "INITIAL_DIR=%cd%"

set "SCRIPTS_DIR=%~dp0"

set FN=%~p0
set FN=%FN:~0,-1%
:getfolder
FOR /F "tokens=1,* delims=\/" %%i in ("%FN%") do (
    if not "%%j"=="" (
        set FN=%%j
        goto getfolder
    )
)
echo Current folder: %FN%

cd "%SCRIPTS_DIR%..\..\"

set "MP_ROOT=%cd%"

if not exist mediapipe ( echo "invalid directory" && exit )

if not exist bazel-bin\mediapipe\modules\face_geometry\data\geometry_pipeline_metadata_landmarks.binarypb (
	bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="%PYTHON_EXE%" mediapipe/modules/face_geometry/data:geometry_pipeline_metadata_landmarks
)

set "DEPLOY_ROOT=%MP_ROOT%\bazel-bin\mediapipe\%FN%"

:: face

md "%DEPLOY_ROOT%\mediapipe\modules\face_detection"
md "%DEPLOY_ROOT%\mediapipe\modules\face_landmark"
md "%DEPLOY_ROOT%\mediapipe\modules\face_geometry\data"

copy /Y "%MP_ROOT%\mediapipe\modules\face_detection\face_detection_short_range.tflite" "%DEPLOY_ROOT%\mediapipe\modules\face_detection\"
copy /Y "%MP_ROOT%\mediapipe\modules\face_landmark\face_landmark.tflite" "%DEPLOY_ROOT%\mediapipe\modules\face_landmark\"
copy /Y "%MP_ROOT%\bazel-bin\mediapipe\modules\face_geometry\data\geometry_pipeline_metadata_landmarks.binarypb" "%DEPLOY_ROOT%\mediapipe\modules\face_geometry\data\"

:: iris

md "%DEPLOY_ROOT%\mediapipe\modules\iris_landmark"

copy /Y "%MP_ROOT%\mediapipe\modules\iris_landmark\iris_landmark.tflite" "%DEPLOY_ROOT%\mediapipe\modules\iris_landmark\"

:: hands

md "%DEPLOY_ROOT%\mediapipe\modules\palm_detection"
md "%DEPLOY_ROOT%\mediapipe\modules\hand_landmark"

copy /Y "%MP_ROOT%\mediapipe\modules\palm_detection\palm_detection.tflite" "%DEPLOY_ROOT%\mediapipe\modules\palm_detection\"
copy /Y "%MP_ROOT%\mediapipe\modules\hand_landmark\hand_landmark.tflite" "%DEPLOY_ROOT%\mediapipe\modules\hand_landmark\"
copy /Y "%MP_ROOT%\mediapipe\modules\hand_landmark\handedness.txt" "%DEPLOY_ROOT%\mediapipe\modules\hand_landmark\"

:: pose

md "%DEPLOY_ROOT%\mediapipe\modules\pose_detection"
md "%DEPLOY_ROOT%\mediapipe\modules\pose_landmark"

copy /Y "%MP_ROOT%\mediapipe\modules\pose_detection\pose_detection.tflite" "%DEPLOY_ROOT%\mediapipe\modules\pose_detection\"
copy /Y "%MP_ROOT%\mediapipe\modules\pose_landmark\pose_landmark_full.tflite" "%DEPLOY_ROOT%\mediapipe\modules\pose_landmark\"

:: holistic

md "%DEPLOY_ROOT%\mediapipe\modules\holistic_landmark"

copy /Y "%MP_ROOT%\mediapipe\modules\holistic_landmark\hand_recrop.tflite" "%DEPLOY_ROOT%\mediapipe\modules\holistic_landmark\"

:: objectron

md "%DEPLOY_ROOT%\mediapipe\modules\objectron"

copy /Y "%MP_ROOT%\mediapipe\modules\objectron\object_detection_ssd_mobilenetv2_oidv4_fp16.tflite" "%DEPLOY_ROOT%\mediapipe\modules\objectron\"
copy /Y "%MP_ROOT%\mediapipe\modules\objectron\object_detection_oidv4_labelmap.txt" "%DEPLOY_ROOT%\mediapipe\modules\objectron\"
copy /Y "%MP_ROOT%\mediapipe\modules\objectron\object_detection_3d_cup.tflite" "%DEPLOY_ROOT%\mediapipe\modules\objectron\"

:: graphs

xcopy /Y /E "mediapipe\unreal2\graphs" "%DEPLOY_ROOT%\mediapipe\unreal\"

set /p answer="DEPLOY FILE TO %UNREAL_PLUGIN_DIR% ?" (Y/N)[n]:

if not %answer% == y goto QUIT

:: UE4

SET "DLL_DIR=%UNREAL_PLUGIN_DIR%\Source\ThirdParty\mediapipe\DLL\Win64"

md "%DLL_DIR%"
copy /Y "%DEPLOY_ROOT%\mediapipe_api.dll" "%DLL_DIR%\"
copy /Y "%DEPLOY_ROOT%\opencv_world3410.dll" "%DLL_DIR%\"
:: copy /Y "%DEPLOY_ROOT%\opencv_world3410.dll" "%UNREAL_PLUGIN_DIR%\ThirdParty\mediapipe\Binaries\Win64\"
copy /Y "%SCRIPTS_DIR%ump_api.h" "%UNREAL_PLUGIN_DIR%\Source\MediaPipe\Public\"
xcopy /Y /E "%MP_ROOT%\bazel-bin\mediapipe\%FN%\mediapipe" "%UNREAL_PLUGIN_DIR%\Source\ThirdParty\mediapipe\Data\mediapipe\"

cd "%INITIAL_DIR%"

:QUIT
pause