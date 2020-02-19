
echo "run-http.bat"

EXE_PATH=$1
SRC_PATH=$2

echo ${EXE_PATH} ${SRC_PATH}

killall -9 HttpServer.exe 
sleep 1

echo "start server: ${EXE_PATH}/HttpServer.exe --config=${SRC_PATH}/examples/HttpDemo/HttpServer/config.conf &"

${EXE_PATH}/HttpServer.exe --config=${SRC_PATH}/examples/HttpDemo/HttpServer/config.conf &

sleep 3

echo "client: ${EXE_PATH}/HttpClient.exe"

${EXE_PATH}/HttpClient.exe --count=10000 --thread=2 --call=basehttp
#${EXE_PATH}/HttpClient.exe --count=10000 --thread=2 --call=synchttp

sleep 1

killall -9 HttpServer.exe 


