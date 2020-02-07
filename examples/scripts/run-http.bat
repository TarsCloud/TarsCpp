
echo "run-http.bat"

killall -2 HttpServer.exe 
sleep 1

echo "start server: .\\bin\\Release\\HttpServer.exe --config=..\\examples\\HttpDemo\\HttpServer\\config.conf &"

.\\bin\\Release\\HttpServer.exe --config=..\\examples\\HttpDemo\\HttpServer\\config.conf &

sleep 1

echo "client: .\\bin\\Release\\HttpClient.exe"

.\\bin\\Release\\HttpClient.exe 2 10000

sleep 1

killall -2 HttpServer.exe 


