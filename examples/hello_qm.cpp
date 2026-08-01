#include <iostream>
#include "qm.hpp"

int main() {
    // qm::Scalar is an alias to the library's Complex implementation
    qm::Scalar a{1.0, 0.5};
    std::cout << "qm::Scalar a = " << a << std::endl;

    // quick smoke test success message
    std::cout << "QM headers included successfully." << std::endl;
    return 0;
}
