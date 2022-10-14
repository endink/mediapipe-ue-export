:: TODO => USE BAZEL TO DEPLOY?

@echo off

set "UNREAL_PLUGIN_DIR=D:\3D_Works\UE\MediaPipe4U\MediaPipe4U"
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


FOR /F %%i in ('where python') do (
    set PYTHON_EXE=%%i
	goto GET_PY
)
:GET_PY

set "PYTHON_EXE=%PYTHON_EXE:\=\\%"

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
copy /Y "%MP_ROOT%\mediapipe\modules\face_landmark\face_landmark_with_attention.tflite" "%DEPLOY_ROOT%\mediapipe\modules\face_landmark\"
copy /Y "%MP_ROOT%\bazel-bin\mediapipe\modules\face_geometry\data\geometry_pipeline_metadata_landmarks.binarypb" "%DEPLOY_ROOT%\mediapipe\modules\face_geometry\data\"

:: iris

md "%DEPLOY_ROOT%\mediapipe\modules\iris_landmark"

copy /Y "%MP_ROOT%\mediapipe\modules\iris_landmark\iris_landmark.tflite" "%DEPLOY_ROOT%\mediapipe\modules\iris_landmark\"

:: hands

md "%DEPLOY_ROOT%\mediapipe\modules\palm_detection"
md "%DEPLOY_ROOT%\mediapipe\modules\hand_landmark"

copy /Y "%MP_ROOT%\mediapipe\modules\palm_detection\palm_detection.tflite" "%DEPLOY_ROOT%\mediapipe\modules\palm_detection\"
copy /Y "%MP_ROOT%\mediapipe\modules\hand_landmark\handedness.txt" "%DEPLOY_ROOT%\mediapipe\modules\hand_landmark\"
copy /Y "%MP_ROOT%\mediapipe\modules\hand_landmark\hand_landmark_full.tflite" "%DEPLOY_ROOT%\mediapipe\modules\hand_landmark\"

:: pose

md "%DEPLOY_ROOT%\mediapipe\modules\pose_detection"
md "%DEPLOY_ROOT%\mediapipe\modules\pose_landmark"

copy /Y "%MP_ROOT%\mediapipe\modules\pose_detection\pose_detection.tflite" "%DEPLOY_ROOT%\mediapipe\modules\pose_detection\"
copy /Y "%MP_ROOT%\mediapipe\modules\pose_landmark\pose_landmark_full.tflite" "%DEPLOY_ROOT%\mediapipe\modules\pose_landmark\"
copy /Y "%MP_ROOT%\mediapipe\modules\pose_landmark\pose_landmark_lite.tflite" "%DEPLOY_ROOT%\mediapipe\modules\pose_landmark\"
copy /Y "%MP_ROOT%\mediapipe\modules\pose_landmark\pose_landmark_heavy.tflite" "%DEPLOY_ROOT%\mediapipe\modules\pose_landmark\"

:: holistic

md "%DEPLOY_ROOT%\mediapipe\modules\holistic_landmark"

copy /Y "%MP_ROOT%\mediapipe\modules\holistic_landmark\hand_recrop.tflite" "%DEPLOY_ROOT%\mediapipe\modules\holistic_landmark\"
copy /Y "%MP_ROOT%\mediapipe\modules\holistic_landmark\handedness.txt" "%DEPLOY_ROOT%\mediapipe\modules\holistic_landmark\"

:: objectron

md "%DEPLOY_ROOT%\mediapipe\modules\objectron"

copy /Y "%MP_ROOT%\mediapipe\modules\objectron\object_detection_ssd_mobilenetv2_oidv4_fp16.tflite" "%DEPLOY_ROOT%\mediapipe\modules\objectron\"
copy /Y "%MP_ROOT%\mediapipe\modules\objectron\object_detection_oidv4_labelmap.txt" "%DEPLOY_ROOT%\mediapipe\modules\objectron\"
copy /Y "%MP_ROOT%\mediapipe\modules\objectron\object_detection_3d_cup.tflite" "%DEPLOY_ROOT%\mediapipe\modules\objectron\"

:: graphs
copy /Y "%MP_ROOT%\mediapipe\graphs\holistic_tracking\holistic_tracking_cpu.pbtxt" "%SCRIPTS_DIR%\graphs\"
copy /Y "%MP_ROOT%\mediapipe\graphs\pose_tracking\pose_tracking_cpu.pbtxt" "%SCRIPTS_DIR%\graphs\"
copy /Y "%MP_ROOT%\mediapipe\graphs\face_mesh\face_mesh_desktop_live.pbtxt" "%SCRIPTS_DIR%\graphs\"
copy /Y "%MP_ROOT%\mediapipe\modules\face_geometry\face_geometry_from_landmarks.pbtxt" "%SCRIPTS_DIR%\graphs\"

copy /Y "%MP_ROOT%\mediapipe\modules\holistic_landmark\holistic_landmark_cpu.pbtxt" "%SCRIPTS_DIR%\graphs\"



xcopy /Y /E "%SCRIPTS_DIR%\graphs" "%DEPLOY_ROOT%\mediapipe\graphs\"

set /p answer="DEPLOY FILE TO %UNREAL_PLUGIN_DIR% ? (y/n)[n]" :

if not %answer% == y goto QUIT

:: UE4

SET "DLL_DIR=%UNREAL_PLUGIN_DIR%\Source\ThirdParty\mediapipe\DLL\Win64"

md "%DLL_DIR%"
copy /Y "%DEPLOY_ROOT%\mediapipe_api.dll" "%DLL_DIR%\"
:: copy /Y "%DEPLOY_ROOT%\mediapipe_api.dll" "%UNREAL_PLUGIN_DIR%\Binaries\ThirdParty\Win64\"
copy /Y "%DEPLOY_ROOT%\opencv_world3410.dll" "%DLL_DIR%\"
:: copy /Y "%DEPLOY_ROOT%\opencv_world3410.dll" "%UNREAL_PLUGIN_DIR%\ThirdParty\mediapipe\Binaries\Win64\"
copy /Y "%SCRIPTS_DIR%\ump_commons.h" "%UNREAL_PLUGIN_DIR%\Source\MediaPipe\Public\Core\"
copy /Y "%SCRIPTS_DIR%\ump_api.h" "%UNREAL_PLUGIN_DIR%\Source\MediaPipe\Public\Core\"
copy /Y "%SCRIPTS_DIR%\ump_packet.h" "%UNREAL_PLUGIN_DIR%\Source\MediaPipe\Public\Core\"
xcopy /Y /E "%MP_ROOT%\bazel-bin\mediapipe\%FN%\mediapipe" "%UNREAL_PLUGIN_DIR%\Source\ThirdParty\mediapipe\Data\mediapipe\"

cd "%INITIAL_DIR%"

:QUIT
pause