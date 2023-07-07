# mdio 

Ethernet PHY Chip read/write tool on Linux.

## Install

- `make` ，build executable file
- `make clean` , remove executable file

## Usage

mdio <dev> <phyad> <r/w> <address> [value]

- dev: network device name , eg 'eth0'
- phyad: PHY address , eg '1~9'
- r/w: data direction, read or write
- address: register address, hex
- value: the value of write to register , hex

Example:

```
~# ./mdio eth0 1 r 1f
Read from eth0.1
Register 0x001f = 0x0000
~# ./mdio eth0 1 w 1f d04
Wirte to eth0.1
Register 0x001f = 0x0D04
```
