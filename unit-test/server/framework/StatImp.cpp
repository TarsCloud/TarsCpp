

#include "StatImp.h"
#include "../FrameworkServer.h"

///////////////////////////////////////////////////////////
//
int StatImp::reportMicMsg( const map<tars::StatMicMsgHead, tars::StatMicMsgBody>& statmsg,bool bFromClient, tars::CurrentPtr current )
{
//	LOG_CONSOLE_DEBUG << "report---------------------------------access size:" << statmsg.size() << "|bFromClient:" <<bFromClient << endl;

    if(bFromClient)
	{
		_clientStatData.push_back(statmsg);
	}
    else
	{
		_serverStatData.push_back(statmsg);
	}

    return 0;
}

int StatImp::reportSampleMsg(const vector<StatSampleMsg> &msg,tars::CurrentPtr current )
{
    TLOGINFO("sample---------------------------------access size:" << msg.size() << endl);

    return 0;
}
