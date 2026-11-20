#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Rainer Größlinger
#
# Live argv round-trip probe for the terminal catalog. The ctest suite pins
# the constructed command line and runs the wrapper through a stub; it never
# starts a real terminal, so it cannot see a terminal that takes a command
# string instead of argv (tilix -e), mangles argv when forwarding to a
# running instance (terminator -x joins the list with spaces for DBus and
# the receiver re-parses it through a shell), or expands "$" a second time
# (ptyxis). This probe can: it starts every installed catalog terminal with
# hostile tokens, fresh and forwarded to a running instance, and compares
# the argv the child receives byte for byte.
#
# Usage: tests/terminal-probe.sh [terminal ...]
# Needs a display (X11 or Wayland); windows open and close during the run.
# Exits non-zero when nothing could be probed.
# Instances the probe starts are closed again; gnome-terminal relies on
# its server exiting with the last window. A terminal that was already
# running is left alone, so exited probe tabs may remain in it (kgx keeps
# tabs open after their child exits).
# Without arguments every Unix catalog terminal is probed. Names and mode
# arguments are read from src/terminalcatalog.cpp, so a new entry is probed
# in exactly the form the app builds; a form that does not pass argv
# verbatim (a command-string mode) fails here until launch() learns its
# transport. A new terminal must pass both modes before it is offered.
set -u

root=$(cd "$(dirname "$0")/.." && pwd)
catalog="$root/src/terminalcatalog.cpp"

if [ -z "${DISPLAY:-}${WAYLAND_DISPLAY:-}" ]; then
    echo "terminal-probe: no DISPLAY or WAYLAND_DISPLAY; the probe needs a desktop session." >&2
    exit 2
fi

work=$(mktemp -d "${TMPDIR:-/tmp}/terminal-probe.XXXXXX")
holders=()
owned=()
cleanup() {
    for pid in "${holders[@]:-}"; do
        [ -n "$pid" ] && kill_holder "$pid"
    done
    # Every launcher is recorded; a forwarding client is long dead by now,
    # so only instances the probe started are still alive and get closed
    # (kgx keeps a tab open after its child exits). A terminal that was
    # already running is never a launcher and stays untouched.
    for entry in "${owned[@]:-}"; do
        [ -n "$entry" ] || continue
        pid=${entry%%:*}
        name=${entry#*:}
        kill_terminal "$pid" "$name"
        for _ in $(seq 1 25); do
            stat=$(ps -p "$pid" -o stat= 2>/dev/null)
            case "$stat" in
                ''|Z*) break ;;
            esac
            sleep 0.2
        done
    done
    rm -rf "$work"
}
trap cleanup EXIT

# Kill only while the pid still belongs to the expected program. comm is
# truncated to 15 characters and some terminals re-exec under a new name
# (wezterm-gui), so match a comm or args prefix; a recycled pid rarely
# passes either.
kill_terminal() {
    local comm args
    comm=$(ps -p "$1" -o comm= 2>/dev/null)
    case "$comm" in
        "$2"*)
            kill "$1" 2>/dev/null
            return
            ;;
    esac
    args=$(ps -p "$1" -o args= 2>/dev/null)
    case "$args" in
        "$2"*) kill "$1" 2>/dev/null ;;
    esac
}

# The holder execs sleep; its exact command line is the reuse guard.
kill_holder() {
    if [ "$(ps -p "$1" -o args= 2>/dev/null)" = "sleep 300" ]; then
        kill "$1" 2>/dev/null
    fi
}

# gnome-terminal's long-lived process is the server, and comm truncation
# hides it from pgrep -x on the catalog name.
terminal_running() {
    case "$1" in
        gnome-terminal) pgrep -f 'gnome-terminal-server' > /dev/null 2>&1 ;;
        *) pgrep -x "$1" > /dev/null 2>&1 ;;
    esac
}

# Terminals that hand a launch to an already-running instance; the others
# start a new process even when a same-name process exists (foot --server
# only serves footclient).
forwards_to_instance() {
    case "$1" in
        gnome-terminal|kgx|konsole|xfce4-terminal|wezterm) return 0 ;;
        *) return 1 ;;
    esac
}

# The child whose argv is compared: one marked line per argument. It runs
# through sh, like the app's wrapper, so the temp file needs no exec bit
# and a noexec TMPDIR still works.
cat > "$work/argvdump.sh" <<'EOF'
#!/bin/sh
out="$1"
shift
{
    printf 'ARGC=%s\n' "$#"
    i=0
    for a in "$@"; do
        i=$((i + 1))
        printf 'ARGV[%s]=<%s>\n' "$i" "$a"
    done
} > "$out"
EOF

