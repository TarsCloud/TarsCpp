#ifndef _Const_SipMsg_String_H_
#define _Const_SipMsg_String_H_

// Register process msg

static const std::string CONST_SIPMSG_REGISTER1  = "REGISTER sip:13957854053@gd.ims.mnc000.mcc460.3gppnetwork.org:5060 SIP/2.0\r\n"
"To: \"13957854053\"<sip:13957854053@gd.ims.mnc000.mcc460.3gppnetwork.org:5060>\r\n"
"From: \"13957854053\"<sip:13957854053@gd.ims.mnc000.mcc460.3gppnetwork.org:5060>;tag=0f9dd7fe01286732\r\n"
"Contact: <sip:13957854053@192.168.8.100:53890;transport=tcp>;expires=5800;+g.3gpp.iari-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.im\";+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\";video\r\n"
"Via: SIP/2.0/UDP 192.168.2.97:5060;branch=z9hG4bKf83a87d9fcd38540\r\n"
"Allow: ACK,INFO\r\n"
"Supported: 100rel,eventlist\r\n"
"Authorization: Digest username=\"460007894556039\",realm=\"gd.ims.mnc000.mcc460.3gppnetwork.org\",nc=120245,uri=\"sip:gd.ims.mnc000.mcc460.3gppnetwork.org:5060\",algorithm=MD5\r\n"
"Call-ID: 18d3937959c2dba3@192.168.8.100\r\n"
"CSeq: 0 REGISTER\r\n"
"Expires: 3600\r\n"
"User-Agent: CPM-client/OMA1.0 MSP/1.0\r\n"
"Allow: ACK,INFO,NOTIFY\r\n"
"Supported: 100rel,eventlist,gruu\r\n"
"Content-Length: 0\r\n\r\n";

static const std::string CONST_SIPMSG_REGISTER_100 = "SIP/2.0 100 Trying\r\n"
"Via: SIP/2.0/UDP 114.55.253.221:5060;branch=z9hG4bK736817DCFD9DACE0\r\n"
"To: \"15802586775\"<sip:15802586775@ebupt.com>\r\n"
"From: \"15802586775\"<sip:15802586775@ebupt.com>;tag=540A250C1016584D\r\n"
"Call-ID: B46BE11D212433E04875C2F580BCA367@114.55.253.221\r\n"
"CSeq: 3 REGISTER\r\n"
"Content-Length: 0\r\n\r\n";

static const std::string CONST_SIPMSG_REGSTER_RSP_401 = "SIP/2.0 401 Unauthorized\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:53890;received=36.23.0.152;rport=22884;branch=z9hG4bKGviwPUTdUJrR_921RfVkH0r43C\r\n"
"To: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=ztesipvXU1qAVj*13-7-16648*heea.13\r\n"
"From: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=GviwPUTdUJrR*9213KPfbGr43C\r\n"
"Call-ID: GviwPUTdUJrR+921QfVkH0r43C@192.168.8.100\r\n"
"CSeq: 1 REGISTER\r\n"
"WWW-Authenticate: Digest realm=\"ims.mnc000.mcc460.3gppnetwork.org\",domain=\"sip:nfrscscf01azx.rcs1.chinamobile.com\",nonce=\"9f1cd6464b451c681bc18245b20ae247\",opaque=\"aW1zLmNvbS5jbg==\",stale=TRUE,algorithm=MD5,qop=\"auth\"\r\n"
"Content-Length: 0\r\n\r\n";

static const std::string CONST_SIPMSG_REGISTER2 = "REGISTER sip:gd.ims.mnc000.mcc460.3gppnetwork.org SIP/2.0\r\n"
"To: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>\r\n"
"From: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=GviwPUTdUJrR*9213KPfbGr43C\r\n"
"Contact: <sip:13957854053@192.168.102.111:5050;transport=tcp>;expires=5800;+g.3gpp.iari-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.im\";+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\";video\r\n"
"Allow: INVITE,ACK,NOTIFY\r\n"
"Supported: 100rel,eventlist,gruu\r\n"
"CSeq: 2 REGISTER\r\n"
"Call-ID: GviwPUTdUJrR+921QfVkH0r43C@192.168.8.100\r\n"
"Authorization: Digest realm=\"ims.mnc000.mcc460.3gppnetwork.org\",opaque=\"aW1zLmNvbS5jbg==\",algorithm=MD5,qop=auth,nonce=\"9f1cd6464b451c681bc18245b20ae247\",response=\"27c7b900ff600abef00c7c7251e2799a\",uri=\"sip:gd.ims.mnc000.mcc460.3gppnetwork.org\",username=\"13957854053\"\r\n"
"Max-Forwards: 0\r\n"
"User-Agent: Yes/Life/1.2.86 (Android 4.4.2)\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:53890;branch=z9hG4bKGviwPUTdUJrR_921RfVkH0r43C\r\n"
"Content-Length: 0\r\n\r\n";

