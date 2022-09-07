
echo "run-push-callback.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im PushCallbackServer.exe /t /f

timeout /T 1

echo "start server: %EXE_PATH%/PushCallbackServer.exe --config=%SRC_PATH%/examples/PushCallbackDemo/Server/config.conf"

start /b %EXE_PATH%\\PushCallbackServer.exe --config=%SRC_PATH%\\examples\\PushCallbackDemo\\Server\\config.conf

timeout /T 3

echo "client: ${EXE_PATH}/PushCallbackClient.exe"

%EXE_PATH%\\PushCallbackClient.exe

timeout /T 1

taskkill /im PushCallbackServer.exe /t /f




