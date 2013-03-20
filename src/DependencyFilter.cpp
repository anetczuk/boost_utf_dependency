/*
 * DependencyFilter.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "DependencyFilter.h"


using namespace boost::unit_test;


namespace dependency {


	std::string serialize_names(const DependencyNameSet& depSet) {
		std::stringstream stream;
		if (depSet.size()>0) {
			DependencyNameSet::iterator iter = depSet.begin();
			DependencyNameSet::iterator eiter = depSet.end();
			for(; iter!=eiter; iter++) {
				stream << *iter << ",";
			}
			std::string ret = stream.str();
			return ret.substr(0, ret.size()-1);
		}
		return stream.str();
	}

	DependencyIdSet TestUnitIdReader::filter(const boost::unit_test::test_suite& suite, const DependencyName& name) {
		TestUnitIdReader filter(name);
		traverse_test_tree(suite, filter);
		return filter.ids;
	}

	DependencyIdSet TestUnitIdReader::filter(const test_suite& suite, const DependencyNameSet& depSet) {
		std::string names = serialize_names(depSet);
		TestUnitIdReader filter(names);
//			master_test_suite_t& suite = framework::master_test_suite();
		traverse_test_tree(suite, filter);
		return filter.ids;
	}

	TestUnitIdReader::TestUnitIdReader(const std::string& names) : m_depth(0) {

		string_token_iterator tit( names, (dropped_delimeters = "/", kept_delimeters = dt_none) );

		while( tit != string_token_iterator() ) {
			m_filters.push_back(
				std::vector<single_filter>( string_token_iterator( *tit, (dropped_delimeters = ",", kept_delimeters = dt_none)  ),
											string_token_iterator() ) );

			++tit;
		}

//			printFilters();
	}

	std::string TestUnitIdReader::print(const std::vector<single_filter>& single) const {
		std::stringstream str;
		size_t fSize = single.size();
		for(size_t i=0 ;i< fSize; i++) {
			const single_filter& filter = single[i];
			str << "kind[" << filter.m_kind << "] value[" << filter.m_value << "] ";
		}
		return str.str();
	}

	std::string TestUnitIdReader::printFilters() const {
		std::stringstream str;
		str << "Filters: " << std::endl;
		size_t fSize = m_filters.size();
		for(size_t i=0 ;i< fSize; i++) {
			const std::vector<single_filter>& single =  m_filters[i];
			str << print(single) << std::endl;
		}
		return str.str();
	}

	//----------------#################################

	TestUnitsFilter::TestUnitsFilter(const DependencyIdSet& units) : searchUnits(units), m_depth(0) {
	}

	TestUnitsFilter::~TestUnitsFilter() {
	}

} /* namespace dependency */
