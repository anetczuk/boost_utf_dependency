boost_utf_dependency
====================

Boost Unit Test tests dependency automatic registration facility.

Main purpose of the project is to provide functionality of automatic registration and management of dependencies rather than taking care of performance.
For now, dependency solver wasn't tested on big amount of unit tests.

Features:
- auto registration of dependencies (macro BOOST_AUTO_TEST_DEPENDENCY used similar to BOOST_AUTO_TEST_CASE),
- ordering test units according to dependency,
- calling '--run_test' command with dependent test units,
- handles non existing test units at time of auto registration,
- filtering test units similar to '--run_test' command,
- option '--auto_dependency' to enable/disable the feature,
- examples how to access private members

Limitations:
- unresolved cycles in dependencies,
- unable to handle dependencies registered by test_case::dependes_on() method.


use
===

Project contains unit tests of itself and simple example of use.

