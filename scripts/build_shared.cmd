set "SCRIPTS_DIR=%~dp0"
call "%SCRIPTS_DIR%workspace.cmd"

cd "%SCRIPTS_DIR%..\..\..\"
bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 --action_env PYTHON_BIN_PATH="%PYTHON_EXE%" mediapipe/unreal:ump_shared
cd "%INITIAL_DIR%"
