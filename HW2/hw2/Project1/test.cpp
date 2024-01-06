#include<iostream>
#include <time.h>
using namespace std;
class Polynomial;
class Term {//定義Term物件
	friend Polynomial;
	friend ostream& operator<<(ostream& os, const Polynomial& p);//多載輸出運算子
	friend istream& operator>>(istream& os, const Polynomial& p);//多載輸入運算子
private:
	float coef;//係數 
	int exp;//指數次方 
};

class Polynomial {//定義Polynomial類別
	friend ostream& operator<<(ostream& os, const Polynomial& p);//多載輸出運算子
	friend istream& operator>>(istream& os, const Polynomial& p);//多載輸入運算子
private:
	Term* termArray;//項的陣列
	int capacity;//記憶體大小  
	int terms;//多項式項數 
public:
	Polynomial();
	float Eval(float f);//多項式求值 
	void NewTerm(const float newCoef, const int newExp);//加入新項 
	Polynomial Mult(Polynomial poly);//多項式相乘 
	Polynomial Add(Polynomial b);//多項式相加 
};

Polynomial::Polynomial() :capacity(2), terms(0) {//建構子
	termArray = new Term[capacity];
}

void Polynomial::NewTerm(const float newCoef, const int newExp = 0) {//加入新項 
	if (terms == capacity)//空間不足
	{
		capacity *= 2;//空間翻倍
		Term* tmp = new Term[capacity];
		copy(termArray, termArray + terms, tmp);
		delete[]termArray;//刪除原陣列，釋放記憶體
		termArray = tmp;
	}
	termArray[terms].coef = newCoef;//放入係數
	termArray[terms++].exp = newExp;//放入次方數
}

float Polynomial::Eval(float f) {//多項式求值 
	float t = 0;
	for (int i = 0; i < terms; i++)//將f的值依序代入相加
	{
		t += termArray[i].coef * pow(f, termArray[i].exp);
	}
	return t;
}

Polynomial Polynomial::Add(Polynomial b) {//多項式相加 
	Polynomial c;//結果多項式
	int apos = 0, bpos = 0;
	while ((apos < terms) && (bpos < b.terms))//次方相同係數相加
	{
		if (termArray[apos].exp == b.termArray[bpos].exp)
		{
			float t = termArray[apos].coef + b.termArray[bpos].coef;
			if (t)
			{
				c.NewTerm(t, termArray[apos].exp);//加入結果多項式中
			}
			apos++;
			bpos++;
		}
		else if (termArray[apos].exp < b.termArray[bpos].exp)//poly1次方<poly2次方，poly2加入結果多項式中
		{
			c.NewTerm(b.termArray[bpos].coef, b.termArray[bpos].exp);
			bpos++;
		}
		else//poly1次方>poly2次方，poly1加入結果多項式中
		{
			c.NewTerm(termArray[apos].coef, termArray[apos].exp);
			apos++;
		}
	}
	for (; apos < terms; apos++)//將poly1剩餘項加入結果多項式中
	{
		c.NewTerm(termArray[apos].coef, termArray[apos].exp);
	}
	for (; bpos < b.terms; bpos++)//將poly2剩餘項加入結果多項式中
	{
		c.NewTerm(b.termArray[bpos].coef, b.termArray[bpos].exp);
	}
	return c;

}

Polynomial Polynomial::Mult(Polynomial poly) {//多項式相乘 
	Polynomial res;
	int use_cap = termArray[0].exp * poly.termArray[0].exp; //已用過的次方旗標陣列大小
	bool* use = new bool[use_cap]();//用於檢查是否有重複次方數的項
	for (int i = 0; i < terms; i++)//每項一一相乘
	{
		for (int j = 0; j < poly.terms; j++)
		{
			int mult_exp = termArray[i].exp + poly.termArray[j].exp;//相乘後的次方	
			if (mult_exp >= use_cap)
			{
				bool* tmp = new bool[use_cap * 2];
				copy(use, use + use_cap, tmp);
				delete[]use;
				use = tmp;
				use_cap *= 2;
			}
			if (use[mult_exp])//已用過，將相乘完的項使用加法功能加入多項式
			{
				Polynomial temp;
				temp.NewTerm(termArray[i].coef * poly.termArray[j].coef, mult_exp);
				res = res.Add(temp);
			}
			else//未用過，將相乘完的項加入多項式
			{
				res.NewTerm(termArray[i].coef * poly.termArray[j].coef, mult_exp);
				use[mult_exp] = true;
			}
		}
	}
	return res;
}

ostream& operator<<(ostream& os, const Polynomial& p) {//多載輸出運算子
	for (int i = 0; i < p.terms; i++) {
		if (p.termArray[i].exp == 0) {
			os << p.termArray[i].coef;
			continue;
		}
		if (i == p.terms - 1) {//最後項不加"+"
			os << p.termArray[i].coef << "X^" << p.termArray[i].exp;
		}
		else {
			os << p.termArray[i].coef << "X^" << p.termArray[i].exp << "+";
		}
	}

	return os;
}
istream& operator>>(istream& os, Polynomial& p) {//多載輸入運算子
	float coeftmp;//暫存coef
	int exptmp;//暫存exp
	os.ignore();//忽略(
	while (1)
	{
		os >> coeftmp;//讀入係數
		os.ignore(2);//忽略 X^
		os >> exptmp;//讀入次方
		p.NewTerm(coeftmp, exptmp);//加入新term
		char c;
		os >> c;
		if (c == ')')break;//讀到括號尾跳出
	}
	os.get();//吃掉換行
	return os;
}
int main() {
	//宣告三個多項式物件
	Polynomial poly1;
	Polynomial poly2;
	Polynomial poly3;
	clock_t start;
	int x;
	cout << "輸入格式 (aX^n1+bX^n2+cX^n3)" << endl;
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

	cout << "輸入要代入的數:";
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