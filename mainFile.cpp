#include <iostream>
#include <memory>

using namespace std;

class Pracownik {

protected:
    int lata_pracy_{};
    double pensja_;
public:
    //W konstruktorze nie inicjujemy zmiennej pensja_ <- dla kazdego przypadku bedzie liczona osobno
    Pracownik(int l) : lata_pracy_(l){};
    virtual void Pensja() = 0;
    virtual void Wypisz() = 0;

    // Funkcja dodatek ma tylko ulatwic liczenie tego dodatku
    double dodatek(int lata_pracy, double pensja){

        if(lata_pracy <= 3){
            
            cout<<"Temu pracownikowi nie przysługuje dodatek plłacowy"<<endl;
            return 0;
        }

        double dodatek = (((double)lata_pracy_ - 2.0) * pensja) * 0.01;

        cout<<"Dodatek dla tego pracownika wynosi: "<< dodatek << endl; 

        return dodatek;
    }
};

class Dyrektor : public Pracownik{

private:
    double pensja_zasadnicza_;

public:
    Dyrektor(int l, double pz) : Pracownik(l), pensja_zasadnicza_(pz){};

    //Dla dyrektora pensja_ = pensja_zasadnicza_ + dodatek stazowy
    void Pensja() override {                  // Funkcja dodatek ma tylko ulatwic liczenie tego dodatku
        this->pensja_ = pensja_zasadnicza_ + dodatek(this->lata_pracy_, this->pensja_zasadnicza_);
    }

    void Wypisz(){

        cout<<"Lata pracy: " << this->lata_pracy_ << endl;
        cout<<"Pensja: " << this->pensja_ << endl;
        cout<<"Pensja zasadnicza: " << this->pensja_zasadnicza_ << endl;

    }

    void koryguj(double x){

        cout<<"Zmiana dla tego pracownika"<<endl;
        cout<<endl;
        this->pensja_zasadnicza_ = x;
    }

    friend ostream& operator<<(ostream& o, unique_ptr<Dyrektor>& d){

        return  o <<"Lata pracy: " << d->lata_pracy_ << endl
                  <<"Pensja: " << d->pensja_ << endl
                  <<"Pensja zasadnicza: " << d->pensja_zasadnicza_ << endl;

    }
};

class Magazynier : public Pracownik{

private:
    int liczba_dni_;
    double stawka_dzien_;
public:
    Magazynier(int l, int ld, double sd) : Pracownik(l), liczba_dni_(ld), stawka_dzien_(sd){};

    //Dla magazyniera pensja_ = liczba dni * stawka za dzien + dodatek stazowy
    void Pensja() override {
        this->pensja_ = (liczba_dni_ * stawka_dzien_) + dodatek(this->lata_pracy_, liczba_dni_*stawka_dzien_);
    }

    void Wypisz(){

        cout<<"Lata pracy: " << this->lata_pracy_ << endl;
        cout<<"Pensja: " << this->pensja_ << endl;
        cout<<"Liczba dni: " << this->liczba_dni_ << endl;
        cout<<"Stawka za dzien: " << this->stawka_dzien_ << endl;

    }

    void koryguj(int x, double y){

        cout<<"Zmiana dla tego pracownika"<<endl;
        cout<<endl;
        this->liczba_dni_ = x;
        this->stawka_dzien_ = y;

    }

    friend ostream& operator<<(ostream& o, unique_ptr<Magazynier>& d){

        return  o <<"Lata pracy: " << d->lata_pracy_ << endl
                  <<"Pensja: " << d->pensja_ << endl
                  <<"Liczba dni: " << d->liczba_dni_ << endl
                  <<"Stawka za dzien: " << d->stawka_dzien_ << endl;

    }

};

class Ksiegowy : public Pracownik{

private:
    int ilosc_godzin_;
    double stawka_godzina_;
public:

    Ksiegowy(int l, int ig, double sg) : Pracownik(l), ilosc_godzin_(ig), stawka_godzina_(sg){};

    //Dla ksiegowego pensja_ = liczba godzin * stawka za godzine + dodatek stazowy
    void Pensja(){
        this->pensja_ = (ilosc_godzin_ * stawka_godzina_) + dodatek(this->lata_pracy_, ilosc_godzin_*stawka_godzina_);
    }

    void Wypisz(){

        cout<<"Lata pracy: " << this->lata_pracy_ << endl;
        cout<<"Pensja: " << this->pensja_ << endl;
        cout<<"Liczba godzin: " << this->ilosc_godzin_ << endl;
        cout<<"Stawka za godzine: " << this->stawka_godzina_ << endl;

    }

    void koryguj(int x, double y){

        cout<<"Zmiana dla tego pracownika"<<endl;
        cout<<endl;
        this->ilosc_godzin_ = x;
        this->stawka_godzina_ = y;

    }

    friend ostream& operator<<(ostream& o, unique_ptr<Ksiegowy>& d){

        return  o <<"Lata pracy: " << d->lata_pracy_ << endl
                  <<"Pensja: " << d->pensja_ << endl
                  <<"Liczba godzin: " << d->ilosc_godzin_ << endl
                  <<"Stawka za godzine: " << d->stawka_godzina_ << endl;

    }

};

template<typename TO, typename FROM>
unique_ptr<TO> static_unique_pointer_cast (unique_ptr<FROM>&& old){
    return unique_ptr<TO>{static_cast<TO*>(old.release())};
    //konwersja: unique_ptr<FROM>->FROM*->TO*->unique_ptr<TO>
}

int main(){

    //-------------- Sprawdzenie i casting: Pracownik -> Dyrektor ------------------------

    cout<<endl;

    unique_ptr<Pracownik> p1 = make_unique<Dyrektor>(5, 3000);

    p1->Pensja();
    p1->Wypisz();

    cout<<endl;

    if(unique_ptr<Dyrektor> d = static_unique_pointer_cast<Dyrektor> (std::move(p1))){

        d->koryguj(4000);
        d->Pensja();
        cout << d << endl;

    }
    
    cout<<endl;

    //-------------- Sprawdzenie i casting: Pracownik -> Magazynier ----------------------

    cout<<"Pracownik -> Magazynier"<<endl;
    cout<<endl;

    unique_ptr<Pracownik> p2 = make_unique<Magazynier>(5, 25, 15.5);

    p2->Pensja();
    p2->Wypisz();

    cout<<endl;

    if(unique_ptr<Magazynier> m = static_unique_pointer_cast<Magazynier> (std::move(p2))){

        m->koryguj(500, 40.);
        m->Pensja();
        cout << m << endl;

    }

    cout<<endl;

    //-------------- Sprawdzenie i casting: Pracownik -> Ksiegowy ----------------------

    cout<<"Pracownik -> Ksiegowy"<<endl;
    cout<<endl;

    unique_ptr<Pracownik> p3 = make_unique<Ksiegowy>(5, 25, 15.5);

    p3->Pensja();
    p3->Wypisz();

    cout<<endl;

    if(unique_ptr<Ksiegowy> k = static_unique_pointer_cast<Ksiegowy> (std::move(p3))){

        k->koryguj(230, 55.);
        k->Pensja();
        cout << k << endl;

    }

    cout<<endl;

    return 0;
}
