#pragma once
#include "lib.h"
const uint16_t GF_SIZE = 256;
const uint16_t irreduciblePolynomial = 0b111000011;

class GF256 {
private:
	static uint16_t GFMultiply(uint16_t a, uint16_t b);

	static pair<vector<uint16_t>, vector<uint16_t>> buildLogarithmTables();
	static void printTable(const vector<uint16_t>& logarithm);

	uint16_t num;

public:
	GF256();
	GF256(uint16_t n);
	void operator = (const GF256& g);
	GF256 operator + (const GF256& g) const;
	GF256 operator - (const GF256& g) const;
	GF256 operator - () const;
	GF256 operator * (const GF256& g) const;
	GF256 operator / (const GF256& g) const;
	GF256 operator ^ (const uint16_t& num) const;
	GF256 operator ^ (const GF256& g) const;
	void operator += (const GF256& g);
	void operator *= (const GF256& g);
	void operator /= (const GF256& g);
	void operator ^= (const GF256& g);
	bool operator == (const GF256& g);
	bool operator != (const GF256& g);
	bool operator > (const GF256& g);
	bool operator < (const GF256& g);
	bool operator > (const uint16_t& num);
	bool operator < (const uint16_t& num);
	explicit operator bool() const; 
	GF256 inv() const;
	uint16_t getNum() const;

	static inline const vector<uint16_t> logTab = buildLogarithmTables().first;
	static inline const vector<uint16_t> antilogTab = buildLogarithmTables().second;
	static void printLogTable();
	static void printAntilogTable();

	friend ostream& operator << (ostream& out, const GF256& Q);
};