/*
 * UTDependency.cpp
 *
 *  Created on: Mar 10, 2013
 *      Author: Arkadiusz Netczuk
 */


#include "UTDependency.h"

#include <sstream>

#include "DependencySolver.h"

#include <boost/test/unit_test.hpp>


using namespace boost::unit_test;


namespace dependency {

	#if defined(__CYGWIN__)
	DependencyContainer* get_dependency_map_ptr() {
		static DependencyContainer* the_inst = 0;
		if(!the_inst)
			the_inst = new DependencyContainer;
		return the_inst;
	}

	DependencyContainer& get_dependency_container_mutable() {
		return *(get_dependency_map_ptr());
	}

	void release_dependency_resource() {
		//release
		DependencyContainer* map = get_dependency_map_ptr();
		delete map;
	}
	#else
	DependencyContainer& get_dependency_container_mutable() {
		static DependencyContainer the_inst;
		return the_inst;
	}

	void release_dependency_resource() {
		//do nothing
	}
	#endif

	const DependencyContainer& get_dependency_container() {
		return get_dependency_container_mutable();
	}

//	void add_dependency(const DependencyName& unit, const DependencyName& dependency) {
//		DependencyContainer& container = get_dependency_container();
//		//DependencyNameSet& set = container[unit];
//		//set.insert(dependency);
//
//		DependencyEntry entry(unit, dependency);
//		container.push_back(entry);
//
////		LogInfo << "Dependency added: " << unit << "->" << dependency << std::endl;
//	}

	void add_dependency(const DependencyName& unit, const DependencyName& dependency, const std::string& file, int line) {
		DependencyContainer& container = get_dependency_container_mutable();
		DependencyEntry entry(unit, dependency, file, line);
		container.push_back(entry);
	}

	std::string print(const DependencyNameSet& set) {
		std::stringstream str;
		DependencyNameSet::iterator iter = set.begin();
		DependencyNameSet::iterator eiter = set.end();
		for(; iter!=eiter; iter++) {
			str << *iter << ", ";
		}
		return str.str();
	}

	std::string print_dependency(const DependencyContainer& dependency) {
		std::stringstream str;
		DependencyContainer::const_iterator iter = dependency.begin();
		DependencyContainer::const_iterator eiter = dependency.end();
		for(; iter!=eiter; iter++) {
			str << iter->print() << std::endl;
		}
		return str.str();
	}

	std::string print(const DependencyIdMap& idMap) {
		std::stringstream str;
		DependencyIdMap::const_iterator iter = idMap.begin();
		DependencyIdMap::const_iterator eiter = idMap.end();
		for(; iter!=eiter; iter++) {
			str << "dependency[" << iter->first << "]: " << print(iter->second) << std::endl;
		}
		return str.str();
	}

	std::string print(const DependencyLevelMap& idMap) {
		std::stringstream str;
		DependencyLevelMap::const_iterator iter = idMap.begin();
		DependencyLevelMap::const_iterator eiter = idMap.end();
		for(; iter!=eiter; iter++) {
			str << "" << iter->first << ": " << iter->second << std::endl;
		}
		return str.str();
	}

	//--------------------############################

	void resolve_dependency(::boost::unit_test::test_suite& suite, const DependencyContainer& dependency) {
		DependencySolver solver(suite);
		solver.resolve(dependency);
	}

	void resolve_dependency() {
		master_test_suite_t& master = framework::master_test_suite();
		const DependencyContainer& container = get_dependency_container();
		resolve_dependency(master, container);
	}


	//-----------######################################################


	void removeQuotes(std::string& quoted) {
		const std::size_t quotedSize = quoted.size();
		if ( quoted[0] == '"' && quoted[quotedSize-1]=='"') {
			quoted.erase( quotedSize - 1 ); // erase the last character
			quoted.erase( 0, 1 ); // erase the first character
		}
	}

	// Constructors
	//auto_test_unit_registrar_dep( const_string tc_name, const_string dep) {
	auto_test_unit_registrar_dependency::auto_test_unit_registrar_dependency( std::string tc_name_param, std::string dep_name_param, std::string file, int line) {
		dependency::add_dependency( tc_name_param, dep_name_param, file, line );
	}

};
