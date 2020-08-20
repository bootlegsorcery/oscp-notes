---
title: Exploitation
---

[Home](./)

_Ensuring you don't have to re-exploit over and over again_

## Executing in Memory - TMPFS

```bash
mount | grep ^tmp

# Example:
# tmpfs on /dev/shm type tmpfs (rw,nosuid,nodev)
# tmpfs on /tmp type tmpfs (rw,nosuid,nodev)
#
# Beware of the "noexec" flag; executing programs will fail
```

## Get full TTY shell
```bash
python -c 'import pty; pty.spawn("/bin/bash")'
```