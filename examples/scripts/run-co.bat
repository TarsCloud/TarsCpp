
echo "run-co.bat"


killall -9 CoroutineDemoAServer.exe 

sleep 1

echo "start server: .\\bin\\Release\\CoroutineDemoAServer.exe --config=..\\examples\\CoroutineDemo\\AServer\\config.conf &"

.\\bin\\Release\\CoroutineDemoAServer.exe --config=..\\examples\\CoroutineDemo\\AServer\\config.conf &

#-------------------------------------------------------------------------------------------------------

killall -9 CoroutineDemoBServer.exe 

sleep 1

echo "start server: .\\bin\\Release\\CoroutineDemoBServer.exe --config=..\\examples\\CoroutineDemo\\BServer\\config.conf &"

.\\bin\\Release\\CoroutineDemoBServer.exe --config=..\\examples\\CoroutineDemo\\BServer\\config.conf &


#-------------------------------------------------------------------------------------------------------
sleep 1

echo "client: .\\bin\\CoroutineDemoClient"

.\\bin\\Release\\CoroutineDemoClient.exe 2 10000 0

.\\bin\\Release\\CoroutineDemoClient.exe 2 10000 1

.\\bin\\Release\\testCoro.exe 1000

.\\bin\\Release\\testParallelCoro.exe 1000

#-------------------------------------------------------------------------------------------------------

sleep 1

killall -9 CoroutineDemoAServer.exe 
killall -9 CoroutineDemoBServer.exe 


