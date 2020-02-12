
echo "run-http.bat"

killall -9 HttpServer.exe 
sleep 1

echo "start server: .\\bin\\Release\\HttpServer.exe --config=..\\examples\\HttpDemo\\HttpServer\\config.conf &"

.\\bin\\Release\\HttpServer.exe --config=..\\examples\\HttpDemo\\HttpServer\\config.conf &

sleep 3

echo "client: .\\bin\\Release\\HttpClient.exe"

.\\bin\\Release\\HttpClient.exe --count=10000 --thread=2 --call=basehttp
.\\bin\\Release\\HttpClient.exe --count=10000 --thread=2 --call=synchttp

sleep 1

killall -9 HttpServer.exe 


