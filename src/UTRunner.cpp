/*
 * TestsRunner.cpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "UTRunner.h"
#include "DependencySolver.h"
#include "DependencyFilter.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/results_reporter.hpp>
#include <boost/test/results_collector.hpp>


dependency::DependencyIdMap manage_dependency(::boost::unit_test::test_suite& master, const dependency::DependencyContainer& dependencyContainer) {

	DEPENDENCY_LOG_UNITS( "Added dependency:" << std::endl << dependency::print_dependency(dependencyContainer) );

	dependency::DependencySolver solver(master);
	solver.resolve(dependencyContainer);

	const dependency::DependencyIdMap& dependencyMap = solver.getDependencyMap();

	return dependencyMap;
}

//----------------------------

dependency::DependencyIdSet extractDependentUnits(const dependency::DependencyIdSet& matchedUnits, ::boost::unit_test::test_suite& suite, const dependency::DependencyIdMap& dependencyMap) {
	dependency::DependencyIdMap acestorsDepMap = dependencyMap;
	dependency::DependencySolver::expandDependencyAncestors(suite, acestorsDepMap);

	//convert ancestorsMap to deepDependencyMap
	const dependency::DependencyIdMap deepDependencyMap = dependency::DependencySolver::expandDeepDependency(acestorsDepMap);

	dependency::DependencyIdSet retSet = dependency::DependencySolver::extractDeepDependencyUnits(matchedUnits, deepDependencyMap);
	retSet.insert(matchedUnits.begin(), matchedUnits.end());
	return retSet;
}


namespace boost {
	namespace unit_test {

		int BOOST_TEST_DECL unit_test_main_extended( init_unit_test_func init_func, int argc, char* argv[], ::boost::unit_test::test_suite& suite, const dependency::DependencyContainer* dependencyContainer ) {
			try {
				if (framework::is_initialized()==false) {
					framework::init( init_func, argc, argv );
				}

				if (dependencyContainer!=NULL) {
					dependency::DependencyIdMap dependencyMap = manage_dependency(suite, *dependencyContainer);

					if( !runtime_config::test_to_run().is_empty() ) {
						boost::unit_test::const_string param = runtime_config::test_to_run();
						std::string filterName( param.begin(), param.end() );

						dependency::DependencyIdSet matchedUnits = dependency::TestUnitIdReader::filter(suite, filterName );
//						LogInfo << "matchedUnits: " << dependency::print(matchedUnits) << std::endl;

						dependency::DependencyIdSet ascensorUnits = dependency::TestAncestorsReader::read(suite, matchedUnits );
//						LogInfo << "acensorUnits: " << dependency::print(ascensorUnits) << std::endl;

						dependency::DependencyIdSet filteredUnits = extractDependentUnits(ascensorUnits, suite, dependencyMap);
//						LogInfo << "filteredUnits: " << dependency::print(filteredUnits) << std::endl;

						dependency::TestUnitsFilter filter(filteredUnits);

						traverse_test_tree( suite.p_id, filter );
					}
				} else {
					if( !runtime_config::test_to_run().is_empty() ) {
						//old behaviour
						test_case_filter filter( runtime_config::test_to_run() );

						traverse_test_tree( suite.p_id, filter );
					}
				}

				framework::run();

				results_reporter::make_report();

				return runtime_config::no_result_code()
							? boost::exit_success
							: results_collector.results( suite.p_id ).result_code();
			}
			catch( framework::nothing_to_test const& ) {
				return boost::exit_success;
			}
			catch( framework::internal_error const& ex ) {
				results_reporter::get_stream() << "Boost.Test framework internal error: " << ex.what() << std::endl;

				return boost::exit_exception_failure;
			}
			catch( framework::setup_error const& ex ) {
				results_reporter::get_stream() << "Test setup error: " << ex.what() << std::endl;

				return boost::exit_exception_failure;
			}
			catch( ... ) {
				results_reporter::get_stream() << "Boost.Test framework internal error: unknown reason" << std::endl;

				return boost::exit_exception_failure;
			}
		}

		int BOOST_TEST_DECL unit_test_main_extended( init_unit_test_func init_func, int argc, char* argv[]) {
			test_suite& master = framework::master_test_suite();

			const dependency::DependencyContainer& dependency = dependency::get_dependency_container();

			int ret = unit_test_main_extended( init_func, argc, argv, master, &dependency);

			dependency::release_dependency_resource();

			return ret;
		}

	}
}



#ifndef BOOST_TEST_MODULE
bool init_unit_test() {
	return true;
}
#endif


int run_tests(int argc, char *argv[]) {
	return ::boost::unit_test::unit_test_main_extended( &init_unit_test, argc, argv);
}
