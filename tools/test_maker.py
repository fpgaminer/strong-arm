import random
import hashlib
import hmac
import sys
import os

def convert(name, x):
	result = "{"
	for i in range(8):
		result += "0x%08X," % ((x >> (i * 32) & 0xFFFFFFFF))
	result = result[:-1] + "};"

	if name is not None:
		result = "const FF_NUM " + name + " = " + result
	
	return result

def convert_hashstr(name, s):
	result = "{"
	while len(s) > 0:
		result += "0x%02X," % ord(s[0])
		s = s[1:]
	result = result[:-1] + "};"

	if name is not None:
		result = "const uint8_t " + name + "[] = " + result
	
	return result

def hashstr_to_int (s):
	result = 0
	while len(s) > 0:
		result = (result << 8) | ord(s[0])
		s = s[1:]
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


#print convert("m1", 0xAA5E28D6A97A2479A65527F7290311A3624D4CC0FA1578598EE3C2613BF99522)
#print convert("X1", 0x34F9460F0E4F08393D192B3C5133A6BA099AA0AD9FD54EBCCFACDFA239FF49C6)
#print convert("Y1", 0x0B71EA9BD730FD8923F6D25A7A91E7DD7728A960686CB5A901BB419E0F2CA232)

#print convert("m2", 0x376A3A2CDCD12581EFFF13EE4AD44C4044B8A0524C42422A7E1E181E4DEECCEC)
#print convert("X2", 0x14890E61FCD4B0BD92E5B36C81372CA6FED471EF3AA60A3E415EE4FE987DABA1)
#print convert("Y2", 0x297B858D9F752AB42D3BCA67EE0EB6DCD1C2B7B0DBE23397E66ADC272263F982)




# HMAC_DRBG
# Test vectors taken from HMAC_DRBG.txt, from the NIST DRBG Test Vectors zip file.
# Used to test the Python implementation. Then random vectors are generated to test Strong-ARM,
# since Strong-ARM has slightly different requirements (larger seed, etc).

def HMAC (key, msg):
	return hmac.new(key, msg, hashlib.sha256).digest()

def drbg_init (data):
	K = "\x00" * 32
	V = "\x01" * 32
	return update (K, V, data)

def reseed (K, V, entropy):
	K, V = update (K, V, entropy)
	return (K,V)

def gen (K, V):
	V = HMAC (K, V)
	result = V
	K,V = update (K, V, "")
	return (K,V,result)

def update (K, V, data):
	K = HMAC (K, V + "\x00" + data)
	V = HMAC (K, V)
	if len(data) == 0:
		return (K, V)
	K = HMAC (K, V + "\x01" + data)
	V = HMAC(K, V)
	return (K, V)


K, V = drbg_init ("\x0e\xd3\x9d\x3b\xfd\x41\x6b\xbb\x51\xf4\x39\xde\x8e\xfa\xd2\x70\x14\x17\xc5\x52\x3f\x13\x9f\x65\xbe\x44\x69\xb5\x32\x58\x64\xe8\x0f\x12\x1e\xde\x10\x78\x0c\x66\x94\x43\x6f\x95\xd5\xef\x98\xb4")

K, V = reseed (K, V, "\x90\x61\x4d\x0c\x26\x81\xae\xf9\xae\x21\x10\x9f\xd9\x96\xd3\x97\xc1\xbc\x27\xe2\x0d\x41\x4d\xcf\x47\x32\x11\x2b\x7b\xc5\x85\xa9")

K, V, result = gen (K, V)

K, V = reseed (K, V, "\x13\xc4\xc3\x95\x2f\xc1\x49\x8b\xae\x2d\x41\x7d\x23\xbe\x94\x2e\x9c\xfc\x8e\x96\xc2\x97\x1c\xca\x33\xea\x43\x2e\x1e\xc9\xe2\xdd")

K, V, result = gen (K, V)
result = hashstr_to_int(result)

if result != 0xa6fd7a30679da6c24d1cad86a66bc5f24fa950742bc826e34eaddfab6c8facb4:
	print "ERROR!! Python HMAC_DRBG implementation is wrong!"
	sys.exit(0)


# Generate random HMAC_DRBG test vectors
seed = os.urandom(96)
print convert_hashstr("seed0", seed)

K,V = drbg_init (seed)
K,V,result = gen (K, V)
print convert_hashstr("result00", result)
K,V,result = gen (K, V)
print convert_hashstr("result01", result)
for i in range(100):
	K,V,result = gen (K, V)
print convert_hashstr("result02", result)

