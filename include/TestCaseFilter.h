/*
 * TestCaceFilter.h
 *
 *  Created on: Mar 10, 2013
 *      Author: Arkadiusz Netczuk
 *
 *      File contains parts of Boost Unit Test Framework implementation
 *      to keep things going.
 */

#ifndef TEST_CASE_FILTER_H_
#define TEST_CASE_FILTER_H_

#include <boost/test/unit_test.hpp>

// Boost
#include <boost/cstdlib.hpp>
#include <boost/bind.hpp>

// STL
#include <stdexcept>
#include <iostream>

#if !defined(__BORLANDC__) && !BOOST_WORKAROUND( BOOST_MSVC, < 1300 ) && !BOOST_WORKAROUND( __SUNPRO_CC, < 0x5100 )
#define BOOST_TEST_SUPPORT_RUN_BY_NAME
#include <boost/test/utils/iterator/token_iterator.hpp>
#endif


namespace boost {

	namespace unit_test {

		class test_case_filter : public test_tree_visitor {
		public:
			struct single_filter {
				single_filter( const_string in )
				{
					if( in == "*" )
						m_kind  = SFK_ALL;
					else if( first_char( in ) == '*' && last_char( in ) == '*' ) {
						m_kind  = SFK_SUBSTR;
						m_value = in.substr( 1, in.size()-1 );
					}
					else if( first_char( in ) == '*' ) {
						m_kind  = SFK_TRAILING;
						m_value = in.substr( 1 );
					}
					else if( last_char( in ) == '*' ) {
						m_kind  = SFK_LEADING;
						m_value = in.substr( 0, in.size()-1 );
					}
					else {
						m_kind  = SFK_MATCH;
						m_value = in;
					}
				};

				bool            pass( test_unit const& tu ) const
				{
					const_string name( tu.p_name );

					switch( m_kind ) {
					default:
					case SFK_ALL:
						return true;

					case SFK_LEADING:
						return name.substr( 0, m_value.size() ) == m_value;

					case SFK_TRAILING:
						return name.size() >= m_value.size() && name.substr( name.size() - m_value.size() ) == m_value;

					case SFK_SUBSTR:
						return name.find( m_value ) != const_string::npos;

					case SFK_MATCH:
						return m_value == tu.p_name.get();
					}
				}
				enum kind { SFK_ALL, SFK_LEADING, SFK_TRAILING, SFK_SUBSTR, SFK_MATCH };

				kind            m_kind;
				const_string    m_value;
			};
			// Constructor
		#ifndef BOOST_TEST_SUPPORT_RUN_BY_NAME
			explicit        test_case_filter( const_string ) : m_depth( 0 ) {}
		#else
			explicit        test_case_filter( const_string tc_to_run )
			: m_depth( 0 )
			{
				string_token_iterator tit( tc_to_run, (dropped_delimeters = "/", kept_delimeters = dt_none) );

				while( tit != string_token_iterator() ) {
					m_filters.push_back(
						std::vector<single_filter>( string_token_iterator( *tit, (dropped_delimeters = ",", kept_delimeters = dt_none)  ),
													string_token_iterator() ) );

					++tit;
				}
			}
		#endif

			virtual ~test_case_filter() {
			}

			void            filter_unit( test_unit const& tu )
			{
				if( (++m_depth - 1) > m_filters.size() ) {
					tu.p_enabled.value = true;
					return;
				}

				if( m_depth == 1 )
					return;

				std::vector<single_filter> const& filters = m_filters[m_depth-2];

				tu.p_enabled.value =
					std::find_if( filters.begin(), filters.end(), bind( &single_filter::pass, _1, boost::ref(tu) ) ) != filters.end();
			}

			// test tree visitor interface
			virtual void    visit( test_case const& tc )
			{
				if( m_depth < m_filters.size() ) {
					tc.p_enabled.value = false;
					return;
				}

				filter_unit( tc );

				--m_depth;
			}

			virtual bool    test_suite_start( test_suite const& ts )
			{
				filter_unit( ts );

				if( !ts.p_enabled )
					--m_depth;

				return ts.p_enabled;
			}

			virtual void    test_suite_finish( test_suite const& )  { --m_depth; }

		protected:
			// Data members
			std::vector<std::vector<single_filter> >    m_filters;
			unsigned                                    m_depth;
		};

	}

}

#endif /* TEST_CASE_FILTER_H_ */
