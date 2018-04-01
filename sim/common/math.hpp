//
// Created by jhwangbo on 07/09/17.
//

#ifndef BENCHMARK_MATH_HPP
#define BENCHMARK_MATH_HPP

#include <Eigen/Core>
#include <vector>
#include <memory>
#include <numeric>
#include "math.h"
#include <iostream>
#include <stdlib.h>
#include "float.h"
#include "raiCommon/rai_utils.hpp"
#include "immintrin.h"

namespace benchmark {

class VecDyn;
class MatDyn;

class DynamicArray{
 public:

  friend class benchmark::VecDyn;
  friend class benchmark::MatDyn;

  double *v = nullptr;

  double* ptr() {return v;}
  const double* ptr() const {return v;}

 private:
  void allocate(int size) {
    free(v);
    v = static_cast<double*>(aligned_alloc(32, size*sizeof(double)));
  }

};

template<int n>
class Vec {
 public:
  union {
    double v[n];
    std::aligned_storage<n, 32> alignment_only;
  };

  typedef Eigen::Map<Eigen::Matrix<double, n, 1> > EigenVecn;

  EigenVecn e() {
    return EigenVecn(v, n, 1);
  }

  inline void operator/=(double val) {
    for (int i = 0; i < n; i++)
      v[i] /= val;
  }

  inline void operator*=(double val) {
    for (int i = 0; i < n; i++)
      v[i] *= val;
  }

  inline void setZero() {
    memset(ptr(), 0, n* sizeof(double));
  }

  inline double norm() {
    double sum=0.0;
    for (int i = 0; i < n; i++)
      sum += v[i]*v[i];
    return sqrt(sum);
  }

  inline double squaredNorm() {
    double sum=0.0;
    for (int i = 0; i < n; i++)
      sum += v[i]*v[i];
    return sum;
  }

  double operator [](int i) const    {return v[i];}
  double & operator [](int i) {return v[i];}

  double* ptr() {return &(v[0]);}
  const double* ptr() const {return &(v[0]);}

  inline Vec<n>& operator=(const DynamicArray &rhs) {
    memcpy(ptr(), rhs.ptr(), n * sizeof(double));
    return *this;
  }

  inline void operator-=(const Vec<n> &rhs) {
    for (int i = 0; i < n; i++)
      v[i] -= rhs[i];
  }

  inline void operator+=(const Vec<n> &rhs) {
    for (int i = 0; i < n; i++)
      v[i] += rhs[i];
  }

  inline Vec<n>& operator=(const Vec<n> &rhs) {
    memcpy(ptr(), rhs.ptr(), n * sizeof(double));
    return *this;
  }

  template<int rows, int cols>
  inline Vec<n>& operator=(const Eigen::Matrix<double, rows, cols> &rhs) {
    memcpy(ptr(), rhs.data(), n * sizeof(double));
    return *this;
  }

};


template<int n, int m>
class Mat {
 public:

  union {
    double v[n * m];
    std::aligned_storage<n * m, 32> alignment_only;
  };

  Mat() {};

  double operator [](int i) const    {return v[i];}
  double & operator [](int i) {return v[i];}

  double* ptr() {return &(v[0]);}
  const double* ptr() const {return &(v[0]);}

  Eigen::Map<Eigen::Matrix<double, n, m> > e() {
    return Eigen::Map<Eigen::Matrix<double, n, m> >(v, n, m);
  }

  template<int j, int k>
  void fillSub(int startRow, int startColumn, Mat<j, k> &in) {
    for (int i = 0; i < j; i++) /// column index
      for (int o = 0; o < k; o++) /// row index
        v[n * (startColumn + i) + (o + startRow)] = in[i * j + o];
  }

  void fillSub(int startRow, int startColumn, Mat<3, 3> &in) {
    for (int i = 0; i < 3; i++) /// column index
      for (int o = 0; o < 3; o++) /// row index
        v[n * (startColumn + i) + (o + startRow)] = in[i * 3 + o];
  }

  void setIdentity() {
    static_assert(n == m, "setIdentity: the matrix must be a square matrix");
    memset(ptr(), 0, n * n * sizeof(double));
    for (int i = 0; i < n; i++)
      v[i * n + i] = 1.0;
  }

  inline void setZero() {
    memset(ptr(), 0, n*m* sizeof(double));
  }

  inline double sum() const {
    double sum = 0;
    for(int i=0; i< n; i++)
      for(int j=0; j<m; j++)
        sum += v[i+j*n];
    return sum;
  }

  inline Mat<n,m>& operator=(const Mat<n,m> &rhs) {
    memcpy(ptr(), rhs.ptr(), n*m * sizeof(double));
    return *this;
  }

