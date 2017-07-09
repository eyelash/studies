template <class T> class MaxHeap {
	T* data;
	int size;
	void swap (int i1, int i2) {
		T tmp (data[i1]);
		data[i1] = data[i2];
		data[i2] = tmp;
	}
	void sift_down (int i) {
		int left_child = i * 2;
		int right_child = left_child + 1;
		if (right_child <= size && data[right_child] > data[i]) {
			if (data[left_child] > data[right_child]) {
				swap (i, left_child);
				sift_down (left_child);
			} else {
				swap (i, right_child);
				sift_down (right_child);
			}
		} else if (left_child <= size && data[left_child] > data[i]) {
			swap (i, left_child);
			sift_down (left_child);
		}
	}
	void build () {
		for (int i = size/2; i >= 1; i--) {
			sift_down (i);
		}
	}
public:
	MaxHeap (T* first, int size): data(first-1), size(size) {
		build ();
	}
	int get_size () const {
		return size;
	}
	void delete_max () {
		swap (1, size--);
		sift_down (1);
	}
};

template <class T> void heapsort (T* first, int size) {
	MaxHeap<T> heap (first, size);
	while (heap.get_size() > 1) {
		heap.delete_max ();
	}
}
