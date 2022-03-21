#ifndef __TC_DOCKER_H_
#define __TC_DOCKER_H_

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <assert.h>

#include <stdio.h>

#include "util/tc_autoptr.h"
using namespace std;

namespace tars
{
class TC_HttpRequest;
class TC_HttpResponse;

/////////////////////////////////////////////////
// 说明: docker http request/response
/////////////////////////////////////////////////

/**
 * 操作docker, 提供几个关键的api, tarsregistry/tarsnode中会使用到
 * 注意:
 * - 同步等待操作docker, 会阻塞
 */
class TC_Docker
{
public:

	/**
	 * 构造
	 */
	TC_Docker() {}

	/**
	 * 设置请求超时时间(ms)
	 * @param requestTimeout
	 */
	void setRequestTimeout(size_t requestTimeout) { _requestTimeout = requestTimeout; }

	/**
	 * 操作docker unixSocket 的地址(默认是: /var/run/docker.sock)
	 * @param unixSocket
	 */
	void setDockerUnixLocal(const string &unixSocket);

	/**
	 * api的version, 默认是: v1.41
	 * @param apiVersion
	 */
	void setDockerApiVersion(const string &apiVersion) { _dockerApiVersion = apiVersion; }

	/**
	 * 获取操作结果的数据
	 * @return
	 */
	const string &getResponseMessage() { return _responseMessage; }

	/**
	 * 获取操作结果的数据
	 * @return
	 */
	const string &getErrMessage() { return _errMessage; }

	/**
	 * 设置授权
	 * @param useName
	 * @param password
	 * @param serverAddress
	 */
	void setAuthentication(const string &useName, const string &password, const string &serverAddress);

	/**
	 * 登录
	 * @param useName
	 * @param password
	 * @param email
	 * @param serverAddress
	 *
	 * @return
	 */
	bool login(const string &useName, const string &password, const string &serverAddress);

	/**
	 * 同步拉取镜像
	 * @param image
	 */
	bool pull(const string &image);

	/**
	 * 创建容器(https://docs.docker.com/engine/api/v1.41/#operation/ContainerStart)
	 * @param containerId
	 * @param name
	 * @param image
	 * @param entrypoint
	 * @param commands
	 * @param envs
	 * @param mounts: target: source
	 * @param ports: <port>/<protocol>: <hostIp, hostPort>, 80/tcp:<127.0.0.1, 80>
	 * @param restartPolicy: "" "no" "always" "unless-stopped" "on-failure"
	 * @param maximumRetryCount: 最大启动次数 restartPolicy == "on-failure" 才有效
	 * @param networkMode: bridge, host, none, and container:<name|id>
	 * @param ipcMode: "none", "private", "shareable", "container:<name|id>", "host"
	 * @param autoRemove
	 * @param privileged
	 * @return
	 */
	bool create(const string &name,
			const string &image,
			const vector<string> &entrypoint,
			const vector<string> &commands,
			const vector<string> &envs,
			const map<string, string> &mounts,
			const map<string, pair<string, int>> &ports,
			const string &restartPolicy,
			int maximumRetryCount,
			const string &networkMode,
			const string &ipcMode,
			bool autoRemove,
			bool privileged);

	/**
	 * 启动容器(https://docs.docker.com/engine/api/v1.41/#operation/ContainerStart)
	 * @param containerId: 容器Id or name
	 * @return
	 */
	bool start(const string &containerId);

	/**
	 * 停止容器
	 * @param containerId: 容器Id or name
	 * @param waitSeconds: kill之前等待时间
	 * @return
	 */
	bool stop(const string &containerId, size_t waitSeconds);

	/**
	 * 删除容器
	 * @param containerId 容器Id or name
	 * @param force; 强制删除(正在运行则kill然后删除)
	 * @return
	 */
	bool remove(const string &containerId, bool force);

	/**
	 * 查看容器的信息
	 * @param containerId 容器Id or name
	 * @return
	 */
	bool inspectContainer(const string &containerId);

	/**
	 * 查看容器的信息
	 * @param imageId: 镜像id or name
	 * @return
	 */
	bool inspectImage(const string &imageId);

protected:
	/**
	 * 创建http请求
	 * @param req
	 * @return
	 */
	bool createHttpRequest(TC_HttpRequest &req);

	struct Authentication
	{
		string useName;
		string password;
		string serverAddress;
	};
protected:

	/**
	 *
	 */
	Authentication _authentication;

	/**
	 * 授权字符串
	 */
	string 		   _authenticationStr;

	/**
	 * 请求超时时间(ms)
	 */
	size_t _requestTimeout = 60 * 1000;

	string _dockerUrl = "$var$run$docker.sock:0";

	string _dockerApiVersion = "v1.40";

	string _responseMessage;

	string _errMessage;
};
}

#endif

