set "SCRIPTS_DIR=%~dp0"
call "%SCRIPTS_DIR%workspace.cmd"

cd "%SCRIPTS_DIR%..\..\..\"
bazel clean --expunge
cd "%INITIAL_DIR%"
