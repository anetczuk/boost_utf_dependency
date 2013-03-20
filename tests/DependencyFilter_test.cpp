/*
 * DependencyFilter_test.cpp
 *
 *  Created on: Mar 15, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "UTDependency.h"
#include "DependencyFilter.h"

using namespace boost::unit_test;


BOOST_AUTO_TEST_SUITE(DependencyFilterSuite)

	BOOST_AUTO_TEST_CASE( TestUnitIdReader001 )
	{
		{
			test_suite* master = BOOST_TEST_SUITE( "master" );
			test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
			test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
			master->add(suiteA);
			master->add(suiteB);

			test_suite* suiteC = BOOST_TEST_SUITE( "suiteC" );
			test_suite* suiteD = BOOST_TEST_SUITE( "suiteD" );
			suiteA->add( suiteC );
			suiteB->add( suiteD );

			{
				dependency::DependencyIdSet retSet = dependency::TestUnitIdReader::filter(*master, suiteA->p_name);
				BOOST_CHECK_EQUAL( retSet.size(), 1 );
				BOOST_CHECK_EQUAL( retSet.count(suiteA->p_id), 1 );
			}
			{
				dependency::DependencyNameSet nameSet;
				nameSet.insert( suiteA->p_name );
				nameSet.insert( suiteB->p_name );
				dependency::DependencyIdSet retSet = dependency::TestUnitIdReader::filter(*master, nameSet);
				BOOST_CHECK_EQUAL( retSet.size(), 2 );
				BOOST_CHECK_EQUAL( retSet.count(suiteA->p_id), 1 );
				BOOST_CHECK_EQUAL( retSet.count(suiteB->p_id), 1 );
			}
			{
				dependency::DependencyNameSet nameSet;
				nameSet.insert( "*" );
				dependency::DependencyIdSet retSet = dependency::TestUnitIdReader::filter(*master, nameSet);
				BOOST_CHECK_EQUAL( retSet.size(), 2 );
				BOOST_CHECK_EQUAL( retSet.count(suiteA->p_id), 1 );
				BOOST_CHECK_EQUAL( retSet.count(suiteB->p_id), 1 );
			}
			{
				dependency::DependencyNameSet nameSet;
				nameSet.insert( "suiteC" );
				dependency::DependencyIdSet retSet = dependency::TestUnitIdReader::filter(*master, nameSet);
				BOOST_CHECK_EQUAL( retSet.size(), 0 );
			}
			{
				dependency::DependencyNameSet nameSet;
				nameSet.insert( "suiteA/suiteC" );
				dependency::DependencyIdSet retSet = dependency::TestUnitIdReader::filter(*master, nameSet);
				BOOST_CHECK_EQUAL( retSet.size(), 1 );
				BOOST_CHECK_EQUAL( retSet.count(suiteC->p_id), 1 );
			}
			{
				dependency::DependencyNameSet nameSet;
				nameSet.insert( "*/*" );
				dependency::DependencyIdSet retSet = dependency::TestUnitIdReader::filter(*master, nameSet);
				BOOST_CHECK_EQUAL( retSet.size(), 2 );
				//BOOST_CHECK_EQUAL( retSet.count(suiteA->p_id), 0 );
				//BOOST_CHECK_EQUAL( retSet.count(suiteB->p_id), 0 );
				BOOST_CHECK_EQUAL( retSet.count(suiteC->p_id), 1 );
				BOOST_CHECK_EQUAL( retSet.count(suiteD->p_id), 1 );
			}
		}
	}

	BOOST_AUTO_TEST_CASE( TestUnitReader001 )
	{
		{
			test_suite* master = BOOST_TEST_SUITE( "master" );
			test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
			test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
			master->add(suiteA);
			master->add(suiteB);

			test_suite* suiteC = BOOST_TEST_SUITE( "suiteC" );
			test_suite* suiteD = BOOST_TEST_SUITE( "suiteD" );
			suiteA->add( suiteC );
			suiteB->add( suiteD );

			{
				dependency::DependencyUnitPtr retSet = dependency::TestUnitReader::read(*master, suiteC->p_id);
				BOOST_CHECK_EQUAL( retSet.size(), 1 );
				BOOST_CHECK_EQUAL( retSet.count(suiteC), 1 );
				//BOOST_CHECK_EQUAL( retSet.count(suiteC->p_id), 1 );
			}
		}

	}

	BOOST_AUTO_TEST_CASE( TestAncestorsReader001 )
	{
		{
			test_suite* master = BOOST_TEST_SUITE( "master" );
			test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
			test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
			master->add(suiteA);
			master->add(suiteB);

			test_suite* suiteC = BOOST_TEST_SUITE( "suiteC" );
			test_suite* suiteD = BOOST_TEST_SUITE( "suiteD" );
			suiteA->add( suiteC );
			suiteB->add( suiteD );

			{
				dependency::DependencyIdSet retSet = dependency::TestAncestorsReader::read(*master, suiteC->p_id);
				BOOST_CHECK_EQUAL( retSet.size(), 3 );
				BOOST_CHECK_EQUAL( retSet.count(suiteA->p_id), 1 );
				BOOST_CHECK_EQUAL( retSet.count(suiteC->p_id), 1 );
				BOOST_CHECK_EQUAL( retSet.count(master->p_id), 1 );
			}
		}
	}
	BOOST_AUTO_TEST_DEPENDENCY("DependencyFilterSuite/TestAncestorsReader001", "DependencyFilterSuite/TestUnitReader001");

	BOOST_AUTO_TEST_CASE( TestUnitsFilter001 )
	{
		{
			test_suite* master = BOOST_TEST_SUITE( "master" );
			test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
			test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
			master->add(suiteA);
			test_suite* suiteC = BOOST_TEST_SUITE( "suiteC" );
			suiteA->add( suiteC );
			master->add(suiteB);
			test_suite* suiteD = BOOST_TEST_SUITE( "suiteD" );
			suiteB->add( suiteD );

			//dependency::TestUnitReader::filter(*master, "suiteA");

			dependency::DependencyIdSet set;
			set.insert( suiteA->p_id );
			dependency::TestUnitsFilter filter( set );
			traverse_test_tree( *master, filter );

			BOOST_CHECK_EQUAL( master->p_enabled.value, true );
			BOOST_CHECK_EQUAL( suiteA->p_enabled.value, true );
			BOOST_CHECK_EQUAL( suiteB->p_enabled.value, false );
			BOOST_CHECK_EQUAL( suiteC->p_enabled.value, false );
			BOOST_CHECK_EQUAL( suiteD->p_enabled.value, true );
		}
	}

BOOST_AUTO_TEST_SUITE_END()