  inline void operator*=(const double val) {
    for(int i=0; i< n; i++)
      for(int j=0; j<m; j++)
        v[i+j*n] *= val;
  }

  template<int rows, int cols>
  inline Mat<n,m>& operator=(const Eigen::Matrix<double, rows, cols> &rhs) {
    memcpy(ptr(), rhs.data(), n *m* sizeof(double));
    return *this;
  }

  inline Mat<n,m>& operator=(const DynamicArray &rhs) {
    memcpy(ptr(), rhs.ptr(), n * m * sizeof(double));
    return *this;
  }
};


class VecDyn: public DynamicArray {
 public:

  int n;
  typedef Eigen::Map<Eigen::Matrix<double, -1, 1> > EigenVecn;

  VecDyn(){}

  VecDyn(int size){
    resize(size);
  }

  VecDyn(const VecDyn& vec){
    resize(vec.n);
    memcpy(ptr(), vec.ptr(), n* sizeof(double));
  }

  double operator [](int i) const    {return v[i];}
  double & operator [](int i) {return v[i];}

  ~VecDyn(){
    free(v);
  }

  inline double sum() const {
    double sum = 0;
    for(int i=0; i< n; i++)
        sum += v[i];
    return sum;
  }

  void resize(int size) {
    n = size;
    allocate(size);
  }

  inline double squaredNorm() {
    double sum=0.0;
    for (int i = 0; i < n; i++)
      sum += v[i]*v[i];
    return sum;
  }

  inline double norm() {
    double sum=0.0;
    for (int i = 0; i < n; i++)
      sum += v[i]*v[i];
    return sqrt(sum);
  }

  EigenVecn e() {
    return EigenVecn(v, n, 1);
  }

  inline void operator/=(double val) {
    for (int i = 0; i < n; i++)
      v[i] /= val;
  }

  inline void operator*=(double val) {
    for (int i = 0; i < n; i++)
      v[i] *= val;
  }

  inline VecDyn& operator=(const VecDyn &rhs) {
    memcpy(ptr(), rhs.ptr(), n * sizeof(double));
    return *this;
  }

  template<int rows, int cols>
  inline VecDyn& operator=(const Eigen::Matrix<double, rows, cols> &rhs) {
    memcpy(ptr(), rhs.data(), n * sizeof(double));
    return *this;
  }

  template<int size>
  inline VecDyn& operator=(const Vec<size> &rhs) {
    memcpy(ptr(), rhs.ptr(), n * sizeof(double));
    return *this;
  }

  inline void setZero() {
    memset(ptr(), 0, n* sizeof(double));
  }

  inline void setZero(int n) {
    resize(n);
    memset(ptr(), 0, n* sizeof(double));
  }
};


class MatDyn: public DynamicArray {
 public:

  int n, m;

  MatDyn(){}

  MatDyn(int rows, int cols){
    resize(rows, cols);
  }

  MatDyn(const MatDyn& vec){
    resize(vec.n, vec.m);
    memcpy(ptr(), vec.ptr(), n*m* sizeof(double));
  }

  ~MatDyn() {
    free(v);
  }

  Eigen::Map<Eigen::Matrix<double, -1, -1> > e() {
    return Eigen::Map<Eigen::Matrix<double, -1, -1> >(v, n, m);
  }

  double operator [](int i) const    {return v[i];}
  double & operator [](int i) {return v[i];}

  void resize(int rows, int cols) {
    n = rows;
    m = cols;
    allocate(rows * cols);
  }

  template<int j, int k>
  void fillSub(int startRow, int startColumn, Mat<j, k> &in) {
    for (int i = 0; i < j; i++) /// column index
      for (int o = 0; o < k; o++) /// row index
        v[n * (startColumn + i) + (o + startRow)] = in[i * j + o];
  }

  void fillSub(int startRow, int startColumn, Mat<3, 3> &in) {
    for (int i = 0; i < 3; i++) /// column index
      for (int o = 0; o < 3; o++) /// row index
        v[n * (startColumn + i) + (o + startRow)] = in[i * 3 + o];
  }

  void setIdentity() {
    memset(ptr(), 0, n * n * sizeof(double));
    for (int i = 0; i < n; i++)
      v[i * n + i] = 1.0;
  }

  inline void setZero() {
    memset(v, 0, n*m* sizeof(double));
  }

  inline void setZero(int rows, int cols) {
    resize(rows,cols);
    memset(ptr(), 0, n*m* sizeof(double));
  }