static const std::string CONST_SIPMSG_REGSTER_RSP_200 = "SIP/2.0 200 OK\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:54017;received=36.23.0.152;rport=23031;branch=z9hG4bKaJ8ol4TdNlbJF421bITq20qioP\r\n"
"To: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=ztesipOe_fCXt5nPqD*13-7-16640*hffj.13\r\n"
"From: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=aJ8ol4TdNlbJE4212k_-cqqioP\r\n"
"Call-ID: OQ_9R3Tdpsfnr421_ujHIsra4l@192.168.8.100\r\n"
"CSeq: 3 REGISTER\r\n"
"Contact: <sip:+8613957854053@192.168.8.100:54017;transport=TCP>;+sip.instance=\"<urn:uuid:95CAE616-2C2E-466B-BE73-87BD2BFC3C64>\";expires=1800\r\n"
"P-Associated-URI: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>\r\n"
"P-Associated-URI: <tel:+8613957854053>\r\n"
"Date: Wed, 28 Jan 2015 09:48:11 GMT\r\n"
"Content-Length: 0\r\n\r\n";


// Subscribe msg
static const std::string CONST_SIPMSG_SUBSCRIBE = "SUBSCRIBE sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org SIP/2.0\r\n"
"To: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>\r\n"
"From: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=c1b74994d602251d\r\n"
"Contact: <sip:+8613957854053@255.255.255.255>;+g.3gpp.iari-ref=\"urn%3Aurn-7%3A3gpp-application.ims.iari.rcse.im\";+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\";video\r\n"
"Supported: path,gruu\r\n"
"Event: reg\r\n"
"Allow: NOTIFY,MESSAGE\r\n"
"Accept: application/reginfo+xml\r\n"
"Expires: 3600\r\n"
"Call-ID: OQ_9R3Tdpsfnr421_ujHIsra4l@192.168.8.100\r\n"
"CSeq: 1 SUBSCRIBE\r\n"
"Max-Forwards: 70\r\n"
"User-Agent: CPM-client/OMA1.0 (MSP/1.0)\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:54017;branch=z9hG4bK10a665a56f8df8b7\r\n"
"Content-Length: 0\r\n\r\n";

static const std::string CONST_SIPMSG_SUBSCRIBE_RSP_200 = "SIP/2.0 200 OK\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:53890;received=36.23.0.152;rport=22884;branch=z9hG4bK03C20UTd*bLhd921RfVkH2t_dE\r\n"
"To: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=ztesipmwiT7d8v*17-12-16648*ghae.17\r\n"
"From: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=03C20UTd*bLhc921wG+8bst_dE\r\n"
"Call-ID: GviwPUTdUJrRb921QfVkH0r43C@192.168.8.100\r\n"
"CSeq: 1 SUBSCRIBE\r\n"
"Expires: 1832\r\n"
"Contact: <sip:117.136.239.32:5460;transport=TCP;zte-did=17-12-16648-1192-21-489>\r\n"
"Record-Route: <sip:DIAG_13_0_0d00002b@117.136.239.32:5460;transport=TCP;lr>\r\n"
"Content-Length: 0\r\n\r\n";



