
echo "run-push.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im PushServer.exe /t /f

timeout /T 1

echo "start server: %EXE_PATH%\\PushServer.exe --config=%SRC_PATH%\\examples\\PushDemo\\PushServer\\config.conf"

start /b %EXE_PATH%\\PushServer.exe --config=%SRC_PATH%\\examples\\PushDemo\\PushServer\\config.conf

timeout /T 3

::-------------------------------------------------------------------------------------------------------

echo "client: %EXE_PATH%\\PushClient.exe"

%EXE_PATH%\\PushClient.exe 5

taskkill /im PushServer.exe /t /f

