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

DEMO_PATH=/usr/local/tars/cpp/script/demo

#make cleanall -C $DEMO_PATH

cp -rf $DEMO_PATH/* $SERVER/

cd $SERVER

SRC_FILE="DemoServer.h DemoServer.cpp DemoServantImp.h DemoServantImp.cpp DemoServant.tars makefile"

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
mv DemoServant.tars ${SERVANT}.tars

make

echo "[done.]"
