## Notes

- changed /proc/sys/kernel/perf_event_paranoid from 4 to 1

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