
echo "run-quick-start.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im QuickStartDemo.exe /t /f
taskkill /im ProxyServer.exe /t /f
timeout /T 1

echo "start server: %EXE_PATH%\\QuickStartDemo.exe --config=%SRC_PATH%\\examples\\QuickStartDemo\\HelloServer\\Server\\config.conf "

start /b %EXE_PATH%\\QuickStartDemo.exe --config=%SRC_PATH%\\examples\\QuickStartDemo\\HelloServer\\Server\\config.conf
start /b %EXE_PATH%\\ProxyServer.exe --config=%SRC_PATH%\\examples\\QuickStartDemo\\ProxyServer\\Server\\config.conf

timeout /T 3

echo "client: %EXE_PATH%\\QuickStartDemoClient.exe"

%EXE_PATH%\\QuickStartDemoClient.exe --count=100000 --call=sync --thread=2 --buffersize=100 --netthread=2

%EXE_PATH%\\QuickStartDemoClient.exe --count=100000 --call=async --thread=2 --buffersize=100 --netthread=2

%EXE_PATH%\\QuickStartDemoClient.exe --count=100000 --call=synctup --thread=2 --buffersize=100 --netthread=2

%EXE_PATH%\\QuickStartDemoClient.exe --count=100000 --call=asynctup --thread=2 --buffersize=100 --netthread=2

echo "client: %EXE_PATH%\\ProxyServerClient.exe"

%EXE_PATH%\\ProxyServerClient.exe

timeout /T 1

taskkill /im QuickStartDemo.exe /t /f
taskkill /im ProxyServer.exe /t /f
