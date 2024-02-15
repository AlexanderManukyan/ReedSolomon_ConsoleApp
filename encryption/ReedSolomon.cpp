#include "lib.h"
#include "ReedSolomon.h"

polynomial<GF256> RS::syndrome(const polynomial<GF256>& RSP) {
	polynomial<GF256> syndromeP;
	for (uint8_t i = 1; i <= r; i++) {
		GF256 answer = RSP.answerFor(GF256::antilogTab[i]);
		if (answer) {
			syndromeP.pushBack(answer.getNum(), i - 1);
		}
	}
	return syndromeP;
}

polynomial<GF256> RS::locator(const polynomial<GF256>& syndromeP) {
	polynomial<GF256> locatorP, LP1;
	locatorP.pushBack(1, 0);
	LP1.pushBack(1, 0);
	for (uint8_t i = 0; i < r; i++) {
		polynomial<GF256> buf;
		buf.monomial(1, 1);
		LP1 = LP1 * buf;
		GF256 Delta = syndromeP.getValByPower(i);
		for (uint8_t j = 1; j <= locatorP.getMaxPower(); j++) {
			Delta += locatorP.getValByPower(j) * syndromeP.getValByPower(i - j);
		}
		if (Delta) {
			if (LP1.getMaxPower() > locatorP.getMaxPower()) {
				polynomial<GF256> LP2 = LP1 * Delta;
				LP1 = locatorP * Delta.inv();
				locatorP = LP2;
			}
			locatorP += LP1 * Delta;
		}
	}
	return locatorP;
}

polynomial<GF256> RS::errors(const polynomial<GF256>& syndromeP, const polynomial<GF256>& locatorP) {
	polynomial<GF256> buf;
	buf.monomial(1, r);
	return syndromeP * locatorP % buf;
}

vector<uint16_t> RS::errorPositions(const polynomial<GF256>& locatorP) {
	vector<uint16_t> result;
	for (short i = 0; i <= GF_SIZE - 1; i++)
		if (!locatorP.answerFor(i)) {
			GF256 x = i;
			x = x.inv();
			result.push_back(GF256::logTab[x.getNum()]);
		}
	if (result.empty()) {
		throw invalid_argument("Позиции ошибок не определены");
	}
	return result;
}

polynomial<GF256> RS::Forney(const polynomial<GF256>& syndromeP) {
	polynomial<GF256> locatorP = locator(syndromeP);
	polynomial<GF256> errorsP = errors(syndromeP, locatorP);
	polynomial<GF256> derLocatorP = locatorP.formal_derivative();
	polynomial<GF256> ForneyP;
	vector<uint16_t> ep = RS::errorPositions(locatorP);	

	for (uint16_t i = 0; i < ep.size(); i++) {
		GF256 xi = GF256::antilogTab[ep[i]];
		xi = xi.inv();
		if (ForneyP.isEmpty()) {
			ForneyP.monomial(errorsP.answerFor(xi) / derLocatorP.answerFor(xi), ep[i]);
		}
		else {
			polynomial<GF256> buf;
			buf.monomial(errorsP.answerFor(xi) / derLocatorP.answerFor(xi), ep[i]);
			ForneyP += buf;
		}
	}
	return ForneyP;
}

polynomial<GF256> RS::code(const polynomial<GF256>& msgP) {
	polynomial<GF256> generatorP, buf, excessP;
	for (int i = 1; i <= r; i++) {
		buf.pushBack(GF256::antilogTab[i], 0);
		buf.pushBack(1, 1);
		if (generatorP.isEmpty()) {
			generatorP = buf;
		}
		else {
			generatorP = generatorP * buf;
		}
		buf.clear();
	}
	buf.monomial(1, r);
	excessP = msgP * buf % generatorP;
	return excessP + msgP * buf;
}

polynomial<GF256> RS::decode(const polynomial<GF256>& RSP) {
	polynomial<GF256> syndromeP = syndrome(RSP), buf;
	buf.monomial(1, r);
	if (syndromeP.isEmpty())
		return RSP / buf;
	else {
		polynomial<GF256> F = Forney(syndromeP);
		return (RSP + F) / buf;
	}
}
