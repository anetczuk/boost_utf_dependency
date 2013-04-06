/*
 * DependencyLog.h
 *
 *  Created on: Apr 6, 2013
 *      Author: bob
 */

#ifndef DEPENDENCYLOG_H_
#define DEPENDENCYLOG_H_

//includes required for macros
#include <boost/test/results_reporter.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>


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

} /* namespace dependency */


//-----------------------------------------------------------------------------------


//#define BOOST_JOIN( X, Y ) BOOST_DO_JOIN( X, Y )
//#define BOOST_DO_JOIN( X, Y ) BOOST_DO_JOIN2(X,Y)
//#define BOOST_DO_JOIN2( X, Y ) X##Y

/**
 * Disable or enable logging.
 */
#define DEPENDENCY_LOG_SET_STATE(B) dependency::log::set_log_state( B )

#define DEPENDENCY_LOG_DISABLE DEPENDENCY_LOG_SET_STATE(false)

#define DEPENDENCY_LOG_ENABLE DEPENDENCY_LOG_SET_STATE(true)

#define DEPENDENCY_LOG_DISABLE_IN_SCOPE			\
		dependency::log::LogDisableInScope BOOST_JOIN(dependencyLogDisabler_, __LINE__)

#define DEPENDENCY_LOG_SINK() ::boost::unit_test::runtime_config::log_sink()

#define DEPENDENCY_REPORT_SINK() ::boost::unit_test::runtime_config::report_sink()

#define DEPENDENCY_REPORT_STREAM() &(::boost::unit_test::results_reporter::get_stream())

/**
 * ll - log level of type ::boost::unit_test_::log::log_level
 * M - message
 */
#define DEPENDENCY_LOG( ll, M )				                                																			\
	if (dependency::log::get_log_state_value() && ::boost::unit_test::runtime_config::log_level() <= ll && DEPENDENCY_LOG_SINK() != NULL) {		\
		*(DEPENDENCY_LOG_SINK()) << __FILE__ << "(" << __LINE__ << "): " << M << std::endl;														\
	}

#define DEPENDENCY_LOG_FATAL( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_fatal_errors, "fatal: " << M )

#define DEPENDENCY_LOG_ERROR( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_all_errors, "error: " << M )

#define DEPENDENCY_LOG_WARN( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_warnings, "warning: " << M )

#define DEPENDENCY_LOG_MESSAGE( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_messages, "message: " << M )

#define DEPENDENCY_LOG_INFO( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_messages, "info: " << M )

#define DEPENDENCY_LOG_UNITS( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_test_units, M )

#define DEPENDENCY_LOG_SUCCESS( M )				                                	\
	DEPENDENCY_LOG( ::boost::unit_test::log_successful_tests, M )


#endif /* DEPENDENCYLOG_H_ */