# One path quoted for a POSIX shell, for the canary tokens: if a broken
# terminal expands them, the inner shell must still see one exact path.
shquote() {
    local s=${1//\'/\'\\\'\'}
    printf "'%s'" "$s"
}

tokens=(
    'plain' 'a b' "it's" '$HOME'
    '$(touch '"$(shquote "$work/pwned")"')'
    '`touch '"$(shquote "$work/pwned2")"'`'
    'a"b' 'a\b' '*' $'line1\nline2' 'héllo→世界' '' '-m' '2500'
)

expected="$work/expected.txt"
{
    printf 'ARGC=%s\n' "${#tokens[@]}"
    i=0
    for a in "${tokens[@]}"; do
        i=$((i + 1))
        printf 'ARGV[%s]=<%s>\n' "$i" "$a"
    done
} > "$expected"

# The catalog's mode arguments, read per terminal into current_mode, then
# the command as argv. A command-string terminal would quote the whole
# command into one token here instead of passing argv. Terminal stderr
# goes to a log; probe failures show its tail.
launch() {
    local terminal="$1"
    shift
    local -a cmd=("$terminal")
    if [ "${#current_mode[@]}" -gt 0 ]; then
        cmd+=("${current_mode[@]}")
    fi
    cmd+=("$@")
    "${cmd[@]}" 2>> "$work/terminal.log" &
    launcher=$!
}

# A failure shows the terminal's own stderr tail; the log is per probe.
fail() {
    echo "FAIL: $*"
    if [ -s "$work/terminal.log" ]; then
        echo "--- terminal stderr (tail):"
        tail -n 15 "$work/terminal.log"
    fi
    return 1
}

# probe <terminal> <fresh|forward>: start the dump command, wait for its
# output, compare. forward holds a window open first so single-instance
# terminals must hand the command to the running process.
probe() {
    local terminal="$1" mode="$2"
    local out="$work/result.txt" ready="$work/ready" holder=""
    rm -f "$out" "$ready" "$work/pwned" "$work/pwned2"
    : > "$work/terminal.log"

    if [ "$mode" = forward ]; then
        # The ready path travels as a positional parameter, never spliced
        # into the script text: any TMPDIR survives, spaces and all.
        launch "$terminal" sh -c 'echo $$ > "$1"; exec sleep 300' holder-sh "$ready"
        owned+=("$launcher:$terminal")
        # -s, not -f: the redirect creates the file before echo writes the
        # pid, and an empty read would lose the holder's kill.
        for _ in $(seq 1 75); do
            [ -s "$ready" ] && break
            sleep 0.2
        done
        holder=$(cat "$ready" 2>/dev/null)
        case "$holder" in
            ''|*[!0-9]*)
                kill_terminal "$launcher" "$terminal"
                fail "$terminal ($mode) - the holding instance never started"
                return 1
                ;;
        esac
        holders+=("$holder")
    fi

    launch "$terminal" sh "$work/argvdump.sh" "$out" "${tokens[@]}"
    owned+=("$launcher:$terminal")
    for _ in $(seq 1 100); do
        [ -s "$out" ] && break
        sleep 0.2
    done
    sleep 0.5
    kill_terminal "$launcher" "$terminal"
    if [ -n "$holder" ]; then
        kill_holder "$holder"
    fi

    if [ ! -s "$out" ]; then
        fail "$terminal ($mode) - the command never ran (20 s)"
        return 1
    fi
    if [ -e "$work/pwned" ] || [ -e "$work/pwned2" ]; then
        fail "$terminal ($mode) - a token was expanded as code"
        return 1
    fi
    if diff -u "$expected" "$out" > "$work/diff.txt" 2>&1; then
        echo "PASS: $terminal ($mode)"
        return 0
    fi
    echo "FAIL: $terminal ($mode) - argv differs:"
    cat "$work/diff.txt"
    return 1
}

