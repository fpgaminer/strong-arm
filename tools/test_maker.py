import random

def convert(name, x):
	result = "{"
	for i in range(8):
		result += "0x%08X," % ((x >> (i * 32) & 0xFFFFFFFF))
	result = result[:-1] + "};"

	if name is not None:
		result = "const FF_NUM " + name + " = " + result
	
	return result



a = random.randrange(0, 2**256-1)
b = random.randrange(0, 2**256-1)
p1 = 2**256 - 1
p2 = 0x47
print a
print b
print convert("a", a)
print convert("b", b)
print convert("c", 0)
print convert("p1", p1)
print convert("p2", p2)
print convert("r1", (a * b) % p1)
print convert("r2", (a * b) % p2)
print convert("r3", (a * 0) % p1)
print convert("r4", (a * a) % p1)