  template<int rows, int cols>
  inline MatDyn& operator=(const Mat<rows,cols> &rhs) {
    memcpy(ptr(), rhs.ptr(), n*m * sizeof(double));
    return *this;
  }

  template<int rows, int cols>
  inline MatDyn& operator=(const Eigen::Matrix<double, rows, cols> &rhs) {
    memcpy(ptr(), rhs.data(), n *m* sizeof(double));
    return *this;
  }

  inline MatDyn& operator=(const MatDyn &rhs) {
    memcpy(ptr(), rhs.ptr(), n * m * sizeof(double));
    return *this;
  }

};

struct Transformation {
  Mat<3,3> rot;
  Vec<3> pos;
};

inline void quatToRotMat(const Vec<4> &q, Mat<3, 3> &R) {
  R[0] = q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
  R[1] = 2 * q[0] * q[3] + 2 * q[1] * q[2];
  R[2] = 2 * q[1] * q[3] - 2 * q[0] * q[2];

  R[3] = 2 * q[1] * q[2] - 2 * q[0] * q[3];
  R[4] = q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3];
  R[5] = 2 * q[0] * q[1] + 2 * q[2] * q[3];

  R[6] = 2 * q[0] * q[2] + 2 * q[1] * q[3];
  R[7] = 2 * q[2] * q[3] - 2 * q[0] * q[1];
  R[8] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
}

inline void rotMatToQuat(const Mat<3, 3> &R, Vec<4> &q) {
  double tr = R[0] + R[4] + R[8];
  if (tr > 0.0) {
    double S = sqrt(tr + 1.0) * 2.0; // S=4*qw
    q[0] = 0.25 * S;
    q[1] = (R[5] - R[7]) / S;
    q[2] = (R[6] - R[2]) / S;
    q[3] = (R[1] - R[3]) / S;
  } else if ((R[0] > R[4]) & (R[0] > R[8])) {
    double S = sqrt(1.0 + R[0] - R[4] - R[8]) * 2.0; // S=4*qx
    q[0] = (R[5] - R[7]) / S;
    q[1] = 0.25 * S;
    q[2] = (R[3] + R[1]) / S;
    q[3] = (R[6] + R[2]) / S;
  } else if (R[4] > R[8]) {
    double S = sqrt(1.0 + R[4] - R[0] - R[8]) * 2.0; // S=4*qy
    q[0] = (R[6] - R[2]) / S;
    q[1] = (R[3] + R[1]) / S;
    q[2] = 0.25 * S;
    q[3] = (R[7] + R[5]) / S;
  } else {
    double S = sqrt(1.0 + R[8] - R[0] - R[4]) * 2.0; // S=4*qz
    q[0] = (R[1] - R[3]) / S;
    q[1] = (R[6] + R[2]) / S;
    q[2] = (R[7] + R[5]) / S;
    q[3] = 0.25 * S;
  }
}

template<int n, int m, int l>
inline void matmul(const Mat<n, m> &mat1, const Mat<m, l> &mat2, Mat<n, l> &mat) {
  for (int j = 0; j < 3; j++) // col
    for (int k = 0; k < 3; k++) // row
      mat[j * 3 + k] =
        mat1[k] * mat2[j * 3] + mat1[k + 3] * mat2[j * 3 + 1] + mat1[k + 6] * mat2[j * 3 + 2];
}

/// first matrix is transposed
template<int n, int m, int l>
inline void transposedMatMul(const Mat<m, n> &mat1, const Mat<m, l> &mat2, Mat<n, l> &mat) {
  for (int j = 0; j < 3; j++) // col
    for (int k = 0; k < 3; k++) // row
      mat[j * 3 + k] =
        mat1[k * 3] * mat2[j * 3] + mat1[k * 3 + 1] * mat2[j * 3 + 1] + mat1[k * 3 + 2] * mat2[j * 3 + 2];
}

/// second matrix is transposed
template<int n, int m, int l>
inline void transposed2MatMul(const Mat<n, m> &mat1, const Mat<l, m> &mat2, Mat<n, l> &mat) {
  for (int j = 0; j < 3; j++) // col
    for (int k = 0; k < 3; k++) // row
      mat[j * 3 + k] =
        mat1[k] * mat2[j] + mat1[k + 3 * 1] * mat2[j + 3 * 1] + mat1[k + 3 * 2] * mat2[j + 3 * 2];
}

template<int n, int l>
inline void matScalarMul(double scalar, Mat<n, l> &mat) {
  for (int j = 0; j < n*l; j++)
    mat[j] *= scalar;
};

template<int n, int m>
inline void transpose(const Mat<n, m> &mat1, Mat<m, n> &mat) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      mat[j + i * m] = mat1[i + j * n];
}

