#pragma once
#include "lib.h"
#include "GF256.h"

template <typename templ>
class polynomial {

private:
	class node {
	public:
		templ val;
		int16_t power;
		node* next;
		node* prev;
		node(templ val, int16_t power) {
			this->val = val;
			this->power = power;
			next = nullptr;
			prev = nullptr;
		}
	};

private:
	node* first = 0;
	node* last = 0;
	int16_t length = 0;

	bool isEmpty() const;
	node* partition(node* first, node* last);
	void quickSort(node*& first, node*& last);
	void quickSort();

public:
	void pushBack(const templ& val, int16_t power);
	void pushBack(const node& nd);
	void monomial(templ val, int16_t power);
	void print() const;
	void clear();

	int16_t getLength() const;
	templ getValByPower(int16_t power) const;
	int16_t getMaxPower();
	templ answerFor(const templ& x) const;
	polynomial derivative();
	polynomial<GF256> formal_derivative();

	void operator = (const polynomial& p);
	polynomial operator + (const polynomial& p) const;
	polynomial operator + (const templ& secObj) const;
	polynomial operator - () const;
	polynomial operator - (const polynomial& p) const;
	polynomial operator - (const templ& secObj) const;
	polynomial operator * (const polynomial& p) const;
	polynomial operator * (const templ& secObj) const;
	polynomial operator / (const polynomial& p) const;
	polynomial operator / (const templ& secObj) const;
	polynomial operator % (const polynomial& p) const;
	void operator += (const polynomial& p);
	void operator += (const templ& secObj);
	void operator -= (const polynomial& p);
	void operator -= (const templ& secObj);
	void operator *= (const polynomial& p);
	void operator *= (const templ& secObj);
	void operator /= (const polynomial& p);
	void operator /= (const templ& secObj);
	void operator %= (const polynomial& p);
	friend class RS;
};

#include "polynomial.impl"

