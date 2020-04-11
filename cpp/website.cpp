	
    #include <iostream>

    /* Funkcja przyjmuje wartosc obiektu (kopie) 
    oryginalne zmienne NIE ulegaja zmianie */
    void copy(int copied) {
        std::cout << ++copied << std::endl;
        std::cout << "Memory address copied: " << &copied << std::endl;
    }

    /* Funkcja otrzymuje adres do przeslanego obiektu 
    oryginalne zmienne ulegaja zmianie */
    void ref(int& reference) {
        std::cout << --reference << std::endl;    
        std::cout << "Memory address reference: " << &reference << std::endl;
    }

    int main()
    {
        int c = 5;
        int r = 10;
        
        copy(c); // wyswietli 6
        std::cout << c << std::endl; // wyswietli 5
        std::cout << "Memory address copied: " << &c << std::endl;
    
        ref(r); // wyswietli 9
        std::cout << r << std::endl; // wyswietli 9
        std::cout << "Memory address reference: " << &r << std::endl;
    }


