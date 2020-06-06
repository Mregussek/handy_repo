    
    #include <iostream>
    #define FOO(var) foo(#var, var)

    enum PassedExam { // definiujemy typ wyliczeniowy
        No,         /* = 0 */
        Yes,        /* = 1 */
        Undefined   /* = 2 */
    };

    void foo(const char* name, const PassedExam val) {
        switch(val) {
        case PassedExam::Yes:
            std::cout << name << " - Passed! = " << val << std::endl;
            break;
        case PassedExam::No:
            std::cout << name << " - Try again! = " << val << std::endl;
            break;
        default:
            std::cout << "Undefined exam value = " << val << std::endl;
            break;
        }
    }

    int main() {
        PassedExam LinearAlgebra = PassedExam::Yes;
        PassedExam Physics = PassedExam::No;
        
        FOO(LinearAlgebra);
        FOO(Physics);
        FOO(PassedExam::Undefined);
    }


