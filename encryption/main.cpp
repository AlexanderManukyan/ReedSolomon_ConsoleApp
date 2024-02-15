#include "lib.h"
#include "polynomial.h"
#include "GF256.h"
#include "ReedSolomon.h"

int main() {
	setlocale(LC_ALL, "RUS");
	try {

		polynomial<GF256> p1;
		p1.pushBack(241, 0);
		p1.pushBack(238, 1);
		p1.pushBack(238, 2);
		p1.pushBack(225, 3);
		p1.pushBack(249, 4);
		p1.pushBack(229, 5);
		p1.pushBack(237, 6);
		p1.pushBack(232, 7);
		p1.pushBack(229, 8);
		polynomial<GF256> p2 = RS::code(p1);
		p2.pushBack(241, 4);
		p2.pushBack(32, 4);
		p2.pushBack(238, 5);
		p2.pushBack(32, 5);
		p1.print();
		p2.print();
		p2 = RS::decode(p2);
		p2.print();
	}
	catch(std::invalid_argument& ex) {
		cout << ex.what();
	}
}