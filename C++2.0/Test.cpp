#include <cstdlib>
#include <iostream>
#include <bitset>
#include <vector>
#include <functional>
#include <list>
#include <deque>
#include <tuple>

using namespace std;

namespace Variadic_Templates {
	void printX() {}
	template <typename T, typename... Types>
	void printX(const T& firstArg, const Types&... args) {
		cout << firstArg << endl;
		printX(args...);
	}

	int max(int n) {
		return n;
	}
	template<typename... Args>
	int max(int n, Args... args) {
		return std::max(n, max(args));
	}

	//output operator for tuples
	template<typename...Args>
	ostream& operator<<(ostream& os, const std::tuple<Args...>& t) {
		os << "[";
		PRINT_TUPLE<0, sizeof...(Args), Args...>::print(os, t);
		return os << "]";
	}
	template<int IDX, int MAX, typename...Args>
	struct PRINT_TUPLE {
		static void print(std::ostream& os, const std::tuple<Args...>) {
			os << get<IDX>(t) << (IDX + 1 == MAX ? "" : ",");
			PRINT_TUPLE<IDX + 1, MAX, Args...>::print(os, t);
		}
	};
	template<int MAX, typename...Args>		//�������ػ��ģ����ƥ����ȵ���
	struct PRINT_TUPLE<MAX, MAX, Args...> {
		static void print(std::ostream& os, const tuple<Args...>& t) {
		}
	};


	template <typename T>
	inline void hash_combine(size_t& seed, const T& val) {
		seed ^= hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	//auxiliary generic function
	template <typename... Types>
	inline size_t hash_val(const Types&... args) {
		size_t seed = 0;
		hash_val(seed, args...);
		return seed;
	}
	template <typename T, typename... Types>
	inline void hash_val(size_t& seed, const T& val, const Types&... args) {
		hash_combine(seed, val);
		hash_val(seed, args...);
	}
	template <typename T>
	inline void hash_val(size_t& seed, const T& val) {
		hash_combine(seed, val);
	}

	namespace MyTuple_inherited {
		template<typename... Values> class tuple;
		template<>class tuple<> {};

		template<typename Head, typename...Tail>
		class tuple<Head, Tail...>
			:private tuple<Tail...>
		{
			typedef tuple<Tail...> inherited;
		protected:
			Head m_head;
		public:
			tuple() {}
			tuple(Head v, Tail... vtail)
				:m_head(v), inherited(vtail...) {}	//�����inherited�Ǹ���Ĺ���

			//auto head()->decltype(m_head) { return m_head; }//�븴����
			Head head() { return m_head; }
			inherited& tail() { return *this; }		//��thisתΪ���ࣨ���С��һ����tuple��
		};
	}

	namespace MyTuple_composition {
		template<typename... Values> class tuple;
		template<> class tuple<> {};
		template<typename Head,typename...Tail>
		class tuple<Head, Tail...>
		{
			typedef tuple<Tail...> composited;
		protected:
			composited m_tail;
			Head m_head;
		public:
			tuple() {}
			tuple(Head v, Tail... vtail)
				:m_tail(vtail), m_head(v) {}

			Head head() { return m_head; }
			composited& tail() { return m_tail; }
		};
	}
}

namespace initialzerLst {
	void print(std::initializer_list<int> vals) {
		for (auto p = vals.begin(); p != vals.end(); ++p) {
			std::cout << *p << '\n';
		}
	}
}

namespace Alias_Template {
	//����һ�ֽⷨ���ٶ�Containerһ����iterator����traits��ȡ��valuetype���Ϳ��Է��ض��������
	template<typename Container>
	void test_moveable(Container c) {
		typedef typename iterator_traits<typename Container::iterator>::value_type Valtype;
	}

	namespace Template_Template_Parameter {
		template<typename T,
			template<class T>	//���T�ɼӿɲ���
		class Container
		>
			class XCLs {
			private:
				Container<T> c;
#define SIZE 6
			public:
				XCLs() {
					for (long i = 0; i < SIZE; ++i)
						c.insert(c.end(), T());

					output_static_data(T());
					Container<T> c1(c);
					Container<T> c2(std::move(c));
					c1.swap(c2);
				}
		};

		template<typename T>
		using Vec = std::vector<T, allocator<T>>;

		template<typename T>
		using Lst = std::list<T, allocator<T>>;

		template<typename T>
		using Deq = std::deque<T, allocator<T>>;
	}
}


int main() {
	std::cout << __cplusplus << std::endl;
	std::cout << std::max({ 54,56,89,15,7,26 });

	Variadic_Templates::printX(7.5, "hello", bitset<16>(377), 42);
	std::cout << Variadic_Templates::max(7, 5, 0, 95, 949, 262, 47, 6) << std::endl;

	initialzerLst::print({ 1,2,3,7,5,6,8,4 });

	//��Ҫ��ʽָ��allocator����Ȼ�޷��Ƶ�
	Alias_Template::Template_Template_Parameter::XCLs<int, Alias_Template::Template_Template_Parameter::Vec> cl;
}