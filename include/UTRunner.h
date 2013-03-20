/*
 * UTRunner.h
 *
 *  Created on: Mar 8, 2013
 *      Author: Arkadiusz Netczuk
 */

#ifndef UT_RUNNER_H_
#define UT_RUNNER_H_


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

//#define BOOST_TEST_MODULE "Master Test Suite"


#include <boost/test/unit_test.hpp>
#include "UTDependency.h"


int run_tests(int argc, char *argv[], ::boost::unit_test::test_suite& suite, const dependency::DependencyIdMap* container);

int run_tests(int argc, char *argv[]);


#endif /* UT_RUNNER_H_ */
