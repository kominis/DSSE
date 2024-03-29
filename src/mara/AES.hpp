#pragma once

#include "Types.hpp"
#include <sodium.h>

// Encryption constants (in bytes)
constexpr int IV = 16;
constexpr int Key = 128;
constexpr int Blake2Key = crypto_generichash_KEYBYTES;

/*
 * Performs encryption using AES-256-CBC
 * and provides various helper functions
 */
class AES {
	// Encrypts a plaintext into a given ciphertext buffer
	static int EncryptBytes(bytes<Key> key, bytes<IV> iv, byte_t *plaintext, int plen, byte_t *ciphertext);

	// Decrypts a ciphertext, putting the resulting plaintext into a given buffer
	static int DecryptBytes(bytes<Key> key, bytes<IV> iv, byte_t *ciphertext, int clen, byte_t *plaintext);

	// Encrypts/decrypts a block of data
	static block Encryptblock(bytes<Key> key, bytes<IV> iv, block plaintext,size_t clen_size,size_t plaintext_size);
	static block Decryptblock(bytes<Key> key, bytes<IV> iv, block ciphertext,size_t clen_size);


public:
	static void Setup();
	static void Cleanup();
	// Probabilistically encrypts a block using a
	// random IV, and places it at the start of
	// the ciphertext
	static block Encrypt(bytes<Key> key, block b,size_t clen_size,size_t plaintext_size);

	// Decrypts a ciphertext which has the IV at
	// the beginning of it
	static block Decrypt(bytes<Key> key, block b,size_t clen_size);

	// Gets the length of the corresponding ciphertext
	// given the length of a plaintext
	static int GetCiphertextLength(int plen);

	static void ComputeToken(unsigned char* key, unsigned int key_len, char *plaintext, unsigned char* token);
	static unsigned long ComputePath(unsigned char* key, char *plaintext);

	// Generate a randomised initialisation vector
	// and encryption keys
	static bytes<IV> GenerateIV();
};
