import random

def convert(name, x):
	result = "{"
	for i in range(8):
		result += "0x%08X," % ((x >> (i * 32) & 0xFFFFFFFF))
	result = result[:-1] + "};"

	if name is not None:
		result = "const FF_NUM " + name + " = " + result
	
	return result



#a = random.randrange(0, 2**256-1)
#b = random.randrange(0, 2**256-1)
#p1 = 2**256 - 1
#p2 = 0x47
#print a
#print b
#print convert("a", a)
#print convert("b", b)
#print convert("c", 0)
#print convert("p1", p1)
#print convert("p2", p2)
#print convert("r1", (a * b) % p1)
#print convert("r2", (a * b) % p2)
#print convert("r3", (a * 0) % p1)
#print convert("r4", (a * a) % p1)


print convert("m1", 0xAA5E28D6A97A2479A65527F7290311A3624D4CC0FA1578598EE3C2613BF99522)
print convert("X1", 0x34F9460F0E4F08393D192B3C5133A6BA099AA0AD9FD54EBCCFACDFA239FF49C6)
print convert("Y1", 0x0B71EA9BD730FD8923F6D25A7A91E7DD7728A960686CB5A901BB419E0F2CA232)

print convert("m2", 0x376A3A2CDCD12581EFFF13EE4AD44C4044B8A0524C42422A7E1E181E4DEECCEC)
print convert("X2", 0x14890E61FCD4B0BD92E5B36C81372CA6FED471EF3AA60A3E415EE4FE987DABA1)
print convert("Y2", 0x297B858D9F752AB42D3BCA67EE0EB6DCD1C2B7B0DBE23397E66ADC272263F982)


