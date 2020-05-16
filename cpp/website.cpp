    
    #include <iostream>

    struct A_empty { };
    struct A_onevar { char c; };

    struct A {    
        char m_c = 'a';
        int m_i = 10;
        
        void print() 
        { std::cout << m_c << " - " << m_i << std::endl; }
    };

    int main() {
        int a = 5;
        
        std::cout << sizeof(char) << std::endl; // 1
        std::cout << sizeof(int) << std::endl; // 4
        std::cout << sizeof(a) << std::endl; // 4
        
        std::cout << sizeof(A_empty) << std::endl; // 1
        std::cout << sizeof(A_onevar) << std::endl; // 1
        std::cout << sizeof(A) << std::endl; // 8
} 


