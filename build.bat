echo off
set "SOURCE_DIR=%~dp0"
pushd %~dp0
cd..
cd..
set BAZEL_VS="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise"
set BAZEL_VC="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC"
set BAZEL_VC_FULL_VERSION=14.29.30133
@rem C:\Program Files (x86)\Windows Kits
set BAZEL_WINSDK_FULL_VERSION=10.0.19041.0

set FN=%~p0
set FN=%FN:~0,-1%
:getfolder
FOR /F "tokens=1,* delims=\/" %%i in ("%FN%") do (
    if not "%%j"=="" (
        set FN=%%j
        goto getfolder
    )
)

FOR /F %%i in ('where python') do (
    set PY=%%i
	goto GET_PY
)
:GET_PY

set "PY=%PY:\=\\%"

echo on
@rem bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="C:\\Users\\endink\\AppData\\Local\\Programs\\Python\\Python310\\python.exe" mediapipe/examples/desktop/hello_world

bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="%PY%" mediapipe/%FN%:mediapipe_api --verbose_failures

pause