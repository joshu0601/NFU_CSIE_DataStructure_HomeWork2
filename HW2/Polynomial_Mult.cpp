#include<bits/stdc++.h>
using namespace std; 

class Polynomial;
class Term{
    friend Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    private:
        float coef;//係數 
        int exp;//指數次方 
};

class Polynomial{
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    private:
        Term *termArray;
        int capacity;//記憶體大小  
        int terms;//項數 
    public:
        Polynomial();//
        float Eval(float f);//求值 
        void NewTerm(const float newCoef,const int newExp);//加入新項 
        Polynomial Mult(Polynomial poly);//多項式相乘 
        Polynomial Add(Polynomial b);//多項式相加 
};

Polynomial::Polynomial():capacity(2),terms(0){
    termArray = new Term[capacity];
}

void Polynomial::NewTerm(const float newCoef,const int newExp){
	if(terms==capacity)
    {
    	capacity*=2;
    	Term *tmp=new Term[capacity];
		copy(termArray,termArray+terms,tmp);
		delete []termArray;
		termArray= tmp;
	}
	termArray[terms].coef=newCoef;
	termArray[terms++].exp=newExp;
}

float Polynomial::Eval(float f){
	int t=0;
    for(int i=0;i<terms;i++)
    {
    	t+=termArray[i].coef*pow(f,termArray[i].exp);
	}
	return t;
}

Polynomial Polynomial::Add(Polynomial b){
	Polynomial c;
	int apos=0,bpos=0;
	while((apos<terms)&&(bpos<terms))
	{
		if(termArray[apos].exp == b.termArray[bpos].exp)
		{
			float t=termArray[apos].coef+b.termArray[bpos].coef;
			if(t)
			{
				c.NewTerm(t,termArray[apos].exp);
			}
			apos++;
			bpos++;
		}
		else if(termArray[apos].exp<termArray[bpos].exp)
		{
			c.NewTerm(b.termArray[bpos].coef,b.termArray[bpos].exp);
			bpos++;
		}
		else
		{
			c.NewTerm(termArray[apos].coef,termArray[apos].exp);
			apos++;
		}
	} 
	for(;apos<terms;apos++)
	{
		c.NewTerm(termArray[apos].coef,termArray[apos].exp);	
	}	
	for(;bpos<terms;bpos++)
	{
		c.NewTerm(termArray[bpos].coef,termArray[bpos].exp);	
	}
	return c;
}

Polynomial Polynomial::Mult(Polynomial poly){
	Polynomial res;
	int use_cap=10;
	bool *use = new bool[10];
	for(int i=0;i<terms;i++)
	{
		for(int j=0;j<poly.terms;j++)
		{
			int mult_exp=termArray[i].exp + poly.termArray[j].exp;
			if(mult_exp>=use_cap)
			{
				bool *tmpp=new bool[use_cap*2];
				copy(use,use+use_cap,tmpp);
				delete []use;
				use = tmpp;
				use_cap*=2;
			}
			if(use[mult_exp]==1)
			{
				Polynomial tmp;
				tmp.NewTerm(termArray[i].coef * poly.termArray[j].coef,mult_exp);
				cout<<termArray[i].coef * poly.termArray[j].coef<<" "<<mult_exp<<endl;
				res.Add(tmp);
			}
			else
			{
				res.NewTerm(termArray[i].coef * poly.termArray[j].coef,mult_exp);
				use[mult_exp]=true;
			}
		}
	}
	return res;
}

ostream& operator<<(ostream& os, const Polynomial& p){
    for(int i = 0;i<p.terms;i++){
        if(p.termArray[i].exp == 0){
            os<<p.termArray[i].coef;
            continue;
        }
        if(i == p.terms-1){
            os<<p.termArray[i].coef<<"X^"<<p.termArray[i].exp;
        }
        else{
            os<<p.termArray[i].coef<<"X^"<<p.termArray[i].exp<<"+";
        }
    }
    
    return os;
}

int main(){
    Polynomial poly1;
    Polynomial poly2;
  
    poly1.NewTerm(2,2);
	poly1.NewTerm(2,1);
	poly1.NewTerm(2,0);
	
	poly2.NewTerm(2,2);
	poly2.NewTerm(2,1);
	poly2.NewTerm(2,0);
    
    cout<<"("<<poly1<<") + ("<<poly2<<") = ";
    cout<<poly1.Add(poly2)<<endl;
    cout<<"("<<poly1<<") X ("<<poly2<<") = ";
    cout<<poly1.Mult(poly2)<<endl;
    return 0;
}
