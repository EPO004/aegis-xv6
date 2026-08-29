#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

cpu_count="${CPUS:-1}"
timeout_seconds="${XV6_TIMEOUT:-30}"
output_file="${XV6_OUTPUT:-/tmp/aegis-xv6-smoke.txt}"

mkdir -p "$(dirname "$output_file")"

make clean
make -j4 kernel/kernel fs.img

export CPUS="$cpu_count"
export XV6_TIMEOUT="$timeout_seconds"

set +e
expect <<'EXPECT' >"$output_file" 2>&1
set timeout $env(XV6_TIMEOUT)

spawn make CPUS=$env(CPUS) qemu

expect {
    -re {init: starting sh} {
        send "\001x"
        expect eof
        exit 0
    }
    timeout {
        send "\001x"
        expect eof
        exit 1
    }
    eof {
        exit 2
    }
}
EXPECT
status=$?
set -e

if [[ $status -ne 0 ]]; then
    cat "$output_file"
    echo "smoke-test: FAIL (CPUS=$cpu_count)" >&2
    exit "$status"
fi

if ! grep -q "init: starting sh" "$output_file"; then
    cat "$output_file"
    echo "smoke-test: shell marker missing" >&2
    exit 1
fi

echo "smoke-test: PASS (CPUS=$cpu_count)"
