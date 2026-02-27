#include <iostream>
#include <stack>
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;
struct nod{
    string info;
    nod* stg = nullptr;
    nod* drp = nullptr;
    float x = 0;
    float y = 0;
};

bool esteCaracter(char c){
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <='9')){
        return true;
    }
    return false;
}

bool esteOperator(char c){
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^'){
        return true;
    }
    return false;
}

bool esteFunctie(string s){
    if(s== "sqrt" || s == "sin" || s == "cos" || s == "ln" || s == "tg" || s == "ctg" || s == "arcsin" || s == "arccos" || s == "arctg" || s== "arcctg"){
        return true;
    }
    return false;
}

bool esteZero(nod* n){
    if(n!=nullptr && n->info == "0"){
        return true;
    }
    return false;
}

bool esteUnu(nod* n){
    if(n!=nullptr && n->info == "1"){
        return true;
    }
    return false;
}

bool esteNumar(nod* n){
    if(n == nullptr){
        return false;
    }
    if(n->info.empty()){
        return false;
    }
    if(isdigit(n->info[0])){
        return true;
    }
    if(n->info[0] == '-' && n->info.size() > 1 && isdigit(n->info[1])){
        return true;
    }
    return false;
}

int prioritate(string s){
    if(esteFunctie(s)){
        return 4;
    }
    if(s == "^"){
        return 3;
    }
    if(s == "*" || s == "/"){
        return 2;
    }
    if(s == "+" || s == "-"){
        return 1;
    }
return 0;
}

string formatNr(double nr){
    string s = to_string(nr);
    if(s.find('.')!=string::npos){ //noposition
        s = s.substr(0, s.find_last_not_of('0') + 1); 
        if(s.back() == '.'){
            s.pop_back();
        }
    }
    return s;
}


string validareaExpresiei(string s){
    if(s.empty()){
        return "Expresie vida.";
    }
    int paranteze = 0;
    bool asteaptaOperand = true;

    for(int i = 0; i < s.length(); i++){
        char c = s[i];
        if(c == ' '){
            continue;
        }

        if(!isalnum(c) && string("+-*/^().").find(c) == string::npos){
            string mesaj = "Caracter invalid gasit: '";
            mesaj+=c;
            mesaj+="'";
            return mesaj;
        }
        if(c == '('){
            paranteze++;
            asteaptaOperand = true;
        } else if(c == ')'){
            paranteze--;
            if(paranteze < 0){
                return "Prea multe paranteze inchise ')'";
            }
            if(asteaptaOperand){
                return "Paranteza goala '()'";
            }
            asteaptaOperand = false;
        } else if(esteOperator(c)){
            if(c != '-' && asteaptaOperand){ //operator pus gresit
                string mesaj = "Operatorul '";
                mesaj+=c;
                mesaj+="' este pus gresit";
                return mesaj;
            }
            if(c == '-' && asteaptaOperand){ //-5
                asteaptaOperand = true;
            } else {
                asteaptaOperand = true;
            }
            if(i == s.length() - 1){
                return "Expresia se termina cu un operator"; 
            }
        } else if(isalpha(c)){ 
            string cuv = ""; //compunere cuvant
            while(i < s.length() && isalpha(s[i])){
                cuv += s[i];
                i++;
            }
            i--; 

            if(cuv != "x" && !esteFunctie(cuv)){
                return "Functie/variabila necunoscuta: " + cuv;
            }
            asteaptaOperand = false;
        } else if(isdigit(c) || c == '.'){
            while(i < s.length() && (isdigit(s[i]) || s[i] == '.')){
                i++;
            }
            i--;
            asteaptaOperand = false;
        }
    }
    if(paranteze!=0){
        return "Lipseste o paranteza inchisa.";
    }
    if(asteaptaOperand){
        return "Expresia este incompleta";
    }
    return "";
}

string expresiaPostfixata(string s){
    stack<string> stiva;
    string rezultat = "";

    for(int i = 0; i < s.length();i++){
        char c = s[i];
        if(c == ' '){
            continue;
        }
        if(isalnum(c) || c=='.'){
            string operand = "";
            while(i < s.length() && (isalnum(s[i]) || s[i] == '.')){
                operand += s[i];
                i++;    
            }
            i--;

            if(esteFunctie(operand)){
                stiva.push(operand); //functia apare dupa arg ei
            } else {
                rezultat += operand + " ";
            }
            
        } else if(c == '('){
            stiva.push("(");
        } else if(c == ')'){
            while(!stiva.empty() && stiva.top() != "("){
                rezultat += stiva.top();
                rezultat += " ";
                stiva.pop();
            }
            if(!stiva.empty()){
                stiva.pop();
            }
            if(!stiva.empty() && esteFunctie(stiva.top())){
                rezultat += stiva.top() + " ";
                stiva.pop();
            }
        } else {
            string op(1, c);
            while(!stiva.empty() && prioritate(stiva.top()) >= prioritate(op) && stiva.top() != "("){
                rezultat += stiva.top();
                rezultat += " ";
                stiva.pop();
            }
            stiva.push(op);
        }
    }
    while(!stiva.empty()){
        rezultat += stiva.top();
        rezultat += " ";
        stiva.pop();
    }
    return rezultat;
}


