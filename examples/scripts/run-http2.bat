
echo "run-http2.bat"

killall -9 Http2Server.exe 
sleep 1

echo "start server: .\\bin\\Release\\Http2Server.exe --config=..\\examples\\HttpDemo\\Http2Server\\config.conf &"

.\\bin\\Release\\Http2Server.exe --config=..\\examples\\HttpDemo\\Http2Server\\config.conf &

sleep 3

echo "client: .\\bin\\Release\\Http2Client.exe"

.\\bin\\Release\\Http2Client.exe --count=10000 --thread=2 --call=synchttp2
.\\bin\\Release\\Http2Client.exe --count=10000 --thread=2 --call=asynchttp2

sleep 1

killall -9 Http2Server.exe 


