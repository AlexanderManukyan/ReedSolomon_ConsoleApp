#include "GF256.h"

uint16_t GF256::GFMultiply(uint16_t a, uint16_t b) {
	uint16_t result = 0;

	while (b > 0) {
		if (b & 1) {
			result ^= a;
		}

		a <<= 1;

		if (a & GF_SIZE) {
			a ^= irreduciblePolynomial;
		}

		b >>= 1;
	}

	return result;
}

pair<vector<uint16_t>, vector<uint16_t>> GF256::buildLogarithmTables() {
	vector<uint16_t> logarithm(GF_SIZE, -1);
	vector<uint16_t> antiLogarithm(GF_SIZE, GF_SIZE - 1);

	uint16_t current = 1;
	for (uint16_t i = 0; i < GF_SIZE - 1; i++) {
		logarithm[current] = i; 
		antiLogarithm[i] = current;
		current = GFMultiply(current, 2);
	}
	return make_pair(logarithm, antiLogarithm);
}

void GF256::printTable(const vector<uint16_t>& logarithm) {
	for (uint16_t i = 0; i < GF_SIZE; ++i) {
		cout << logarithm[i];
		if (i != GF_SIZE - 1) {
			cout << ", ";
		}
	}
	cout << endl << endl;
}

void GF256::printLogTable() {
	printTable(logTab);
}

void GF256::printAntilogTable() {
	printTable(antilogTab);
}

void GF256::operator = (const GF256& g) {
	this->num = g.num;
}

GF256::GF256(uint16_t n) {
	while (n > GF_SIZE) { 
		n = n % GF_SIZE;
	}
	num = n;
}

GF256::GF256() {
	num = 0;
}

GF256 GF256::operator + (const GF256& g) const {
	return GF256(num ^ g.num);
}

GF256 GF256::operator - (const GF256& g) const {
	return GF256(num ^ g.num);
}

GF256 GF256::operator - () const {
	return *this;
}

GF256 GF256::operator * (const GF256& g) const {
	uint16_t x = logTab[num];
	uint16_t y = logTab[g.num];
	return antilogTab[(x + y) % (GF_SIZE - 1)];
}

GF256 GF256::operator / (const GF256& g) const {
	if (g.num == 1) {
		return *this;
	}
	else if (g.num > 1) {
		return *this * g.inv();
	}
	else {
		throw invalid_argument("Деление на ноль");
	}
}

GF256 GF256::operator ^ (const uint16_t& num) const {

	if (num == 1) {
		return *this;
	}
	else if (num == 0) {
		return GF256(1);
	}
	else {
		GF256 temp = *this;
		GF256 res;
		for (int i = 1; i < num; i++) {
			if (i == 1) {
				res = *this * temp;
			}
			else {
				res *= temp;
			}
		}
		return GF256(res);
	}
}

GF256 GF256::operator ^ (const GF256& g) const {

	if (g.num == 1) {
		return *this;
	}
	else if (g.num == 0) {
		return GF256(1);
	}
	else {
		GF256 temp = *this;
		GF256 res;
		for (uint8_t i = 1; i < g.num; i++) {
			if (i == 1) {
				res = *this * temp;
			}
			else {
				res *= temp;
			}
		}
		return GF256(res);
	}

}

void GF256::operator += (const GF256& g) {
	*this = *this + g;
}

void GF256::operator *= (const GF256& g) {
	*this = *this * g;
}

void GF256::operator /= (const GF256& g) {
	*this = *this / g;
}

void GF256::operator ^= (const GF256& g) {
	*this = *this ^ g;
}

bool GF256::operator == (const GF256& g) {
	return (num == g.num);
}

bool GF256::operator != (const GF256& g) {
	return (num != g.num);
}

bool GF256::operator > (const GF256& g) {
	return (this->num > g.num);
}

bool GF256::operator < (const GF256& g) {
	return (this->num < g.num);
}

bool GF256::operator > (const uint16_t& num) {
	return (this->num > num);
}

bool GF256::operator < (const uint16_t& num) {
	return (this->num < num);
}

GF256::operator bool() const {
	return (num != 0);
}

ostream& operator << (ostream& out, const GF256& g) {
	out << g.num;
	return out;
}

GF256 GF256::inv() const {
	uint16_t s = 1;
	uint16_t x = this->num;
	for (uint16_t i = 1; i < 8; i++) {
		x = GFMultiply(x, x);
		s = GFMultiply(s, x);
	}
	return GF256(s);
}

uint16_t GF256::getNum() const {
	return num;
}
