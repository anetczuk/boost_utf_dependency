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

	BOOST_AUTO_TEST_CASE( DISABLE_IN_SCOPE )
	{
		DEPENDENCY_LOG_DISABLE_IN_SCOPE();
	}

BOOST_AUTO_TEST_SUITE_END()
