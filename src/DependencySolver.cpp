/*
 * DependencySolver.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "DependencySolver.h"

#include <deque>

#include "DependencyFilter.h"


using namespace boost::unit_test;


namespace dependency {

	DependencyIdSet extractDependencyUnits(const dependency::DependencyIdSet& entrySet, const dependency::DependencyIdMap& ancestorsDependencyMap) {
		dependency::DependencyIdSet retSet;

		dependency::DependencyIdSet::const_iterator iter = entrySet.begin();
		dependency::DependencyIdSet::const_iterator eiter = entrySet.end();
		for(; iter!=eiter; iter++) {
			//check if unit have dependencies
			if (ancestorsDependencyMap.count(*iter) > 0) {
				const dependency::DependencyIdSet& depSet = ancestorsDependencyMap.at(*iter);
				retSet.insert(depSet.begin(), depSet.end());
			}
		}

		return retSet;
	}

	/**
	 * Returns set fully checked against dependency (deep, recursive)
	 */
	DependencyIdSet DependencySolver::extractDeepDependencyUnits( const dependency::DependencyIdSet& entrySet, const dependency::DependencyIdMap& ancestorsDependencyMap) {
		dependency::DependencyIdSet retSet;			//copy

		dependency::DependencyIdSet firstStep = extractDependencyUnits(entrySet, ancestorsDependencyMap);

		//contains element that are not in retSet (new one)
		dependency::DependencyIdVector newElems(firstStep.begin(), firstStep.end());	//copy

		while(newElems.size()>0) {
			dependency::DependencyId unit = newElems.back();
			newElems.pop_back();
			retSet.insert(unit);
			//check if unit have dependencies
			if (ancestorsDependencyMap.count(unit) > 0) {
				const dependency::DependencyIdSet& depSet = ancestorsDependencyMap.at(unit);
				dependency::DependencyIdSet::const_iterator diter = depSet.begin();
				dependency::DependencyIdSet::const_iterator deiter = depSet.end();
				for(; diter!=deiter; diter++) {
					DependencyId dId = *diter;
					if(retSet.count(dId)<=0) {
						//new element
						newElems.push_back(dId);
					}
				}
			}
		}

		return retSet;
	}

