
echo "run-push.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im CustomServer.exe /t /f

timeout /T 1

echo "start server: %EXE_PATH%\\CustomServer.exe --config=%SRC_PATH%\\examples\\CustomDemo\\CustomServer\\config.conf "

start /b %EXE_PATH%\\CustomServer.exe --config=%SRC_PATH%\\examples\\CustomDemo\\CustomServer\\config.conf

timeout /T 1

::-------------------------------------------------------------------------------------------------------

echo "client: %EXE_PATH%\\CustomClient.exe"

%EXE_PATH%\\CustomClient.exe --count=10000 --thread=2 --call=sync --netthread=2 --buffersize=100
%EXE_PATH%\\CustomClient.exe --count=10000 --thread=2 --call=async --netthread=2 --buffersize=100

taskkill /im CustomServer.exe /t /f

