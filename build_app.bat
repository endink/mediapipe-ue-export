echo off
set "SCRIPTS_DIR=%~dp0"

FOR /F %%i in ('where python') do (
    set PY=%%i
	goto GET_PY
)
:GET_PY

set "PY=%PY:\=\\%"

set FN=%~p0
set FN=%FN:~0,-1%
:getfolder
FOR /F "tokens=1,* delims=\/" %%i in ("%FN%") do (
    if not "%%j"=="" (
        set FN=%%j
        goto getfolder
    )
)


cd "%SCRIPTS_DIR%..\..\"

echo on

bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="%PY%" mediapipe/%FN%:ump_app

echo off
cd "%SCRIPTS_DIR%"

pause
