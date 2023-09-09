import PikaStdLib

print('hello pikapython!')
print('Run in HPM6750 MCU!')

mem = PikaStdLib.MemChecker()
print('mem.max :')
mem.max()
print('mem.now :')
mem.now()
