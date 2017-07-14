#include <complex>

template <class T> class Data {
	T* data;
	int stride;
public:
	Data(T* data, int stride = 1): data(data), stride(stride) {}
	T& operator [](int i) const {
		return data[i * stride];
	}
	Data subdata(int offset, int stride_factor) const {
		return Data(data + offset * stride, stride * stride_factor);
	}
};

void fft(Data<const std::complex<float>> input, Data<std::complex<float>> output, int N) {
	if (N == 1) {
		output[0] = input[0];
	}
	else {
		fft(input.subdata(0, 2), output, N/2);
		fft(input.subdata(1, 2), output.subdata(N/2, 1), N/2);
		for (int k = 0; k < N/2; ++k) {
			const std::complex<float> factor = std::exp(std::complex<float>(0, -2*M_PI*k/N));
			const std::complex<float> tmp = output[k];
			output[k] = tmp + factor * output[N/2+k];
			output[N/2+k] = tmp - factor * output[N/2+k];
		}
	}
}

void ifft(Data<const std::complex<float>> input, Data<std::complex<float>> output, int N) {
	if (N == 1) {
		output[0] = input[0];
	}
	else {
		ifft(input.subdata(0, 2), output, N/2);
		ifft(input.subdata(1, 2), output.subdata(N/2, 1), N/2);
		for (int k = 0; k < N/2; ++k) {
			const std::complex<float> factor = std::exp(std::complex<float>(0, 2*M_PI*k/N));
			const std::complex<float> tmp = output[k];
			output[k] = (tmp + factor * output[N/2+k]) / std::complex<float>(2);
			output[N/2+k] = (tmp - factor * output[N/2+k]) / std::complex<float>(2);
		}
	}
}
