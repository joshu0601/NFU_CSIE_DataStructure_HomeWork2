#include<iostream>
#include <time.h>
using namespace std;
class Polynomial;
class Term {//�w�qTerm����
	friend Polynomial;
	friend ostream& operator<<(ostream& os, const Polynomial& p);//�h����X�B��l
	friend istream& operator>>(istream& os, const Polynomial& p);//�h����J�B��l
private:
	float coef;//�Y�� 
	int exp;//���Ʀ��� 
};

class Polynomial {//�w�qPolynomial���O
	friend ostream& operator<<(ostream& os, const Polynomial& p);//�h����X�B��l
	friend istream& operator>>(istream& os, const Polynomial& p);//�h����J�B��l
private:
	Term* termArray;//�����}�C
	int capacity;//�O����j�p  
	int terms;//�h�������� 
public:
	Polynomial();
	float Eval(float f);//�h�����D�� 
	void NewTerm(const float newCoef, const int newExp);//�[�J�s�� 
	Polynomial Mult(Polynomial poly);//�h�����ۭ� 
	Polynomial Add(Polynomial b);//�h�����ۥ[ 
};

Polynomial::Polynomial() :capacity(2), terms(0) {//�غc�l
	termArray = new Term[capacity];
}

void Polynomial::NewTerm(const float newCoef, const int newExp = 0) {//�[�J�s�� 
	if (terms == capacity)//�Ŷ�����
	{
		capacity *= 2;//�Ŷ�½��
		Term* tmp = new Term[capacity];
		copy(termArray, termArray + terms, tmp);
		delete[]termArray;//�R����}�C�A����O����
		termArray = tmp;
	}
	termArray[terms].coef = newCoef;//��J�Y��
	termArray[terms++].exp = newExp;//��J�����
}

float Polynomial::Eval(float f) {//�h�����D�� 
	float t = 0;
	for (int i = 0; i < terms; i++)//�Nf���Ȩ̧ǥN�J�ۥ[
	{
		t += termArray[i].coef * pow(f, termArray[i].exp);
	}
	return t;
}

Polynomial Polynomial::Add(Polynomial b) {//�h�����ۥ[ 
	Polynomial c;//���G�h����
	int apos = 0, bpos = 0;
	while ((apos < terms) && (bpos < b.terms))//����ۦP�Y�Ƭۥ[
	{
		if (termArray[apos].exp == b.termArray[bpos].exp)
		{
			float t = termArray[apos].coef + b.termArray[bpos].coef;
			if (t)
			{
				c.NewTerm(t, termArray[apos].exp);//�[�J���G�h������
			}
			apos++;
			bpos++;
		}
		else if (termArray[apos].exp < b.termArray[bpos].exp)//poly1����<poly2����Apoly2�[�J���G�h������
		{
			c.NewTerm(b.termArray[bpos].coef, b.termArray[bpos].exp);
			bpos++;
		}
		else//poly1����>poly2����Apoly1�[�J���G�h������
		{
			c.NewTerm(termArray[apos].coef, termArray[apos].exp);
			apos++;
		}
	}
	for (; apos < terms; apos++)//�Npoly1�Ѿl���[�J���G�h������
	{
		c.NewTerm(termArray[apos].coef, termArray[apos].exp);
	}
	for (; bpos < b.terms; bpos++)//�Npoly2�Ѿl���[�J���G�h������
	{
		c.NewTerm(b.termArray[bpos].coef, b.termArray[bpos].exp);
	}
	return c;

}

Polynomial Polynomial::Mult(Polynomial poly) {//�h�����ۭ� 
	Polynomial res;
	int use_cap = termArray[0].exp * poly.termArray[0].exp; //�w�ιL������X�а}�C�j�p
	bool* use = new bool[use_cap]();//�Ω��ˬd�O�_�����Ʀ���ƪ���
	for (int i = 0; i < terms; i++)//�C���@�@�ۭ�
	{
		for (int j = 0; j < poly.terms; j++)
		{
			int mult_exp = termArray[i].exp + poly.termArray[j].exp;//�ۭ��᪺����	
			if (mult_exp >= use_cap)
			{
				bool* tmp = new bool[use_cap * 2];
				copy(use, use + use_cap, tmp);
				delete[]use;
				use = tmp;
				use_cap *= 2;
			}
			if (use[mult_exp])//�w�ιL�A�N�ۭ��������ϥΥ[�k�\��[�J�h����
			{
				Polynomial temp;
				temp.NewTerm(termArray[i].coef * poly.termArray[j].coef, mult_exp);
				res = res.Add(temp);
			}
			else//���ιL�A�N�ۭ��������[�J�h����
			{
				res.NewTerm(termArray[i].coef * poly.termArray[j].coef, mult_exp);
				use[mult_exp] = true;
			}
		}
	}
	return res;
}

ostream& operator<<(ostream& os, const Polynomial& p) {//�h����X�B��l
	for (int i = 0; i < p.terms; i++) {
		if (p.termArray[i].exp == 0) {
			os << p.termArray[i].coef;
			continue;
		}
		if (i == p.terms - 1) {//�̫ᶵ���["+"
			os << p.termArray[i].coef << "X^" << p.termArray[i].exp;
		}
		else {
			os << p.termArray[i].coef << "X^" << p.termArray[i].exp << "+";
		}
	}

	return os;
}
istream& operator>>(istream& os, Polynomial& p) {//�h����J�B��l
	float coeftmp;//�Ȧscoef
	int exptmp;//�Ȧsexp
	os.ignore();//����(
	while (1)
	{
		os >> coeftmp;//Ū�J�Y��
		os.ignore(2);//���� X^
		os >> exptmp;//Ū�J����
		p.NewTerm(coeftmp, exptmp);//�[�J�sterm
		char c;
		os >> c;
		if (c == ')')break;//Ū��A�������X
	}
	os.get();//�Y������
	return os;
}
int main() {
	//�ŧi�T�Ӧh��������
	Polynomial poly1;
	Polynomial poly2;
	Polynomial poly3;
	clock_t start;
	int x;
	cout << "��J�榡 (aX^n1+bX^n2+cX^n3)" << endl;
	cout << "poly1:";
	cin >> poly1;
	cout << "poly2:";
	cin >> poly2;

	cout << "(" << poly1 << ") + (" << poly2 << ") = ";
	poly3 = poly1.Add(poly2);

	cout << poly3 << endl;
	cout << "(" << poly1 << ") X (" << poly2 << ") = ";
	poly3 = poly1.Mult(poly2);
	cout << poly3 << endl;

	cout << "��J�n�N�J����:";
	cin >> x;
	cout << "X = " << x << ":" << endl;
	cout << "(" << poly1 << ") + (" << poly2 << ") = ";
	poly3 = poly1.Add(poly2);
	cout << poly3.Eval(x) << endl;
	cout << "(" << poly1 << ") X (" << poly2 << ") = ";
	poly3 = poly1.Mult(poly2);
	cout << poly3.Eval(x) << endl;
	return 0;
}