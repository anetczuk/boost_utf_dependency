/*
 * TestFriendExample_test.cpp
 *
 *  Created on: Mar 28, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "UTDependency.h"


namespace TestFriendExampleSuite {
	struct methodA1;
}

class ClassA {
	friend struct TestFriendExampleSuite::methodA1;
	friend class TestAccessClass;

public:
	ClassA() {
	}

private:
	bool hiddenMethod() const {
		return true;
	}

	bool hiddenMethod2(int a) const {
		return true;
	}
};

class TestAccessClass {
public:
	void methodA() {
		ClassA friendClass;

		bool ret = friendClass.hiddenMethod();
		BOOST_CHECK_EQUAL( ret, true );
	}
};

//------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(TestFriendExampleSuite)

	BOOST_AUTO_TEST_CASE( methodA1 ) {
		ClassA friendClass;

		bool ret = friendClass.hiddenMethod();
		BOOST_CHECK_EQUAL( ret, true );
	}

	BOOST_AUTO_TEST_CASE( methodA2 ) {
		TestAccessClass a1;
		a1.methodA();
	}

BOOST_AUTO_TEST_SUITE_END()