# The Unix catalog entries: name and mode arguments from the same table
# entry, so the probe cannot drift from the form the app builds. PowerShell
# is launched by its own script path, not probed. The whole table is
# consumed entry by entry; a skipped entry, anything left over, or no Unix
# entry at all is an error, never a silent pass. As a constructor-agnostic
# backstop, the brace count must fit the parsed entries: the declaration
# opens one brace, every entry two. (A comment holding braces would trip
# this too - loudly, which is the safe direction.)
names=()
arglists=()
current_mode=()
table=$(awk '/static const QMap<QString, QStringList> table = \{/,/^    \};/' "$catalog")
opens=$(printf '%s\n' "$table" | awk '{n += gsub(/\{/, "{")} END {print n + 0}')
rest=$table
matched=0
entry_re='\{QStringLiteral\("([A-Za-z0-9_.-]+)"\),[[:space:]]*\{([^}]*)\}\},'
while [[ $rest =~ $entry_re ]]; do
    matched=$((matched + 1))
    name=${BASH_REMATCH[1]}
    argstr=${BASH_REMATCH[2]}
    # Text before the match must hold no entry the regex could not read;
    # stripping to the match would silently drop a malformed middle entry.
    prefix=${rest%%"${BASH_REMATCH[0]}"*}
    case "$prefix" in
        *'QStringLiteral('*)
            echo "terminal-probe: cannot parse the catalog entry before $name." >&2
            exit 2
            ;;
    esac
    rest=${rest#*"${BASH_REMATCH[0]}"}
    args=''
    while [[ $argstr == *'QStringLiteral("'* ]]; do
        argstr=${argstr#*'QStringLiteral("'}
        args+="${argstr%%'"'*}"$'\n'
        argstr=${argstr#*'"'}
    done
    case "$argstr" in
        *'QStringLiteral('*)
            echo "terminal-probe: cannot parse the mode arguments of $name." >&2
            exit 2
            ;;
    esac
    case "$name" in
        powershell.exe|pwsh) continue ;;
    esac
    names+=("$name")
    arglists+=("$args")
done

if [ "$opens" -ne "$((2 * matched + 1))" ]; then
    echo "terminal-probe: the catalog table holds entries the parser cannot read" >&2
    echo "($opens opening braces, $matched parsed entries)." >&2
    exit 2
fi
case "$rest" in
    *'QStringLiteral('*)
        echo "terminal-probe: unparsed catalog text remains; the table format" >&2
        echo "changed and the parser must follow it: $rest" >&2
        exit 2
        ;;
esac
if [ "${#names[@]}" -eq 0 ]; then
    echo "terminal-probe: no Unix terminals parsed from $catalog." >&2
    exit 2
fi

# Explicit names select catalog entries; anything else is an error.
if [ "$#" -gt 0 ]; then
    sel_names=()
    sel_args=()
    for want in "$@"; do
        found=0
        i=0
        while [ "$i" -lt "${#names[@]}" ]; do
            if [ "${names[$i]}" = "$want" ]; then
                sel_names+=("$want")
                sel_args+=("${arglists[$i]}")
                found=1
            fi
            i=$((i + 1))
        done
        if [ "$found" -eq 0 ]; then
            case "$want" in
                powershell.exe|pwsh)
                    echo "terminal-probe: $want is a catalog entry, but the probe does not" >&2
                    echo "cover the PowerShell script path." >&2
                    ;;
                *)
                    echo "terminal-probe: $want is not in the terminal catalog." >&2
                    ;;
            esac
            exit 2
        fi
    done
    names=("${sel_names[@]}")
    arglists=("${sel_args[@]}")
fi

failed=0
probed=0
i=0
while [ "$i" -lt "${#names[@]}" ]; do
    terminal=${names[$i]}
    if ! command -v "$terminal" > /dev/null 2>&1; then
        echo "SKIP: $terminal (not installed)"
    else
        if terminal_running "$terminal" && forwards_to_instance "$terminal"; then
            case "$terminal" in
                wezterm)
                    echo "NOTE: $terminal is already running; probes are forwarded to it unless its config or executable differs"
                    ;;
                *)
                    echo "NOTE: $terminal is already running; both probes are forwarded to it"
                    ;;
            esac
        fi
        current_mode=()
        while IFS= read -r arg; do
            [ -n "$arg" ] && current_mode+=("$arg")
        done <<< "${arglists[$i]}"
        probe "$terminal" fresh || failed=1
        probe "$terminal" forward || failed=1
        probed=$((probed + 1))
    fi
    i=$((i + 1))
done

if [ "$probed" -eq 0 ]; then
    echo "terminal-probe: nothing was probed; no catalog terminal is installed." >&2
    exit 2
fi
if [ "$failed" -ne 0 ]; then
    echo "terminal-probe: failures above; do not offer the terminal until it passes."
    exit 1
fi
echo "terminal-probe: all probed terminals pass."
