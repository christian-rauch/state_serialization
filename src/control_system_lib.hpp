#ifndef SERIALIZATION_CONTROLSYSTEMLIB_HPP
#define SERIALIZATION_CONTROLSYSTEMLIB_HPP

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_free.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(control::Ode)
BOOST_SERIALIZATION_SPLIT_FREE(control::StateSpaceC)
BOOST_SERIALIZATION_SPLIT_FREE(control::StateSpaceD)

namespace boost {
namespace serialization {


// control::Ode

template<class Archive>
void save(Archive &ar, const control::Ode &o, const unsigned int version) {
    const Eigen::VectorXd input = const_cast<control::Ode*>(&o)->getInput();
    const Eigen::VectorXd state = const_cast<control::Ode*>(&o)->getState();
    ar << input << state;
}

template<class Archive>
void load(Archive &ar, control::Ode &o, const unsigned int version) {
    Eigen::VectorXd input, state;
    ar >> input >> state;
    // Ode object must be initialized at this state
    o.setInput(input);
    o.setState(state);
}


// control::StateSpaceC

template<class Archive>
void save(Archive &ar, const control::StateSpaceC &s, const unsigned int version) {
    const Eigen::MatrixXd A = const_cast<control::StateSpaceC*>(&s)->getA();
    const Eigen::MatrixXd B = const_cast<control::StateSpaceC*>(&s)->getB();
    const Eigen::MatrixXd C = const_cast<control::StateSpaceC*>(&s)->getC();
    const Eigen::MatrixXd D = const_cast<control::StateSpaceC*>(&s)->getD();
    ar << A << B << C << D;
    
    // store remaining members from inherited Ode
    ar << boost::serialization::base_object<control::Ode>(s);
}

template<class Archive>
void load(Archive &ar, control::StateSpaceC &s, const unsigned int version) {
    // reinitialize given StateSpaceC object
    ::new(&s)control::StateSpaceC();
    Eigen::MatrixXd A, B, C, D;
    ar >> A >> B >> C >> D;
    s.setA(A);
    s.setB(B);
    s.setC(C);
    s.setD(D);
    
    // load inherited Ode members and store them in the newly created StateSpaceC object
    ar >> boost::serialization::base_object<control::Ode>(s);
}


// control::StateSpaceD

template<class Archive>
void save(Archive & ar, const control::StateSpaceD &s, const unsigned int version) {
    ar & boost::serialization::base_object<control::StateSpaceC>(s);
}

template<class Archive>
void load(Archive & ar, control::StateSpaceD &s, const unsigned int version) {}

template<class Archive>
inline void save_construct_data(Archive &ar, const control::StateSpaceD *s, const unsigned int version){
    double dt = const_cast<control::StateSpaceD*>(s)->getSampleTime();
    ar << dt;
}

template<class Archive>
inline void load_construct_data(Archive &ar, control::StateSpaceD *s, const unsigned int version){
    double dt;
    ar >> dt;

    // restore StateSpaceC object first for initializing memory
    ar & boost::serialization::base_object<control::StateSpaceC>(*s);
    Eigen::MatrixXd A = s->getA();
    Eigen::MatrixXd B = s->getB();
    Eigen::MatrixXd C = s->getC();
    Eigen::MatrixXd D = s->getD();

    // reinitialize given StateSpaceD object
    ::new(s)control::StateSpaceD(A, B, C, D, dt);
}

} // serialization
} // boost

#endif // SERIALIZATION_CONTROLSYSTEMLIB_HPP
