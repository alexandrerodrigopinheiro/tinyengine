#ifndef TINYENGINE_LTE_MATRIX_H_#define TINYENGINE_LTE_MATRIX_H_#pragma once#include "LTE_Utils.h"#include "LTE_Randomizer.h"#define LTE_MATRIX_SYMBOL_JACKPOT		9#define LTE_MATRIX_SYMBOL_BONUS_GAME	8#define LTE_MATRIX_SYMBOL_FREE_SPIN		7#define LTE_MATRIX_SYMBOL_WILD			6#define NO_REPEAT						1class LTE_Matrix {public:	LTE_Matrix(std::size_t cols, std::size_t rows, std::size_t symbols) : LOCAL_SIZE_COLUMNS(cols)	, LOCAL_SIZE_ROWS(rows)	, LOCAL_SIZE_SYMBOL(symbols)	, LOCAL_SIZE_TAIL(rows + symbols)	, _randomizer()	, _tail(std::vector<std::vector<int>>())	, _cell(std::vector<std::vector<int>>())	, _swap(std::vector<std::vector<int>>()) {		srand((unsigned long long int)time(NULL));		this->_randomizer.seed();		this->LOCAL_SIZE_COLUMNS = cols;		this->LOCAL_SIZE_ROWS = rows;		this->LOCAL_SIZE_SYMBOL = symbols;		this->LOCAL_SIZE_TAIL = rows + symbols;		this->_cell.resize(LOCAL_SIZE_COLUMNS);#if NO_REPEAT		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0, n = 0; r < this->LOCAL_SIZE_SYMBOL; r++, n++) {				this->_cell[c].push_back(n);			}			this->_randomizer.Shuffle(this->_cell[c].begin(), this->_cell[c].end());		}#else		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0; r < this->LOCAL_SIZE_ROW; r++) {				this->_cell[c].push_back(rand() % this->LOCAL_SIZE_SYMBOL);			}		}#endif		this->_swap.resize(LOCAL_SIZE_COLUMNS);#if NO_REPEAT		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0, n = 0; r < this->LOCAL_SIZE_SYMBOL; r++, n++) {				this->_swap[c].push_back(n);			}			this->_randomizer.Shuffle(this->_swap[c].begin(), this->_swap[c].end());		}#else		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0; r < this->LOCAL_SIZE_ROW; r++) {				this->_swap[c].push_back(rand() % this->LOCAL_SIZE_SYMBOL);			}		}#endif		this->_tail.resize(LOCAL_SIZE_TAIL);		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0; r < this->LOCAL_SIZE_TAIL; r++) {				this->_tail[c].push_back(rand() % this->LOCAL_SIZE_SYMBOL);			}		}		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = this->LOCAL_SIZE_TAIL - this->LOCAL_SIZE_ROWS, n = 0; r < this->LOCAL_SIZE_TAIL; r++, n++) {				this->_tail[c][r] = this->_swap[c][n];			}		}		this->Swap();	}	~LTE_Matrix() {		this->_cell.empty();		this->_swap.empty();		this->_tail.empty();	}	void Fill() {#if NO_REPEAT		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			this->_randomizer.Shuffle(this->_swap[c].begin(), this->_swap[c].end());		}#else		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0; r < this->LOCAL_SIZE_ROWS; r++) {#if DEBUG_JACKPOT				if (r == 0) {					this->_swap[c][r] = LTE_MATRIX_SYMBOL_JACKPOT;				} else {					this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;				}#elif DEBUG_BONUSGAME_1ST				if (r == 0 && c < 3) {					this->_swap[c][r] = LTE_MATRIX_SYMBOL_BONUS_GAME;				} else {					this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;				}#elif DEBUG_BONUSGAME_2ND				if (r == 0 && c < 4) {					this->_swap[c][r] = LTE_MATRIX_SYMBOL_BONUS_GAME;				} else {					this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;				}#elif DEBUG_BONUSGAME_3RD				if (r == 0 && c < 5) {					this->_swap[c][r] = LTE_MATRIX_SYMBOL_BONUS_GAME;				} else {					this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;				}#elif DEBUG_FREESPIN_1ST				if (r == 0 && c < 3) {					this->_swap[c][r] = LTE_MATRIX_SYMBOL_FREE_SPIN;				} else {					this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;				}#elif DEBUG_FREESPIN_2ND				if (r == 0 && c < 4) {					this->_swap[c][r] = LTE_MATRIX_SYMBOL_FREE_SPIN;				} else {					this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;				}#elif DEBUG_FREESPIN_3RD				if (r == 0 && c < 5) {					this->_swap[c][r] = LTE_MATRIX_SYMBOL_FREE_SPIN;				} else {					this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;				}#elif DEBUG_UPPER				if (r == 0) {					this->_swap[c][r] = rand() % 3 + 7;				} else {					this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;				}#elif DEBUG_ALL				if (r == 0) {					this->_swap[c][r] = rand() % 4 + 6;				} else {					this->_swap[c][r] = rand() % 4 + 6;				}#elif DEBUG_PRIZE				if (r == 0) {					this->_swap[c][r]= rand() % 6;				} else {					this->_swap[c][r]= rand() % 6;				}#else				this->_swap[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;#endif			}		}#endif		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0; r < this->LOCAL_SIZE_TAIL; r++) {				this->_tail[c][r] = rand() % this->LOCAL_SIZE_SYMBOL;			}		}		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0, n = this->LOCAL_SIZE_TAIL - this->LOCAL_SIZE_ROWS; r < this->LOCAL_SIZE_ROWS; r++) {				this->_tail[c][n] = this->_swap[c][r];			}		}	}	void Swap() {		for (std::size_t c = 0; c < this->LOCAL_SIZE_COLUMNS; c++) {			for (std::size_t r = 0; r < this->LOCAL_SIZE_ROWS; r++) {				this->_cell[c][r] = this->_swap[c][r];			}		}	}	int GetCell(std::size_t cols, std::size_t rows) const {		if (cols >= this->_cell.size()) {			std::cerr << "ERRO: Illegal index at cell column as '" << cols << "' when  maximum as '" << this->_cell.size() << "', failed" << std::endl;			exit(0);		}		if (rows >= this->_cell[cols].size()) {			std::cerr << "ERRO: Illegal index at cell row as '" << rows << "' when  maximum as '" << this->_cell[cols].size() << "', failed" << std::endl;			exit(0);		}		return this->_cell[cols][rows];	}	int GetSwap(std::size_t cols, std::size_t rows) const {		if (cols >= this->_swap.size()) {			std::cerr << "ERRO: Illegal index at swap column as '" << cols << "' when  maximum as '" << this->_swap.size() << "', failed" << std::endl;			exit(0);		}		if (rows >= this->_swap[cols].size()) {			std::cerr << "ERRO: Illegal index at swap row as '" << rows << "' when  maximum as '" << this->_swap[cols].size() << "', failed" << std::endl;			exit(0);		}		return this->_swap[cols][rows];	}	int GetTail(std::size_t cols, std::size_t rows) const {		if (cols >= this->_tail.size()) {			std::cerr << "ERRO: Illegal index at tail column as '" << cols << "' when  maximum as '" << this->_tail.size() << "', failed" << std::endl;			exit(0);		}		if (rows >= this->_tail[cols].size()) {			std::cerr << "ERRO: Illegal index at tail row as '" << rows << "' when  maximum as '" << this->_tail[cols].size() << "', failed" << std::endl;			exit(0);		}		return this->_tail[cols][rows];	}	std::size_t GetSizeCell() const {		return this->LOCAL_SIZE_COLUMNS;	}	std::size_t GetSizeSwap() const {		return this->LOCAL_SIZE_COLUMNS;	}	std::size_t GetSizeTail() const {		return this->LOCAL_SIZE_TAIL;	}	std::size_t GetSizeRow() const {		return this->LOCAL_SIZE_ROWS;	}	std::size_t GetSizeColumn() const {		return this->LOCAL_SIZE_ROWS;	}private:	std::size_t LOCAL_SIZE_COLUMNS { 0 };	std::size_t LOCAL_SIZE_ROWS { 0 };	std::size_t LOCAL_SIZE_SYMBOL { 0 };	std::size_t LOCAL_SIZE_TAIL { 0 };protected:	LTE_Randomizer _randomizer;	std::vector<std::vector<int>> _tail { std::vector<std::vector<int>>() };	std::vector<std::vector<int>> _cell { std::vector<std::vector<int>>() };	std::vector<std::vector<int>> _swap { std::vector<std::vector<int>>() };};#endif /* MATRIX_H_ */