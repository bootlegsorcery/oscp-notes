---
title: Active Information Gathering
---

[Home](./)
| | |
|:-|-:|
| [Network](#Port-Scanning) | [Local](#Local-Enumeration) |

_Information gathering in which you interact directly with the target system/network._


## Port Scanning

### Classful Addressing

| Class | Leading Bits | Start Address | End Address | Default Subnet Mask | CIDR Notation |
| :---: | :----------: | :-----------: | :---------: | :-----------------: | :-----------: |
| A | 0 | 0.0.0.0 | 127.255.255.255 | 255.0.0.0 | /8 |
| B | 10 | 128.0.0.0 | 191.255.255.255 | 255.255.0.0 | /16 |
| C | 110 | 192.0.0.0 | 223.255.255.255 | 255.255.255.0 | /24 |
| D <br/> (multicast) | 1110 | 224.0.0.0 | 239.255.255.255 | [undefined] | [undefined] |
| E <br/> (reserved) | 1111 | 240.0.0.0 | 255.255.255.255 | [undefined] | [undefined] |

<sup>[source](https://en.wikipedia.org/wiki/Classful_network)</sup>

### Unicorn Scan

Performing a Network Sweep

```bash
sudo unicornscan -mT $RHOSTS/$CLASS
```

Against a single target.

```bash    
# Standard
sudo unicornscan $RHOST

# Full Range
sudo unicornscan $RHOST:0-65535
```

### NetDiscover (ARP Scan)

```bash
sudo netdiscover -r $RHOSTS/$CLASS
```

## Service Enumeration

### Port 21 - FTP
```bash
ftp $RHOST

# Anonymous login
nmap --script=ftp-anon -p 21 $RHOST

# Bruteforce logins
nmap --script=ftp-brute -p 21 $RHOST
```

### Port 25 - SMTP
```bash
nmap –script smtp-commands,smtp-enum-users,smtp-vuln-cve2010-4344, smtp-vuln-cve2011-1720,smtp-vuln-cve2011-1764 -p 25 $RHOST
```

### Port 80 - HTTP

```bash
nikto -h $RHOST

curl $RHOST/robots.txt

# Suggested Wordlist: /usr/share/wordlists/seclists/Discovery/Web-Content/directory-list-2.3-medium.txt
dirbuster

gobuster dir -u $URL -w $WORDLIST
```
#### Wordpress
```bash
nmap --script=http-wordpress* -p 80 $RHOST

# Enumerate
wpscan --url $RHOST --enumerate u[$LOW-$HIGH]vp

# Bruteforce
wpscan --url $RHOST --usernames [$USER_WORDLIST] --passwords [$PASS_WORDLIST]
```

### Port 110 - POP3
```bash
telnet $RHOST 110
    USER $USER
    PASS $PASS
    LIST
    RETR
    QUIT
```

### Port 111 - RPCBind
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

### Port 135 - MSRPC
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

### Port 139/445 - SMB
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

### Port 161 - SNMP
```bash
snmpwalk -c public -v1 $RHOST

snmpcheck -t $RHOST -c public

onesixtyone -c $NAMES_FILE -i $RHOST_FILE

snmpenum -t $RHOST
```

### Port 443 - HTTPS
```bash
nmap -sV --script=ssl-heartbleed $RHOST
```

### Port 2049 - NFS
```bash
# Enumeration
showmount -e $RHOST

# Mounting
mount $RHOST:/ /tmp/NFS
mount -t $RHOST:/ /tmp/NFS
```

### Port 5900 - VNC
```bash
vncviewer $RHOST
```

## DNS Enumeration

DNS Servers are the address books of the internet. As such, they store a wealth of knowledge about the layout of a target's network.

### Manual Enumeration
```bash
# Find IP address from domain (Forward Lookup)
host $DOMAIN

# Find domain from IP address (Reverse Lookup)
host $IP

# Find Name Servers
host -t ns $DOMAIN

# Find Mail Servers
host -t mx $DOMAIN

# Perform a Zone Transfer
host -l $DOMAIN $NAME_SERVER
```

### DNSRecon

Performs an "Asynchronous Transfer Full Range"

```bash
dnsrecon -d $DOMAIN -t axfr
```

### DNSEnum

Performs a Zone Transfer

```bash
dnsenum $DOMAIN
```

## Local Enumeration

### Linux | Enumeration Scripts:
- [LinEnum.sh](https://raw.githubusercontent.com/rebootuser/LinEnum/master/LinEnum.sh) <sup><sub>[[archive](assets/files/LinEnum.sh)]

- [linuxprivchecker.py](https://raw.githubusercontent.com/sleventyeleven/linuxprivchecker/master/linuxprivchecker.py) <sup><sub>[[archive](assets/files/linuxprivchecker.py)]

- [unix-privesc-check.sh](https://raw.githubusercontent.com/pentestmonkey/unix-privesc-check/master/upc.sh) <sup><sub>[[archive](assets/files/upc.sh)]

### Windows | Enumeration Scripts
- [windows-exploit-suggester.py](https://raw.githubusercontent.com/AonCyberLabs/Windows-Exploit-Suggester/master/windows-exploit-suggester.py)  <sup><sub>[[archive](assets/files/windows-exploit-suggester.py)]

- [windows-privesc-check](https://github.com/pentestmonkey/windows-privesc-check) <sup><sub>[[archive](assets/files/windows_privesc_check.py)]

- [Sherlock](https://github.com/rasta-mouse/Sherlock) <sup><sub>[[archive](assets/files/Sherlock.ps1)]

#### Resources
- https://guif.re/windowseop
