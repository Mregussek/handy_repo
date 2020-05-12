    
    #include <iostream>
    #include <limits>

    int main() {
        int integer = 5;
        unsigned int unsigned_integer = 5;
        signed int signed_integer = 5;
        
        std::cout << "integer: " << integer // 5
            << " unsigned integer: " << unsigned_integer // 10
            << " signed integer: " << signed_integer // 5
            << std::endl;
            
        std::cout << "integer: " << integer - 20 // -15
            << " unsigned integer: " << unsigned_integer - 20 // 4294967281
            << " signed integer: " << signed_integer - 20 // -15
            << std::endl;
            
        std::cout << "min int: " << std::numeric_limits<int>::min() // -2147483648 
            << " max int: " << std::numeric_limits<int>::max() // 2147483647 
            << " min unsigned int: " << std::numeric_limits<unsigned int>::min() // 0
            << " max unsigned int: " << std::numeric_limits<unsigned int>::max() // 4294967295
            << std::endl;
        
        std::cout << "Difference : "
            << std::numeric_limits<unsigned int>::max() - (unsigned_integer - 20) // 14
            << std::endl;
    } 

