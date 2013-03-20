/*
 * DependencySolver.h
 *
 *  Created on: Mar 13, 2013
 *      Author: Arkadiusz Netczuk
 */

#ifndef DEPENDENCYSOLVER_H_
#define DEPENDENCYSOLVER_H_

#include "DependencyType.h"
#include "DependencyFilter.h"

#include <boost/test/unit_test.hpp>

/**
 * Friend class for protected access inside unit tests.
 */
class UnitTestAccessor;

namespace dependency {

	typedef long DependencyLevel;
	typedef std::map<DependencyId, DependencyLevel> DependencyLevelMap;

	typedef std::vector<DependencyId> DependencyIdPath;
	typedef std::vector<DependencyIdPath> DependencyPathsContainer;

	class DependencySolver {
		friend class ::UnitTestAccessor;
	public:
		static void expandDependencyAncestors(::boost::unit_test::test_suite& suite, dependency::DependencyIdMap& dependencyMap) {
			dependency::DependencyIdMap::iterator iter = dependencyMap.begin();
			dependency::DependencyIdMap::iterator eiter = dependencyMap.end();
			for(; iter!=eiter; iter++) {
				dependency::DependencyId id = iter->first;
				iter->second = dependency::TestAncestorsReader::read(suite, iter->second);
			}
		}

		static DependencyIdSet extractDeepDependencyUnits( const dependency::DependencyIdSet& entrySet, const dependency::DependencyIdMap& ancestorsDependencyMap);

		static dependency::DependencyIdMap expandDeepDependency( const dependency::DependencyIdMap& dependencyMap) {
			dependency::DependencyIdMap deepDependencyMap;

			dependency::DependencyIdMap::const_iterator iter = dependencyMap.begin();
			dependency::DependencyIdMap::const_iterator eiter = dependencyMap.end();
			for(; iter!=eiter; iter++) {
				const dependency::DependencyIdSet& depends = dependencyMap.at(iter->first);
				deepDependencyMap[iter->first] = dependency::DependencySolver::extractDeepDependencyUnits(depends, dependencyMap);
				deepDependencyMap[iter->first].insert(depends.begin(), depends.end());
			}

			return deepDependencyMap;
		}


	protected:
		::boost::unit_test::test_suite& masterSuite;

		DependencyLevelMap levels;

		DependencyIdMap dependencyMap;

		/**
		 * Contains dependency map between test units on the same level
		 */
		DependencyIdMap normalizedMap;

	public:
//		DependencySolver();

		DependencySolver(boost::unit_test::test_suite& suite);

		virtual ~DependencySolver();

		const DependencyIdMap& getDependencyMap() const {
			return dependencyMap;
		}

		bool resolve(const DependencyContainer& container);


	protected:

		bool filterDependencies(const DependencyContainer& container, DependencyIdMap& retMap) const;

		bool normalizeDepenencies();

		bool normalizeDepenencies(DependencyId unitId, const DependencyIdSet& depSet);

		bool checkAncestor(DependencyLevel level, DependencyId unitId, DependencyId currId);

		bool orderDependency() const;

		void registerDependencies(const DependencyIdMap& depMap) const;

		void registerDependencies(DependencyId unitId, const DependencyIdSet& depSet) const;

	};

} /* namespace dependency */
#endif /* DEPENDENCYSOLVER_H_ */