// Notify msg
static const std::string CONST_SIPMSG_NOTIFY = "NOTIFY sip:+8613957854053@192.168.8.100:53890;transport=tcp SIP/2.0\r\n"
"Via: SIP/2.0/TCP 117.136.239.32:5460;branch=z9hG4bK-*13*d8089b334632917bb62btaN1\r\n"
"To: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=03C20UTd*bLhc921wG+8bst_dE\r\n"
"From: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=ztesipmwiT7d8v*17-12-16648*ghae.17\r\n"
"Call-ID: GviwPUTdUJrRb921QfVkH0r43C@192.168.8.100\r\n"
"CSeq: 1001 NOTIFY\r\nEvent: reg\r\n"
"Subscription-State: active;expires=1832\r\n"
"Max-Forwards: 69\r\n"
"Contact: <sip:117.136.239.32:5460;transport=TCP;zte-did=17-12-16648-1192-21-489>\r\n"
"Content-Type: application/reginfo+xml\r\nContent-Disposition: session\r\n"
"Content-Length: 766\r\n\r\n"
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<reginfo xmlns=\"urn:ietf:params:xml:ns:reginfo\" xmlns:gr=\"urn:ietf:params:xml:ns:gruuinfo\" xmlns:eri=\"urn:3gpp:ns:extRegInfo:1.0\" version=\"0\" state=\"full\">\r\n"
"   <registration aor=\"sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org\" id=\"2549103\" state=\"active\">\r\n"
"      <contact state=\"active\" event=\"registered\" duration-registered=\"0\" expires=\"1800\" id=\"630978\">\r\n"
"         <uri>sip:+8613957854053@10.188.2.3:5960</uri>\r\n"
"      </contact>\r\n"
"   </registration>\r\n"
"   <registration aor=\"tel:+8613957854053\" id=\"333838\" state=\"active\">\r\n"
"      <contact state=\"active\" event=\"created\" duration-registered=\"0\" expires=\"1800\" id=\"630978\">\r\n"
"         <uri>sip:+8613957854053@10.188.2.3:5960</uri>\r\n"
"      </contact>\r\n"
"   </registration>\r\n"
"</reginfo>\r\n\r\n";

static const std::string CONST_SIPMSG_NOTIFY_RSP_200 = "SIP/2.0 200 OK\r\n"
"From: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=ztesipmwiT7d8v*17-12-16648*ghae.17\r\n"
"To: <sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=03C20UTd*bLhc921wG+8bst_dE\r\n"
"Supported: path,gruu\r\n"
"Call-ID: GviwPUTdUJrRb921QfVkH0r43C@192.168.8.100\r\n"
"CSeq: 1001 NOTIFY\r\n"
"Max-Forwards: 70\r\n"
"Server: CPM-client/OMA1.0 MSP/1.0\r\n"
"Via: SIP/2.0/TCP 117.136.239.32:5460;branch=z9hG4bK-*13*d8089b334632917bb62btaN1;received=117.136.239.32\r\n"
"Content-Length: 0\r\n\r\n";

// Invite msg
static const std::string CONST_SIPMSG_INVITE = "INVITE tel:+8616000000330 SIP/2.0\r\n"
"From: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=qD.M_UTdERd.j921A1V4HarKTO\r\n"
"To: \"8616000000330\"<tel:+8616000000330>\r\n"
"P-Preferred-Identity: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>\r\n"
"Contact: <sip:+8613957854053@192.168.8.100:53890;transport=tcp>;+sip.instance=\"<urn:uuid:95CAE616-2C2E-466B-BE73-87BD2BFC3C64>\r\n"
"Accept-Contact: *;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\r\n"
"P-Early-Media: supported\r\n"
"Supported: 100rel,join,norefersub,replaces\r\n"
"Allow: INVITE,ACK,BYE,CANCEL,UPDATE,REGISTER,INFO,PRACK,SUBSCRIBE,NOTIFY,MESSAGE,REFER,PUBLISH\r\n"
"Call-ID: qD.M_UTdERd.i921Hq_UH0rKTO@192.168.8.100\r\n"
"CSeq: 1 INVITE\r\n"
"Max-Forwards: 70\r\n"
"User-Agent: CPM-client/OMA1.0 MSP/1.0\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:53890;branch=z9hG4bKqD.M_UTdERd.k921Iq_UH0rKTO;rport\r\n"
"Content-Type: application/sdp\r\n"
"Content-Length: 512\r\n\r\n"
"v=0\r\n"
"o=+8613957854053 3698645218 3698645218 IN IP4 192.168.8.100\r\n"
"s=-\r\n"
"c=IN IP4 192.168.8.100\r\n"
"t=0 0\r\n"
"m=audio 37068 RTP/AVP 114 116 103 105 0 8 18 104 97\r\n"
"a=rtpmap:114 red/8000\r\n"
"a=fmtp:114 116/103/105/0/8/18/104\r\n"
"a=rtpmap:116 opus/48000\r\n"
"a=rtpmap:103 AMR/8000\r\n"
"a=fmtp:103 mode-set=0,1,2,3,4,5;octet-align=1\r\n"
"a=rtpmap:105 iLBC/8000\r\n"
"a=fmtp:105 mode=30\r\n"
"a=fmtp:18 annexb=yes\r\n"
"a=rtpmap:104 AMR-WB/16000\r\n"
"a=fmtp:104 mode-set=0,1,2,3,4,5,6;octet-align=1\r\n"
"a=ptime:20\r\n"
"a=maxptime:400\r\n"
"a=rtpmap:97 telephone-event/8000\r\n\r\n";


