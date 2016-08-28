#include <algorithm>

template <class T, class Compare = std::less<T>>
class ConditionallyBoundedArray {
protected:
	T* _store;
	size_t _size;
	size_t _boundSize;
	Compare _compare;
	bool _builded;

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	explicit ConditionallyBoundedArray(size_t maxSize, const Compare& compare = Compare())
		: _store(nullptr), _size(0), _boundSize(maxSize), _compare(compare), _builded(false) {
		if (_boundSize)
			_store = new T[_boundSize];
	}

	~ConditionallyBoundedArray() {
		if (_boundSize && _store != nullptr)
			delete[] _store;
	}

	void heapify(size_t root) {
		while (root < _size) {
			size_t max = root, left = 2 * root + 1;
			if (left < _size) {
				if (_compare(_store[max], _store[left]))
					max = left;
				if (left + 1 < _size && _compare(_store[max], _store[left + 1]))
					max = left + 1;
			}
			if (max == root)
				break;
			std::swap(_store[root], _store[max]);
			root = max;
		}
	}

	void push(const T& elem) {
		if (_size < _boundSize) {
			_store[_size++] = elem;
		}
		else {
			if (!_builded) {
				for (size_t i = _boundSize / 2; i; i--)
					heapify(i - 1);
				_builded = true;
			}
			if (_compare(elem, _store[0])) {
				_store[0] = elem;
				heapify(0);
			}
		}
	}

	T& operator [](size_t n) { return _store[n]; }
	const T& operator[](size_t n) const { return _store[n]; }

	iterator begin() { return _store; }
	iterator end() { return _store + _size; }
	const_iterator begin() const { return _store; }
	const_iterator end() const { return _store + _size; }
	iterator rbegin() { return _store + _size; }
	iterator rend() { return _store; }
	const_iterator rbegin() const { return _store + _size; }
	const_iterator rend() const { return _store; }
	T& front() { return _store[0]; }
	T& back() { return _store[_size - 1]; }
	const T& front() const { return _store[0]; }
	const T& back() const { return _store[_size - 1]; }
	size_t size() const { return _size; }
	bool empty() const { return _size == 0; }
	void clear() { _size = 0; }
};