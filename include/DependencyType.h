/*
 * DependencyType.h
 *
 *  Created on: Mar 13, 2013
 *      Author: Arkadiusz Netczuk
 */

#ifndef DEPENDENCYTYPE_H_
#define DEPENDENCYTYPE_H_

#include <vector>
#include <set>
#include <map>

#include <boost/test/unit_test.hpp>


/**
 *
 */
namespace dependency {

	typedef boost::unit_test::test_unit_id DependencyId;
//	typedef unsigned long DependencyId;
	typedef std::string DependencyName;

	typedef std::vector<DependencyId> DependencyIdVector;

	typedef std::set<DependencyId> DependencyIdSet;
	typedef std::set<DependencyName> DependencyNameSet;

	typedef std::map<DependencyId, DependencyIdSet> DependencyIdMap;

	typedef std::set< boost::unit_test::test_unit const* > DependencyUnitPtr;

	struct DependencyEntry {
		DependencyName targetFilter;
		DependencyName dependencyFilter;
		std::string file;
		int line;

		DependencyEntry(const DependencyName& target, const DependencyName& dependency, const std::string& aFile, int aLine) : targetFilter(target), dependencyFilter(dependency), file(aFile), line(aLine) {
		}

		DependencyEntry(const DependencyName& target, const DependencyName& dependency) : targetFilter(target), dependencyFilter(dependency), file(""), line(-1) {
		}

		std::string print() const;

		std::string printPlace() const;

		bool containsFileInfo() const;

	};

	//typedef std::map<DependencyName, DependencyNameSet> DependencyContainer;
	typedef std::vector<DependencyEntry> DependencyContainer;


	//-------------------------------------------------------------------------------------


	std::string print(const DependencyIdSet& set);

	std::string print(const DependencyIdMap& idMap);


	//-------------------------------------------------------------------------------------


	/**
	 * Auto dependency parameter
	 */
	enum auto_dependency {
		auto_dependency_on = 0,
		auto_dependency_off = 1
	};

}

std::istream& operator>>(std::istream& in, dependency::auto_dependency& unit);

std::ostream& operator<<(std::ostream& out, const dependency::auto_dependency& unit);

#endif /* DEPENDENCYTYPE_H_ */
