#!/usr/bin/env bash

set -u

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_dir="$(cd "${script_dir}/../.." && pwd)"
build_dir="${1:-${repo_dir}/build}"
server_bin="${build_dir}/bin/TarsStressServer"
client_bin="${build_dir}/bin/TarsStressClient"
config_file="${script_dir}/TarsStressServer/config.conf"
test_tmp_dir="$(mktemp -d "${TMPDIR:-/tmp}/tars-sigint.XXXXXX")"
server_pid=""
client_pid=""

cleanup()
{
    if [[ -n "${client_pid}" ]] && kill -0 "${client_pid}" 2>/dev/null; then
        kill -TERM "${client_pid}" 2>/dev/null || true
        wait "${client_pid}" 2>/dev/null || true
    fi

    if [[ -n "${server_pid}" ]] && kill -0 "${server_pid}" 2>/dev/null; then
        kill -TERM "${server_pid}" 2>/dev/null || true
        wait "${server_pid}" 2>/dev/null || true
    fi

    rm -rf "${test_tmp_dir}"
}

trap cleanup EXIT

fail()
{
    echo "FAIL: $*" >&2
    exit 1
}

require_executable()
{
    [[ -x "$1" ]] || fail "missing executable: $1"
}

start_server()
{
    local log_file="$1"

    "${server_bin}" --config="${config_file}" >"${log_file}" 2>&1 &
    server_pid=$!
    sleep 1

    if ! kill -0 "${server_pid}" 2>/dev/null; then
        cat "${log_file}" >&2
        fail "TarsStressServer exited during startup"
    fi
}

wait_for_exit()
{
    local pid="$1"
    local name="$2"

    for _ in {1..50}; do
        if ! kill -0 "${pid}" 2>/dev/null; then
            return 0
        fi
        sleep 0.1
    done

    fail "${name} did not exit within five seconds"
}

wait_status()
{
    local pid="$1"

    if wait "${pid}"; then
        return 0
    fi
    return $?
}

assert_clean_server_exit()
{
    local log_file="$1"

    if grep -Eiq "mutex lock failed|libc\\+\\+abi|(^|[^[:alpha:]])abort([^[:alpha:]]|$)" "${log_file}"; then
        cat "${log_file}" >&2
        fail "TarsStressServer aborted during SIGINT shutdown"
    fi
}

require_executable "${server_bin}"
require_executable "${client_bin}"

first_server_log="${test_tmp_dir}/server-first.log"
start_server "${first_server_log}"
kill -INT "${server_pid}"
wait_for_exit "${server_pid}" "TarsStressServer"
wait_status "${server_pid}" || true
server_pid=""
assert_clean_server_exit "${first_server_log}"

second_server_log="${test_tmp_dir}/server-second.log"
client_log="${test_tmp_dir}/client.log"
start_server "${second_server_log}"

"${client_bin}" \
    1 \
    1000000000 \
    "TestApp.StressServer.StressObj@tcp -h 127.0.0.1 -p 9300 -t 10000" \
    100 \
    sync >"${client_log}" 2>&1 &
client_pid=$!
sleep 1

if ! kill -0 "${client_pid}" 2>/dev/null; then
    cat "${client_log}" >&2
    fail "TarsStressClient exited before SIGINT"
fi

kill -INT "${client_pid}"
wait_for_exit "${client_pid}" "TarsStressClient"
if wait_status "${client_pid}"; then
    client_status=0
else
    client_status=$?
fi
client_pid=""

if [[ "${client_status}" -ne 0 ]]; then
    cat "${client_log}" >&2
    fail "TarsStressClient exited with status ${client_status}"
fi

kill -INT "${server_pid}"
wait_for_exit "${server_pid}" "TarsStressServer"
wait_status "${server_pid}" || true
server_pid=""
assert_clean_server_exit "${second_server_log}"

echo "PASS: server shutdown is clean and standalone client handles SIGINT"
