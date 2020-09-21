---
title: House Keeping
---

[Home](./)

## Deleting Windows Admin Account
```bash
net user /delete jack sparrow
```

## Remove Windows User From Group
```bash
net localgroup $GROUP $USER /delete
```