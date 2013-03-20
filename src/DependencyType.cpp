/*
 * DependencyType.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "DependencyType.h"

#include <sstream>


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

	//----------------------------####################-------------------------

	std::string DependencyEntry::print() const {
		std::stringstream stream;
		stream << targetFilter << " -> " << dependencyFilter;
		if (containsFileInfo()) {
			stream << " in " << file << ":" << line;
		}
		return stream.str();
	}

	std::string DependencyEntry::printPlace() const {
		std::stringstream stream;
		if (file.size()>0) {
			stream << file << ":" << line;
		}
		return stream.str();
	}

	bool DependencyEntry::containsFileInfo() const {
		if (file.size()>0) {
			return true;
		}
		return false;
	}

	std::string print(const DependencyIdSet& set) {
		std::stringstream stream;
		if (set.size()>0) {
			DependencyIdSet::iterator iter = set.begin();
			DependencyIdSet::iterator eiter = set.end();
			for(; iter!=eiter; iter++) {
				stream << *iter << ", ";
			}
		} else {
			stream << "empty";
		}
		return stream.str();
	}

}
