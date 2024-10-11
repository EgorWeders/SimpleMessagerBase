#ifndef RSAMUTATOR_H
#define RSAMUTATOR_H

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>

class RSAMutator {
private:
	int public_key, n, private_key;
	int phi(int n) {
		int result = n;
		for (int i = 2; i <= sqrt(n); i++) {
			if (n % i == 0) {
				while (n % i == 0) {
					n /= i;
				}
				result -= result / i;
			}
		}
		if (n > 1) {
			result -= result / n;
		}
		return result;
	}

	// calculate gcd(a, b) using the Euclidean algorithm
	int gcd(int a, int b) {
		if (b == 0) {
			return a;
		}
		return gcd(b, a % b);
	}

	// calculate a^b mod m using modular exponentiation
	int modpow(int a, int b, int m) {
		int result = 1;
		while (b > 0) {
			if (b & 1) {
				result = (result * a) % m;
			}
			a = (a * a) % m;
			b >>= 1;
		}
		return result;
	}

	// generate a random primitive root modulo n
	int generatePrimitiveRoot(int n) {
		int  phiN       = phi(n);
		int *factors    = new int[phiN];
		int  numFactors = 0;
		int  temp       = phiN;
		// get all prime factors of phi(n)
		for (int i = 2; i <= sqrt(temp); i++) {
			if (temp % i == 0) {
				factors[numFactors++] = i;
				while (temp % i == 0) {
					temp /= i;
				}
			}
		}
		if (temp > 1) {
			factors[numFactors++] = temp;
		}
		// test possible primitive roots
		for (int i = 2; i <= n; i++) {
			bool isRoot = true;
			for (int j = 0; j < numFactors; j++) {
				if (modpow(i, phiN / factors[j], n) == 1) {
					isRoot = false;
					break;
				}
			}
			if (isRoot) {
				delete[] factors;
				return i;
			}
		}
		delete[] factors;
		return -1;
	}

public:
	RSAMutator() {
		std::random_device rd; // Get a random seed from the OS entropy device, or whatever
		std::mt19937_64    eng(rd());
		int                p = 7;
		int                q = 53;
		n                    = p * q;
		int phiN             = (p - 1) * (q - 1);
		public_key           = generatePrimitiveRoot(phiN);
		private_key          = 0;
		while ((public_key * private_key) % phiN != 1) {
			private_key++;
		}
		std::cout << "Public key: {" << public_key << ", " << n << "}" << std::endl;
		std::cout << "Private key: {" << private_key << ", " << n << "}" << std::endl;
		int m         = 123456;
		int c         = modpow(m, public_key, n);
		int decrypted = modpow(c, private_key, n);
		std::cout << "Original message: " << m << std::endl;
		std::cout << "Encrypted message: " << c << std::endl;
		std::cout << "Decrypted message: " << decrypted << std::endl;
	}
};

#endif // RSAMUTATOR_H
