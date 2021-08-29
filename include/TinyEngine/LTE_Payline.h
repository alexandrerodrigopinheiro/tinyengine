#ifndef TINYENGINE_LTE_PAYLINE_H_
#define TINYENGINE_LTE_PAYLINE_H_

#pragma once

#include "LTE_Utils.h"

class LTE_Payline {
public:

	LTE_Payline() : LOCAL_SIZE_COLUMN(5)
	, _line(-1)
	, _symbol(-1)
	, _value(-1)
	, _prize(false)
	, _wild(false)
	, _freeSpin(false)
	, _bonusGame(false)
	, _jackpot(false)
	, _active(false)
	, _spot(NULL) {
		this->_spot = (int*)malloc(LOCAL_SIZE_COLUMN * sizeof(int));
		this->Clear();
	}

	LTE_Payline(std::size_t cols) : LOCAL_SIZE_COLUMN(cols)
	, _line(-1)
	, _symbol(-1)
	, _value(-1)
	, _prize(false)
	, _wild(false)
	, _freeSpin(false)
	, _bonusGame(false)
	, _jackpot(false)
	, _active(false)
	, _spot(NULL) {
		this->_spot = (int*)malloc(LOCAL_SIZE_COLUMN * sizeof(int));
		this->Clear();
	}

	~LTE_Payline() {
		this->_spot = NULL;
	}

	void Clear() {
		this->_line = -1;
		this->_symbol = -1;
		this->_value = -1;
		this->_prize = false;
		this->_wild = false;
		this->_freeSpin = false;
		this->_bonusGame = false;
		this->_jackpot = false;
		this->_active = false;
		if (this->_spot == NULL || this->LOCAL_SIZE_COLUMN == 0) {
			std::cerr << "Payline no initialize ...." << std::endl;
			exit(0);
		}
		for (std::size_t i = 0; i < this->LOCAL_SIZE_COLUMN; i++) {
			this->_spot[i] = -1;
		}
	}

	void SetLine(int line) {
		this->_line = line;
	}

	void SetSymbol(int symbol) {
		this->_symbol = symbol;
	}

	void SetValue(int value) {
		this->_value = value;
	}

	void SetPrize(bool prize) {
		this->_prize = prize;
	}

	void SetWild(bool wild) {
		this->_wild = wild;
	}

	void SetFreeSpin(bool free) {
		this->_freeSpin = free;
	}

	void SetBonusGame(bool bonus) {
		this->_bonusGame = bonus;
	}

	void SetJackpot(bool jackpot) {
		this->_jackpot = jackpot;
	}

	void SetActive(bool active) {
		this->_active = active;
	}

	void SetSpot(std::size_t col, std::size_t value) {
		this->_spot[col] = value;
	}

	void SetSpot(int* spot) {
		for (std::size_t i = 0; i < this->LOCAL_SIZE_COLUMN; i++) {
			this->_spot[i] = spot[i];
		}
	}

	int GetLine() const {
		return this->_line;
	}

	int GetSymbol() const {
		return this->_symbol;
	}

	int GetValue() const {
		return this->_value;
	}

	bool IsPrize() const {
		return this->_prize;
	}

	bool IsWild() const {
		return this->_wild;
	}

	bool IsFreeSpin() const {
		return this->_freeSpin;
	}

	bool IsBonusGame() const {
		return this->_bonusGame;
	}

	bool IsJackpot() const {
		return this->_jackpot;
	}

	bool IsActive() const {
		return this->_active;
	}

	int GetSpot(std::size_t col) const {
		if (col >= this->LOCAL_SIZE_COLUMN) {
			std::cout << "Illegal index in pay line at spot as '" << col << "' when maximum as '" << this->LOCAL_SIZE_COLUMN << "', failed" << std::endl;
			exit(0);
		}
		return this->_spot[col];
	}

	int* GetSpot() const {
		return this->_spot;
	}

private:

	std::size_t LOCAL_SIZE_COLUMN;

protected:

	int _line { -1 };
	int _symbol { -1 };
	int _value { -1 };
	bool _prize { false };
	bool _wild { false };
	bool _freeSpin { false };
	bool _bonusGame { false };
	bool _jackpot { false };
	bool _active { false };
	int* _spot { NULL };
};
#endif /* PAYLINE_H_ */
