#pragma once
#include "lib.h"
#include "polynomial.h"
#include "GF256.h"

const uint16_t q = 2;
const uint16_t r = q * 2;

class RS {

private:
	static polynomial<GF256> syndrome(const polynomial<GF256>& RSP);
	static polynomial<GF256> locator(const polynomial<GF256>& syndromeP);
	static polynomial<GF256> errors(const polynomial<GF256>& C, const polynomial<GF256>& L);
	static vector<uint16_t> errorPositions(const polynomial<GF256>& L);
	static polynomial<GF256> Forney(const polynomial<GF256>& C);

public:
	static polynomial<GF256> code(const polynomial<GF256>& S);
	static polynomial<GF256> decode(const polynomial<GF256>& T);

};