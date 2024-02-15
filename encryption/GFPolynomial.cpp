#include "lib.h"
#include "polynomial.h"
#include "GF256.h"

bool polynomial::isEmpty() const { // �������� �� ������� ����������
	return first == nullptr;
}

void polynomial::pushBack(float val, int power) { // ���������� ��������
	if (power < 0) {
		throw invalid_argument("C������ ������ ���� �������������!\n");
	}
	if (val) {
		node* nd = new node(val, power);
		if (isEmpty()) { // ���� ��������� ����, �� ��������� ��� ������
			first = nd;
			last = nd;
			length++;
			return;
		}
		else { // ���� ���
			node* seekNd = first;

			while (seekNd) {
				if (seekNd->power == nd->power) { // �� ���� �������� �� ������ ��������
					seekNd->val += nd->val; // � ��������� ��
					break;
				}
				seekNd = seekNd->next;
			}

			if (!seekNd) { // ���� ����� ���������, �� ��������� � ����� ������ ���� �������
				node* tmp = last;
				last->next = nd;
				last = nd;
				last->prev = tmp;
				this->quickSort();
				length++;
			}
		}
	}
}

void polynomial::print() { // ��������������� ����� ����������
	stringstream ss;
	ss.precision(3); // ���-��� ������ ����� �������
	if (!isEmpty()) {
		node* nd = first;
		while (nd) {
			if (nd->val > 0) {
				if (nd->power == 0) {
					ss << "+" << nd->val;
				}
				else {
					if (nd->val == 1) {
						ss << "+x^(" << nd->power << ")";
					}
					else {
						ss << "+" << nd->val << "x^(" << nd->power << ")";
					}
				}
			}
			else if (nd->val < 0) {
				if (nd->power == 0) {
					ss << nd->val;
				}
				else {
					if (nd->val == -1) {
						ss << "-x^(" << nd->power << ")";
					}
					else {
						ss << nd->val << "x^(" << nd->power << ")";
					}
				}
			}
			nd = nd->next;
		}

		string res = ss.str();
		if (res[0] == '+') {
			res.erase(0, 1);
		}
		cout << res << endl << endl;
	}
	else {
		cout << 0 << endl << endl;
	}
}

polynomial::node* polynomial::partition(node* first, node* last) { // ������� ������� ������ �� ��� �����, ������������ �������� �������� (pivot)
	node* pivot = first; // ����� ������� ��������� ����� ������ ������� ������
	node* front = first; // ������� � ��������� ����� ���������� �������� �������� ��������
	pair<float, int> temp = make_pair(0.0F, 0); // ���� ��� ������ ���������� ����� ����������
	while (front && front != last) { // ���� ������������ ������� �� ����� �� �����
		if (front->power < last->power) { // ���� ������� �������� �������� ������ ������� ��������
			pivot = first; // �� ��������� �������
			temp = make_pair(first->val, first->power); // ���������� ��������
			first->val = front->val;
			first->power = front->power;
			front->val = temp.first;
			front->power = temp.second;
			first = first->next; // � ��������� � ���������� ��������
		}
		front = front->next; // ��������� � ���������� �������� ������
	}
	if (first) {
		temp = make_pair(first->val, first->power); // ���������� ������� ������� � ���������
		first->val = last->val;
		first->power = last->power;
		last->val = temp.first;
		last->power = temp.second;
	}
	return pivot; // ���������� ������� �������
}

void polynomial::quickSort(node*& first, node*& last) {
	if (first == last) { // ���� ������ ���� ��� ����� 1 �������, �� ���������� ��������
		return;
	}
	else {
		node* pivot = partition(first, last); // �������� ������� ������� � �� ���� ��������� 
		if (pivot && pivot->next) { // ������
			quickSort(pivot->next, last);
		}
		if (pivot && first != pivot) { // � ����� �����
			quickSort(first, pivot);
		}
	}
}

void polynomial::quickSort() { // ������� ����� ������� ����������
	quickSort(first, last);
}

int polynomial::getLength() {
	return this->length;
}

