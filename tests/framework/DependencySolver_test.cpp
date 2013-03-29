/*
 * DependencySolver_test.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: Arkadiusz Netczuk
 */


#include "UTDependency.h"
#include "DependencySolver.h"
//#include "TestsRunner.h"

using namespace dependency;
using namespace boost::unit_test;


//class UnitTestAccessor {
//public:
//	static bool filterDependencies(dependency::DependencySolver& solver, const DependencyContainer& container, DependencyIdMap& retMap) {
//		return solver.filterDependencies(container, retMap);
//	}
//};


BOOST_AUTO_TEST_SUITE(DependencySolverSuite)

	BOOST_AUTO_TEST_DEPENDENCY( "DependencySolverSuite", "DependencyFilterSuite");

	BOOST_AUTO_TEST_CASE( resolve001 )
	{
		{
			/**
			 * Empty master case
			 */
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, true );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("master", "master");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("aa", "bb");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
		}
		{
			/**
			 * One member case.
			 */
			{
				/**
				 * Registering dependency upon himself.
				 */
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteA", "suiteA");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("master", "suiteA");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
		}
		{
			/**
			 * suiteA and suiteB are parallel
			 */
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				master->add(suiteA);
				master->add(suiteB);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteB", "suiteA");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, true );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				master->add(suiteA);
				master->add(suiteB);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteA", "suiteB");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, true );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				master->add(suiteA);
				master->add(suiteB);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteA", "suiteB");
					container.push_back(entity);
				}
				{
					DependencyEntry entity("suiteB", "suiteA");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
		}
		{
			/**
			 * suiteA and suiteB are serial
			 */

			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				suiteA->add(suiteB);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteB", "suiteA");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				suiteA->add(suiteB);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteA", "suiteB");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				suiteA->add(suiteB);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteA/suiteB", "suiteA");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				suiteA->add(suiteB);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteA", "suiteA/suiteB");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
		}
		{
			/**
			 * suiteA and suiteB are serial
			 */

			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				suiteA->add(suiteB);
				test_suite* suiteC = BOOST_TEST_SUITE( "suiteC" );
				master->add(suiteC);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteC", "suiteB");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				suiteA->add(suiteB);
				test_suite* suiteC = BOOST_TEST_SUITE( "suiteC" );
				master->add(suiteC);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteC", "suiteA/suiteB");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, true );
			}
			{
				DEPENDENCY_LOG_DISABLE_IN_SCOPE;

				test_suite* master = BOOST_TEST_SUITE( "master" );
				test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
				master->add(suiteA);
				test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
				suiteA->add(suiteB);
				test_suite* suiteC = BOOST_TEST_SUITE( "suiteC" );
				master->add(suiteC);

				dependency::DependencySolver solver(*master);

				DependencyContainer container;
				{
					DependencyEntry entity("suiteC", "suiteA/suiteB");
					container.push_back(entity);
				}
				{
					DependencyEntry entity("suiteA/suiteB", "suiteC");
					container.push_back(entity);
				}
				const bool resolved = solver.resolve(container);

				BOOST_CHECK_EQUAL( resolved, false );
			}
		}
	}

	BOOST_AUTO_TEST_CASE( resolve002 )
	{
		{
			DEPENDENCY_LOG_DISABLE_IN_SCOPE;

			test_suite* master = BOOST_TEST_SUITE( "master" );
			test_suite* suiteA = BOOST_TEST_SUITE( "suiteA" );
			test_suite* suiteB = BOOST_TEST_SUITE( "suiteB" );
			master->add(suiteA);
			master->add(suiteB);

			dependency::DependencySolver solver(*master);

			DependencyContainer container;
			{
				DependencyEntry entity("suiteA", "suiteB");
				container.push_back(entity);
			}
			const bool resolved = solver.resolve(container);
			BOOST_REQUIRE_EQUAL( resolved, true );
		}
	}
	BOOST_AUTO_TEST_DEPENDENCY( "DependencySolverSuite/resolve002", "DependencySolverSuite/resolve001");

	BOOST_AUTO_TEST_CASE( extractDependentUnits001 ){
		{
			DEPENDENCY_LOG_DISABLE_IN_SCOPE;

			DependencyIdSet set;
			DependencyIdMap map;
			DependencyIdSet retSet = DependencySolver::extractDeepDependencyUnits(set, map);
			BOOST_CHECK_EQUAL( retSet.size(), 0 );
		}
		{
			DEPENDENCY_LOG_DISABLE_IN_SCOPE;

			DependencyIdSet set;
			set.insert(5);
			DependencyIdMap map;
			DependencyIdSet retSet = DependencySolver::extractDeepDependencyUnits(set, map);
			BOOST_CHECK_EQUAL( retSet.size(), 0 );
		}
		{
			DEPENDENCY_LOG_DISABLE_IN_SCOPE;

			DependencyIdSet set;
			set.insert(5);
			DependencyIdMap map;
			const DependencyId dep = 4;
			map[5].insert(dep);
			DependencyIdSet retSet = DependencySolver::extractDeepDependencyUnits(set, map);
			BOOST_CHECK_EQUAL( retSet.size(), 1 );
			BOOST_CHECK_EQUAL( retSet.count(dep), 1 );
		}
		{
			DEPENDENCY_LOG_DISABLE_IN_SCOPE;

			DependencyIdSet set;
			set.insert(5);
			DependencyIdMap map;
			const DependencyId dep1 = 4;
			map[5].insert(dep1);
			const DependencyId dep2 = 3;
			map[dep1].insert(dep2);
			DependencyIdSet retSet = DependencySolver::extractDeepDependencyUnits(set, map);
			BOOST_CHECK_EQUAL( retSet.size(), 2 );
			BOOST_CHECK_EQUAL( retSet.count(dep1), 1 );
			BOOST_CHECK_EQUAL( retSet.count(dep2), 1 );
		}
	}

BOOST_AUTO_TEST_SUITE_END()

