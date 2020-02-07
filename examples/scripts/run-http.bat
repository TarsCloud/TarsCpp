
echo "run-http.bat"

killall -9 HttpServer.exe 
sleep 1

echo "start server: .\\bin\\Release\\HttpServer.exe --config=..\\examples\\HttpDemo\\HttpServer\\config.conf &"

.\\bin\\Release\\HttpServer.exe --config=..\\examples\\HttpDemo\\HttpServer\\config.conf &

sleep 3

echo "client: .\\bin\\Release\\HttpClient.exe"

.\\bin\\Release\\HttpClient.exe 2 10000

sleep 1

killall -9 HttpServer.exe 