static const std::string CONST_SIPMSG_INVITE_LINUX = "INVITE sip:+8616000000465@gd.ims.mnc007.mcc460.3gppnetwork.org SIP/2.0\n"
"To: \"+8616000000465\"<sip:+8616000000465@gd.ims.mnc007.mcc460.3gppnetwork.org>\n"
"From: \"+8616000000466\"<sip:+8616000000466@gd.ims.mnc007.mcc460.3gppnetwork.org>;tag=SipGateway+SipProxy+f842878a60a54e35\n"
"Call-ID: R6Bcx27Yad9967c7a085f538@SipGateway\n"
"CSeq: 2062 INVITE\n"
"Via: SIP/2.0/UDP 192.168.2.125:5060;branch=z9hG4bK1e2661a92925a938\n"
"Max-Forwards: 70\n"
"Contact: <sip:+8616000000466@192.168.2.125:5060>\n"
"Allow: INVITE,ACK,BYE\n"
"Supported: 100rel,gruu\n"
"Content-Type: application/sdp\n"
"Content-Length: 344\n\n"
"v=0\n"
"o=13957854053 1023187643 1023187643 IN IP4 192.168.2.125\n"
"c=IN IP4 192.168.2.125\n"
"t=0 0\n"
"m=audio 37076 RTP/AVP 114 116 103 105 0 8 18 104 97\n"
"a=rtpmap:105 AMR/8000\n"
"a=rtpmap:114 red/8000\n"
"a=fmtp:105 mode=30\n"
"a=fmtp:105 mode-set=0,1,2,3,4,5;octet-align=1\n"
"a=fmtp:105 114/116/103/105\n"
"a=rtpmap:116 rtx/8000\n"
"a=rtpmap:180 CelB/5060\n"
"a=sendonly\n"
"a=ptime:20\n\n";




static const std::string CONST_SIPMSG_INVITE_100_TRYING = "SIP/2.0 100 Trying\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:53890;received=36.23.0.152;rport=22884;branch=z9hG4bK03C20UTd*bLhm921zG+8bst_dE\r\n"
"To: \"8616000000330\"<tel:+8616000000330>\r\n"
"From: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=03C20UTd*bLhl921RfVkH2t_dE\r\n"
"Call-ID: 03C20UTd*bLhk921EG+8bst_dE@192.168.8.100\r\n"
"CSeq: 1 INVITE\r\n"
"User-Agent: ZTE-B200\r\n"
"Content-Length: 0\r\n\r\n";



static const std::string CONST_SIPMSG_INVITE_180_RINGNING = "SIP/2.0 180 Ringing\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:53890;received=36.23.0.152;rport=22884;branch=z9hG4bK03C20UTd*bLhm921zG+8bst_dE\r\n"
"To: \"8616000000330\"<tel:+8616000000330>;tag=ztesipSHbs8reA*3-1-20481*ieif.3\r\n"
"From: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=03C20UTd*bLhl921RfVkH2t_dE\r\n"
"Call-ID: 03C20UTd*bLhk921EG+8bst_dE@192.168.8.100\r\n"
"CSeq: 1 INVITE\r\n"
"Contact: <sip:117.136.239.32:5460;transport=TCP;zte-did=3-1-20481-1164-12-573>\r\n"
"Record-Route: <sip:DIAG_13_0_0d00002c@117.136.239.32:5460;transport=TCP;lr>\r\n"
"Supported: 100rel\r\n"
"Allow: REGISTER,INVITE,ACK,CANCEL,BYE,UPDATE,PRACK,REFER,INFO,NOTIFY,MESSAGE,SUBSCRIBE\r\n"
"Server: CPM-client/OMA1.0 MSP/1.0\r\n"
"Accept: application/sdp,multipart/mixed\r\n"
"Content-Length: 0\r\n\r\n";

