
echo "run-http2.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im Http2Server.exe /t /f

timeout /T 1

echo "start server: %EXE_PATH%\\Http2Server.exe --config=%SRC_PATH%\\examples\\HttpDemo\\Http2Server\\config.conf "

start /b %EXE_PATH%\\Http2Server.exe --config=%SRC_PATH%\\examples\\HttpDemo\\Http2Server\\config.conf

timeout /T 3

echo "client: %EXE_PATH%\\Http2Client.exe"

%EXE_PATH%\\Http2Client.exe --count=10000 --thread=2 --call=sync
%EXE_PATH%\\Http2Client.exe --count=10000 --thread=2 --call=async

timeout /T 1

taskkill /im Http2Server.exe /t /f


