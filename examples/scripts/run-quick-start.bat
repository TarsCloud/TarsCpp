
echo "run-quick-start.bat"

killall -9 QuickStartDemo.exe 
killall -9 roxyServer.exe
sleep 1

echo "start server: .\\bin\\Release\\QuickStartDemo.exe --config=..\\examples\\QuickStartDemo\\HelloServer\\Server\\config.conf &"

.\\bin\\Release\\QuickStartDemo.exe --config=..\\examples\\QuickStartDemo\\HelloServer\\Server\\config.conf &
.\\bin\\Release\\ProxyServer.exe --config=..\\examples\\QuickStartDemo\\ProxyServer\\Server\\config.conf &

sleep 3

echo "client: .\\bin\\Release\\QuickStartDemoClient.exe"

.\\bin\\Release\\QuickStartDemoClient.exe --count=100000 --call=sync --thread=2 --buffersize=100 --netthread=2

.\\bin\\Release\\QuickStartDemoClient.exe --count=100000 --call=async --thread=2 --buffersize=100 --netthread=2

.\\bin\\Release\\QuickStartDemoClient.exe --count=100000 --call=synctup --thread=2 --buffersize=100 --netthread=2

.\\bin\\Release\\QuickStartDemoClient.exe --count=100000 --call=asynctup --thread=2 --buffersize=100 --netthread=2

echo "client: .\\bin\\Release\\ProxyServerClient.exe"

.\\bin\\Release\\ProxyServerClient.exe

sleep 2

killall -9 ProxyServer.exe
killall -9 QuickStartDemo.exe