static const std::string CONST_SIPMSG_INVITE_200_OK = "SIP/2.0 200 OK\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:53890;received=36.23.0.152;rport=22884;branch=z9hG4bK03C20UTd*bLhm921zG+8bst_dE\r\n"
"To: \"8616000000330\"<tel:+8616000000330>;tag=ztesipSHbs8reA*3-1-20481*ieif.3\r\n"
"From: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=03C20UTd*bLhl921RfVkH2t_dE\r\n"
"Call-ID: 03C20UTd*bLhk921EG+8bst_dE@192.168.8.100\r\n"
"CSeq: 1 INVITE\r\n"
"Contact: <sip:Anonymous@117.136.239.32:5460;transport=TCP;zte-did=3-1-20481-1164-12-573>\r\n"
"Record-Route: <sip:DIAG_13_0_0d00002c@117.136.239.32:5460;transport=TCP;lr>\r\n"
"Supported: 100rel,replaces\r\n"
"Session-Expires: 1800;refresher=uas\r\n"
"Privacy: id\r\n"
"Allow: REGISTER,INVITE,ACK,CANCEL,BYE,UPDATE,PRACK,REFER,INFO,NOTIFY,MESSAGE,SUBSCRIBE\r\n"
"Server: CPM-client/OMA1.0 MSP/1.0\r\n"
"Content-Type: application/sdp\r\n"
"Content-Disposition: session\r\n"
"Content-Length: 243\r\n\r\n"
"v=0\r\n"
"o=+8616000000330 973962699 1671092924 IN IP4 117.136.239.79\r\n"
"s=-\r\n"
"c=IN IP4 117.136.239.79\r\n"
"t=0 0\r\n"
"m=audio 20232 RTP/AVP 105 0 8 97\r\n"
"a=rtpmap:105 iLBC/8000\r\n"
"a=fmtp:105 mode=30\r\n"
"a=ptime:20\r\n"
"a=maxptime:400\r\n"
"a=rtpmap:97 telephone-event/8000\r\n\r\n";


// ACK msg
static const std::string CONST_SIPMSG_ACK = "ACK sip:Anonymous@117.136.239.32:5460;transport=tcp;zte-did=3-1-20481-1164-12-573 SIP/2.0\r\n"
"From: \"13957854053\"<sip:+8613957854053@gd.ims.mnc000.mcc460.3gppnetwork.org>;tag=03C20UTd*bLhl921RfVkH2t_dE\r\n"
"To: \"8616000000330\"<tel:+8616000000330>;tag=ztesipSHbs8reA*3-1-20481*ieif.3\r\n"
"Contact: <sip:+8613957854053@192.168.8.100:53890>;+sip.instance=\"<urn:uuid:95CAE616-2C2E-466B-BE73-87BD2BFC3C64>\"\r\n"
"Route: <sip:DIAG_13_0_0d00002c@117.136.239.32:5460;transport=tcp;lr>\r\n"
"Call-ID: 03C20UTd*bLhk921EG+8bst_dE@192.168.8.100\r\n"
"CSeq: 1 ACK\r\n"
"Max-Forwards: 70\r\n"
"User-Agent: CPM-client/OMA1.0 MSP/1.0\r\n"
"Via: SIP/2.0/TCP 192.168.8.100:53890;branch=z9hG4bKaGL.aUTdoUTqn921A9Eubrsmos;rport\r\n"
"Content-Length: 0\r\n\r\n";

