---
title: Exploitation
---

[Home](./)

_Ensuring you don't have to re-exploit over and over again_

## Executing in Memory
Some linux distros utilise `/dev/shm`. This is a location like tmp but entirely in RAM.

## Get full TTY shell
```bash
python -c 'import pty; pty.spawn("/bin/bash")'
```