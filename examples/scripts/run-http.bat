
echo "run-http.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im HttpServer.exe /t /f

timeout /T 1

echo "start server: ${EXE_PATH}/HttpServer.exe --config=%SRC_PATH%/examples/HttpDemo/HttpServer/config.conf"

start /b %EXE_PATH%\\HttpServer.exe --config=%SRC_PATH%\\examples\\HttpDemo\\HttpServer\\config.conf

timeout /T 3

echo "client: ${EXE_PATH}/HttpClient.exe"

%EXE_PATH%\\HttpClient.exe --count=10000 --thread=2 --call=basehttp

timeout /T 1

taskkill /im HttpServer.exe /t /f


