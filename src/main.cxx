#include <iostream>

#include <vector>

#include "any.hxx"
#include "optional.hxx"

/* A very barebone test for optional and any.
 * Automated testing is planned in order to catch some behavioral failures.
 * TODO : Do some real testing.
 */

struct noconstexpr
{
	private:
	int m;


	public:
	noconstexpr() : m{}
	{};
};

struct constexprcons
{
	private:
	int m;

	public:
	constexpr constexprcons() : m{}
	{};
};

struct overloadedAddr
{
	overloadedAddr* operator&()
	{
		return nullptr;
	}
};

struct noOverloadedAddr
{
		
};

struct testConstructor
{
	testConstructor()
	{
		std::cout << "Hello ! I'm a constructor" << std::endl;
		value_ = 12;
	}
	
	~testConstructor()
	{
		std::cout << "Hello, destruction !!" << std::endl;
	}
	
	int value_;
};
 
template<class T>
void print_vector(std::vector<T>& vec)
{
	std::cout << "Begin vector printing\n";
	if(vec.size() == 0)
 	{
 		std::cout << "Empty vector !\n";
 	}
 	else
 	{
	 	for(auto elem : vec)
	 	{
	 		std::cout << elem << "\n";
	 	}
 	}
 	std::cout << "End\n";
}
 
class FooBaz{};
 
class FooDer : public FooBaz{};

int main()
{
	// Show alphanumeric values for booleans.
	std::cout << std::boolalpha;
	
	optional<int> coucou = 5;
	
	std::cout << coucou.value() << std::endl;
	std::cout << *coucou << std::endl;

	
	optional<std::vector<int>> vec = std::vector<int>{5, 6, 8};
	std::cout << vec->back() << std::endl;
	
	print_vector(*vec);
	
	vec = nullopt;

	vec = std::vector<int>{4, 2, 12};

	print_vector(*vec);
	vec->push_back(46);
	print_vector(*vec);

	auto tmpVec = vec.value_or(std::vector<int>{});
	print_vector(tmpVec);
	

	vec = nullopt;

	tmpVec = vec.value_or(std::vector<int>{});
	print_vector(tmpVec);
	
	constexpr optional<int> optConstexpr = 12;
	std::cout << *optConstexpr << std::endl;
  	
  	any hellv = 5;
  	std::cout << "hellv as int : " << hellv.as<int>().value() << std::endl;
	
	any hellv2 = FooDer{};
	std::cout << "hellv2 as FooBaz : " << hellv2.as<FooBaz>() << std::endl;
	
	optional<std::vector<int>> optVec = {1, 5, 6};
	optVec = {1, 3, 5};
	optVec = {1, 12, 546, 8};
	
	for(auto elem : *optVec)
	{
		std::cout << elem << std::endl;
	}
	
	int default1 = 12;
	int default2 = 42;
	optional<testConstructor> opt = testConstructor{};
	opt = testConstructor{};
	
	
	constexpr optional<int> opt1 {5};
	optional<int> opt2 {6};
	
	std::cout << opt.initialized() << std::endl;
	std::cout << opt->value_ << std::endl;
	std::cout << opt1.value() << std::endl;
	
	std::cout << "5 == 6 : " << (opt1 == opt2) << std::endl;
	std::cout << "5 < 6 : " << (opt1 < opt2) << std::endl;
	std::cout << "5 <= 5 : " << (opt1 <= opt1) << std::endl;
	std::cout << "5 < 5 : " << (opt1 < opt1) << std::endl;
	
	opt2 = {};
	std::cout << "5 == nullopt : " << (opt1 == opt2) << std::endl;
	std::cout << "nullopt == nullopt : " << (opt2 == opt2) << std::endl;
	std::cout << "opt2 == nullopt : " << (opt2 == nullopt) << std::endl;
	
	opt2 = 12;
	std::cout << "Value of opt 2 after opt2 = 12 : " << opt2.value() << std::endl; 
	
	opt2 = opt1;
	std::cout << "Value of opt 2 after opt2 = opt1 : " << opt1.value() << std::endl;
	
	optional<int&> optRef1 = coucou.value(); // Tmp object lifetime extension.
	std::cout << "Value of optRef1 : " << optRef1.value() << std::endl;
	
	optRef1 = {};
	std::cout << "Value of optRef1 (nullopt) with value_or(12) : " << optRef1.value_or(default1) << std::endl; 
	
	optRef1.value_or(default2);
	
	std::cout << has_overloaded_address_operator<overloadedAddr>::value << std::endl;
	std::cout << has_overloaded_address_operator<noOverloadedAddr>::value << std::endl;
	
	return 0;
}