nod* creareArbore(string postfix){
    stack<nod*> stivaNoduri;

    for(int i = 0; i < postfix.size(); i++){
        char c = postfix[i];
        if(c == ' ') continue;

        string element = "";
        if(isalnum(c) || c == '.'){
            while(i < postfix.size() && postfix[i] != ' '){
                element += postfix[i];
                i++;
            }
            i--;
        } else { //daca nu e operand sau f
            element = string(1, c);
        }

        if(esteFunctie(element)){ //functii
            nod* nou = new nod;
            nou->info = element;
            if(!stivaNoduri.empty()){
                nou->stg = stivaNoduri.top();
                stivaNoduri.pop();
            }
            stivaNoduri.push(nou); // se pune nodul functiei cu tot cu copil inapoi
        }
        else if(isdigit(element[0]) || isalpha(element[0])){  //operanzi
            nod* nou = new nod;
            nou->info = element;
            stivaNoduri.push(nou); //se pune nodul operandului in stiva
        }
        else { //op
            nod* nou = new nod;
            nou->info = element;
            if(!stivaNoduri.empty()){
                nou->drp = stivaNoduri.top(); // primul scos e dreapta
                stivaNoduri.pop();
            }
            if(!stivaNoduri.empty()){
                nou->stg = stivaNoduri.top();
                stivaNoduri.pop();
            }
            stivaNoduri.push(nou);
        }
    }
    if(stivaNoduri.empty()) return nullptr;
    return stivaNoduri.top();
}

nod* copieArbore(nod* radacina){
    if(radacina == nullptr){
        return nullptr;
    }

    nod* nou = new nod;
    nou->info = radacina->info;
    nou->stg = copieArbore(radacina->stg);
    nou->drp = copieArbore(radacina->drp);

    return nou;
    
}

void stergereArbore(nod* radacina){
    if(radacina == nullptr){
        return;
    }
    stergereArbore(radacina->stg);
    stergereArbore(radacina->drp);
    delete radacina;

}

bool arboriIdentici(nod* a, nod* b){
    if(a == nullptr && b == nullptr){
        return true;
    }
    if(a == nullptr || b == nullptr){
        return false;
    }
    if(a->info != b->info){
        return false;
    }
    return arboriIdentici(a->stg,b->stg) && arboriIdentici(a->drp,b->drp);
}


