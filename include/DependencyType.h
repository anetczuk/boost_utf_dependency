/*
 * DependencyType.h
 *
 *  Created on: Mar 13, 2013
 *      Author: Arkadiusz Netczuk
 */

#ifndef DEPENDENCYTYPE_H_
#define DEPENDENCYTYPE_H_

#include <iostream>
#include <fstream>

#include <vector>
#include <set>
#include <map>

#include <boost/test/unit_test_log.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/unit_test.hpp>


namespace dependency {
	namespace log {

		class LogState {
			bool state;
		public:
			LogState();

			bool getState() const;

			bool setState(bool newState);
		};

		//---------------------------------

		LogState& get_log_state();

		bool get_log_state_value();

		bool set_log_state(bool state);

		//---------------------------------

		class LogDisableInScope {
			bool oldState;
		public:
			LogDisableInScope();

			~LogDisableInScope();
		};
	}

	/**
	 * Auto dependency parameter
	 */
	enum auto_dependency {
		auto_dependency_on = 0,
		auto_dependency_off = 1
	};


}

std::istream& operator>>(std::istream& in, dependency::auto_dependency& unit);

std::ostream& operator<<(std::ostream& out, const dependency::auto_dependency& unit);


//-------------------------------------------------------------------------------------


//#define BOOST_JOIN( X, Y ) BOOST_DO_JOIN( X, Y )
//#define BOOST_DO_JOIN( X, Y ) BOOST_DO_JOIN2(X,Y)
//#define BOOST_DO_JOIN2( X, Y ) X##Y

#define DEPENDENCY_LOG_SET_STATE(B) dependency::log::set_log_state( B )

#define DEPENDENCY_LOG_DISABLE DEPENDENCY_LOG_SET_STATE(false)

#define DEPENDENCY_LOG_ENABLE DEPENDENCY_LOG_SET_STATE(true)

#define DEPENDENCY_LOG_DISABLE_IN_SCOPE			\
		dependency::log::LogDisableInScope BOOST_JOIN(dependencyLogDisabler_, __LINE__)

/**
 * ll - log level of type ::boost::unit_test_::log::log_level
 * M - message
 */

#define DEPENDENCY_LOG_SINK ::boost::unit_test::runtime_config::log_sink()

#define DEPENDENCY_LOG( ll, M )				                                																			\
	if (dependency::log::get_log_state_value() && ::boost::unit_test::runtime_config::log_level() <= ll && DEPENDENCY_LOG_SINK != NULL) {		\
		*(DEPENDENCY_LOG_SINK) << __FILE__ << "(" << __LINE__ << "): " << M << std::endl;														\
	}

#define DEPENDENCY_LOG_FATAL( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_fatal_errors, M )

#define DEPENDENCY_LOG_ERROR( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_all_errors, M )

#define DEPENDENCY_LOG_WARN( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_warnings, M )

#define DEPENDENCY_LOG_MESSAGE( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_messages, M )

#define DEPENDENCY_LOG_INFO( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_messages, M )

#define DEPENDENCY_LOG_UNITS( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_test_units, M )

#define DEPENDENCY_LOG_SUCCESS( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_successful_tests, M )


//-----------------------------------------------------------------------


/**
 *
 */
namespace dependency {

	typedef boost::unit_test::test_unit_id DependencyId;
//	typedef unsigned long DependencyId;
	typedef std::string DependencyName;

	typedef std::vector<DependencyId> DependencyIdVector;

	typedef std::set<DependencyId> DependencyIdSet;
	typedef std::set<DependencyName> DependencyNameSet;

	typedef std::map<DependencyId, DependencyIdSet> DependencyIdMap;

	typedef std::set< boost::unit_test::test_unit const* > DependencyUnitPtr;

	struct DependencyEntry {
		DependencyName targetFilter;
		DependencyName dependencyFilter;
		std::string file;
		int line;

		DependencyEntry(const DependencyName& target, const DependencyName& dependency, const std::string& aFile, int aLine) : targetFilter(target), dependencyFilter(dependency), file(aFile), line(aLine) {
		}

		DependencyEntry(const DependencyName& target, const DependencyName& dependency) : targetFilter(target), dependencyFilter(dependency), file(""), line(-1) {
		}

		std::string print() const;

		std::string printPlace() const;

		bool containsFileInfo() const;

	};

	//typedef std::map<DependencyName, DependencyNameSet> DependencyContainer;
	typedef std::vector<DependencyEntry> DependencyContainer;


	//-------------------------------------------------------------------------------------


	std::string print(const DependencyIdSet& set);

	std::string print(const DependencyIdMap& idMap);

}

#endif /* DEPENDENCYTYPE_H_ */
