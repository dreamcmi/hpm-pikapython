import PikaStdLib
import time
import PikaStdDevice
import binascii
import random
import configparser
import re
import aes
import base64
import hashlib
import hmac
import PikaMath

print('hello pikapython!')
print('Run in HPM6750 MCU!')

mem = PikaStdLib.MemChecker()
print('mem.max :')
mem.max()
print('mem.now :')
mem.now()