#include <boost/test/unit_test.hpp>

#include "server.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Service_Interface)
{
    server::Service_Interface service_interface("/test");

    // Testing initialization
    BOOST_CHECK_EQUAL(service_interface.get_root_prefix(), "/test");
    BOOST_CHECK_EQUAL(service_interface.root_prefix, "/test");

    // Testing default get method - should return NOT_FOUND
    std::string         in     = "";
    std::string         out    = "";
    server::Http_Status status = service_interface.get(in, out, "/test/something", "token");
    BOOST_CHECK_EQUAL(status, server::Http_Status::NOT_FOUND);

    // Testing default post method - should return NOT_FOUND
    status = service_interface.post(in, out, "/test/something", "token");
    BOOST_CHECK_EQUAL(status, server::Http_Status::NOT_FOUND);
}
