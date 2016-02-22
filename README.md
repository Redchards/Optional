# Optional
This is an attempt to implement an optional type in C++14. The implementation is following as closely as possible the proposal for std::optional, which can be found here : http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3793.html
However, there is points where some liberty are taken, as with initializer_list for example. Although the proposal disallow initialization of optional type from a converting constructor which take an initializer_list, the present implementation do allow it.

The present production was inspired by the reference TS implementation : https://github.com/akrzemi1/Optional

Other than that, the interface is similar as the on described here, at cppreference : http://en.cppreference.com/w/cpp/experimental/optional

## Use of optional

Here are some very basic examples of how you can use the optional type.
``` C++
// Simple optional intialization.
optional<int> opt1 = 5;
// Literal type optional. Can be constexpr too.
constexpr optional<int> opt2 = 12;
// Disengaging the opt1 (two options)
opt1 = {}
opt1 = nullopt;
// Using std::vector<int>
optional<std::vector<int>> optVec {1, 2, 3};
// Printing 1 2 3
for(auto elem : *optVec)
{
   std::cout << elem << "\n";
}
optVec->push_back(8);
// Printing 1 2 3 8
for(auto elem : *optVec)
{
   std::cout << elem << "\n";
}
// Disengaging
optVec = {}
// Attempting to access the value of a disengaged optional throws an exception of type "optional_access_failure"
for(auto elem : *optVec)
{
   std::cout << elem << "\n";
}
```

Of course, these are too simple to be particularly interesting. An example which could be a bit more interesting is the "any" type provided along with the optional implementation. It shows how to cast the type without throwing if the casting fails, and returning an optional instead.

#Any
The basic example provided as a somewhat motivational example. Basically, there is three solution to handle a failing cast with an any type : 
- Throwing an exception on failure. The user can't be wrong about the type, and it can lead to frustration to some degree.
- Returning a naked pointer, NULL if the cast failed. Not very explicit, and let you change the value of the object from the outside.
- Returning a std::pair<T, bool>, where T is the desired resulting type from the cast. However, this is not very explicit, and "T" must be default initializable.

The optional type aims to solve this kind of problem by providing a nullable object.