//Message msg
static const std::string CONST_SIPMSG_MESSAGE = "MESSAGE sip:+8615888196659@192.168.8.2:50955;transport=TCP SIP/2.0\r\n"
"Via: SIP/2.0/TCP 117.136.239.32:5460;branch=z9hG4bK-*8*915f5b7d21f3eccbe8fftaN1\r\n"
"To: <tel:+8615888196659>\r\n"
"From: <sip:+8613957854053@zj.ims.mnc000.mcc460.3gppnetwork.org>;tag=hyjjxh171.102.801162748.1182333231\r\n"
"Call-ID: 906564884j171.102.3326059@10.188.2.151\r\n"
"CSeq: 100 MESSAGE\r\n"
"Max-Forwards: 68\r\n"
"P-Called-Party-ID: <sip:+8615888196659@zj.ims.mnc000.mcc460.3gppnetwork.org>\r\n"
"P-Asserted-Identity: <sip:+8613957854053@zj.ims.mnc000.mcc460.3gppnetwork.org>\r\n"
"User-Agent: IM-serv\r\n"
"P-Asserted-Service: urn:urn-7:3gpp-service.ims.icsi.oma.cpm.msg\r\n"
"Contribution-ID: HPdcbW81u48Qavgc\r\n"
"Conversation-ID: GOdcbW81u48Qavgc\r\n"
"Date: Thu, 05 Feb 2015 09:58:30 GMT\r\n"
"Accept-Contact: *;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.oma.cpm.msg\"\r\n"
"X-ZTE-Cookie: 7zs4rm1;id=906564884j171.102.3326059@10.188.2.151\r\n"
"Content-Type: message/cpim\r\n"
"Content-Length: 387\r\n\r\n"
"From: +8613957854053<sip:+8613957854053@zj.ims.mnc000.mcc460.3gppnetwo\r\n"
"To: <sip:+8615888196659@zj.ims.mnc000.mcc460.3gppnetwork.org>\r\n"
"NS: imdn<urn:ietf:params:imdn>\r\n"
"imdn.Message-ID: JNdcbW81u48Qavgc\r\n"
"DateTime: 2015-02-05T17:58:30+08:00\r\n"
"imdn.Disposition-Notification: positive-delivery, display\r\n"
"Content-type: text/plain;charset=UTF-8\r\n"
"Content-length: 25\r\n\r\n"
"Hjjjhjjhhhhhh............................................................................................................hjhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhkjhghgf";




//BYE

static const  std::string CONST_SIPMSG_BYE_200 = "SIP/2.0 200 OK\r\n"
"From: <tel:16000000330>;tag=ztesipwNMaFTWus*3-1-20481*iejc.3\r\n"
"To: <tel:+8613957854053>;tag=0JUo6UTd*07JP9216Dpu1dq8EA\r\n"
"Call-ID: PIOX_9qdIpDKSRxjbDWV1bibf@zteims\r\n"
"CSeq: 1001 BYE\r\n"
"Max-Forwards: 70\r\n"
"Server: CPM-client/OMA1.0 MSP/1.0\r\n"
"Via: SIP/2.0/TCP 117.136.239.32:5460;branch=z9hG4bK-*13*616e1f478109fa9eecf8taN1;received=117.136.239.32\r\n"
"Content-Length: 0\r\n\r\n";


static const  std::string CONST_SDP_STRING = "v=0\r\n"
"o=+8613957854053 1023187643 1023187643 IN IP4 192.168.8.100\r\n"
"s=-\r\n"
"c=IN IP4 192.168.8.100\r\n"
"t=0 0\r\n"
"m=audio 37076 RTP/AVP 114 116 103 105 0 8 18 104 97\r\n"
"a=rtpmap:114 red/8000\r\n"
"a=fmtp:114 116/103/105/0/8/18/104\r\n"
"a=rtpmap:116 opus/48000\r\n"
"a=rtpmap:103 AMR/8000\r\n"
"a=fmtp:103 mode-set=0,1,2,3,4,5;octet-align=1\r\n"
"a=rtpmap:105 iLBC/8000\r\n"
"a=fmtp:105 mode=30\r\n"
"a=fmtp:18 annexb=yes\r\n"
"a=rtpmap:104 AMR-WB/16000\r\n"
"a=fmtp:104 mode-set=0,1,2,3,4,5,6;octet-align=1\r\n"
"a=ptime:20\r\n"
"a=maxptime:400\r\n"
"a=rtpmap:97 telephone-event/8000\r\n\r\n";


static const std::string CONST_SDP_1 =   "v=0\r\n"
"o=- 78038937293030595 0 IN IP4 218.205.56.211\r\n"
"s=SBC call\r\n"
"c=IN IP4 218.205.56.211\r\n"
"t=0 0\r\n"
"m=audio 10864 RTP/AVP 8\r\n"
"a=ptime:20\r\n"
"a=rtpmap:8 VND.3GPP.IUFP/16000\r\n\r\n";


