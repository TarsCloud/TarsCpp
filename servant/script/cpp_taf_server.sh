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

if [ ! -d $APP/$SERVER ]
then
	echo "[mkdir: $APP/$SERVER]"
	mkdir -p $APP/$SERVER
fi

echo "[create server: $APP.$SERVER ...]"

DEMO_PATH=/usr/local/taf/cpp/script/demo

make cleanall -C $DEMO_PATH

cp $DEMO_PATH/* $APP/$SERVER/

cd $APP/$SERVER/

SRC_FILE="DemoServer.h DemoServer.cpp DemoServantImp.h DemoServantImp.cpp DemoServant.jce makefile"

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

cd ../../

echo "[done.]"
