# SIGINT Ownership Design

## Problem

`Application` and its internal `Communicator` both register SIGINT callbacks.
`TC_Port` executes those callbacks on a detached worker thread. The
`Communicator` callback calls `exit(0)`, which starts global destruction on that
worker while the application's main thread and framework threads are still
active. On macOS this can destroy `ServantProxyThreadData` state before
`Communicator::~Communicator()` tries to lock it, producing:

```text
std::__1::system_error: mutex lock failed: Invalid argument
```

## Required Behavior

- A standalone `Communicator` keeps its current default SIGINT behavior so a
  looping RPC client still exits on Ctrl+C.
- A communicator owned by `Application` does not install its own SIGINT
  callback. `Application` is the sole owner of graceful server shutdown.
- `ignore-signal=true` continues to disable framework signal handling.
- Existing standalone client source code requires no changes.

## Design

Add an explicit signal-handling setting to `Communicator`. It defaults to
enabled, preserving standalone client compatibility. The setting must be
configured before `Communicator::initialize()`.

`Application::initializeClient()` disables communicator-level signal handling
for the communicator it manages, then initializes it. `Application` retains its
existing SIGINT and SIGTERM callbacks when `ignore-signal` is not true.

`Communicator::initialize()` registers its Ctrl+C callback only when signal
handling is enabled and `ignore-signal` is not true. `Communicator::terminate()`
unregisters the callback only if it was registered.

## Lifecycle

### Server

1. `Application` creates or obtains its communicator.
2. `Application` disables communicator-level signal handling.
3. `Communicator` initializes without registering SIGINT.
4. `Application` registers its graceful SIGINT/SIGTERM callbacks.
5. Ctrl+C terminates the epoll server and communicator through the normal
   `Application::waitForShutdown()` path.
6. `main()` returns only after shutdown completes; no callback calls `exit()`
   during global destruction.

### Standalone Client

1. A standalone `Communicator` retains signal handling by default.
2. Initialization registers its Ctrl+C callback.
3. Ctrl+C terminates communicator threads and exits the client as before.

## Tests

- A server process based on `Application` starts, receives SIGINT, exits
  normally, and emits neither `mutex lock failed` nor an abort.
- A standalone looping client initializes a `Communicator`, receives SIGINT,
  and exits without requiring application code to handle the signal.
- A communicator configured with `ignore-signal=true` does not install its
  callback.

## Scope

This change only separates signal ownership. It does not redesign
`TC_Port::sighandler`, change client exit semantics, or alter unrelated
communicator shutdown behavior.
