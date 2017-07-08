#include <iterator>

template <class I, class F> class Map {
	I _begin;
	I _end;
	F f;
public:
	Map(const I& _begin, const I& _end, const F& f): _begin(_begin), _end(_end), f(f) {}
	class Iterator {
		I i;
		Map* map;
	public:
		Iterator(const I& i, Map* map): i(i), map(map) {}
		bool operator !=(const Iterator& rhs) const {
			return i != rhs.i;
		}
		decltype(map->f(*i)) operator *() {
			return map->f(*i);
		}
		Iterator& operator ++() {
			++i;
			return *this;
		}
	};
	Iterator begin() {
		return Iterator(_begin, this);
	}
	Iterator end() {
		return Iterator(_end, this);
	}
};

template <class I, class F> Map<I, F> map(const I& _begin, const I& _end, const F& f) {
	return Map<I, F>(_begin, _end, f);
}
template <class C, class F> auto map(const C& c, const F& f) -> Map<decltype(std::begin(c)), F> {
	return map(std::begin(c), std::end(c), f);
}
