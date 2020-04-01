
echo "run-auth.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im AuthServer.exe /t /f

timeout /T 1

echo "start server: %EXE_PATH%/AuthServer.exe --config=%SRC_PATH%/examples/AuthDemo/Server/config.conf"

start /b %EXE_PATH%\\AuthServer.exe --config=%SRC_PATH%\\examples\\AuthDemo\\Server\\config.conf

timeout /T 3

echo "client: ${EXE_PATH}/AuthClient.exe"

%EXE_PATH%\\AuthClient.exe --count=10000 --thread=2 --call=sync  --buffersize=1000 --netthread=1
%EXE_PATH%\\AuthClient.exe --count=10000 --thread=2 --call=async  --buffersize=1000 --netthread=1

timeout /T 1

taskkill /im AuthServer.exe /t /f




