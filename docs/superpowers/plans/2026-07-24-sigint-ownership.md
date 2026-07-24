# SIGINT Ownership Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Preserve Ctrl+C termination for standalone RPC clients while making `Application` the sole SIGINT owner for TARS servers, eliminating concurrent global destruction.

**Architecture:** `Communicator` receives a pre-initialization signal-handling switch that defaults to enabled. `Application` disables that switch on its managed communicator and continues to register its own graceful shutdown callbacks; standalone clients retain the existing default behavior.

**Tech Stack:** C++17, TARS servant/util libraries, CMake, POSIX signals, Bash integration regression test.

## Global Constraints

- Standalone `Communicator` source compatibility and Ctrl+C exit behavior must remain unchanged.
- `Application`-managed communicators must not register their own SIGINT callback.
- `ignore-signal=true` must disable framework signal handling.
- Do not redesign `TC_Port::sighandler` or alter unrelated shutdown behavior.

---

### Task 1: Add an End-to-End SIGINT Regression Test

**Files:**
- Create: `examples/StressDemo/test_sigint_ownership.sh`

**Interfaces:**
- Consumes: `bin/TarsStressServer`, `bin/TarsStressClient`, and `examples/StressDemo/TarsStressServer/config.conf`.
- Produces: an executable regression script accepting the build directory as its optional first argument.

- [ ] **Step 1: Write the failing regression script**

Create a Bash script that:

1. Resolves the build directory from `$1`, defaulting to the repository `build` directory.
2. Starts `TarsStressServer` with its checked-in config and captures output in a temporary directory.
3. Sends SIGINT, waits for exit, and fails if output contains `mutex lock failed`, `libc++abi`, or `abort`.
4. Starts the server again, starts `TarsStressClient` with a very large call count, sends SIGINT to the client, and requires the client to exit within five seconds with status zero.
5. Stops the second server and applies the same clean-server-exit assertions.
6. Uses a trap to terminate remaining child processes and remove only the script-owned temporary directory.

- [ ] **Step 2: Run the script to verify RED**

Run:

```bash
bash examples/StressDemo/test_sigint_ownership.sh build
```

Expected: FAIL during the first server shutdown with output containing `mutex lock failed` or `libc++abi`.

- [ ] **Step 3: Commit the failing test**

```bash
git add examples/StressDemo/test_sigint_ownership.sh
git commit -m "test: cover server and client SIGINT ownership"
```

### Task 2: Separate Application and Standalone Communicator Signal Ownership

**Files:**
- Modify: `servant/servant/Communicator.h`
- Modify: `servant/libservant/Communicator.cpp`
- Modify: `servant/libservant/Application.cpp`
- Test: `examples/StressDemo/test_sigint_ownership.sh`

**Interfaces:**
- Consumes: `Communicator::initialize()`, `Communicator::terminate()`, and `Application::initializeClient()`.
- Produces: `void Communicator::setHandleSignal(bool handleSignal)`; default communicator behavior remains enabled.

- [ ] **Step 1: Add the public pre-initialization setting**

Declare this method in `Communicator`:

```cpp
void setHandleSignal(bool handleSignal);
```

Add private state:

```cpp
bool _handleSignal = true;
size_t _sigId = 0;
```

Implement the setter under `_recMutex` and update `_handleSignal` only before initialization:

```cpp
void Communicator::setHandleSignal(bool handleSignal)
{
    TC_LockT<TC_ThreadRecMutex> lock(_recMutex);
    if (!_initialized)
    {
        _handleSignal = handleSignal;
    }
}
```

- [ ] **Step 2: Make Communicator registration conditional**

In `Communicator::initialize()`, register Ctrl+C only when both conditions hold:

```cpp
if (_handleSignal && getProperty("ignore-signal") != "true")
{
    _sigId = TC_Port::registerCtrlC([&] {
        TC_Common::msleep(50);
        this->terminate();
#if TARGET_PLATFORM_WINDOWS
        ExitProcess(0);
#else
        exit(0);
#endif
    });
}
```

In `Communicator::terminate()`, unregister only a valid registration:

```cpp
if (_sigId != 0)
{
    TC_Port::unregisterCtrlC(_sigId);
    _sigId = 0;
}
```

- [ ] **Step 3: Assign server signal ownership to Application**

Immediately before `_applicationCommunicator->initialize()` in `Application::initializeClient()`, add:

```cpp
_applicationCommunicator->setHandleSignal(false);
```

This leaves the existing Application SIGINT/SIGTERM registration and `ignore-signal` checks intact.

- [ ] **Step 4: Build the affected binaries**

Run:

```bash
cmake --build build --target TarsStressServer TarsStressClient -j4
```

Expected: both targets build successfully without new warnings or errors.

- [ ] **Step 5: Run the regression test to verify GREEN**

Run:

```bash
bash examples/StressDemo/test_sigint_ownership.sh build
```

Expected: PASS; server shutdown emits no mutex/abort diagnostics, and the standalone client exits on SIGINT with status zero.

- [ ] **Step 6: Check source formatting and focused diff**

Run:

```bash
git diff --check
git diff -- servant/servant/Communicator.h servant/libservant/Communicator.cpp servant/libservant/Application.cpp examples/StressDemo/test_sigint_ownership.sh
```

Expected: no whitespace errors and only the planned signal-ownership changes.

- [ ] **Step 7: Commit the implementation**

```bash
git add servant/servant/Communicator.h servant/libservant/Communicator.cpp servant/libservant/Application.cpp
git commit -m "fix: separate server and client SIGINT ownership"
```

### Task 3: Final Verification

**Files:**
- Verify: `docs/superpowers/specs/2026-07-24-sigint-ownership-design.md`
- Verify: `examples/StressDemo/test_sigint_ownership.sh`

**Interfaces:**
- Consumes: the completed regression script and rebuilt binaries.
- Produces: reproducible evidence for clean server shutdown and retained standalone client Ctrl+C behavior.

- [ ] **Step 1: Repeat the regression test**

Run:

```bash
bash examples/StressDemo/test_sigint_ownership.sh build
```

Expected: PASS on a second consecutive run, proving port/process cleanup is reliable.

- [ ] **Step 2: Inspect repository state**

Run:

```bash
git status --short
git log -3 --oneline
```

Expected: only pre-existing generated/untracked files remain; the design, test, and implementation commits are present.
