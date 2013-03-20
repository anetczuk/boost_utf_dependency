/*
 * UTDependency.h
 *
 *  Created on: Mar 9, 2013
 *      Author: Arkadiusz Netczuk
 */

#ifndef UT_DEPENDENCY_H_
#define UT_DEPENDENCY_H_

#include <string>

#include "DependencyType.h"
#include <boost/test/unit_test.hpp>


namespace dependency {

	const DependencyContainer& get_dependency_container();

//	void add_dependency(const DependencyName& unit, const DependencyName& dependency);

	void add_dependency(const DependencyName& unit, const DependencyName& dependency, const std::string& file, int line);

	std::string print_dependency(const DependencyContainer& dependency);

	void resolve_dependency(::boost::unit_test::test_suite& suite, const DependencyContainer& dependency);

	void resolve_dependency();

	void release_dependency_resource();

	//------------------------------------------------------------------------

	struct auto_test_unit_registrar_dependency {

		// Constructors
		auto_test_unit_registrar_dependency( std::string tc_name_param, std::string dep_name_param, std::string file, int line);
	};

};


//--------------------######################################################3


	#define BOOST_AUTO_TU_REGISTRAR_DEP( test_name )    \
		static dependency::auto_test_unit_registrar_dependency BOOST_JOIN( BOOST_JOIN( test_name, _registrar ), __LINE__ )


	/**
	 * test_name and dep_name have to be defined in "--run_tests" compatible mode.
	 *
	 * Both test_name and dep_name don't have to exist in time of registration.
	 */
	#define BOOST_AUTO_TEST_DEPENDENCY( test_name, dep_name )               \
		BOOST_AUTO_TU_REGISTRAR_DEP(dependency)(test_name, dep_name, __FILE__, __LINE__);		\


#endif /* UT_DEPENDENCY_H_ */
