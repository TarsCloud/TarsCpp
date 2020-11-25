#!/bin/bash

if [ $# -lt 3 ]
then
	echo "<Usage:  $0  App  Server  Servant>"
	exit 0
fi

APP=$1
SERVER=$2
SERVANT=$3

echo "APP:$APP, SERVER:$SERVER, SERVANT:$SERVANT"

if [ "$SERVER" == "$SERVANT" ]
then
	echo "Error!(ServerName == ServantName)"
	exit -1
fi

if [ ! -d $SERVER ]
then
	echo "[mkdir: $SERVER]"
	mkdir -p $SERVER
fi

echo "[create server: $APP.$SERVER ...]"

TARSPATH=$4

if [ "$TARSPATH" == "" ]; then
	TARSPATH=/usr/local/tars
fi 

DEMO_PATH=${TARSPATH}/cpp/script/cmake_http_demo

#make cleanall -C $DEMO_PATH

cp -rf $DEMO_PATH/* $SERVER/

cd $SERVER/src

SRC_FILE="DemoServer.h DemoServer.cpp DemoServantImp.h DemoServantImp.cpp CMakeLists.txt"

for FILE in $SRC_FILE
do
	cat $FILE | sed "s/DemoServer/$SERVER/g" > $FILE.tmp
	mv $FILE.tmp $FILE

	cat $FILE | sed "s/DemoApp/$APP/g" > $FILE.tmp
	mv $FILE.tmp $FILE

	cat $FILE | sed "s/DemoServant/$SERVANT/g" > $FILE.tmp
	mv $FILE.tmp $FILE
done

mv DemoServer.h ${SERVER}.h
mv DemoServer.cpp ${SERVER}.cpp
mv DemoServantImp.h ${SERVANT}Imp.h
mv DemoServantImp.cpp ${SERVANT}Imp.cpp

cd ..
mkdir build; cd build
cmake ..
cmake --build . --config Release

#cd ../../

echo "[done.]"