//	DependencySolver::DependencySolver() : masterSuite( framework::master_test_suite() ) {
//	}

	DependencySolver::DependencySolver(boost::unit_test::test_suite& suite) : masterSuite(suite) {
	}

	DependencySolver::~DependencySolver() {
	}

	//----------------#################################

	class LevelCalculateVisitor : public test_tree_visitor {
	public:

		static DependencyLevelMap readLevels(const test_suite& suite) {
			LevelCalculateVisitor visitor;
//			master_test_suite_t& suite = framework::master_test_suite();
			traverse_test_tree(suite, visitor);
			return visitor.map;
		}

		DependencyLevel level;
		DependencyLevelMap map;

		LevelCalculateVisitor() : level(0), map() {
		}

		virtual ~LevelCalculateVisitor() {
		}

		virtual bool test_suite_start( test_suite const& c)   {
			DependencyId id = c.p_id;
			map[ id ] = level;
			level++;
			return true;
		}

		virtual void test_suite_finish( boost::unit_test::test_suite const& c)  {
			level--;
		}

		virtual void visit( boost::unit_test::test_case const& c) {
			DependencyId id = c.p_id;
			map[ id ] = level;
		}
	};

	//----------------#################################

	test_suite* getSuite( DependencyId unitId ) {
		test_unit& unit = framework::get(unitId, tut_suite);
		test_suite* suite = static_cast<test_suite*>(&unit);
		return suite;
	}

	//------------------------------------------------------------------------------------------

	bool DependencySolver::checkAncestor(DependencyLevel level, DependencyId unitId, DependencyId currId) {
		if (unitId==currId) {
			//the same sub-tree
			return false;
		} else {
			//different sub-tree: normal case
			if (level==0) {
				///if there is 0 level (tree root), so id's have to be the same (there in no more root's)
				DEPENDENCY_LOG_ERROR( "Invalid internal state" );
				return false;
			} else {
				test_unit* unit = getUnit(unitId);
				test_unit* curr = getUnit(currId);
				for(DependencyLevel i=0; i< level; i++) {
					//normalizedMap[unitAncestorId].insert(currId);
					DependencyId parentId = unit->p_parent_id;
					DependencyId currParentId = curr->p_parent_id;
					if (parentId==currParentId) {
						DependencyId id1 = unit->p_id;
						DependencyId id2 = curr->p_id;
						normalizedMap[id1].insert(id2);
						break;
					}
				}
				return true;
			}
		}
	}

	DependencyId getAncestorId(DependencyId unitId, DependencyLevel level) {
		DependencyId parentId = unitId;
		while(level>0) {
			test_unit* unit = getUnit(parentId);
			parentId = unit->p_parent_id;
			level--;
		}
		return parentId;
	}

	bool DependencySolver::normalizeDepenencies(DependencyId unitId, const DependencyIdSet& depSet) {
		bool ret = true;
		//framework::get(unit);
		test_unit* unit = getUnit(unitId);

		//DependencyId unitParentId = unit->parent_id_t;
		DependencyLevel unitLevel = levels.at(unitId);

		DependencyIdSet::iterator iter = depSet.begin();
		DependencyIdSet::iterator eiter = depSet.end();
		for(; iter!=eiter; iter++) {
			//LogInfo << *iter << ", ";
			//check_dep(levels, unitId, *iter);

			DependencyId currId = *iter;

			DependencyLevel currLevel = levels.at(currId);
			DependencyLevel levelDiff = unitLevel - currLevel;
			if (levelDiff>0) {
				//unit is deeper
				DependencyId unitAncestorId = getAncestorId(unitId, levelDiff);
				bool valid = checkAncestor(currLevel, unitAncestorId, currId);
				if (valid==false) {
					DEPENDENCY_LOG_ERROR( "Unit test [" << unit->p_name << "] depends on his sub-tree (not registered) #1" );
					ret = false;
				}
			} else {
				//current is deeper or on the same level
				DependencyId currAncestorId = getAncestorId(currId, -levelDiff);
				bool valid = checkAncestor(unitLevel, unitId, currAncestorId);
				if (valid==false) {
					if (levelDiff==0) {
						DEPENDENCY_LOG_ERROR( "Unit test [" << unit->p_name << "] depends on himself (not registered)" );
					} else {
						DEPENDENCY_LOG_ERROR( "Unit test [" << unit->p_name << "] depends on his sub-tree (not registered) #2" );
					}
					ret = false;
				}
			}
		}
		return ret;
	}

	//-----------#######################################################

	class ReadDirectMembersVisitor : public test_tree_visitor {
	private:
		bool visitSuite;
	public:
		std::deque<DependencyId> members;

		ReadDirectMembersVisitor() : visitSuite(true), members() {
		}

		virtual ~ReadDirectMembersVisitor() {
		}

		virtual bool test_suite_start( test_suite const& c)   {
			if (visitSuite==true) {
				visitSuite = false;
				return true;
			} else {
				members.push_back( c.p_id );
				return false;
			}
		}

		virtual void test_suite_finish( boost::unit_test::test_suite const& c)  {
			//do nothing
		}

		virtual void visit( boost::unit_test::test_case const& c) {
			members.push_back( c.p_id );
		}
	};

	//-----------#######################################################

	int getIndexOfLastDependency(const std::deque<DependencyId>& members, const DependencyIdSet& deps) {
		const std::size_t memSize = members.size();
		for(int i=memSize-1; i>=0; i--) {
			if ( deps.count( members[i] )>0 ) {
				return i;
			}
		}
		return -1;
	}

	bool sortMembers(const DependencyIdMap& depMap, std::deque<DependencyId>& members) {
		//organize members dependencies
		const std::size_t membersSize = members.size();
		if (membersSize>1) {
			//bool sorting = true;
			std::size_t index = 0;
			std::size_t moveCounter = 0;
			while(index < membersSize) {
				bool moved = false;
				DependencyId currId = members[index];
				if (depMap.count(currId)>0) {
					//have dependencies
					const DependencyIdSet& deps = depMap.at(currId);
					int lastDep = getIndexOfLastDependency(members, deps);
					if (lastDep > index) {
						//move
						std::deque<DependencyId>::iterator indexIter = members.begin() + index;
						members.erase(indexIter);
						std::deque<DependencyId>::iterator lastDepIter = members.begin() + lastDep;
						members.insert(lastDepIter, currId);
						moved = true;
						moveCounter++;
					}
				}
				if (moved==false) {
					index++;
					moveCounter = 0;
				} else {
					std::size_t rest = membersSize - index;
					if (moveCounter>=rest) {
						//cycle detected - break
						return false;
					}
				}
			}
		}
		return true;
	}

	bool actualizeMembers(test_suite& suite, const std::deque<test_unit_id>& members) {
		const std::size_t suiteSize = suite.size();
		const std::size_t memSize = members.size();
		if (suiteSize != memSize) {
			//diff size
			DEPENDENCY_LOG_ERROR( "Members size[" << memSize << "] differs from sute size[" << suiteSize << "]" );
			return false;
		} else {
			//obtain all pointers
			std::vector<test_unit*> units(memSize);
			bool gotAllUnits = true;
			//std::deque<test_unit_id>::const_iterator iter = members.begin();
			//std::deque<test_unit_id>::const_iterator eiter = members.end();
			for(std::size_t i =0; i<memSize; i++) {
				DependencyId unitId = members[i];
				test_unit* unit = getUnit(unitId);
				if (unit!=NULL) {
					units[i] = unit;
				} else {
					DEPENDENCY_LOG_ERROR( "Could not get member[" << unitId << "]" );
					gotAllUnits = false;
				}
			}

			if (gotAllUnits==false) {
				DEPENDENCY_LOG_ERROR( "Could not get all members" );
				return false;
			} else {
				//remove old tests
				for(std::size_t i=0; i<suiteSize; i++) {
					suite.remove( members[i] );
				}
				const std::size_t newSuiteSize = suite.size();
				if (newSuiteSize>0) {
					DEPENDENCY_LOG_ERROR( "Could not clear the suite" );
					return false;
				} else {
					//add tests in correct order

					for(std::size_t i=0; i<memSize; i++) {
						test_unit* unit = units[i];
						suite.add(unit, unit->p_expected_failures, unit->p_timeout);
					}
				}
			}
			return true;
		}
	}

	/**
	 * As a input method takes normalized map of dependency
	 */
	bool DependencySolver::orderDependency() const {
		DependencyIdSet parentsSet;

		/**
		 * Read parents to sort members of
		 */
		{
			DependencyIdMap::const_iterator iter = normalizedMap.begin();
			DependencyIdMap::const_iterator eiter = normalizedMap.end();
			for(; iter!=eiter; iter++) {
				//LogInfo << "dependency[" << iter->first << "]: ";
				//print(iter->second);
				//LogInfo << std::endl;

				DependencyId unitId = iter->first;
				//DependencyIdSet depsSet = iter->second;
				//fix_dependency(unitId, depsSet);

				test_unit* unit = getUnit(unitId);
				DependencyId parentId = unit->p_parent_id;

				parentsSet.insert(parentId);
			}
		}

		/**
		 * Sort members
		 */
		{
			DependencyIdSet::iterator iter = parentsSet.begin();
			DependencyIdSet::iterator eiter = parentsSet.end();
			for(; iter!=eiter; iter++) {
				//LogInfo << *iter << ", ";
				//check_dep(levels, unitId, *iter);

				DependencyId currId = *iter;

				test_suite* curr = getSuite(currId);

				ReadDirectMembersVisitor reader;
				traverse_test_tree(*curr, reader);
				std::deque<DependencyId>& members = reader.members;

				bool sorted = sortMembers(normalizedMap, members);
				if (sorted) {
					//exchange elements
					bool exchanged = actualizeMembers(*curr, members);
					if (exchanged==false) {
						DEPENDENCY_LOG_ERROR( "Unable to actualize suite[" << currId << "] with dependencies" );
						return false;
					}
					//else actualized
				} else {
					//unable to resolve dependencies
					DEPENDENCY_LOG_ERROR( "Unable to sort dependencies" );
					return false;
				}

			}
			return true;
		}
	}

	//-------------------------------------------------

	bool DependencySolver::filterDependencies(const DependencyContainer& container, DependencyIdMap& retMap) const {
		bool valid = true;
		DependencyContainer::const_iterator iter = container.begin();
		DependencyContainer::const_iterator eiter = container.end();
		for(; iter!=eiter; iter++) {
//			LogInfo << "dependency[" << iter->first << "]: ";
//			print(iter->second);
//			LogInfo << std::endl;

			const DependencyEntry& entity = *iter;
			const DependencyName& target = entity.targetFilter;
			DependencyIdSet targetSet = TestUnitIdReader::filter( masterSuite, target );
			if (targetSet.size()<=0) {
				valid = false;
				if (entity.containsFileInfo()) {
					DEPENDENCY_LOG_ERROR( "Unable to find target case/suite[" << target << "] registered in " << entity.printPlace() );
				} else {
					DEPENDENCY_LOG_ERROR( "Unable to find target case/suite[" << target << "]" );
				}
			} else {
				const DependencyName& depend = entity.dependencyFilter;
				DependencyIdSet depSet = TestUnitIdReader::filter( masterSuite, depend );
				if (depSet.size()<=0) {
					valid = false;
					if (entity.containsFileInfo()) {
						DEPENDENCY_LOG_ERROR( "Unable to find dependency case/suite[" << depend << "] registered in " << entity.printPlace() );
					} else {
						DEPENDENCY_LOG_ERROR( "Unable to find dependency case/suite[" << depend << "]" );
					}
				} else {
					DependencyIdSet::const_iterator titer = targetSet.begin();
					DependencyIdSet::const_iterator teiter = targetSet.end();
					for(; titer!=teiter; titer++) {
						DependencyId targetId = *titer;

						retMap[targetId].insert(depSet.begin(), depSet.end());
					}
				}
			}
		}
		return valid;
	}

	/**
	 * Return false if something went wrong.
	 */
	bool DependencySolver::normalizeDepenencies() {
		bool ret = true;
		DependencyIdMap::const_iterator iter = dependencyMap.begin();
		DependencyIdMap::const_iterator eiter = dependencyMap.end();
		for(; iter!=eiter; iter++) {
			DependencyId targetId = iter->first;
			const DependencyIdSet& depSet = iter->second;

//			LogInfo << "DependencySolver::resolve(): registering target[" << targetId << "]";
//			LogInfo << " deps["; print(depSet); LogInfo << "]" << std::endl;
			bool valid = normalizeDepenencies(targetId, depSet);
			if (valid==false)
				ret = false;
		}
		return ret;
	}

	void DependencySolver::registerDependencies(DependencyId unitId, const DependencyIdSet& depSet) const {
		test_unit* unit = getUnit(unitId);

		DependencyIdSet::iterator iter = depSet.begin();
		DependencyIdSet::iterator eiter = depSet.end();
		for(; iter!=eiter; iter++) {
			DependencyId currId = *iter;
			test_unit* curr = getUnit(currId);
			if (curr!=NULL) {
				unit->depends_on(curr);
			} else {
				DEPENDENCY_LOG_ERROR("Unable to register dependency unit test[" << currId << "]");
			}
		}
	}

	void DependencySolver::registerDependencies(const DependencyIdMap& depMap) const {
		DependencyIdMap::const_iterator iter = dependencyMap.begin();
		DependencyIdMap::const_iterator eiter = dependencyMap.end();
		for(; iter!=eiter; iter++) {
			DependencyId targetId = iter->first;
			const DependencyIdSet& depSet = iter->second;
			registerDependencies(targetId, depSet);
		}
	}

	/**
	 * Returns false if one of steps failed.
	 */
	bool DependencySolver::resolve(const DependencyContainer& container) {
		if (container.size()<=0)
			return true;

		bool ret = true;
		levels = LevelCalculateVisitor::readLevels(masterSuite);

		const bool fil = filterDependencies(container, dependencyMap);
		if (!fil) {
			ret = false;
		}

		const bool norm = normalizeDepenencies();
		if (!norm) {
			ret = false;
		}

		bool ordered = orderDependency();
		if (ordered==true && dependencyMap.size()>0) {
			registerDependencies(dependencyMap);
			if (ret) {
				DEPENDENCY_LOG_SUCCESS( "Successfully registered dependencies" << std::endl );
			} else {
				DEPENDENCY_LOG_WARN( "Not all dependencies was successfully registered" << std::endl );
			}
			return ret;
		} else {
			DEPENDENCY_LOG_FATAL( "Unable to order dependencies - dependencies not registered" << std::endl );
			return false;
		}
	}

} /* namespace dependency */
