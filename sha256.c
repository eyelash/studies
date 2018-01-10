#include <stdint.h>
#include <string.h>
#include <stdio.h>

static uint32_t const K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static uint32_t ROTR(uint32_t const x, uint32_t const n) {
	return (x >> n) | (x << (32 - n));
}

static void print_hash(uint32_t const *const H) {
	printf("%08x%08x%08x%08x%08x%08x%08x%08x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
}

static void sha256_block(uint32_t const *const M, uint32_t *const H) {
	uint32_t W[64];
	unsigned int t;
	for (t = 0; t < 16; ++t) {
		W[t] = M[t];
	}
	for (t = 16; t < 64; ++t) {
		uint32_t const s0 = ROTR(W[t-15], 7) ^ ROTR(W[t-15], 18) ^ (W[t-15] >> 3);
		uint32_t const s1 = ROTR(W[t-2], 17) ^ ROTR(W[t-2], 19) ^ (W[t-2] >> 10);
		W[t] = s1 + W[t-7] + s0 + W[t-16];
	}
	uint32_t a = H[0];
	uint32_t b = H[1];
	uint32_t c = H[2];
	uint32_t d = H[3];
	uint32_t e = H[4];
	uint32_t f = H[5];
	uint32_t g = H[6];
	uint32_t h = H[7];
	for (t = 0; t < 64; ++t) {
		uint32_t const Ch = (e & f) ^ (~e & g);
		uint32_t const Maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t const S0 = ROTR(a, 2) ^ ROTR(a, 13) ^ ROTR(a, 22);
		uint32_t const S1 = ROTR(e, 6) ^ ROTR(e, 11) ^ ROTR(e, 25);
		uint32_t const T1 = h + S1 + Ch + K[t] + W[t];
		uint32_t const T2 = S0 + Maj;
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;
	}
	H[0] = a + H[0];
	H[1] = b + H[1];
	H[2] = c + H[2];
	H[3] = d + H[3];
	H[4] = e + H[4];
	H[5] = f + H[5];
	H[6] = g + H[6];
	H[7] = h + H[7];
}

void sha256(char const *data, size_t const size, uint32_t *const H) {
	H[0] = 0x6a09e667;
	H[1] = 0xbb67ae85;
	H[2] = 0x3c6ef372;
	H[3] = 0xa54ff53a;
	H[4] = 0x510e527f;
	H[5] = 0x9b05688c;
	H[6] = 0x1f83d9ab;
	H[7] = 0x5be0cd19;
	uint32_t M[16];
	char const *const end = data + size;
	unsigned int i;
	while (data + 64 <= end) {
		for (i = 0; i < 16; ++i) {
			M[i] = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
			data += 4;
		}
		sha256_block(M, H);
	}
	for (i = 0; data + 4 <= end; ++i) {
		M[i] = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
		data += 4;
	}
	switch (end - data) {
	case 3:
		M[i] = data[0] << 24 | data[1] << 16 | data[2] << 8 | 0x80;
		break;
	case 2:
		M[i] = data[0] << 24 | data[1] << 16 | 0x80 << 8;
		break;
	case 1:
		M[i] = data[0] << 24 | 0x80 << 16;
		break;
	case 0:
		M[i] = 0x80 << 24;
		break;
	}
	++i;
	if (i + 2 > 16) {
		for(; i < 16; ++i) {
			M[i] = 0;
		}
		sha256_block(M, H);
		i = 0;
	}
	for (; i + 2 < 16; ++i) {
		M[i] = 0;
	}
	uint64_t const L = size * 8;
	M[i] = L >> 32;
	M[i+1] = L & 0xFFFFFFFF;
	sha256_block(M, H);
}

int main(int argc, char **argv) {
	if (argc > 1) {
		uint32_t hash[8];
		sha256(argv[1], strlen(argv[1]), hash);
		print_hash(hash);
	}
	else {
		printf("Usage: %s string\n", argv[0]);
	}
	return 0;
}
