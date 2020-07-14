---
title: Active Information Gathering
---

[Home](./)

_Information gathering in which you interact directly with the target system/network._

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

## Port Scanning

### Unicorn Scan

Performing a Network Sweep

```bash
# Class A Network
unicornscan $RHOST/8

# Class B Network
unicornscan $RHOST/16

# Class C Network
unicornscan $RHOST/24
```

Against a single target.

```bash    
# Standard
unicornscan $RHOST

# Full Range
unicornscan $RHOST:0-65535
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
nikto $RHOST

curl $RHOST/robots.txt

dirbuster
```
#### Wordpress
```bash
nmap --script=http-wordpress* -p 80 $RHOST

wpscan $RHOST
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