template<int n, int m>
inline void matvecmul(const Mat<n, m> &mat1, const Vec<m> &vec1, Vec<n> &vec) {
  for (int j = 0; j < n; j++) {
    vec[j] = 0;
    for (int i = 0; i < m; i++) // col
      vec[j] += mat1[i * n + j] * vec1[i];
  }
}

template<int n, int m>
inline void matTransposevecmul(const Mat<n, m> &mat1, const Vec<m> &vec1, Vec<n> &vec) {
  for (int j = 0; j < n; j++) {
    vec[j] = 0;
    for (int i = 0; i < m; i++) // col
      vec[j] += mat1[i + j* n] * vec1[i];
  }
}

inline void matvecmul(const MatDyn &mat1, const VecDyn &vec1, VecDyn &vec) {
  for (int j = 0; j < mat1.n; j++) {
    vec[j] = 0;
    for (int i = 0; i < mat1.m; i++) // col
      vec[j] += mat1[i * mat1.n + j] * vec1[i];
  }
}

inline void matvecmul(const Mat<3, 3> &mat1, const Vec<3> &vec1, double *vec) {
  for (int j = 0; j < 3; j++) // col
    vec[j] = mat1[j] * vec1[0] + mat1[j + 3] * vec1[1] + mat1[j + 6] * vec1[2];
}

template<int size>
inline void vecsub(const Vec<size> &vec1, const Vec<size> &vec2, Vec<size> &vec) {
  for (int j = 0; j < size; j++) // col
    vec[j] = vec1[j] - vec2[j];
}

inline void vecsub(const VecDyn &vec1, const VecDyn &vec2, VecDyn &vec) {
  for (int j = 0; j < vec1.n; j++) // col
    vec[j] = vec1[j] - vec2[j];
}

inline void vecadd(const VecDyn &vec1, const VecDyn &vec2, VecDyn &vec) {
  for (int j = 0; j < vec1.n; j++) // col
    vec[j] = vec1[j] + vec2[j];
}

template<int size>
inline void vecsub(const Vec<size> &vec1, Vec<size> &vec) {
  for (int j = 0; j < size; j++) // col
    vec[j] -= vec1[j];
}

template<int n, int m>
inline void matadd(const Mat<n, m> &mat1, Mat<n, m> &mat) {
  for (int i = 0; i < n; i++) // col
    for (int j = 0; j < m; j++) // col
      mat[i + n * j] += mat1[i + n * j];
}

inline void matadd(const MatDyn &mat1, MatDyn &mat) {
  for (int i = 0; i < mat1.n; i++) // col
    for (int j = 0; j < mat1.m; j++) // col
      mat[i + mat1.n * j] += mat1[i + mat1.n * j];
}

template<int n, int m>
inline void matadd(const Mat<n, m> &mat1, const Mat<n, m> &mat2, Mat<n, m> &mat) {
  for (int i = 0; i < n; i++) // col
    for (int j = 0; j < m; j++) // col
      mat[i + n * j] = mat1[i + n * j] + mat2[i + n * j];
}

template<int n, int m>
inline void matsub(const Mat<n, m> &mat1, Mat<n, m> &mat) {
  for (int i = 0; i < n; i++) // col
    for (int j = 0; j < m; j++) // col
      mat[i + n * j] -= mat1[i + n * j];
}

template<int n, int m>
inline void matsub(const Mat<n, m> &mat1, const Mat<n, m> &mat2, Mat<n, m> &mat) {
  for (int i = 0; i < n; i++) // col
    for (int j = 0; j < m; j++) // col
      mat[i + n * j] = mat1[i + n * j] - mat2[i + n * j];
}

template<int n>
inline void mataddIdentity(Mat<n, n> &mat) {
  for (int i = 0; i < n; i++)
      mat[i + n * i] += 1;
}


template<int size>
inline void vecadd(const Vec<size> &vec1, Vec<size> &vec) {
  for (int j = 0; j < size; j++) // col
    vec[j] += vec1[j];
}

inline void vecadd(const VecDyn &vec1, VecDyn &vec) {
  for (int j = 0; j < vec1.n; j++) // col
    vec[j] += vec1[j];
}

template<int size>
inline void vecadd(const Vec<size> &vec1, const Vec<size> &vec2, Vec<size> &vec) {
  for (int j = 0; j < size; j++) // col
    vec[j] = vec1[j] + vec2[j];
}

// vec = a*vec1
template<int size>
inline void add_aX(double a, const Vec<size> &vec1, Vec<size> &vec) {
  for (int j = 0; j < size; j++) // col
    vec[j] += a * vec1[j];
}