nod* derivare(nod* radacina){
    if(radacina == nullptr){
        return nullptr;
    }

    if(isdigit(radacina->info[0]) || (radacina->info.size() > 1 && isdigit(radacina->info[1]))){
        nod* nou = new nod;
        nou->info = "0";
        return nou;
    }

    if(radacina->info == "x"){
        nod* nou = new nod;
        nou->info = "1";
        return nou;
    }

    if(radacina->info == "+" || radacina->info == "-"){
        nod* nou = new nod;
        nou->info = radacina->info;
        nou->stg = derivare(radacina->stg);
        nou->drp = derivare(radacina->drp);
        return nou;
    }

    if(radacina->info == "*"){
        nod* plus = new nod;
        plus->info = "+";

        nod* Stanga = new nod;
        Stanga->info = "*";
        nod* Dreapta = new nod;
        Dreapta->info = "*";

        Stanga->stg = derivare(radacina->stg);
        Stanga->drp = copieArbore(radacina->drp);

        Dreapta->stg = copieArbore(radacina->stg);
        Dreapta->drp = derivare(radacina->drp);

        plus->stg = Stanga;
        plus->drp = Dreapta;

        return plus;

    }

    if(radacina->info == "/"){
        nod* impartire = new nod;
        impartire->info = "/";
        nod* scadere = new nod;
        scadere->info = "-";

        nod* ori1 = new nod;
        ori1->info = "*";
        nod* ori2 = new nod;
        ori2->info = "*";
        nod* ori3 = new nod;
        ori3->info = "*";

        ori1->stg = derivare(radacina->stg);
        ori1->drp = copieArbore(radacina->drp);

        ori2->stg = copieArbore(radacina->stg);
        ori2->drp = derivare(radacina->drp);

        scadere->stg = ori1;
        scadere->drp = ori2;

        impartire->stg = scadere; //1

        ori3->stg = copieArbore(radacina->drp);
        ori3->drp = copieArbore(radacina->drp);

        impartire->drp = ori3;  //2


        return impartire;

    }

    if(radacina->info == "ln"){
        nod* div = new nod;
        div->info = "/";
        div->stg = derivare(radacina->stg);
        div->drp = copieArbore(radacina->stg);

        return div;
    }


    if(radacina->info == "sqrt"){
        nod* div = new nod;
        div->info = "/";

        nod* ori = new nod;
        ori->info = "*";

        nod* doi = new nod;
        doi->info = "2";
        nod* radical = copieArbore(radacina);

        ori->stg = doi;
        ori->drp = radical;

        div->stg = derivare(radacina->stg);
        div->drp = ori;

        return div;

    }

    if(radacina->info == "^"){ // v * u^v-1 * u' + u^v * ln(u) * v'
        nod* plus = new nod;
        plus->info = "+";

        nod* stanga = new nod;
        stanga->info = "*";

        nod* v_ori_u_minus_1 = new nod;
        v_ori_u_minus_1->info = "*";
        v_ori_u_minus_1->stg = copieArbore(radacina->drp); //v

        nod* puterea = new nod;
        puterea->info = "^";
        puterea->stg = copieArbore(radacina->stg); //u

        nod* scadere = new nod;
        scadere->info = "-";
        scadere->stg = copieArbore(radacina->drp);
        nod* unu = new nod;
        unu->info = "1";
        scadere->drp = unu;

        puterea->drp = scadere;

        v_ori_u_minus_1->drp = puterea;

        nod* derivataU = derivare(radacina->stg);

        stanga->stg = v_ori_u_minus_1;
        stanga->drp = derivataU;


        nod* dreapta = new nod;
        dreapta->info = "*";

        nod* u_la_v_ori_ln = new nod;
        u_la_v_ori_ln->info = "*";

        u_la_v_ori_ln->stg = copieArbore(radacina);

        nod* logaritm = new nod;
        logaritm->info = "ln";
        logaritm->stg = copieArbore(radacina->stg);

        u_la_v_ori_ln->drp = logaritm;

        nod* derivataV = derivare(radacina->drp);

        dreapta->stg = u_la_v_ori_ln;
        dreapta->drp = derivataV;

        plus->stg = stanga;
        plus->drp = dreapta;

        return plus;
    }

    if(radacina->info == "sin"){ // cosu * u'
        nod* ori = new nod;
        ori->info = "*";

        nod* cosinus = new nod;
        cosinus->info = "cos";
        cosinus->stg = copieArbore(radacina->stg);

        nod* derivataU = derivare(radacina->stg);

        ori->stg = cosinus;
        ori->drp = derivataU;

        return ori;
    }
    if(radacina->info == "cos"){ // -sinu * u'
        nod* ori = new nod;
        ori->info = "*";

        nod* stanga = new nod;
        stanga->info = "*";

        nod* minusUnu = new nod;
        minusUnu->info = "-1";
        stanga->stg = minusUnu;

        nod* sinus = new nod;
        sinus->info = "sin";
        sinus->stg = copieArbore(radacina->stg);
        stanga->drp = sinus;

        nod* dreapta = derivare(radacina->stg);

        ori->stg = stanga;
        ori->drp = dreapta;

        return ori;
    }

    if(radacina->info == "tg"){ // u' / cos^2(u)
        nod* div = new nod;
        div->info = "/";

        nod* stanga = derivare(radacina->stg);
        div->stg = stanga;

        nod* ori = new nod;
        ori->info = "*";

        nod* cosinus1 = new nod;
        cosinus1->info = "cos";
        cosinus1->stg = copieArbore(radacina->stg);
        nod* cosinus2 = new nod;
        cosinus2->info = "cos";
        cosinus2->stg = copieArbore(radacina->stg);

        ori->stg = cosinus1;
        ori->drp = cosinus2;

        div->drp = ori;
        return div;
    }

    if(radacina->info == "ctg"){ // -u' / cos^2(u)
        nod* div = new nod;
        div->info = "/";

        nod* ori1 = new nod; ori1->info = "*";
        nod* ori2 = new nod; ori2->info = "*";

        nod* minusUnu = new nod;
        minusUnu->info = "-1";

        nod* dreapta = derivare(radacina->stg);
        ori1->stg = minusUnu;
        ori1->drp = dreapta;

        nod* sinus1 = new nod;
        sinus1->info = "sin";
        sinus1->stg = copieArbore(radacina->stg);
        nod* sinus2 = new nod;
        sinus2->info = "sin";
        sinus2->stg = copieArbore(radacina->stg);

        ori2->stg = sinus1;
        ori2->drp = sinus2;

        div->stg = ori1;
        div->drp = ori2;
        return div;
    }

    if(radacina->info == "arcsin"){ // u' / rad(1-u^2)
        nod* div = new nod;
        div->info = "/";
        nod* stanga = derivare(radacina->stg);
        div->stg = stanga;

        nod* radicalu = new nod;
        radicalu->info = "sqrt";

        nod* minus = new nod;
        minus->info = "-";

        nod* unu = new nod;
        unu->info = "1";
        
        nod* putere = new nod;
        putere->info = "^";
        putere->stg = copieArbore(radacina->stg);

        nod* doi = new nod;
        doi->info = "2";
        putere->drp = doi;

        minus->stg = unu;
        minus->drp = putere;
        radicalu->stg = minus;
        div->drp = radicalu;

        return div;
    }

    if(radacina->info == "arccos"){ // -u' / rad(1 - u^2)
        nod* div = new nod; 
        div->info = "/";

        nod* sus = new nod;
        sus->info = "*";

        nod* minusUnu = new nod;
        minusUnu->info = "-1";
        sus->stg = minusUnu;
        sus->drp = derivare(radacina->stg);
        div->stg = sus;

        nod* radicalu = new nod;
        radicalu->info = "sqrt";

        nod* minus = new nod;
        minus->info = "-";

        nod* unu = new nod;
        unu->info = "1";
        
        nod* putere = new nod;
        putere->info = "^";
        putere->stg = copieArbore(radacina->stg);

        nod* doi = new nod;
        doi->info = "2";
        putere->drp = doi;

        minus->stg = unu;
        minus->drp = putere;
        radicalu->stg = minus;
        div->drp = radicalu;

        return div;
    }

    if(radacina->info == "arctg"){ // u' / 1 + u^2
        nod* div = new nod;
        div->info = "/";
        div->stg = derivare(radacina->stg);

        nod* plus = new nod;
        plus->info = "+";

        nod* unu = new nod; unu->info = "1";
        
        nod* putere = new nod;
        putere->info = "^";
        putere->stg = copieArbore(radacina->stg);

        nod* doi = new nod; doi->info = "2";
        putere->drp = doi;

        plus->stg = unu;
        plus->drp = putere;
        div->drp = plus;
        return div;
    }
    if(radacina->info == "arcctg"){ // -u' / 1 + u^2
        nod* div = new nod; div->info = "/";

        nod* sus = new nod; sus->info = "*";
        
        nod* minusUnu = new nod; minusUnu->info = "-1";
        sus->stg = minusUnu;
        sus->drp = derivare(radacina->stg);
        div->stg = sus;

        nod* plus = new nod;
        plus->info = "+";

        nod* unu = new nod; unu->info = "1";
        
        nod* putere = new nod;
        putere->info = "^";
        putere->stg = copieArbore(radacina->stg);

        nod* doi = new nod; doi->info = "2";
        putere->drp = doi;

        plus->stg = unu;
        plus->drp = putere;
        div->drp = plus;
        return div;

    }
    return nullptr;
}

