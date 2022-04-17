## Notes

- changed /proc/sys/kernel/perf_event_paranoid from 4 to 1

## TODO
- figure out how to run benchmark tests
- create disk with benchmark tests
- figure out how to parse data from stats.txt (m5.stats.dump())

## Run Tests
```bash
# run system configuration
./gem5/build/X86/gem5.opt x86-spec-hpc-benchmarks.py --benchmark <name>
# view run output
cat gem5/m5out/board.pc.com_1.device | less +G
```

## Tmux
useful for creating a shell that will persist when a ssh session is closed

- create new tmux window session
  - `tmux`
- detach window:
  - <kbd>ctrl</kbd> + <kbd>b</kbd> then <kbd>d</kbd>
- kill window:
  - <kbd>ctrl</kbd> + <kbd>b</kbd> then <kbd>&</kbd> then confirm with <kbd>y</kbd>
- name window:
  - <kbd>ctrl</kbd> + <kbd>b</kbd> then <kbd>$</kbd> then type `custom_name`
- list sessions
  - `tmux ls`
- attach to tmux terminal
  - `tmux attach`
  - `tmux attach -t <session name>`


## I think I may have found the bug.

The last line in `/home/gem5project/spec-hpc-tests/m5out/system.pc.com_1.device` says:
`./script.sh: line 1: /home/gem5/spec-hpc/bin/tealeaf.base.x: No such file or directory`


## uncomment in line 178 x86-spec-hpc-benchmarks.py, the simulation runs.