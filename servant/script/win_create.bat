#!/bin/sh

if [ $# -lt 3 ]
then
	echo "<Usage:  $0  App  Server  Servant>"
	exit 0
fi

APP=$1
SERVER=$2
SERVANT=$3

if [ "$SERVER" == "$SERVANT" ]
then
	echo "Error!(ServerName == ServantName)"
	exit -1
fi

if [ ! -d $APP/$SERVER/src ]
then
	echo "[mkdir: $APP/$SERVER/src]"
	mkdir -p $APP/$SERVER/src
	echo "[mkdir: $APP/$SERVER/build]"
	mkdir -p $APP/$SERVER/build
fi

echo "[create server: $APP.$SERVER ...]"

DEMO_PATH=c:\\taf\\cpp\\script\\demo

SRC_FILE="DemoServer.h DemoServer.cpp DemoServantImp.h DemoServantImp.cpp DemoServant.tars makefile CMakeLists.txt"

for FILE in $SRC_FILE
do
	cp -rf $DEMO_PATH\\$FILE $APP\\$SERVER\\src
done

cd $APP\\$SERVER\\src

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
mv DemoServant.jce ${SERVANT}.jce

cd ..\\build
cmake ..\\src -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Debug
cmake ..\\src -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Release

echo "[done.]"
