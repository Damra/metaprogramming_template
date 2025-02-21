# Metaprogramming Template with SFINAE

Bu şablon, **SFINAE (Substitution Failure Is Not An Error)** prensibi ile metaprogramming örnekleri içermektedir. 
Şablonlar kullanarak derleme zamanında faktöriyel hesaplama, 
Recursive Fibonacci hesaplama ve tür özelliklerini kontrol etme işlemleri yapılmaktadır.

## Derleme Zamanında Faktöriyel Hesaplama

Faktöriyel hesaplama, derleme zamanında **recursive template** kullanılarak yapılır. Aşağıdaki şablon, derleme zamanında faktöriyel hesaplamasına örnek vermektedir:

```cpp
template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static const int value = 1;
};
```

Kullanım Örneği:
```cpp
std::cout << Factorial<5>::value << std::endl;  // Çıktı: 120
```


## Recursive Fibonacci Hesaplama
Bu fonksiyon, n değeri 0 veya 1 olduğunda sonlanır ve diğer durumlarda recursive fibonacci serisini hesaplar.
```cpp
int fibonacci(int n) {
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
```

Kullanım Örneği:
```cpp
std::cout << fibonacci(10) << std::endl;  // Çıktı: 55
```

## SFINAE ile Tür Kontrolü
SFINAE (Substitution Failure Is Not An Error), tür kontrolünü derleme zamanında yapmayı sağlar. Aşağıdaki örnekler, türün belirli fonksiyonlara sahip olup olmadığını kontrol eder.

has_size: Bir Türün size() Fonksiyonuna Sahip Olup Olmadığını Kontrol Etme
```cpp
template <typename T>
class has_size {
private:
    template <typename U> static auto test(int) -> decltype(std::declval<U>().size(), std::true_type());
    template <typename> static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};
```

is_iterable: Bir Türün begin() ve end() Fonksiyonlarına Sahip Olup Olmadığını Kontrol Etme
```cpp
template <typename T>
class is_iterable {
private:
    template <typename U> static auto test(int) -> decltype(std::declval<U>().begin(), std::declval<U>().end(), std::true_type());
    template <typename> static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};
```

Kullanım Örneği:
```cpp
std::cout << "has_size<int>: " << has_size<int>::value << std::endl;  // Çıktı: false
std::cout << "is_iterable<std::vector<int>>: " << is_iterable<std::vector<int>>::value << std::endl;  // Çıktı: true

```

processContainer Fonksiyonu
SFINAE kullanılarak tür özelliklerine göre farklı işlemler yapılır. Eğer türün size() fonksiyonu varsa, boyutu yazdırılır; eğer tür iterasyon yapılabilir ise, elemanları yazdırılır.

```cpp
template <typename T>
void processContainer(T& container) {
    if constexpr (has_size<T>::value) {
        std::cout << "Container size: " << container.size() << std::endl;
    }

    if constexpr (is_iterable<T>::value) {
        for (const auto& elem : container) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}
```

Kullanım Örneği:
```cpp
std::vector<int> vec = {1, 2, 3, 4, 5};
processContainer(vec);  // Çıktı: Container size: 5 \n 1 2 3 4 5
```

```cpp
int main() {
    // Derleme Zamanında Faktöriyel Hesaplama
    std::cout << "5! = " << Factorial<5>::value << std::endl;

    // Çalışma Zamanında Fibonacci Hesaplama
    std::cout << "Fibonacci(10) = " << fibonacci(10) << std::endl;

    // Tür Kontrolü ve İşlem
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lst = {6, 7, 8, 9};
    int number = 42;

    // vector, list ve int türleri üzerinde tür kontrolü yapar ve sonuçları işler.
    processContainer(vec);  // Çıktı: Container size: 5 \n 1 2 3 4 5
    processContainer(lst);  // Çıktı: Container size: 4 \n 6 7 8 9
    processContainer(number);  // Çıktı: (no output, çünkü int türü üzerinde işlem yapılmaz)

    return 0;
}
```
