#ifndef TINYENGINE_LTE_CRYPTO_H_
#define TINYENGINE_LTE_CRYPTO_H_

// Compile:
// CXXFLAGS=-lblkid make hdio
// Run:
// sudo ./hdio /dev/<device>

#pragma once

#if defined(__linux__) || defined(__linux) || defined(linux)
#include <blkid/blkid.h>
#endif
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#if defined(__linux__) || defined(__linux) || defined(linux)
#include <linux/hdreg.h>
#endif

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#if defined(_WIN32)
#  include <NTSecAPI.h>
#endif

#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "LTE_Utils.h"

#define LTE_CRYPTO_USER_PASS_LENGTH	24

std::string LTE_CryptoUUID(const char *devname) {
#if defined(__linux__) || defined(__linux) || defined(linux)
	blkid_probe pr = blkid_new_probe_from_filename(devname);
	if (!pr) {
		// err(2, "Failed to open %s", devname);
		std::cerr << "Failed to open :" << devname << std::endl;
		return "";
	}
	blkid_do_probe(pr);
	const char *uuid;
	int ret = blkid_probe_lookup_value(pr, "UUID", &uuid, NULL);
	if (ret < 0) { return ""; }
	std::string result = uuid;
	blkid_free_probe(pr);
#else
	LTE_Unused(devname);
	std::string result = "";
#endif
	return result;
}

bool LTE_CryptoSerial(struct hd_driveid *hd, const char *devname) {
#if defined(__linux__) || defined(__linux) || defined(linux)
	int fd = open(devname, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		fprintf(stderr, "ERROR: Cannot open device %s\n", devname);
		return false;
	}
	if (ioctl(fd, HDIO_GET_IDENTITY, hd) == 0) {
		// Success
		close(fd);
		return true;
	} else if (errno == -ENOMSG) {
		fprintf(stderr, "No hard disk identification information available\n");
	} else {
		perror("ERROR: HDIO_GET_IDENTITY");
	}
	close(fd);
	return false;
#else
	LTE_Unused(hd, devname);
	return false;
#endif
}

bool LTE_CryptoStartup(void) {
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
	OpenSSL_add_all_algorithms(); // Add all cipher and digest algorithms
	ERR_load_crypto_strings();
	ERR_load_OBJ_strings();
	return true;
}

void LTE_CryptoShutdown(void) {
	ERR_free_strings(); // for ERR_load_crypto_strings
	EVP_cleanup(); // for OpenSSL_add_all_algorithms
	RAND_cleanup();
	CRYPTO_cleanup_all_ex_data();
}

bool LTE_CryptoSeedPRNG(void) {
#if defined(_WIN32)
	char buffer[1024];
	// RtlGenRandom is provided by ADVAPI32.DLL on Windows >= XP.
	// Windows's rand_s() internally calls RtlGenRandom.
	// Python's urandom() uses /dev/[u]random on Unix-based systems and CryptGenRandom on Windows systems.
	// Crypto++ uses RtlGenRandom on Windows.
	RtlGenRandom(&buffer, sizeof(buffer));
	RAND_add(buffer, sizeof(buffer), sizeof(buffer));
#elif defined(__linux__)
	// Stick to /dev/urandom on Linux, because /dev/random is blocking :-(
	RAND_load_file("/dev/urandom", 1024);
#else
	RAND_load_file("/dev/random", 1024);
#endif
	return true;
}

std::string LTE_CryptoBytesToHexstring(const uint8_t *bytes, size_t size) {
	std::ostringstream ss;
	ss << std::hex << std::nouppercase << std::setfill('0');
	std::for_each(bytes, bytes+size, [&](int c) { ss << std::setw(2) << c; });
	return ss.str();
}

std::string LTE_CryptoRandomString(size_t size) {
	unsigned char *buf = new unsigned char[size];
	int ret = RAND_bytes(buf, size);
	if (ret != 1) {
		delete buf;
		fprintf(stderr, "ERROR: %s\n", ERR_error_string(ERR_get_error(), NULL));
		return "";
	}
	std::string result = LTE_CryptoBytesToHexstring(buf, size);
	delete [] buf;
	return result;
}

std::string LTE_CryptoMD5(const std::string& source) {
	MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, source.c_str(), source.length());
    unsigned char digest[16];
    MD5_Final(digest, &ctx);
	return LTE_CryptoBytesToHexstring(digest, sizeof(digest));
}

