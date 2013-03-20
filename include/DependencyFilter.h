/*
 * DependencyFilter.h
 *
 *  Created on: Mar 14, 2013
 *      Author: Arkadiusz Netczuk
 */

#ifndef DEPENDENCY_FILTER_H_
#define DEPENDENCY_FILTER_H_

#include "TestCaseFilter.h"
#include "DependencyType.h"
#include "DependencyUtils.h"


namespace dependency {

	//class DependencyTestReader : public boost::unit_test::test_case_filter {

	/**
	 * Reader extracts given names (only bottom name).
	 */
	class TestUnitIdReader : public boost::unit_test::test_tree_visitor {
	public:
		typedef boost::unit_test::test_case_filter::single_filter single_filter;

		static DependencyIdSet filter(const boost::unit_test::test_suite& suite, const DependencyName& name);

		static DependencyIdSet filter(const boost::unit_test::test_suite& suite, const DependencyNameSet& depSet);


		//---------------------------------------------


		/**
		 * Contains bottom test units.
		 */
		DependencyIdSet ids;

	protected:
		// Data members
		std::vector<std::vector<single_filter> >    m_filters;
		unsigned                                    m_depth;

	public:

		TestUnitIdReader(const std::string& names);

		virtual ~TestUnitIdReader() {
		}

		std::string print(const std::vector<single_filter>& single) const;

		std::string printFilters() const;

		bool matchUnit( boost::unit_test::test_unit const& tu )
		{
			std::vector<single_filter> const& filters = m_filters[m_depth-1];

			bool match =
				std::find_if( filters.begin(), filters.end(), bind( &single_filter::pass, _1, boost::ref(tu) ) ) != filters.end();
			if (match) {
	//				LogInfo << "Match: " << tu.p_name << " ";
	//				print(filters);
	//				LogInfo << std::endl;

				const std::size_t fSize = m_filters.size();
				if (m_depth==fSize) {
					//add bottom element of match
					ids.insert( tu.p_id );
				}
			}
			return match;
		}

		// test tree visitor interface
		virtual void visit( boost::unit_test::test_case const& tc ) {
			matchUnit( tc );
		}

		virtual bool test_suite_start( boost::unit_test::test_suite const& ts ) {
			bool ret = false;
			if( m_depth == 0 ) {
				//first level - master suite
				ret = true;
			} else {
				const std::size_t fSize = m_filters.size();
				if( (m_depth) > fSize ) {
					//too deep
					ret = false;
				} else {
					bool matches = matchUnit( ts );
					if (matches==false) {
						//--m_depth;
						ret = false;
					} else {
						if( m_depth == fSize ) {
							//last element of filter
							ret = false;
						} else {
							ret = true;
						}
					}
				}
			}
			if (ret) {
				++m_depth;
			}
			return ret;
		}

		virtual void test_suite_finish( boost::unit_test::test_suite const& )  {
			--m_depth;
		}

	};


	//---------------------############################---------------------------------------


	class TestUnitReader : public boost::unit_test::test_tree_visitor {
	public:
		static DependencyUnitPtr read(const boost::unit_test::test_suite& suite, const DependencyId& unit) {
			TestUnitReader filter(unit);
			traverse_test_tree(suite, filter);
			return filter.units;
		}
		static DependencyUnitPtr read(const boost::unit_test::test_suite& suite, const DependencyIdSet& unitSet) {
			TestUnitReader filter(unitSet);
			traverse_test_tree(suite, filter);
			return filter.units;
		}

		//---------------------------------------------------------------

		DependencyIdSet searchUnits;

		DependencyUnitPtr units;

		TestUnitReader(const DependencyId unit) {
			searchUnits.insert(unit);
		}

		TestUnitReader(const DependencyIdSet& unitSet) : searchUnits(unitSet) {
		}

		virtual ~TestUnitReader() {
		}

