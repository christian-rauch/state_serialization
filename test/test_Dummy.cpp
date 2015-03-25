#include <boost/test/unit_test.hpp>
#include <serialization/Dummy.hpp>

using namespace serialization;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    serialization::DummyClass dummy;
    dummy.welcome();
}
