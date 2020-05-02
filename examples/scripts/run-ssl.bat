
echo "run-ssl.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im SSLServer.exe /t /f
timeout /T 1

echo "start server: %EXE_PATH%\\SSLServer.exe --config=%SRC_PATH%\\examples\\SSLDemo\\Server\\config.conf "

start /b %EXE_PATH%\\SSLServer.exe --config=%SRC_PATH%\\examples\\SSLDemo\\Server\\config.conf

sleep 3

echo "client: %EXE_PATH%\\SSLClient.exe"

%EXE_PATH%\\SSLClient.exe --count=10000 --thread=2 --call=sync  --buffersize=1000 --netthread=1
%EXE_PATH%\\SSLClient.exe --count=10000 --thread=2 --call=async  --buffersize=1000 --netthread=1

%EXE_PATH%\\SSLClient.exe --count=10000 --thread=2 --call=syncauth  --buffersize=1000 --netthread=1
%EXE_PATH%\\SSLClient.exe --count=10000 --thread=2 --call=asyncauth  --buffersize=1000 --netthread=1

timeout /T 1

taskkill /im SSLServer.exe /t /f



