#ifndef TINYENGINE_LTE_RANDOMIZER_H_
#define TINYENGINE_LTE_RANDOMIZER_H_

#pragma once

#include <algorithm>
#include <random>

class LTE_Randomizer {
public:
	typedef std::mt19937 rng_type;
	LTE_Randomizer() {}
	LTE_Randomizer(const LTE_Randomizer&) = delete;
	// http://www.codeguru.com/cpp/cpp/cpp_mfc/stl/article.php/c15319/A-TR1-Tutorial-Generating-Random-Numbers.htm
	// Initialize the generator
	void seed() { _prng.seed((unsigned int)time(NULL)); }
	template <class RandomAccessIterator>
	// random_shuffle() provides n > 0. It wants [0, n).
	// uniform_int_distribution's ctor takes a and b with a <= b. It produces [a, b].
	void Shuffle(RandomAccessIterator first, RandomAccessIterator last) {
		auto dist = [this](const ptrdiff_t n) { return std::uniform_int_distribution<ptrdiff_t>(0, n - 1)(_prng); };
		std::random_shuffle(first, last, dist);
	}
protected:
	rng_type _prng; // Mersenne twister generator, MT19937
};

#endif
