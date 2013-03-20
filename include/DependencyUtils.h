/*
 * DependencyUtils.h
 *
 *  Created on: Mar 14, 2013
 *      Author: Arkadiusz Netczuk
 */

#ifndef DEPENDENCYUTILS_H_
#define DEPENDENCYUTILS_H_

#include "DependencyType.h"

#include <boost/test/unit_test.hpp>


namespace dependency {

	::boost::unit_test::test_unit* getUnit( const DependencyId& unitId );

	const ::boost::unit_test::test_unit* getUnit( const boost::unit_test::test_suite& suite, const DependencyId& unitId );

	//----------------------------------------------------------------------

	class PrintTestTreeVisitor : public boost::unit_test::test_tree_visitor {
	public:
		static void print() {
			using namespace boost::unit_test;
			master_test_suite_t& master = framework::master_test_suite();
			PrintTestTreeVisitor v;
			traverse_test_tree(master, v);
		}

		virtual ~PrintTestTreeVisitor() {
		}

		virtual bool test_suite_start( boost::unit_test::test_suite const& c)   {
			DEPENDENCY_LOG_UNITS( "Entering suite: " << c.p_id << ", " << c.p_name );
			return true;
		}

		virtual void test_suite_finish( boost::unit_test::test_suite const& c)  {
			DEPENDENCY_LOG_UNITS( "Leaving suite: " << c.p_name );
		}

		virtual void visit( boost::unit_test::test_case const& c) {
			DEPENDENCY_LOG_UNITS( "Visiting case: " << c.p_id <<", " << c.p_name );
		}
	};

} /* namespace dependency */
#endif /* DEPENDENCYUTILS_H_ */
