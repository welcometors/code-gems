#include <tuple>

// --------------------[ pair ]--------------------
template<class Ch, class Tr, class T1, class T2>
decltype(auto) operator << (std::basic_ostream<Ch, Tr>& os, const std::pair<T1, T2>& val) {
	return os << '(' << val.first << ',' << ' ' << val.second << ')';
}

// --------------------[ tuple ]--------------------
template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple(std::basic_ostream<Ch, Tr>& os, Tuple const& t, std::index_sequence<Is...>) {
	using swallow = int[];
	(void)swallow {
		0, (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...
	};
}

template<class Ch, class Tr, class... Args>
decltype(auto) operator << (std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {
	os << '(';
	print_tuple(os, t, std::make_index_sequence<sizeof...(Args)>());
	return os << ')';
}

// --------------------[ container ]--------------------
template<typename T>
struct has_const_iterator {
private:
	typedef char                      yes;
	typedef struct { char array[2]; } no;

	template<typename C> static yes test(typename C::const_iterator*);
	template<typename C> static no  test(...);
public:
	static const bool value = sizeof(test<T>(0)) == sizeof(yes);
	typedef T type;
};

template <typename T>
struct has_begin_end {
	template<typename C> static char(&f(typename std::enable_if<
		std::is_same<decltype(static_cast<typename C::const_iterator(C::*)() const>(&C::begin)),
		typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

	template<typename C> static char(&f(...))[2];

	template<typename C> static char(&g(typename std::enable_if<
		std::is_same<decltype(static_cast<typename C::const_iterator(C::*)() const>(&C::end)),
		typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

	template<typename C> static char(&g(...))[2];

	static bool const beg_value = sizeof(f<T>(0)) == 1;
	static bool const end_value = sizeof(g<T>(0)) == 1;
};

template<typename T>
struct is_container : std::integral_constant<bool, has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value>
{ };

template<class Ch, class Tr, class T>
typename std::enable_if<is_container<T>::value, std::basic_ostream<Ch, Tr>&>::type
operator << (std::basic_ostream<Ch, Tr>& os, const T& container) {
	os << '{';

	auto start = std::begin(container), end = std::end(container);
	if (start != end) {
		os << *start;
		for (start = std::next(start); start != end; start = std::next(start))
			os << ", " << *start;
	}

	return os << '}';
}