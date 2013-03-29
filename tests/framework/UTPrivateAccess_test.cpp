/*
 * TestPrivateAccessExample_test.cpp
 *
 *  Created on: Mar 28, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "UTDependency.h"

#include "UTPrivateAccess.h"


class ClassA {
private:
	int prvValue;

	const int prvValue2;

public:
	ClassA(int val) : prvValue(val), prvValue2(val) {
	}

private:
	int getV() const {
		return prvValue;
	}

	int getV2() {
		return prvValue;
	}

	int getV3(int x) {
		return x;
	}
};


//---------------------------------


REGISTER_INVOKER_NAMED_CONST(ClassAMethodInvoker1, ClassA, getV, int);

REGISTER_INVOKER_NAMED(ClassAMethodInvoker1b, ClassA, getV2, int);

REGISTER_INVOKER_NAMED1(ClassAMethodInvoker1c, ClassA, getV3, int, int);

REGISTER_FIELD_INVOKER_NAMED(ClassAFieldInvoker1, ClassA, prvValue, int);

REGISTER_FIELD_INVOKER_NAMED_CONST(ClassAFieldInvoker2, ClassA, prvValue2, int);


//---------------------------------------------------------


BOOST_AUTO_TEST_SUITE(UTPrivateAccessSuite)

	BOOST_AUTO_TEST_CASE( methodA1 ) {
		const int aVal = 5;
		ClassA a(aVal);

		int ret = CALL_INVOKER(ClassAMethodInvoker1, a)();

		BOOST_CHECK_EQUAL( ret, aVal );
	}

	BOOST_AUTO_TEST_CASE( methodA1b ) {
		const int aVal = 5;
		ClassA a(aVal);

		int ret = CALL_INVOKER(ClassAMethodInvoker1b, a)();

		BOOST_CHECK_EQUAL( ret, aVal );
	}

	BOOST_AUTO_TEST_CASE( methodA1c ) {
		const int aVal = 5;
		ClassA a(aVal);

		const int paramVal = 7;
		int ret = CALL_INVOKER(ClassAMethodInvoker1c, a)(paramVal);

		BOOST_CHECK_EQUAL( ret, paramVal );
	}

	BOOST_AUTO_TEST_CASE( methodA2 ) {
		const int aVal = 5;
		ClassA a(aVal);

		int ret = CALL_INVOKER(ClassAFieldInvoker1, a);

		BOOST_CHECK_EQUAL( ret, aVal );
	}

	BOOST_AUTO_TEST_CASE( methodA3 ) {
		const int aVal = 5;
		ClassA a(aVal);

		int ret = CALL_INVOKER(ClassAFieldInvoker2, a);

		BOOST_CHECK_EQUAL( ret, aVal );
	}

BOOST_AUTO_TEST_SUITE_END()
