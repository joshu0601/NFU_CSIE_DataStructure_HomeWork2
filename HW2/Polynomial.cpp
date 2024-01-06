#include<bits/stdc++.h>
using namespace std; 

class Polynomial;
class Term{
    friend Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    private:
        float coef;//係數
        int term;//次方項
};

class Polynomial{
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    private:
        Term *termArray;
        int capacity;//空間大小  
        int terms;//非零項數量 
    public:
        Polynomial();//建構子  
        float Eval(float f);//求值  
        void NewTerm(const float newCoef,const int newExp);//新增非零項     
};

Polynomial::Polynomial():capacity(2),terms(0){
    termArray = new Term[capacity];
}

void Polynomial::NewTerm(const float newCoef,const int newExp){
    //空間不足時重新配置空間  
    if(terms==capacity)
    {
    	capacity*=2;
    	Term *tmp=new Term[capacity];
		copy(termArray,termArray+terms,tmp);
		delete []termArray;
		termArray= tmp;
	}
	termArray[terms].coef=newCoef;
	termArray[terms++].term=newExp;
    
}

float Polynomial::Eval(float f){
    //多項式求值
    int t=0;
    for(int i=0;i<terms;i++)
    {
    	t+=termArray[i].coef*pow(f,termArray[i].term);
	}
	return t;
}

ostream& operator<<(ostream& os, const Polynomial& p){
    for(int i = 0;i<p.terms;i++){
        if(p.termArray[i].term == 0){
            os<<p.termArray[i].coef;
            continue;
        }
        if(i == p.terms-1){
            os<<p.termArray[i].coef<<"X^"<<p.termArray[i].term;
        }
        else{
            os<<p.termArray[i].coef<<"X^"<<p.termArray[i].term<<"+";
        }
    }
    
    return os;
}

int main(){
    Polynomial poly;
  
    poly.NewTerm(2,8);
    poly.NewTerm(2,7);
    poly.NewTerm(2,6);
    poly.NewTerm(2,5);
    poly.NewTerm(2,4);
    poly.NewTerm(2,3);
    poly.NewTerm(2,2);
    poly.NewTerm(2,1);
    poly.NewTerm(2,0);
    
    cout<<poly<<endl;
    cout<<poly.Eval(2)<<endl;
    return 0;
}
