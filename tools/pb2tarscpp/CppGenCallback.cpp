// Generates C++ tars service interface out of Protobuf IDL.
//
// This is a Proto2 compiler plugin.  See net/proto2/compiler/proto/plugin.proto
// and net/proto2/compiler/public/plugin.h for more information on plugins.

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/plugin.pb.h>
#include <google/protobuf/descriptor.h>

#include "CppGenCallback.h"

static std::string GenCallbackMethod(const ::google::protobuf::MethodDescriptor* method, const std::string& pkg, int indent) {
    std::string out;
    out.reserve(8 * 1024);

    out = "virtual void callback_" + method->name() + "(const " + ToCppNamespace(method->output_type()->full_name()) + "& ret)" + LineFeed(indent);
    out += "{ throw std::runtime_error(\"callback_" + method->name() + " override incorrect.\"); }" + LineFeed(indent);

    out += "virtual void callback_" + method->name() + "_exception(tars::Int32 ret)" + LineFeed(indent);
    out += "{ throw std::runtime_error(\"callback_" + method->name() + "_exception() override incorrect.\"); }" + LineFeed(indent);

    return out;
}

static std::string GenResponseContext(int indent) {
    std::string out;
    out.reserve(8 * 1024);

    out += "virtual const map<std::string, std::string> & getResponseContext() const";
    out += LineFeed(indent);
    out += "{";
    out += LineFeed(++indent);
    out += "CallbackThreadData * pCbtd = CallbackThreadData::getData();";
    out += LineFeed(indent);
    out += LineFeed(indent);
    out += "assert(pCbtd != NULL);";
    out += LineFeed(indent);
    out += "if (!pCbtd->getContextValid())";
    out += LineFeed(indent);
    out += "{";
    out += LineFeed(++indent);
    out += "throw TC_Exception(\"can't get response context\");";
    out += LineFeed(--indent);
    out += "}";
    out += LineFeed(indent);
    out += "return pCbtd->getResponseContext();";
    out += LineFeed(--indent);
    out += "}";

    return out;
}

std::string GenPrxCallback(const ::google::protobuf::ServiceDescriptor* desc, int indent) {
    std::string out;
    out.reserve(8 * 1024);

    const auto& name = desc->name();
    const auto& pkg = desc->file()->package();
    out += "/* callback of async proxy for client */";
    out += LineFeed(indent);
    out += "class " + name + "PrxCallback : public tars::ServantProxyCallback" + LineFeed(indent);
    out += "{";
    out += LineFeed(indent);
    out += "public:" + LineFeed(++indent) + "virtual ~" + name + "PrxCallback() {}";
    out += LineFeed(indent);
    out += LineFeed(indent);

    //sort by method name
	std::map<std::string, const ::google::protobuf::MethodDescriptor*> m_method;
	for (int i = 0; i < desc->method_count(); ++i)
	{
		m_method[desc->method(i)->name()] = desc->method(i);
	}

	for(auto it = m_method.begin(); it != m_method.end(); ++it)
	{
		auto method = it->second;
		out += GenCallbackMethod(method, pkg, indent);
	}

    out += LineFeed(indent);
    out += LineFeed(indent);

    out += GenResponseContext(indent);

    // gen onDispatch
    out += LineFeed(indent);
    out += LineFeed(indent);
    out +=  "virtual int onDispatch(tars::ReqMessagePtr msg)";
    out += LineFeed(indent);
    out += "{";
    out += LineFeed(++indent);
    out += "static ::std::string __all[] = ";
    out += "{";
    out += LineFeed(++indent);
    for(auto it = m_method.begin(); it != m_method.end(); ++it)
	{
    	auto method = it->second;
        out += "\"" + method->name() + "\",";
        out += LineFeed(indent);
    }
    out += LineFeed(--indent);
    out += "};" + LineFeed(indent);
    out += "pair<string*, string*> r = equal_range(__all, __all + " + std::to_string((long long)desc->method_count()) + ", " + "std::string(msg->request.sFuncName));";
    out += LineFeed(indent);
    out += "if(r.first == r.second) return tars::TARSSERVERNOFUNCERR;" + LineFeed(indent);
    out += "switch(r.first - __all)" + LineFeed(indent);
    out += "{";
    out += LineFeed(++indent);
    int i = 0;
    for(auto it = m_method.begin(); it != m_method.end(); ++it)
	{
    	auto method = it->second;
        out += LineFeed(indent);
        out += "case " + std::to_string((long long)i) + ":" + LineFeed(indent);
        out += "{" + LineFeed(++indent);
        out += "if (msg->response->iRet != tars::TARSSERVERSUCCESS)" + LineFeed(indent);
        out += "{" + LineFeed(++indent);
        out += "callback_" + method->name() + "_exception(msg->response->iRet);" + LineFeed(indent);
        out += "return msg->response->iRet;" + LineFeed(--indent) + "}";
    
        out += LineFeed(indent);
        out += ToCppNamespace(method->output_type()->full_name()) + " _ret;" + LineFeed(indent);
        out += "_ret.ParseFromArray(msg->response->sBuffer.data(), msg->response->sBuffer.size());" + LineFeed(indent);
        out += "CallbackThreadData * pCbtd = CallbackThreadData::getData();" + LineFeed(indent);
        out += "assert(pCbtd != NULL);" + LineFeed(indent);
        out += LineFeed(indent);
        out += "pCbtd->setResponseContext(msg->response->context);" + LineFeed(indent);
        out += "callback_" + method->name() + "(_ret);" + LineFeed(indent);
        out += "pCbtd->delResponseContext();" + LineFeed(indent);
        out += LineFeed(indent);
        out += "return tars::TARSSERVERSUCCESS;";

        out += LineFeed(--indent);
        out += "}";

        ++i;
    }

    // end switch
    out += LineFeed(--indent);
    out += "}";

    out += LineFeed(indent);
    out += LineFeed(indent);
    out += "return tars::TARSSERVERNOFUNCERR;";
    out += LineFeed(--indent);
    out += "}"; // end of onDispatch
    out += LineFeed(--indent);
    out += "};"; // end of class CallbackPrx

    out += LineFeed(indent);
    out += "typedef tars::TC_AutoPtr<" + name + "PrxCallback> " + name + "PrxCallbackPtr;";
    out += LineFeed(indent);

    return out;
}

