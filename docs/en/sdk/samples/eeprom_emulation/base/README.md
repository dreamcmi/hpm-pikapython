# Eeprom emulation base test

## Overview

EEPROM BASE example tests the write, read, and flush interfaces. It's contain the follow:

**write**

- write data
- check return value

**read**

- read written data
- check return value
- compare data

**update data**

- update written data
- view updates  

**flush data**

- flush the data of the set area, keep the latest data

## Board setting

- No special settings

## Notice

- Do not write more than one erase size data once
- EEPROM_MAX_VAR_CNT which default count 100 needs to be set in user_config.h to limit the maximum number of blocks

## Running the example

The serial port output is shown below:

```console
------------ flash->eeprom init ok -----------

start address: 0x80080000
sector count: 128
flash earse granularity: 4096
version: 0x4553
end address: 0x80100000
data write addr = 0x80080000, info write addr = 0x800fffe0, remain flash size = 0x7ffe0

valid count percent info count( 0 / 0 )

----------------------------------------------

 eeprom emulation demo
----------------------------------------
 1 - Test eeprom write
 2 - Test eeprom read
 3 - Test eeprom update data
 4 - Test eeprom flush whole area
 5 - show area base info
 Others - Show index menu

◆1
block_id[0x48504d43] success write, data addr=0x80080000, remain size=0x0007ffc9 crc=0xe2fc9e5d

block_id[0x41000000] success write, data addr=0x80080007, remain size=0x0007ffb4 crc=0x9d46fe3f

block_id[0x41420000] success write, data addr=0x8008000c, remain size=0x0007ff98 crc=0x887db847

block_id[0x41424300] success write, data addr=0x80080018, remain size=0x0007ff7c crc=0x1ee95571

◆2
var1 = abcdef
var2 = 1234
var3 = hello,world
var4 = eeprom_demo

◆3
block_id[0x48504d43] success write, data addr=0x80080024, remain size=0x0007ff65 crc=0xe2fc9e5d

block_id[0x41000000] success write, data addr=0x8008002b, remain size=0x0007ff50 crc=0x9d46fe3f

block_id[0x41420000] success write, data addr=0x80080030, remain size=0x0007ff34 crc=0x887db847

block_id[0x41424300] success write, data addr=0x8008003c, remain size=0x0007ff18 crc=0x1ee95571

block_id[0x48504d43] success write, data addr=0x80080048, remain size=0x0007ff04 crc=0xbbf3f9d2

block_id[0x41000000] success write, data addr=0x8008004c, remain size=0x0007feef crc=0x4c182878

◆5
------------ flash->eeprom init ok -----------

start address: 0x80080000
sector count: 128
flash earse granularity: 4096
version: 0x4553
end address: 0x80100000
data 
[11:39:52.133]收←◆write addr = 0x8008004e, info write addr = 0x800fff40, remain flash size = 0x7fef2

valid count percent info count( 4 / 10 )

----------------------------------------------

◆2
var1 = qwe
var2 = 5678
var3 = hello,world
var4 = eeprom_demo

◆4

block_id[0x41420000] success write, data addr=0x80080000, remain size=0x0007ffc4 crc=0x887db847

block_id[0x41424300] success write, data addr=0x8008000c, remain size=0x0007ffa8 crc=0x1ee95571

block_id[0x48504d43] success write, data addr=0x80080018, remain size=0x0007ff94 crc=0xbbf3f9d2

block_id[0x41000000] success write, data addr=0x8008001c, remain size=0x0007ff7f crc=0x4c182878

------------ flash->eeprom init ok -----------

start address: 0x80080000
sector count: 128
flash earse granularity: 4096
version: 0x4553
end address: 0x80100000
data write addr = 0x80080021, info write addr = 0x800fffa0, remain flash size = 0x7ff7f

valid count percent info count( 4 / 4 )

----------------------------------------------
```

