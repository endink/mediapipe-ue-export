set "SCRIPTS_DIR=%~dp0"
call "%SCRIPTS_DIR%workspace.cmd"

cd "%SCRIPTS_DIR%..\..\..\bazel-bin\mediapipe\unreal"
ump_app.exe
cd "%INITIAL_DIR%"
