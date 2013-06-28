#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

template <typename T>
class GenericImage {
public:
  GenericImage();
  GenericImage(int rows, int cols);

  T& operator()(int row, int col);

  void normalize();
  T& getPixels();
  void print(std::ostream& out = std::cout) const;

private:
  int d_channels;
  int d_rows;
  int d_cols;
  std::vector<T> d_pixels;
};

template <typename T>
GenericImage<T>::GenericImage()
    : d_channels(3), d_rows(0), d_cols(0), d_pixels(0) {}

template <typename T>
GenericImage<T>::GenericImage(int rows, int cols)
    : d_channels(3), d_rows(rows), d_cols(cols), d_pixels(rows * cols * d_channels) {}

template <typename T>
T& GenericImage<T>::operator()(int row, int col) {
  return d_pixels[d_channels * (d_cols * row + col)];
}

template <typename T>
void GenericImage<T>::normalize() {
  T min = 100000.0f;
  T max = 0.0f;
  for (auto itr = d_pixels.begin(); itr != d_pixels.end(); ++itr) {
    if (*itr > max) max = *itr;
    if (*itr < min) min = *itr;
  }

  if (max == 0) return;

  for (auto itr = d_pixels.begin(); itr != d_pixels.end(); ++itr) *itr = (*itr - min) / (max - min);
}

template <typename T>
T& GenericImage<T>::getPixels() {
  return d_pixels[0];
}

template <typename T>
void GenericImage<T>::print(std::ostream& out) const {
  for (int i = 0; i < d_rows; ++i) {
    for (int j = 0; j < d_cols; ++j) {
      std::cout << "(";
      std::string delim = "";
      for (int k = 0; k < d_channels; ++k) {
        std::cout << delim << std::setw(6) << d_pixels[d_channels * (d_cols * i + k) + k];
        delim = ",";
      }
      std::cout << ") ";
    }
    std::cout << std::endl;
  }
}

#endif
