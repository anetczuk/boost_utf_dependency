/*
 * DependencyUtils.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "DependencyUtils.h"
#include "DependencyFilter.h"

namespace dependency {

	::boost::unit_test::test_unit* getUnit( const DependencyId& unitId ) {
		::boost::unit_test::test_unit& unit = ::boost::unit_test::framework::get(unitId, boost::unit_test::tut_any);
		return (&unit);
	}

	const ::boost::unit_test::test_unit* getUnit( const boost::unit_test::test_suite& suite, const DependencyId& unitId ) {
		//::boost::unit_test::test_unit& unit = ::boost::unit_test::framework::get(unitId, boost::unit_test::tut_any);
		//return (&unit);
		DependencyUnitPtr ptr = TestUnitReader::read(suite, unitId);
		if (ptr.size()==0) {
			return NULL;
		} else {
			return *(ptr.begin());
		}
	}

} /* namespace dependency */