		void add( boost::unit_test::test_unit const& unit ) {
			boost::unit_test::test_unit const* ptr = &unit;
			units.insert(ptr);
		}

		void checkUnit( boost::unit_test::test_unit const& unit ) {
			if (searchUnits.count(unit.p_id)>0) {
				add(unit);
			}
		}

		virtual bool test_suite_start( boost::unit_test::test_suite const& ts ) {
			checkUnit( ts );
			return true;
		}

		// test tree visitor interface
		virtual void visit( boost::unit_test::test_case const& tc ) {
			checkUnit( tc );
		}

//		virtual void test_suite_finish( boost::unit_test::test_suite const& )  {
//			//do nothing
//		}
	};

	//-----------------------------------------------------------------------

	class TestAncestorsReader : public boost::unit_test::test_tree_visitor {
	public:

		static DependencyIdSet read(const boost::unit_test::test_suite& suite, const DependencyId& unit) {
			TestAncestorsReader filter(suite, unit);
			traverse_test_tree(suite, filter);
			return filter.ancestors;
		}

		static DependencyIdSet read(const boost::unit_test::test_suite& suite, const DependencyIdSet& unitSet) {
			TestAncestorsReader filter(suite, unitSet);
			traverse_test_tree(suite, filter);
			return filter.ancestors;
		}

		//---------------------------------------------------------------

		const boost::unit_test::test_suite& suite;

		DependencyIdSet searchUnits;

		/**
		 * Ancestors.
		 */
		DependencyIdSet ancestors;

		TestAncestorsReader(const boost::unit_test::test_suite& masterSuite, const DependencyId unit) : suite(masterSuite) {
			searchUnits.insert(unit);
		}

		TestAncestorsReader(const boost::unit_test::test_suite& masterSuite, const DependencyIdSet& unitSet) : suite(masterSuite), searchUnits(unitSet) {
		}

		virtual ~TestAncestorsReader() {
		}

		void addToAncestors( boost::unit_test::test_unit const& unit ) {
			if (ancestors.count(unit.p_id)<=0) {
				ancestors.insert(unit.p_id);
				::boost::unit_test::test_unit const* ancestorUnit = getUnit(suite, unit.p_parent_id);
				if (ancestorUnit!=NULL) {
					addToAncestors(*ancestorUnit);
				}
			}
		}

		void checkUnit( boost::unit_test::test_unit const& unit ) {
			if (searchUnits.count(unit.p_id)>0) {
				addToAncestors(unit);
			}
		}

		virtual bool test_suite_start( boost::unit_test::test_suite const& ts ) {
			checkUnit( ts );
			return true;
		}

		// test tree visitor interface
		virtual void visit( boost::unit_test::test_case const& tc ) {
			checkUnit( tc );
		}

//		virtual void test_suite_finish( boost::unit_test::test_suite const& )  {
//			//do nothing
//		}
	};

	//----------------#################################

	class TestUnitsFilter : public boost::unit_test::test_tree_visitor {
	public:

		DependencyIdSet searchUnits;

		long m_depth;

		TestUnitsFilter(const DependencyIdSet& units);

		virtual ~TestUnitsFilter();


		bool checkUnit( boost::unit_test::test_unit const& unit ) {
			if (m_depth==0 || searchUnits.count(unit.p_id)>0) {
				unit.p_enabled.value = true;
				return true;
			} else {
				unit.p_enabled.value = false;
				return false;
			}
		}

		virtual bool test_suite_start( boost::unit_test::test_suite const& ts ) {
			if (checkUnit( ts )==true) {
				m_depth++;
				return true;
			}
			return false;
		}

		// test tree visitor interface
		virtual void visit( boost::unit_test::test_case const& tc ) {
			checkUnit( tc );
		}

		virtual void test_suite_finish( boost::unit_test::test_suite const& )  {
			m_depth--;
		}

	};

} /* namespace dependency */
#endif /* DEPENDENCY_FILTER_H_ */