static const std::string CONST_SDP_2 =   "v=0\r\n"
"o=- 2025215 0 IN IP4 114.55.253.221\r\n"
"s=SBC call\r\n"
"c=IN IP4 114.55.253.221\r\n"
"t=0 0\r\n"
"m=audio 20526 RTP/AVP 8\r\n"
"a=ptime:20\r\n"
"a=rtpmap:8 VND.3GPP.IUFP/16000\r\n\r\n";



static const std::string CONST_INVITE_200OK =    "SIP/2.0 200 OK\r\n"
"Record-Route: <sip:211.138.24.174:55066;lr=on>\r\n"
"Via: SIP/2.0/UDP 115.29.4.150:5062;rport=20002;branch=z9hG4bKCE90EA5CC90C7D61\r\n"
"From: \"+8615802586775\" <sip:+8615802586775@115.29.4.150;user=phone>;tag=SipGateway.prim.main+SipProxy.prim.0+A9BBB02F37AA71B1\r\n"
"To: \"3102001\" <sip:3102001@211.138.24.174:55066;user=phone>;tag=43rQQy5gD20BS\r\n"
"Call-ID: Rqqz9yln39C7E212C5CFC7F6@SipGateway\r\n"
"CSeq: 6874 INVITE\r\n"
"Contact: <sip:3102001@192.168.101.52:5060;transport=udp>\r\n"
"User-Agent: hisancc\r\n"
"Allow: INVITE, ACK, BYE, CANCEL, OPTIONS, MESSAGE, INFO, UPDATE, REGISTER, REFER, NOTIFY\r\n"
"Supported: path, replaces\r\n"
"Allow-Events: talk, hold, conference, refer\r\n"
"Content-Type: application/sdp\r\n"
"Content-Disposition: session\r\n"
"Content-Length: 268\r\n"
"P-Asserted-Identity: \"Outbound Call\" <sip:3102001@211.138.24.174>\r\n\r\n"
"v=0\r\n"
"o=FreeSWITCH 1496638927 1496638928 IN IP4 211.138.24.174\r\n"
"s=FreeSWITCH\r\n"
"c=IN IP4 211.138.24.174\r\n"
"t=0 0\r\n"
"m=audio 20352 RTP/AVP 0 101\r\n"
"a=rtpmap:0 PCMU/8000\r\n"
"a=rtpmap:101 telephone-event/8000\r\n"
"a=fmtp:101 0-16\r\n"
"a=ptime:60\r\n"
"a=rtcp-mux\r\n"
"a=rtcp:20353\r\n"
"a=nortpproxy:yes\r\n\r\n";


static const std::string CONST_BYE_TWO_BYE = "BYE sip:+8615802586775@115.29.4.150:5062 SIP/2.0\r\n"
"Via: SIP/2.0/UDP 211.138.24.174:55066;branch=z9hG4bKef0b.5aec368b66e2862b2e55da92f676242a.0\r\n"
"Via: SIP/2.0/UDP 192.168.101.52;received=192.168.101.52;rport=5060;branch=z9hG4bK9F82j3cj5aK8H\r\n"
"Max-Forwards: 69\r\n"
"From: \"3102001\" <sip:3102001@211.138.24.174:55066;user=phone>;tag=DF77B0v6e7gQQ\r\n"
"To: \"+8615802586775\" <sip:+8615802586775@115.29.4.150;user=phone>;tag=SipGateway.prim.main+SipProxy.prim.0+5E0DB02BEE955587\r\n"
"Call-ID: Rqv5miXwCDB19D8D4E5FFFB9@SipGateway\r\n"
"CSeq: 108027522 BYE\r\n"
"User-Agent: hisancc\r\n"
"Allow: INVITE, ACK, BYE, CANCEL, OPTIONS, MESSAGE, INFO, UPDATE, REGISTER, REFER, NOTIFY\r\n"
"Supported: path, replaces\r\n"
"Reason: Q.850;cause=16;text=\"NORMAL_CLEARING\"\r\n"
"Content-Length: 0\r\n\r\n";







#endif




