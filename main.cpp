#include <iostream>
#include <type_traits>
#include <vector>
#include <list>

using namespace std;

// Derleme zamanında faktöriyel hesaplama (template metaprogramming)
template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

// Temel durum: 0! = 1
template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

// Özyinelemeli Fibonacci hesaplama
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// SFINAE ile tür kontrolü: size() fonksiyonu var mı?
template <typename T, typename = void>
struct has_size : false_type {};

template <typename T>
struct has_size<T, void_t<decltype(declval<T>().size())>> : true_type {};

// SFINAE ile tür kontrolü: begin() ve end() fonksiyonları var mı?
template <typename T, typename = void>
struct is_iterable : false_type {};

template <typename T>
struct is_iterable<T, void_t<decltype(declval<T>().begin()), decltype(declval<T>().end())>> : true_type {};

// Tür kontrolü kullanarak özel bir işlem yapma
template <typename T>
void processContainer(const T& container) {
    if constexpr (has_size<T>::value) {
        cout << "Container size: " << container.size() << endl;
    } else {
        cout << "Container does not have size() method." << endl;
    }

    if constexpr (is_iterable<T>::value) {
        cout << "Container is iterable. Elements: ";
        for (const auto& element : container) {
            cout << element << " ";
        }
        cout << endl;
    } else {
        cout << "Container is not iterable." << endl;
    }
}

int main() {
    // Derleme zamanında faktöriyel hesaplama
    constexpr int fact5 = Factorial<5>::value;
    cout << "Factorial of 5 (compile-time): " << fact5 << endl;

    // Özyinelemeli Fibonacci hesaplama
    int fib10 = fibonacci(10);
    cout << "Fibonacci of 10 (runtime): " << fib10 << endl;

    // SFINAE ile tür kontrolü
    vector<int> vec = {1, 2, 3, 4, 5};
    list<double> lst = {1.1, 2.2, 3.3};

    cout << "\nProcessing vector:" << endl;
    processContainer(vec);

    cout << "\nProcessing list:" << endl;
    processContainer(lst);

    // int türü üzerinde kontrol (size() ve iterasyon yok)
    int x = 42;
    cout << "\nProcessing int:" << endl;
    processContainer(x);

    return 0;
}
