/*
 * UTPrivateAccessExample_test.cpp
 *
 *  Created on: Mar 28, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "UTDependency.h"

#include "UTPrivateAccess.h"


class ExampleClassA {
private:
	int prvVal;

public:
	ExampleClassA() {
	}

	ExampleClassA(int val) : prvVal(val) {
	}

private:
	bool hiddenMethod() const {
		return true;
	}

	bool hiddenMethod2(int a) const {
		return true;
	}

};


REGISTER_INVOKER_NAMED_CONST(ClassAInvoker1, ExampleClassA, hiddenMethod, bool);

REGISTER_INVOKER_NAMED_CONST1(ClassAInvoker2, ExampleClassA, hiddenMethod2, bool, int);

REGISTER_FIELD_INVOKER_NAMED(ClassAInvoker3, ExampleClassA, prvVal, int);


//-------------------------------------------------------------


BOOST_AUTO_TEST_SUITE(TestPrivateAccessExampleSuite)

	BOOST_AUTO_TEST_CASE( methodA1 ) {
		ExampleClassA a;
//		bool ret = (a.* result<ClassAInvoker1>::accessor)();
		bool ret = CALL_METHOD_INVOKER(ClassAInvoker1, a)();
		BOOST_CHECK_EQUAL( ret, true );
	}

	BOOST_AUTO_TEST_CASE( methodA2 ) {
		ExampleClassA a;
//		bool ret = (a.* result<ClassAInvoker2>::accessor)(1);
		bool ret = CALL_METHOD_INVOKER(ClassAInvoker2, a)(1);
		BOOST_CHECK_EQUAL( ret, true );
	}

	BOOST_AUTO_TEST_CASE( methodA3 ) {
		const int aVal = 3;
		ExampleClassA a(aVal);

//		bool ret = (a.* result<ClassAInvoker2>::accessor);
		int ret = CALL_FIELD_INVOKER(ClassAInvoker3, a);

		BOOST_CHECK_EQUAL( ret, aVal );
	}

BOOST_AUTO_TEST_SUITE_END()
