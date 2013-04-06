/*
 * DependencyLog.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: bob
 */

#include "DependencyLog.h"

namespace dependency {

	namespace log {

		LogState::LogState() : state(true) {
		}

		bool LogState::getState() const {
			return state;
		}

		/**
		 * Return old state value.
		 */
		bool LogState::setState(bool newState) {
			bool oldState = state;
			state = newState;
			return oldState;
		}

		//--------------------------------------

		LogState& get_log_state() {
			static LogState state;
			return state;
		}

		bool get_log_state_value() {
			return get_log_state().getState();
		}

		bool set_log_state(bool state) {
			return get_log_state().setState(state);
		}

		//---------------------------------------

		LogDisableInScope::LogDisableInScope() : oldState( get_log_state().getState() ) {
			DEPENDENCY_LOG_DISABLE;
		}

		LogDisableInScope::~LogDisableInScope() {
			DEPENDENCY_LOG_SET_STATE(oldState);
		}

	}

} /* namespace dependency */
