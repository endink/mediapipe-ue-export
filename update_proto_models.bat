:: TODO => USE BAZEL TO DEPLOY?
echo off
chcp 65001
setlocal

set "UNREAL_PLUGIN_DIR=D:\3D_Works\UE\MediaPipe4U"
set "INITIAL_DIR=%cd%"

set "SCRIPTS_DIR=%~dp0"
for /f "delims=#" %%i in ('prompt #$E#^&echo on^&for %%a in ^(1^) do rem') do set "ESC_CHAR=%%i"

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

SET "ROOT=%CD%"

if not exist mediapipe ( echo "invalid directory" && exit )

if not exist bazel-bin\mediapipe\modules\face_geometry\data\geometry_pipeline_metadata_landmarks.binarypb (
	bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="%PYTHON_EXE%" mediapipe/modules/face_geometry/data:geometry_pipeline_metadata_landmarks
)

set "BIN_DIR=%ROOT%\bazel-bin"

set "FORMAT=%UNREAL_PLUGIN_DIR%\Source\MediaPipe\Private\mediapipe\framework\formats"
set "MODULES=%UNREAL_PLUGIN_DIR%\Source\MediaPipe\Private\mediapipe\modules"

set "S_FORMAT=%BIN_DIR%\mediapipe\framework\formats"
set "S_MODULES=%BIN_DIR%\mediapipe\modules"


echo off 
setlocal enabledelayedexpansion
call :setESC

for /R %FORMAT% %%f in (*.*) do ( 
@rem echo %%f
@rem echo %%~nxf
set "EMPTY="
set "FPATH=%%f"
set "FPATH=!FPATH:%FORMAT%=%S_FORMAT%!"

set "RR=%%f"
set "RR=!RR:%FORMAT%\=!"
	if exist !FPATH! (
      copy /Y "!FPATH!" "%%f"
	  echo !RR! CPYIED
	 
	) else (
	
	   echo !ESC_CHAR![31m!RR! is not existed!ESC_CHAR![m
	)
)


for /R %MODULES% %%f in (*.*) do ( 
@rem echo %%f
@rem echo %%~nxf
set "EMPTY="
set "FPATH=%%f"
set "FPATH=!FPATH:%MODULES%=%S_MODULES%!"

set "RR=%%f"
set "RR=!RR:%MODULES%\=!"
	if exist !FPATH! (
	 copy /Y "!FPATH!" "%%f"
	  echo !RR! CPYIED
	 
	) else (
	
	   echo !ESC_CHAR![31m!RR! is not existed!ESC_CHAR![m
	)
)

pause