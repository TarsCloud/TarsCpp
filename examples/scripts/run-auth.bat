
echo "run-auth.bat"

killall -9 AuthServer.exe
sleep 1

echo "start server: .\\bin\\Release\\AuthServer.exe --config=..\\examples\\AuthDemo\\Server\\config.conf &"

.\\bin\\Release\\AuthServer.exe --config=..\\examples\\AuthDemo\\Server\\config.conf &

sleep 3

echo "client: .\\bin\\Release\\SSLClient.exe"

.\\bin\\Release\\SSLClient.exe --count=10000 --thread=2 --call=sync  --buffersize=1000 --netthread=1
.\\bin\\Release\\SSLClient.exe --count=10000 --thread=2 --call=async  --buffersize=1000 --netthread=1

sleep 1

killall -9 AuthServer.exe


