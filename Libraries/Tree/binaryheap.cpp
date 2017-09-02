template<class T, class Compare = std::less<typename Container::value_type>>
class binaryheap {
private:
	std::vector<T> values;
	std::vector<size_t> indexes;
	
	void heapify(size_t i) {
		const size_t n = values.size();
		const l = i << 1, r = l + 1;
		size_t m = i;
		if (l < n && Compare(values[indexes[l]], values[indexes[m]]))
			m = l;
		if (r < n && Compare(values[indexes[r]], values[indexes[m]]))
			m = r;
		if (m != i) {
			std::swap(indexes[i], indexes[m]);
			heapify(m);
		}
	}

public:
	binaryheap() {}
	
	T& top() {
		return values[indexes.front()];
	}

	void push(T& value) {
		values.push_back
	}
};