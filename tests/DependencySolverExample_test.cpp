/*
 * DependencySolver_test.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "UTDependency.h"


BOOST_AUTO_TEST_SUITE(DependencySolverExampleSuite)

	BOOST_AUTO_TEST_CASE( methodA1 )
	{
//		BOOST_ERROR( "methodA1: Ouch..." );
		BOOST_WARN_MESSAGE( true, "methodA1-1: Ouch..." );
		BOOST_WARN_MESSAGE( false, "methodA1-2: Ouch..." );
		//BOOST_CHECK_EQUAL( 1, 2 );
	}

	BOOST_AUTO_TEST_CASE( methodA2 )
	{
		BOOST_ERROR( "methodA2: Ouch..." );
//		BOOST_WARN( "methodA2: Ouch..." );
		//BOOST_CHECK_EQUAL( 1, 2 );
	}
	BOOST_AUTO_TEST_DEPENDENCY( "DependencySolverExampleSuite/methodA1", "DependencySolverExampleSuite/methodA2");

	BOOST_AUTO_TEST_DEPENDENCY( "DependencySolverExampleSuite", "DependencySolverSuite");

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_CASE( globalMethodA1 )
{
//		BOOST_ERROR( "methodA1: Ouch..." );
//	BOOST_WARN( "global methodA1: Ouch..." );
	BOOST_CHECK_MESSAGE(false, "global methodA1: Ouch..." );
}

BOOST_AUTO_TEST_CASE( globalMethodA2 )
{
//		BOOST_ERROR( "methodA1: Ouch..." );
	BOOST_CHECK_MESSAGE( false, "global methodA2: Ouch..." );
	//BOOST_CHECK_EQUAL( 1, 2 );
}
BOOST_AUTO_TEST_DEPENDENCY( "globalMethodA2", "globalMethodA1");

BOOST_AUTO_TEST_CASE( globalMethodA3 )
{
//		BOOST_ERROR( "methodA1: Ouch..." );
	BOOST_REQUIRE_MESSAGE( false, "global methodA3: Ouch..." );
	//BOOST_CHECK_EQUAL( 1, 2 );
}
