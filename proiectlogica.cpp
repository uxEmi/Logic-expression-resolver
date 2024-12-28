#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

bool evalueaza(char* expresie, vector<bool> valori) {
  bool p = false;

  // negatie
  if (expresie[0] == '!') {
    expresie++;
    return !evalueaza(expresie, valori);
  }
  // variabila
  else if (expresie[0] >= 'a' && expresie[0] <= 'z') {
    p = valori[expresie[0]];
    expresie++;
  }
  // paranteza
  else if (*expresie == '(') {
    expresie++;
    p = evalueaza(expresie,  valori);
    expresie++;
  }

  // al doilea operator
  if (*expresie == '|') {
    expresie++;

    bool q = evalueaza(expresie, valori);
    return p || q;
  }
  else if (expresie[0] == '&') {
    expresie++;

    bool q = evalueaza(expresie, valori);
    return p && q;
  }
  else if (expresie[0] == '-') {
    // -> implicatia
    expresie += 2;

    bool q = evalueaza(expresie, valori);
    return !p || q;
  }
  else if (expresie[0] == '<') {
    // <->
    expresie += 3;

    bool q = evalueaza(expresie, valori);
    return (!p || q) && (!q || p);
  }

  return p;
}

int executaPropozitia(string expresie, vector<char> variabile, vector<bool> valori) {
  // afiseaza valorile
  for (int i = 0; i < variabile.size(); i++) {
    int variabila = variabile[i];
    int valoare = valori[variabila];

    cout << valoare << " ";
  }

  // evaluam expresia
  auto rezultat = evalueaza(&expresie[0], valori);

  // afiseaza rezultatul
  cout << "| " << (int)rezultat << endl;

  return rezultat;
}

int main() {
  
  cout << "Introduceti expresia logica: ";

  string expresie;
  getline(cin, expresie);

  // colecteaza toate variabilele din expresie
  vector<char> variabile;

  for (int i = 0; i < expresie.size(); i++) {
    if (expresie[i] >= 'a' && expresie[i] <= 'z') {
      char variabila = expresie[i];
      variabile.push_back(variabila);
    }
  }

  // elimina variabilele duplicate
  sort(variabile.begin(), variabile.end());
  variabile.erase(unique(variabile.begin(), variabile.end()), variabile.end());

  // afisam antetul
  cout << expresie << endl << endl << endl;

  for (int i = 0; i < variabile.size(); i++) {
    cout << variabile[i] << " ";
  }

  cout << "| rezultatul expresiei : \"" << expresie << "\"";

  cout << endl;
  cout << "---------------------------------------------" << endl;

  int propozitiiAdevarate = 0;
  int propozitiiTotale = (int)pow(2, variabile.size());

  vector<bool> valori(256, false);

  // executam fiecare propozitie
  for (int propozitia = 0; propozitia < propozitiiTotale; propozitia++) {
    
    // iteram toti bitii pentru alocarea valorilor
    for (int i = 0; i < variabile.size(); i++) {
      char variabila = variabile[i];

      valori[variabila] = (bool)(propozitia & (1 << i));
    }

    propozitiiAdevarate += executaPropozitia(expresie, variabile, valori);
  }  

  // afisam rezultatul final
  cout << endl;
  cout << "---------------------------------------------" << endl;

  cout << "Expresia logica este : " << endl << endl;

  if (propozitiiAdevarate == 0) {
    cout << "Nesatisfiabila" << endl;
  } 
  if (propozitiiAdevarate != 0) {
    cout << "Satisfiabila" << endl;
  } 
  if (propozitiiAdevarate == propozitiiTotale) {
    cout << "Tautologie" << endl;
  } 
  if (propozitiiAdevarate != 0 && propozitiiAdevarate != propozitiiTotale) {
    cout << "Contingenta" << endl;
  }

  cout << "---------------------------------------------" << endl;

  cout << "Propozitii totale / adevarate: (" << propozitiiTotale << " / " << propozitiiAdevarate << ")";
  cout << endl;

  return 0;
}