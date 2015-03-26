#ifndef SERIALIZATION_EIGEN_HPP
#define SERIALIZATION_EIGEN_HPP

#include <boost/serialization/split_free.hpp>

// http://stackoverflow.com/questions/12851126/serializing-eigens-matrix-using-boost-serialization
namespace boost {
namespace serialization {

template<class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
inline void serialize(Archive &ar, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &m, const unsigned int version){
    split_free(ar, m, version);
}

// Eigen::Matrix

template<class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
void save(Archive &ar, const Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &m, const unsigned int version) {
    long int rows = m.rows();
    long int cols = m.cols();
    // serialize matrix dimensions and data array
    ar << rows << cols;
    ar << boost::serialization::make_array(m.data(), m.size());
}

template<class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
void load(Archive &ar, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &m, const unsigned int version) {
    long int rows, cols;
    // resize matrix by deserialized dimensions
    ar >> rows >> cols;
    m.resize(rows,cols);
    // deserialize raw data array
    ar >> boost::serialization::make_array(m.data(), m.size());
}

} // serialization
} // boost

#endif // SERIALIZATION_EIGEN_HPP
