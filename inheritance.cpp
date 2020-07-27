#include<iostream>
#include<memory>
#include<vector>

class calciatore 
{
    protected:
    std::string nome_;
    float altezza_;
    float peso_;

    public:
    calciatore(std::string const& nome, float altezza, float peso) : nome_{nome}, altezza_{altezza}, peso_{peso} {}
    virtual ~calciatore() = 0;
    virtual std::string const& nome() const { return nome_; }
    virtual float altezza () const { return altezza_; }
    virtual float peso () const { return peso_; }
    virtual std::string prestazioni (int parametro1, int parametro2, int parametro3) = 0;

};

inline calciatore::~calciatore () = default;

class portiere : public calciatore 
{
    private:
    int riflessi_;
    int rinvio_;

    public:
    portiere(std::string const& nome, float altezza, float peso, int riflessi, int rinvio) : 
                calciatore(nome, altezza, peso), riflessi_{riflessi}, rinvio_{rinvio} {}

    ~portiere() {
        std::cout << "portiere deleted" << '\n';
    };

    int& riflessi () {
        std::cout << nome_ << " - riflessi:  " << riflessi_ << '\n';
        return riflessi_;
    }
    
    int& rinvio () {
         std::cout << nome_ << " - rinvio:  " << rinvio_ << '\n';
        return rinvio_;
    }

    std::string prestazioni (int goal_subiti, int parate, int uscite) {
        std::string prestazioni = "goal subiti: " + std::to_string(goal_subiti)
         + ",  parate: " + std::to_string(parate) + ",  uscite: " + std::to_string(uscite);
         std::cout << prestazioni << '\n';
         return prestazioni;
    }

};

class difensore : public calciatore {

    private:
    int stazza_;
    int lancio_;

    public:
    difensore(std::string const& nome, float altezza, float peso, int stazza, int lancio) :
             calciatore(nome, altezza, peso), stazza_{stazza}, lancio_{lancio} 
             {
                std::cout << nome << ": " << altezza << " m, " << peso << " kg " << '\n' << "stazza: " << 
                stazza_ << ", lancio lungo: " << lancio_ << '\n';
             }
    ~difensore() = default;

    std::string prestazioni (int contrasti, int lanci, int recuperi) {
        std::string prestazioni = "constrasti vinti: " + std::to_string(contrasti)
         + ",  lanci effettuati: " + std::to_string(lanci) + ",  palle recuperate: " + std::to_string(recuperi);
         std::cout << prestazioni << '\n';
         return prestazioni;
    }

    int& lancio_lungo () {
        std::cout << nome_ << " - lancio lungo:  " << lancio_ << '\n';
        return lancio_;
    }
    
    int& stazza () {
        std::cout << nome_ << " - stazza:  " << stazza_ << '\n';
        return stazza_;
    }
};

int main(){

   /* std::unique_ptr<portiere> Buffon = std::make_unique<portiere>("Gigi Buffon", 1.95, 90, 95, 90);

    std::cout << Buffon->nome() << '\n' << Buffon->rinvio() << '\n' << Buffon->riflessi() << '\n';

    std::unique_ptr<portiere> BuffonTOTS(std::move(Buffon));

    std::cout << BuffonTOTS->nome() << '\n' << BuffonTOTS->rinvio() << '\n' << BuffonTOTS->riflessi() << '\n';

    std::cout << BuffonTOTS->prestazioni(0, 7, 5) << '\n';*/

    portiere Buffon("Gigi Buffon", 1.95, 90, 95, 90);

    difensore Chiellini("Giorgio Chiellini", 1.95, 90, 99, 80);

    difensore Bonucci("Leonardo Bonucci", 1.90, 83, 85, 95);

    difensore Barzagli("Andrea Barzagli", 1.83, 75, 80, 90);


    Chiellini->prestazioni(10,3,7);

    std::vector<calciatore> Defence(4);
    Defence.push_back(Buffon);
    Defence.push_back(Chiellini);
    Defence.push_back(Bonucci);
    Defence.push_back(Barzagli);


    std::cout << "My Defence is: ";
    for (auto it : Defence){
        it.nome();
    }
    

}