std::string LTE_CryptoRot13(const std::string& source) {
	std::string transformed;
	for (size_t i = 0; i < source.size(); ++i) {
		if (isalpha(source[i])) {
			if ((tolower(source[i]) - 'a') < 14)
				transformed.append(1, source[i] + 13);
			else
				transformed.append(1, source[i] - 13);
		} else {
			transformed.append(1, source[i]);
		}
	}
	return transformed;
}

std::string LTE_CryptoGenerateUserPass(const std::string& uuid, const std::string& machinePass) {
	const std::string generated = LTE_CryptoMD5(LTE_CryptoRot13(machinePass) + uuid);
	const size_t length = generated.length();
	std::string result;
	// Make the user-pass smaller by stripping-off 1 for each 4 bytes.
	// This will reduce the user-pass from 32 bytes to 24 bytes.
	for (size_t i = 0; i < length; i++) { if (i % 4) { result += generated[i]; } }
	return result;
}

bool LTE_CryptoValidateUserPass(const std::string& uuid, const std::string& machinePass, const std::string& userPass) {
	const std::string generated_user_pass = LTE_CryptoGenerateUserPass(uuid, machinePass);
	return generated_user_pass == userPass;
}

static bool LTE_CryptoIsEmptyString(const char *str) { return str == NULL || str[0] == '\0'; }

bool LTE_CryptoIsRunningAsSuperUser(void) {
	uid_t euid = geteuid();
	return euid == 0;
}

int LTE_CryptoForceChRoot(const char *dirname) {
	if (LTE_CryptoIsEmptyString(dirname)) {
		fprintf(stderr, "Invalid chroot directory.\n");
		return -1;
	}
	if (!LTE_CryptoIsRunningAsSuperUser()) {
		fprintf(stderr, "Unable to chroot: not running as superuser\n");
		return -1;
	}
	if (chdir(dirname) != 0) {
		fprintf(stderr, "Unable to chdir to %s: %s\n", dirname, strerror(errno));
		return -1;
	}
	if (chroot(dirname) != 0) {
		fprintf(stderr, "Unable to chroot to %s: %s (Try using the absolute path)\n",
			dirname, strerror(errno));
		return -1;
	}
	return 0;
}

int LTE_CryptoDropPrivPerm(uid_t new_uid) {
#if defined(__linux__) || defined(__linux) || defined(linux)
	uid_t ruid, euid, suid;
	// On Linux one MUST use setresuid to permanently drop privileges
	if (setresuid(new_uid, new_uid, new_uid) < 0)
		return -1;
	if (getresuid(&ruid, &euid, &suid) < 0)
		return -1;
	if (ruid != new_uid || euid != new_uid || suid != new_uid)
		return -1;
#else
	if (setuid(new_uid) < 0)
		return -1;
	if (geteuid() != new_uid)
		return -1;
#endif
	return 0;
}

int LTE_CryptoForceUID(const char *username) {
	struct passwd *user;
	if (LTE_CryptoIsEmptyString(username)) {
		fprintf(stderr, "Invalid username provided for privileges dropping.\n");
		return -1;
	}
	if (!LTE_CryptoIsRunningAsSuperUser()) {
		fprintf(stderr, "Unable to force uid: not running as superuser\n");
		return -1;
	}
	user = getpwnam(username);
	if (user == NULL) {
		fprintf(stderr, "User %s doesn't exist\n", username);
		return -1;
	}
	if (LTE_CryptoDropPrivPerm(user->pw_uid) < 0) {
		fprintf(stderr, "Unable to setuid to %s: %s\n", username, strerror(errno));
		return -1;
	}
	return 0;
}

std::string LTE_CryptoEncodeString(const std::string& data, const std::string& key) {
	int value = 0;
	int index = 0;
	int max = key.size() - 1;
	std::string phrase;
	for (std::size_t i = 0; i < data.length(); i++) {
		value = static_cast<int>(data[i]) * static_cast<int>(key[index]);
		phrase += LTE_ToString(value) + (i == data.length() - 1 ? "" : " ");
		LTE_Clamp(++index, 0, max);
	}
    return phrase;
}

std::string LTE_CryptoDecodeString(const std::string& data, const std::string& key) {
	int value = 0;
	int index = 0;
	int max = key.size() - 1;
	std::string str = data;
	std::vector<int> ascii = LTE_Explode(str, " ");
	std::string phrase;
	for (std::size_t i = 0; i < ascii.size(); i++) {
		value = ascii[i] / static_cast<int>(key[index]);
		phrase += static_cast<char>(value);
		LTE_Clamp(++index, 0, max);
	}
	return phrase;
}

#endif
