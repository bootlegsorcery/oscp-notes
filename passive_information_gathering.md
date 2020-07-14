---
title: Passive Information Gathering
---

[Home](./)

_Information gathering in which you gather information without directly interacting with the target system/network._

## Google Dorks
Google is the world's most popular search engine. As such, it knows a lot about everyone. It also has simple yet powerful filtering options. We can use this to our advantage.

[Google Hacking Database (GHDB)](https://www.exploit-db.com/google-hacking-database)

## Email Harvesting
Knowing _who_ you are attacking is just as important as knowing _what_ you're attacking. Gathering email addresses can be a invaluable later in the later stages of a penetration test, especially when social engineering is required. 

An automated way to do this is [theHarvester](https://github.com/laramies/theHarvester).

```bash
theharvester -d $DOMAIN -l $RESULT_LIMIT -b $DATA_SOURCE
```

## Indirect Active Information Gathering Services
There are some lovely companies out there that gather information on everyone, seemingly for the hell of it. Use these to get information generally reserved for the active information gathering stage.

1. [Shodan](https://www.shodan.io/)
2. [Netcraft](https://www.netcraft.com/)
3. [Whois](https://who.is/)

## Framework: Recon-ng
A framework similar to Metasploit, that is exclusively for reconnaissance. Get it [here](https://github.com/lanmaster53/recon-ng).