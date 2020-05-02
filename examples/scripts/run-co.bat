
echo "run-co.bat"

set EXE_PATH=%1
set SRC_PATH=%2\\..

echo %EXE_PATH% %SRC_PATH%

taskkill /im CoroutineDemoAServer.exe /t /f

timeout /T 1

echo "start server: %EXE_PATH%\\CoroutineDemoAServer.exe --config=%SRC_PATH%\\examples\\CoroutineDemo\\AServer\\config.conf"

start /b %EXE_PATH%\\CoroutineDemoAServer.exe --config=%SRC_PATH%\\examples\\CoroutineDemo\\AServer\\config.conf

::-------------------------------------------------------------------------------------------------------

taskkill /im CoroutineDemoBServer.exe /t /f

timeout /T 1

echo "start server: %EXE_PATH%\\CoroutineDemoBServer.exe --config=%SRC_PATH%\\examples\\CoroutineDemo\\BServer\\config.conf "

start /b %EXE_PATH%\\CoroutineDemoBServer.exe --config=%SRC_PATH%\\examples\\CoroutineDemo\\BServer\\config.conf


::-------------------------------------------------------------------------------------------------------
timeout /T 1

echo "client: .\\bin\\CoroutineDemoClient"

%EXE_PATH%\\CoroutineDemoClient.exe 2 10000 0

%EXE_PATH%\\CoroutineDemoClient.exe 2 10000 1

%EXE_PATH%\\testCoro.exe 1000

%EXE_PATH%\\testParallelCoro.exe 1000

::-------------------------------------------------------------------------------------------------------

timeout /T 1

taskkill /im CoroutineDemoAServer.exe /t /f
taskkill /im CoroutineDemoBServer.exe /t /f



