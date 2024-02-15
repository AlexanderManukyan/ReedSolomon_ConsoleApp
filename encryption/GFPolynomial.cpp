#include "lib.h"
#include "polynomial.h"
#include "GF256.h"

bool polynomial::isEmpty() const { // проверка на пустоту многочлена
	return first == nullptr;
}

void polynomial::pushBack(float val, int power) { // добавление элемента
	if (power < 0) {
		throw invalid_argument("Cтепень должна быть положительной!\n");
	}
	if (val) {
		node* nd = new node(val, power);
		if (isEmpty()) { // если многочлен пуст, то добавляем как первый
			first = nd;
			last = nd;
			length++;
			return;
		}
		else { // если нет
			node* seekNd = first;

			while (seekNd) {
				if (seekNd->power == nd->power) { // то ищем элементы со схожей степенью
					seekNd->val += nd->val; // и суммируем их
					break;
				}
				seekNd = seekNd->next;
			}

			if (!seekNd) { // если таких элементов, то добавляем в конец списка этот элемент
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

void polynomial::print() { // форматированный вывод многочлена
	stringstream ss;
	ss.precision(3); // кол-вол знаков после запятой
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

polynomial::node* polynomial::partition(node* first, node* last) { // функция деления списка на две части, относительно опорного элемента (pivot)
	node* pivot = first; // пусть опорным элементом будет первый элемент списка
	node* front = first; // элемент с которомым будем сравнивать значения опорного элемента
	pair<float, int> temp = make_pair(0.0F, 0); // пара для обмена значениями между элементами
	while (front && front != last) { // пока сравниваемый элемент не дошел до конца
		if (front->power < last->power) { // если степень текущего элемента меньше степени опорного
			pivot = first; // то обновляем опорный
			temp = make_pair(first->val, first->power); // обмениваем значения
			first->val = front->val;
			first->power = front->power;
			front->val = temp.first;
			front->power = temp.second;
			first = first->next; // и переходим к следующему элементу
		}
		front = front->next; // переходим к следующему элементу списка
	}
	if (first) {
		temp = make_pair(first->val, first->power); // обмениваем опорный элемент с последним
		first->val = last->val;
		first->power = last->power;
		last->val = temp.first;
		last->power = temp.second;
	}
	return pivot; // возвращаем опорный элемент
}

void polynomial::quickSort(node*& first, node*& last) {
	if (first == last) { // если список пуст или имеет 1 элемент, то завершваем рекурсию
		return;
	}
	else {
		node* pivot = partition(first, last); // получаем опорный элемент и по нему сортируем 
		if (pivot && pivot->next) { // правую
			quickSort(pivot->next, last);
		}
		if (pivot && first != pivot) { // и левую части
			quickSort(first, pivot);
		}
	}
}

void polynomial::quickSort() { // удобный вызов функции сортировки
	quickSort(first, last);
}

int polynomial::getLength() {
	return this->length;
}

polynomial polynomial::operator + (const polynomial& p) { // оператор суммирования многочленов
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = first;
		node* seekNd = p.first;
		while (checkNd) {
			resP.pushBack(checkNd->val, checkNd->power); // берем элементы первого многочлена
			checkNd = checkNd->next;
		}
		while (seekNd) {
			resP.pushBack(seekNd->val, seekNd->power); // и суммируем с первым (суммирование реализовано в pushBack)
			seekNd = seekNd->next;
		}
		return resP;
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator - (const polynomial& p) { // оператор разности, аналогично с суммированием 
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

polynomial polynomial::operator - () { // унарный минус
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = this->first;
		while (checkNd) {
			resP.pushBack(checkNd->val * -1, checkNd->power); // умножаем все элементы на (-1)
			checkNd = checkNd->next;
		}
		return resP;
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator * (const polynomial& p) { // оператор произведения многочленов
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = this->first;
		node* seekNd = p.first;
		while (checkNd) {
			while (seekNd) {
				resP.pushBack(checkNd->val * seekNd->val, checkNd->power + seekNd->power); // значения перемножаются, а степени складываются
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

polynomial polynomial::operator * (const float t) { // оператор произведения многочлена на число
	if (!isEmpty()) {
		polynomial resP;
		node* checkNd = this->first;
		while (checkNd) {
			resP.pushBack(checkNd->val * t, checkNd->power); // значения перемножаются на некоторое число
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
		throw invalid_argument("Делитель равен 0!\n");
	}
}

polynomial polynomial::operator % (const polynomial& p) { // оператор вывода остатка от деления
	if (p.isEmpty()) {
		throw invalid_argument("Делитель равен 0!\n");
	}
	else if (!isEmpty()) {
		polynomial editP;
		polynomial delP;
		node* checkNd = this->first;
		node* seekNd = p.first;
		while (checkNd) {
			editP.pushBack(checkNd->val, checkNd->power); // определяем элементы числителя
			checkNd = checkNd->next;
		}

		while (seekNd) {
			delP.pushBack(seekNd->val, seekNd->power); // и знаменателя
			seekNd = seekNd->next;
		}


		if (delP.length == 1 && delP.first->power == 0) {
			return polynomial();
		}

		while (delP.last->power <= editP.last->power) { // пока максимальная степень числителя больше максимальной степени знаменателя
			polynomial multiP;
			multiP.pushBack(editP.last->val / delP.last->val, editP.last->power - delP.last->power); // определяем разницу между элементами, имеющими эти степени
			editP = editP - delP * multiP; // изменяем многочлен
			editP.quickSort(); // сортируем значения, дабы удалить уже испрользованное, т.к. при изменении editP в конец списка добавляется значения после операции, а тот элемент, с которым проводились операции стал предпоследним.

			node* tmp = editP.last; // удаляем вычтенный элемент
			editP.last = editP.last->prev;
			if (editP.last) {
				editP.last->next = 0;
				delete tmp;
			}
			else {
				break;
			}
		}
		return editP; // возвращаем остаток
	}
	else {
		return *this;
	}
}

polynomial polynomial::operator / (const polynomial& p) { // операции аналогичны нахождению остатка
	if (p.isEmpty()) {
		throw invalid_argument("Делитель равен 0!\n");
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
			resP.pushBack(multiP.first->val, multiP.first->power); // только мы сохраняем элементы, на которые умножали делитель
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