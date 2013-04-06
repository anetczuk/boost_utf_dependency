/*
 * DependencyLog_test.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: bob
 */

#include "UTDependency.h"

#include "DependencyLog.h"


using namespace boost::unit_test;


BOOST_AUTO_TEST_SUITE(DependencyLog)

	BOOST_AUTO_TEST_CASE( DEPENDENCY_REPORT_STREAM )
	{
//		std::ostream* stream = DEPENDENCY_REPORT_STREAM();
//		if (stream!=NULL) {
//			(*stream) << "aaa" << std::endl;
//		}
//		BOOST_WARN(false);
	}

	BOOST_AUTO_TEST_CASE( DEPENDENCY_REPORT_STREAM002 )
	{
		BOOST_WARN_MESSAGE(false, "aaaaaaa1111111");
	}

	BOOST_AUTO_TEST_CASE( DEPENDENCY_REPORT_STREAM003 )
	{
//        unit_test_log << ::boost::unit_test::log::begin( "xxxx", 21 )
//                      << ::boost::unit_test::log_fatal_errors << "pref " << "desc " << "suf ";
//
////        if( !pr.has_empty_message() )
////            unit_test_log << ". " << pr.message();
//
//        unit_test_log << ::boost::unit_test::log::end();
	}

	BOOST_AUTO_TEST_CASE( DEPENDENCY_REPORT_STREAM004 )
	{
//		std::stringstream strm;
//
//		strm << "aaaaassssdd" << std::endl << "444444444" << std::endl;
//
//		BOOST_REQUIRE_MESSAGE(false, strm.str() );
	}

BOOST_AUTO_TEST_SUITE_END()
