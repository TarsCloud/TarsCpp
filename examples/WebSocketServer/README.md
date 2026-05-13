WebSocketServer 示例用于演示 `WebSocketProtocol` 的最小可运行闭环：

1. 服务进程同时暴露两个端口：
   - `18081`：返回浏览器测试页
   - `18082`：处理 WebSocket 握手与消息收发
2. 浏览器访问测试页后，可直接连接 `ws://127.0.0.1:18082/ws` 并发送文本消息。
3. 服务端会返回一段文本，包含服务端时间、客户端地址和收到的消息内容。

启动方式示例：

```bash
cd tarscpp/build
cmake --build . --target WebSocketServer
./bin/WebSocketServer --config=../examples/WebSocketServer/config.conf
```

随后在浏览器打开：

```text
http://127.0.0.1:18081/
```