polynomial polynomial::operator + (const polynomial& p) { // �������� ������������ �����������
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = first;
		node* seekNd = p.first;
		while (checkNd) {
			resP.pushBack(checkNd->val, checkNd->power); // ����� �������� ������� ����������
			checkNd = checkNd->next;
		}
		while (seekNd) {
			resP.pushBack(seekNd->val, seekNd->power); // � ��������� � ������ (������������ ����������� � pushBack)
			seekNd = seekNd->next;
		}
		return resP;
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator - (const polynomial& p) { // �������� ��������, ���������� � ������������� 
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = this->first;
		node* seekNd = p.first;
		while (checkNd) {
			resP.pushBack(checkNd->val, checkNd->power);
			checkNd = checkNd->next;
		}
		while (seekNd) {
			resP.pushBack(-seekNd->val, seekNd->power);
			seekNd = seekNd->next;
		}
		return resP;
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator - () { // ������� �����
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = this->first;
		while (checkNd) {
			resP.pushBack(checkNd->val * -1, checkNd->power); // �������� ��� �������� �� (-1)
			checkNd = checkNd->next;
		}
		return resP;
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator * (const polynomial& p) { // �������� ������������ �����������
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = this->first;
		node* seekNd = p.first;
		while (checkNd) {
			while (seekNd) {
				resP.pushBack(checkNd->val * seekNd->val, checkNd->power + seekNd->power); // �������� �������������, � ������� ������������
				seekNd = seekNd->next;
			}
			checkNd = checkNd->next;
			seekNd = p.first;
		}
		return resP;
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator * (const float t) { // �������� ������������ ���������� �� �����
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = this->first;
		while (checkNd) {
			resP.pushBack(checkNd->val * t, checkNd->power); // �������� ������������� �� ��������� �����
			checkNd = checkNd->next;
		}
		return resP;
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator / (const float t) {
	if (t) {
		node* tmp = this->first;
		while (tmp) {
			tmp->val /= t;
			tmp = tmp->next;
		}
		return *this;
	}
	else {
		throw invalid_argument("�������� ����� 0!\n");
	}
}

polynomial polynomial::operator % (const polynomial& p) { // �������� ������ ������� �� �������
	if (p.isEmpty()) {
		throw invalid_argument("�������� ����� 0!\n");
	}
	else if (!isEmpty()) {
		polynomial editP;
		polynomial delP;
		node* checkNd = this->first;
		node* seekNd = p.first;
		while (checkNd) {
			editP.pushBack(checkNd->val, checkNd->power); // ���������� �������� ���������
			checkNd = checkNd->next;
		}

		while (seekNd) {
			delP.pushBack(seekNd->val, seekNd->power); // � �����������
			seekNd = seekNd->next;
		}


		if (delP.length == 1 && delP.first->power == 0) {
			return polynomial();
		}

		while (delP.last->power <= editP.last->power) { // ���� ������������ ������� ��������� ������ ������������ ������� �����������
			polynomial multiP;
			multiP.pushBack(editP.last->val / delP.last->val, editP.last->power - delP.last->power); // ���������� ������� ����� ����������, �������� ��� �������
			editP = editP - delP * multiP; // �������� ���������
			editP.quickSort(); // ��������� ��������, ���� ������� ��� ���������������, �.�. ��� ��������� editP � ����� ������ ����������� �������� ����� ��������, � ��� �������, � ������� ����������� �������� ���� �������������.

			node* tmp = editP.last; // ������� ��������� �������
			editP.last = editP.last->prev;
			if (editP.last) {
				editP.last->next = 0;
				delete tmp;
			}
			else {
				break;
			}
		}
		return editP; // ���������� �������
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator / (const polynomial& p) { // �������� ���������� ���������� �������
	if (p.isEmpty()) {
		throw invalid_argument("�������� ����� 0!\n");
	}
	else if (!isEmpty()) {
		polynomial editP;
		polynomial delP;
		polynomial resP;
		node* checkNd = this->first;
		node* seekNd = p.first;
		while (checkNd) {
			editP.pushBack(checkNd->val, checkNd->power);
			checkNd = checkNd->next;
		}

		while (seekNd) {
			delP.pushBack(seekNd->val, seekNd->power);
			seekNd = seekNd->next;
		}

		while (delP.last->power <= editP.last->power) {
			polynomial multiP;
			multiP.pushBack(editP.last->val / delP.last->val, editP.last->power - delP.last->power);
			editP = editP - delP * multiP;
			editP.quickSort();

			node* tmp = editP.last;
			editP.last = editP.last->prev;
			resP.pushBack(multiP.first->val, multiP.first->power); // ������ �� ��������� ��������, �� ������� �������� ��������
			if (editP.last) {
				editP.last->next = 0;
				delete tmp;
			}
			else {
				delete tmp;
				break;
			}

		}
		return resP;
	}
	else {
		return *this;
	}
}