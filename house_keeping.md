---
title: House Keeping
---

[Home](./)

## Reporting

+ [OSCP Exam Report Template](https://github.com/whoisflynn/OSCP-Exam-Report-Template/blob/master/OSCP-OS-XXXXX-Exam-Report_Template3.2.docx) <sup><sub>[[archive](assets/files/OSCP-OS-XXXXX-Exam-Report_Template3.2.docx)]

## Deleting Windows Admin Account
```bash
net user /delete jack sparrow
```

## Remove Windows User From Group
```bash
net localgroup $GROUP $USER /delete
```