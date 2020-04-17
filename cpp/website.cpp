    
    #include <iostream>
    #include <vector>

    int random_num() { return rand() % 10; }
    constexpr int sum(int x, int y) { return x + y; }
    void print(std::vector<int>&& v) { for(auto const& elem : v) std::cout << elem << " "; }

    int main() {
        int a = 3;
        const int b = 5;
        constexpr int c = 7;
        
        const int d = random_num(); // OK
        // constexpr int error_1 = random_num(); // ERROR!!!
        // constexpr akceptuje wartości obliczone w czasie kompilacji
        
        const int e = sum(10, 20); // OK
        constexpr int f = sum(10, 20); // OK
        
        const int g = sum(a, b); // OK
        //constexpr int error_2 = sum(a, b); // ERROR!!!
        // a nie jest liczba stala
        
        const int h = sum(b, c); // OK
        constexpr int i = sum(b, c); // OK
        
        // drukuje 3 5 7 3 30 30 8 12 12 
        print({a, b, c, d, e, f, g, h, i});
    }