nod* simplificare(nod* radacina){
    if(radacina == nullptr){
        return nullptr;
    }
    if(radacina->stg == nullptr && radacina->drp == nullptr){ //frunze
        return radacina;
    }

    radacina->stg = simplificare(radacina->stg);
    radacina->drp = simplificare(radacina->drp);

    if(radacina->info == "+"){
        if(esteZero(radacina->stg)){ //0+x
            nod* temp = radacina->drp;
            delete radacina->stg;
            delete radacina;
            return temp;
        }
        if(esteZero(radacina->drp)){ // x+0
            nod* temp = radacina->stg;
            delete radacina->drp;
            delete radacina;
            return temp;
        }
        if(esteNumar(radacina->stg) && esteNumar(radacina->drp)){ //c+c
            double suma = stod(radacina->stg->info) + stod(radacina->drp->info);
            radacina->info = formatNr(suma);

            delete radacina->stg;
            delete radacina->drp;

            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }

        if(arboriIdentici(radacina->stg, radacina->drp)){ //x+x
            stergereArbore(radacina->drp);
            radacina->info = "*";
            nod* doi = new nod;
            doi->info = "2";

            radacina->drp = radacina->stg;
            radacina->stg = doi;
            return radacina;
        }

        if(radacina->stg && radacina->drp && radacina->stg->info == "*" && radacina->drp->info == "*"){ //2*x + 3*x
            if(arboriIdentici(radacina->stg->drp, radacina->drp->drp)){
                if(esteNumar(radacina->stg->stg) && esteNumar(radacina->drp->stg)){
                    double val1 = stod(radacina->stg->stg->info);
                    double val2 = stod(radacina->drp->stg->info);
                    double suma = val1 + val2;

                    nod* varCom = copieArbore(radacina->stg->drp);
                    stergereArbore(radacina->stg);
                    stergereArbore(radacina->drp);

                    radacina->info = "*";
                    radacina->stg = new nod;
                    radacina->stg->info = formatNr(suma);

                    radacina->drp = varCom;
                    return radacina;

                }
            }
        }
        if(esteNumar(radacina->stg) && radacina->drp && radacina->drp->info == "+"){ //asociativitate
            if(esteNumar(radacina->drp->stg)){
                double v1 = stod(radacina->stg->info);
                double v2 = stod(radacina->drp->stg->info);
                double suma = v1 + v2;

                nod* restul = radacina->drp->drp;
                radacina->drp->drp = nullptr; 

                stergereArbore(radacina->drp); 
                delete radacina->stg;

                radacina->stg = new nod;
                radacina->stg->info = formatNr(suma);
                radacina->drp = restul;
                
                return radacina;
            }
        }
        if(esteNumar(radacina->stg) && radacina->drp && radacina->drp->info == "+"){ //asoc pt 2
            if(esteNumar(radacina->drp->drp)){
                double v1 = stod(radacina->stg->info);
                double v2 = stod(radacina->drp->drp->info);
                double suma = v1+v2;

                nod* restul = radacina->drp->stg;
                radacina->drp->stg = nullptr;

                stergereArbore(radacina->drp);
                delete radacina->stg;

                radacina->stg = new nod;
                radacina->stg->info = formatNr(suma);
                radacina->drp = restul;

                return radacina;
            }
        }
        if(esteNumar(radacina->drp) && radacina->stg && radacina->stg->info == "+"){
            if(esteNumar(radacina->stg->stg)){
                double v1 = stod(radacina->drp->info);
                double v2 = stod(radacina->stg->stg->info);
                double suma = v1 + v2;

                nod* restul = radacina->stg->drp;
                radacina->stg->drp = nullptr;

                stergereArbore(radacina->stg);
                delete radacina->drp;

                radacina->drp = new nod;
                radacina->drp->info = formatNr(suma);
                radacina->stg = restul;

                return radacina;
            }
        }
        if(esteNumar(radacina->drp) && radacina->stg && radacina->stg->info == "+"){
            if(esteNumar(radacina->stg->drp)){
                double v1 = stod(radacina->stg->drp->info);
                double v2 = stod(radacina->drp->info);
                double suma = v1 + v2;

                nod* restul = radacina->stg->stg;
                radacina->stg->stg = nullptr;

                stergereArbore(radacina->stg);
                delete radacina->drp;

                radacina->drp = new nod;
                radacina->drp->info = formatNr(suma);
                radacina->stg = restul;

                return  radacina;
            }
        }
    }

    if(radacina->info == "-"){
        if(esteZero(radacina->drp)){  //x-0
            nod* temp = radacina->stg;
            delete radacina->drp;
            delete radacina;
            return temp;
        }
        if(esteNumar(radacina->stg) && esteNumar(radacina->drp)){  //c-c
            double dif = stod(radacina->stg->info) - stod(radacina->drp->info);
            radacina->info = formatNr(dif);
            delete radacina->stg;
            delete radacina->drp;

            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(arboriIdentici(radacina->stg, radacina->drp)){ //x-x
            stergereArbore(radacina->stg);
            stergereArbore(radacina->drp);
            radacina->info = "0";
            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(radacina->stg && radacina->drp && radacina->stg->info == "*" && radacina->drp->info == "*"){ //2*x - 3*x
            if(arboriIdentici(radacina->stg->drp, radacina->drp->drp)){
                if(esteNumar(radacina->stg->stg) && esteNumar(radacina->drp->stg)){
                    double val1 = stod(radacina->stg->stg->info);
                    double val2 = stod(radacina->drp->stg->info);
                    double suma = val1 - val2;

                    nod* varCom = copieArbore(radacina->stg->drp);
                    stergereArbore(radacina->stg);
                    stergereArbore(radacina->drp);

                    radacina->info = "*";
                    radacina->stg = new nod;
                    radacina->stg->info = formatNr(suma);

                    radacina->drp = varCom;
                    return radacina;

                }
            }
        }
    }


    if(radacina->info == "*"){  
        //comutativitate
        if(esteNumar(radacina->drp) && !esteNumar(radacina->stg)){
            nod* temp = radacina->stg;
            radacina->stg = radacina->drp;
            radacina->drp = temp;
        }
        if(esteZero(radacina->stg) || esteZero(radacina->drp)){// 0*x sau x*0
            radacina->info = "0";
            stergereArbore(radacina->stg);
            stergereArbore(radacina->drp);
            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(esteUnu(radacina->stg)){ //1*x
            nod* temp = radacina->drp;
            delete radacina->stg;
            delete radacina;
            return temp;
        }
        if(esteUnu(radacina->drp)){ //x*1
            nod* temp = radacina->stg;
            delete radacina->drp;
            delete radacina;
            return temp;
        }
        if(esteNumar(radacina->stg) && esteNumar(radacina->drp)){  //c*c
            double prod = stod(radacina->stg->info) * stod(radacina->drp->info);
            radacina->info = formatNr(prod);

            delete radacina->stg;
            delete radacina->drp;

            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }

        if(radacina->drp && radacina->drp->info == "/"){ // x*1/x
            if(esteUnu(radacina->drp->stg)){
                if(arboriIdentici(radacina->stg, radacina->drp->drp)){
                    stergereArbore(radacina->stg);
                    stergereArbore(radacina->drp);
                    radacina->info = "1";
                    radacina->stg = nullptr;
                    radacina->drp = nullptr;
                    return radacina;
                }
            }   
        }
        if(radacina->stg && radacina->stg->info == "/"){ // 1*x/x
            if(esteUnu(radacina->stg->stg)){
                if(arboriIdentici(radacina->stg->drp, radacina->drp)){
                    stergereArbore(radacina->stg);
                    stergereArbore(radacina->drp);
                    radacina->info = "1";
                    radacina->stg = nullptr;
                    radacina->drp = nullptr;
                    return radacina;
                }
            }
        }
        if(arboriIdentici(radacina->stg, radacina->drp)){ //x*x
            stergereArbore(radacina->drp);
            radacina->info = "^";
            nod* doi = new nod;
            doi->info = "2";

            radacina->drp = doi;
            return radacina;
        }
        if(esteNumar(radacina->stg) && radacina->drp && radacina->drp->info == "*"){  //asociativitate
            if(esteNumar(radacina->drp->stg)){
                double v1 = stod(radacina->stg->info);
                double v2 = stod(radacina->drp->stg->info);
                double prod = v1*v2;

                nod* x = radacina->drp->drp;
                radacina->drp->drp = nullptr;
                stergereArbore(radacina->drp);
                delete radacina->stg;

                radacina->stg = new nod;
                radacina->stg->info = formatNr(prod);
                radacina->drp = x;
                return radacina;
            }
        }
    }

    if(radacina->info == "/"){
        if(esteZero(radacina->stg)){ // 0/x
            radacina->info = "0";
            stergereArbore(radacina->drp);
            delete radacina->stg;

            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(esteUnu(radacina->drp)){  // x/1
            nod* temp = radacina->stg;
            delete radacina->drp;
            delete radacina;
            return temp;
        }
        if(arboriIdentici(radacina->stg,radacina->drp)){  // x/x
            stergereArbore(radacina->stg);
            stergereArbore(radacina->drp);

            radacina->info = "1";
            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(esteNumar(radacina->stg) && esteNumar(radacina->drp)){ //c/c
            double div = stod(radacina->stg->info) / stod(radacina->drp->info);
            radacina->info = formatNr(div);

            delete radacina->stg;
            delete radacina->drp;

            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(arboriIdentici(radacina->stg->stg, radacina->drp)){ //(x*a) / x
            nod* rezultat = radacina->stg->drp;
            radacina->stg->drp = nullptr;

            stergereArbore(radacina->stg);
            delete radacina->drp;

            radacina->info = rezultat->info;
            radacina->stg = rezultat->stg;
            radacina->drp = rezultat->drp;
            delete rezultat;               
            return radacina;
        }
        if(radacina->stg && radacina->stg->info == "*" && radacina->drp && radacina->drp->info == "^"){ // a*x / x^n
            if(arboriIdentici(radacina->stg->drp, radacina->drp->stg)){
                if(esteNumar(radacina->drp->drp)){
                    double expVechi = stod(radacina->drp->drp->info);
                    double expNou = expVechi - 1.0;

                    nod* a = radacina->stg->stg;
                    nod* x = radacina->drp->stg;

                    radacina->stg->stg = nullptr;
                    radacina->drp->stg = nullptr;

                    stergereArbore(radacina->stg);
                    delete radacina->drp->drp;
                    delete radacina->drp;
                    if(abs(expNou - 1.0) < 0.0001){ // daca exp e 1. nr cu double au precizie limitata
                        radacina->stg = a;
                        radacina->drp = x;
                    } else {
                        nod* putereNoua = new nod;
                        putereNoua->info = "^";
                        putereNoua->stg = x;
                        putereNoua->drp = new nod;
                        putereNoua->drp->info = formatNr(expNou);

                        radacina->stg = a;
                        radacina->drp = putereNoua;
                    }
                    return radacina;
                }
            }
        }
    }
    if(radacina->info == "ln"){ //ln(1)
        if(esteUnu(radacina->stg)){
            radacina->info = "0";
            delete radacina->stg;
            radacina->stg = nullptr; 
            radacina->drp = nullptr;
            return radacina;
        }
    }
    if(radacina->info == "sqrt"){
        if(esteUnu(radacina->stg)){ //rad1
            delete radacina->stg;
            radacina->info = "1";
            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(esteZero(radacina->stg)){ //rad0
            delete radacina->stg;
            radacina->info = "0";
            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
    }

    if(radacina->info == "^"){
        if(esteZero(radacina->drp)){  //x^0
            stergereArbore(radacina->stg);
            delete radacina->drp;
            radacina->info = "1";
            radacina->stg = nullptr; 
            radacina->drp = nullptr;
            return radacina;
        }
        if(esteUnu(radacina->drp)){   //x^1
            nod* temp = radacina->stg;
            delete radacina->drp;
            delete radacina;
            return temp;
        }
        if(esteZero(radacina->stg)){   //0^x
            stergereArbore(radacina->drp);
            delete radacina->stg;
            radacina->info = "0";
            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(esteUnu(radacina->stg)){ //1^x
            stergereArbore(radacina->drp);
            radacina->info = "1";
            delete radacina->stg;
            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(esteNumar(radacina->stg) && esteNumar(radacina->drp)){ 
            double baza = stod(radacina->stg->info);
            double exp = stod(radacina->drp->info);
            double rez = pow(baza,exp);

            radacina->info = formatNr(rez);
            delete radacina->stg;
            delete radacina->drp;
            radacina->stg = nullptr;
            radacina->drp = nullptr;
            return radacina;
        }
        if(radacina->stg && radacina->stg->info == "^"){ // (x^2)^3
            if(esteNumar(radacina->drp) && esteNumar(radacina->stg->drp)){
                double a = stod(radacina->stg->drp->info);
                double b = stod(radacina->drp->info);
                double nouExp = a * b;

                nod* baza = radacina->stg->stg; 
                radacina->stg->stg = nullptr; 

                stergereArbore(radacina->stg); 
                delete radacina->drp; 

                radacina->stg = baza;
                radacina->drp = new nod;
                radacina->drp->info = formatNr(nouExp);

                return radacina;
            }
        }
    }

    if(radacina->info == "sin"){
        if(esteZero(radacina->stg)){
            radacina->info = "0";
            delete radacina->stg;
            radacina->stg = nullptr;
            return radacina;
        }
    }
    
    if(radacina->info == "cos"){
        if(esteZero(radacina->stg)){
            radacina->info = "1";
            delete radacina->stg;
            radacina->stg = nullptr;
            return radacina;
        }
    }
    if(radacina->info == "tg"){
        if(esteZero(radacina->stg)){
            radacina->info = "0";
            delete radacina->stg;
            radacina->stg = nullptr;
            return radacina;
        }
    }
    if(radacina->info == "arctg"){
        if(esteZero(radacina->stg)){
            radacina->info = "0";
            delete radacina->stg;
            radacina->stg = nullptr;
            return radacina;
        }
    }
    if(radacina->info == "arcsin"){
        if(esteZero(radacina->stg)){
            radacina->info = "0";
            delete radacina->stg;
            radacina->stg = nullptr;
            return radacina;
        }
    }
    if(radacina->info == "arccos"){
        if(esteUnu(radacina->stg)){
            radacina->info = "1";
            delete radacina->stg;
            radacina->stg = nullptr;
            return radacina;
        }
    }


    return radacina;

}

string transformareArboreInString(nod* radacina){
    if(radacina == nullptr){
        return "";
    }
    string rez = "";
    bool puneParanteze = (prioritate(radacina->info) > 0 && prioritate(radacina->info) < 4);
    if(puneParanteze){
        rez += "(";
    }
    if(esteFunctie(radacina->info)){
        rez +=  radacina->info + "(" + transformareArboreInString(radacina->stg) + ")";
    } else {
        rez += transformareArboreInString(radacina->stg);
        rez += radacina->info;
        rez += transformareArboreInString(radacina->drp);
    }

    if(puneParanteze){
        rez += ")";
    }
    return rez;
}


sf::Color backGroundCol = sf::Color(30,30,30);
sf::Color nodeCol = sf::Color(39,44,51);
sf::Color nodeTextCol = sf::Color::White;
sf::Color textCol = sf::Color::White;
sf::Color muchieCol = sf::Color::White;
sf::Color derivataCol = sf::Color::Cyan;
sf::Color statusCol = sf::Color::Yellow;
bool dark = true;



void afisareInordine(nod* radacina) {
    if(radacina){
        bool puneParanteze = (prioritate(radacina->info) > 0 && prioritate(radacina->info) < 4);
        if(puneParanteze){
            cout << "(";
        }
        if(esteFunctie(radacina->info)){
            cout << radacina->info << "(";
            afisareInordine(radacina->stg);
            cout << ")";
        } else {
            afisareInordine(radacina->stg);
            cout << radacina->info;
            afisareInordine(radacina->drp);
        }
        if(puneParanteze){
            cout << ")";
        }
    }
}

void calcularePoz(nod* radacina, int& indexFrunza, int nivel){ //indexFrunza - contor global
    if(radacina == nullptr){
        return;
    }
    radacina->y = nivel*80 + 100;
    if(radacina->stg == nullptr && radacina->drp == nullptr){ //daca e frunza
        radacina->x = indexFrunza*60 + 50; //spatiul intre frunze = 60px
        indexFrunza ++;
        return;
    }
    calcularePoz(radacina->stg, indexFrunza, nivel + 1);
    calcularePoz(radacina->drp, indexFrunza, nivel + 1);

    if(radacina->stg && radacina->drp){ //daca e op
        radacina->x = (radacina->stg->x + radacina->drp->x)/2.0f;
    } else if(radacina->stg){ //daca e functie
        radacina->x = radacina->stg->x;
    }
}

void deseneazaNod(sf::RenderWindow &window, float x, float y, string text, sf::Font &font){
    sf::CircleShape cerc(25);
    cerc.setPosition(x-25,y-25);
    cerc.setFillColor(nodeCol);
    cerc.setOutlineThickness(2);
    cerc.setOutlineColor(sf::Color(97,175,239));

    sf::Text label;
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(16);
    label.setFillColor(nodeTextCol);

    sf::FloatRect textRect = label.getLocalBounds();
    label.setOrigin(textRect.left+textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    label.setPosition(x,y);

    window.draw(cerc);
    window.draw(label);
}

void deseneazaArbore(sf::RenderWindow& window, nod* radacina, sf::Font& font){
    if(radacina == nullptr){
        return;
    }
    if(radacina->stg){
        sf::Vertex linie[] = {
            sf::Vertex(sf::Vector2f(radacina->x,radacina->y), muchieCol),
            sf::Vertex(sf::Vector2f(radacina->stg->x, radacina->stg->y), muchieCol)
        };
        window.draw(linie, 2, sf::Lines);
        deseneazaArbore(window, radacina->stg, font);
    }
    if(radacina->drp){
        sf::Vertex linie[] = {
            sf::Vertex(sf::Vector2f(radacina->x, radacina->y), muchieCol),
            sf::Vertex(sf::Vector2f(radacina->drp->x, radacina->drp->y), muchieCol)
        };
        window.draw(linie, 2, sf::Lines);
        deseneazaArbore(window, radacina->drp, font);
    }
    deseneazaNod(window, radacina->x, radacina->y, radacina->info, font);
}


void deplasareArbore(nod* radacina, float xx){
    if(radacina == nullptr){
        return;
    }
    radacina->x += xx;
    deplasareArbore(radacina->stg, xx);
    deplasareArbore(radacina->drp, xx);
}



int main(){
    /*string s;
    cout << "Introdu expresia: ";
    getline(cin, s);

    if(validareaExpresiei(s)){
        string postfix = expresiaPostfixata(s);
        cout << "Postfix: " << postfix << endl;

        nod* radacina = creareArbore(postfix);
        cout << "Expresia initiala: "; 
        afisareInordine(radacina); 
        cout << endl;

        radacina = simplificare(radacina);
        radacina = simplificare(radacina);

        cout << "Expresia simplificata: ";
        afisareInordine(radacina);
        cout << endl;

        nod* derivat = derivare(radacina);
        cout << "Derivata: "; 
        afisareInordine(derivat); 
        cout << endl;

        derivat = simplificare(derivat);
        derivat = simplificare(derivat); 
        
        cout << "Derivata Simplificata: "; 
        afisareInordine(derivat); 
        cout << endl;

        stergereArbore(radacina);
        stergereArbore(derivat);

    } else {
        cout << "Expresie invalida" << endl;
    }
    return 0;*/


    sf::RenderWindow window(sf::VideoMode(1200, 800), "Derivarea Formala");
    window.setFramerateLimit(60);

    sf::Font font;
    if(!font.loadFromFile("arial.ttf")){
        return -1;
    }

    string inputString = "";
    string status = "Scrie functia si apasa Enter.";
    string textDerivata = "";
    nod* arboreCurent = nullptr;
    bool butonAjutor = false;
    bool fullscreen = false;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::TextEntered){
                if(event.text.unicode == 8){
                    if(!inputString.empty()){
                        inputString.pop_back();
                    }
                } else if(event.text.unicode >=32 && event.text.unicode < 128 && event.text.unicode != 'd' && event.text.unicode!= 'D' && event.text.unicode != 'p' && event.text.unicode != 'P' && event.text.unicode != 'h' && event.text.unicode != 'H'){
                    inputString += static_cast<char>(event.text.unicode); 
                }
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Enter){
                    if(arboreCurent!=nullptr){
                        stergereArbore(arboreCurent);
                        arboreCurent = nullptr;
                    }
                    string eroare = validareaExpresiei(inputString);
                    if(eroare == ""){
                        string postfix = expresiaPostfixata(inputString);
                        arboreCurent = creareArbore(postfix);
                        arboreCurent = simplificare(arboreCurent);
                        status = "Arbore creat - Apasa Space pentru derivare.";
                        textDerivata = "";
                    } else {
                        status = "Eroare: " + eroare;
                    }
                }
                if(event.key.code == sf::Keyboard::Space && arboreCurent!=nullptr){
                    nod* derivat = derivare(arboreCurent);
                    derivat = simplificare(derivat);
                    derivat = simplificare(derivat);
                    stergereArbore(arboreCurent);
                    arboreCurent = derivat;
                    status = "Derivata a fost calculata!";
                    textDerivata = "f'(x) = " + transformareArboreInString(arboreCurent);
                }
                if(event.key.code == sf::Keyboard::D && arboreCurent!=nullptr){
                    nod* derivata2 = derivare(arboreCurent);
                    derivata2 = simplificare(derivata2);
                    derivata2 = simplificare(derivata2);
                    stergereArbore(arboreCurent);
                    arboreCurent = derivata2;

                    status = "Derivata de ordinul 2 a fost calculata!";
                    textDerivata = "f''(x) = " + transformareArboreInString(arboreCurent);
                }
                if(event.key.code == sf::Keyboard::P){
                    sf::Texture texture;
                    texture.create(window.getSize().x, window.getSize().y);
                    texture.update(window);
                    if(texture.copyToImage().saveToFile("arbore_expresie.png")){
                        status = "Imaginea a fost salvata: arbore_expresie.png";
                    }
                }
                if(event.key.code == sf::Keyboard::T && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
                    dark = !dark;
                    if(dark){
                        backGroundCol = sf::Color(30,30,30);
                        nodeCol = sf::Color(39,44,51);
                        nodeTextCol = sf::Color::White;
                        textCol = sf::Color::White;
                        muchieCol = sf::Color::White;
                        derivataCol = sf::Color::Cyan;
                        statusCol = sf::Color::Yellow;
                        status = "Tema schimbata - Dark Mode";

                    } else {
                        backGroundCol = sf::Color(240,240,240);
                        nodeCol = sf::Color(70,130,180);
                        nodeTextCol = sf::Color::White;
                        textCol = sf::Color::Black;
                        muchieCol = sf::Color::Black;
                        derivataCol = sf::Color(0, 0, 139);
                        statusCol = sf::Color(200,50,0);
                        status = "Tema schimbata - Light Mode";

                    }
                }
                if(event.key.code == sf::Keyboard::H){
                    butonAjutor = !butonAjutor;
                }
                if(event.key.code == sf::Keyboard::R && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
                    if(arboreCurent!=nullptr){
                        stergereArbore(arboreCurent);
                        arboreCurent = nullptr;
                    }
                    inputString = "";
                    textDerivata = "";
                    status = "Aplicatia a fost resetata. Introdu noua expresie";
                    butonAjutor = false;
                }
                if(event.key.code == sf::Keyboard::X && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
                    window.close();
                }
            }
        }

        window.clear(backGroundCol);
        sf::Text inputText;
        inputText.setFont(font);
        inputText.setString("f(x) = " + inputString);
        inputText.setPosition(20,720);
        inputText.setFillColor(textCol);
        window.draw(inputText);

        sf::Text status2;
        status2.setFont(font);
        status2.setString(status);
        status2.setPosition(20,760);
        status2.setFillColor(statusCol);
        window.draw(status2);

        if(!textDerivata.empty()){
            sf::View viewFix(sf::FloatRect(0.f, 0.f, (float)window.getSize().x, (float)window.getSize().y));
            window.setView(viewFix);

            sf::Text printDeriv;
            printDeriv.setFont(font);
            printDeriv.setString(textDerivata);
            printDeriv.setCharacterSize(24);
            printDeriv.setPosition(20, 20); 
            printDeriv.setFillColor(derivataCol);
            printDeriv.setScale(1.0f, 1.0f);

            float latimeText = printDeriv.getGlobalBounds().width;
            float spatiuMaxim = (float)window.getSize().x - 40.0f;

            if(latimeText > spatiuMaxim){
                float factor = spatiuMaxim / latimeText;
                printDeriv.setScale(factor, factor);
            }

            window.draw(printDeriv);
        }

        if(arboreCurent!=nullptr){
            int nrFrunze = 0;
            calcularePoz(arboreCurent, nrFrunze, 0);
            float latimeArb = nrFrunze * 60;
            float latimeEcran = 1200;
            float deplasare = (latimeEcran - latimeArb)/2.0f - 50;
            deplasareArbore(arboreCurent, deplasare);

            sf::View camera = window.getDefaultView();
            if(latimeArb > latimeEcran - 100){
                float zoomf = latimeArb / (latimeEcran - 100);
                camera.zoom(zoomf);
            }
            window.setView(camera);
            deseneazaArbore(window, arboreCurent, font);
            window.setView(window.getDefaultView());
        }

        if(!butonAjutor){
            sf::Text hintText;
            hintText.setFont(font);
            hintText.setString("[H] - AJUTOR");
            hintText.setCharacterSize(18);
            hintText.setPosition(1060, 767);
            hintText.setFillColor(dark ? sf::Color::Yellow : sf::Color::Red);
            window.draw(hintText);
        }
        if(butonAjutor){
            sf::RectangleShape overlay(sf::Vector2f(1200, 800));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(overlay);

            sf::RectangleShape chenar(sf::Vector2f(700, 550));
            chenar.setOrigin(350, 275);
            chenar.setPosition(600, 400);
            chenar.setFillColor(sf::Color(40, 40, 40));
            chenar.setOutlineThickness(3);
            chenar.setOutlineColor(sf::Color::White);
            window.draw(chenar);

            sf::Text titlu;
            titlu.setFont(font);
            titlu.setString("GHID DE UTILIZARE");
            titlu.setCharacterSize(30);
            titlu.setFillColor(sf::Color::Cyan);
            sf::FloatRect t = titlu.getLocalBounds();
            titlu.setOrigin(t.width/2, 0);
            titlu.setPosition(600,160);
            window.draw(titlu);

            sf::Text instructiuni;
            instructiuni.setFont(font);
            instructiuni.setCharacterSize(20);
            instructiuni.setFillColor(sf::Color::White);
            instructiuni.setString(
                "TASTATURA SI COMENZI:\n\n"
                "[ENTER] - Construieste arborele functiei\n"
                "[SPACE] - Calculeaza derivata de ordinul 1\n"
                "[D] - Calculeaza derivata de ordinul 2\n"
                "[P] - Salveaza png-ul arborelui\n"
                "[CTRL+T] - Schimba tema\n"
                "[CTRL + X] - Inchide fereastra\n"
                "[CTRL + R] - Reseteaza expresia\n"
                "[H] - Inchide aceasta fereastra\n\n"
                "EXEMPLE VALIDE DE FORMULE:\n"
                "2*x + ln(x) | arctg(x) | sin(x^2) + sqrt(x)"
            );
            instructiuni.setPosition(300,230);
            window.draw(instructiuni);
        }
        window.display();
    }
    return 0;

}