// vec = vec2+a*vec1
template<int size>
inline void add_b_p_aX(double a, const Vec<size> &vec1, const Vec<size> &vec2, Vec<size> &vec) {
  for (int j = 0; j < 3; j++) // col
    vec[j] = vec2[j] + a * vec1[j];
}

template<int size>
inline void vecDot(const Vec<size> &vec1, const Vec<size> &vec2, double &scalar) {
  scalar = std::inner_product(vec1.ptr(), vec1.ptr() + size, vec2.ptr(), 0.0);
}

template<int size>
inline void vecDot(const double* vec1, const Vec<size> &vec2, double &scalar) {
  scalar = std::inner_product(vec1, vec1 + size, vec2.ptr(), 0.0);
}

template<int size>
inline double vecDot(const Vec<size> vec1, const Vec<size> &vec2) {
  return std::inner_product(vec1.ptr(), vec1.ptr() + size, vec2.ptr(), 0.0);
}

template<int size>
inline void vecDivide(const Vec<size> &vec1, const double scalar, Vec<size> &vec) {
  for (int j = 0; j < size; j++) // col
    vec[j] = vec1[j] / scalar;
}

template<int n, int m>
inline void matDivide(const Mat<n,m> &mat1, const double scalar, Mat<n,m> &mat) {
  for (int j = 0; j < n*m; j++) // col
    mat[j] = mat1[j] / scalar;
}

inline void cross(const Vec<3> &vec1, const Vec<3> &vec2, Vec<3> &vec) {
  vec[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
  vec[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
  vec[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

inline void cross(const double* vec1, const Vec<3> &vec2, Vec<3> &vec) {
  vec[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
  vec[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
  vec[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

inline void cross(const Vec<3> &vec1, const Vec<3> &vec2, double *vec) {
  vec[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
  vec[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
  vec[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

inline void crossThenAdd(const Vec<3> &vec1, const Vec<3> &vec2, Vec<3> &vec) {
  vec[0] += vec1[1] * vec2[2] - vec1[2] * vec2[1];
  vec[1] += vec1[2] * vec2[0] - vec1[0] * vec2[2];
  vec[2] += vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

inline void crossThenSub(const Vec<3> &vec1, const Vec<3> &vec2, Vec<3> &vec) {
  vec[0] -= vec1[1] * vec2[2] - vec1[2] * vec2[1];
  vec[1] -= vec1[2] * vec2[0] - vec1[0] * vec2[2];
  vec[2] -= vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

inline void vecScalarMul(const double scalar, const Vec<3> &vec, Vec<3> &product) {
  for (int i = 0; i < 3; i++)
    product[i] = scalar * vec[i];
}

template<int size>
inline void vecScalarMul(const double scalar, const Vec<size> &vec, Vec<size> &product) {
  for (int i = 0; i < size; i++)
    product[i] = scalar * vec[i];
}

inline void vecScalarMul(const double scalar, const VecDyn &vec, VecDyn &product) {
  for (int i = 0; i < vec.n; i++)
    product[i] = scalar * vec[i];
}

template<int size>
inline void vecScalarMulThenAdd(const double scalar, const double *vec, Vec<size> &sum) {
  for (int i = 0; i < size; i++)
    sum[i] += scalar * vec[i];
}

template<int size>
inline void vecScalarMulThenAdd(const double scalar, const Vec<size> &vec, Vec<size> &sum) {
  for (int i = 0; i < size; i++)
    sum[i] += scalar * vec[i];
}

inline void skewSymMat(const Vec<3> &vec, Mat<3,3> &mat) {
  mat[0] = 0;
  mat[4] = 0;
  mat[8] = 0;
  mat[1] = vec[2];
  mat[2] = -vec[1];
  mat[3] = -vec[2];
  mat[5] = vec[0];
  mat[6] = vec[1];
  mat[7] = vec[0];
}

template<int size>
inline double squaredNormOfDiff(const Vec<size>& vec1, const Vec<size>& vec2){
  double sum=0;

  for(int i=0; i<size; i++) {
    const double diff = vec1[i] - vec2[i];
    sum += diff * diff;
  }
  return sum;
}

inline void matReverse(Mat<3, 3> &mat) {
  mat[0] = mat[0];
  mat[1] = mat[1];
  mat[2] = mat[2];
  mat[3] = - mat[3];
  mat[4] = - mat[4];
  mat[5] = - mat[5];
  mat[6] = - mat[6];
  mat[7] = - mat[7];
  mat[8] = - mat[8];
}
} // benchmark

#endif //BENCHMARK_MATH_HPP