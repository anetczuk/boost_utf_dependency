/*
 * DependencyType.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: Arkadiusz Netczuk
 */

#include "DependencyType.h"

#include <sstream>


namespace dependency {

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

	std::string print(const DependencyIdMap& idMap) {
		std::stringstream str;
		DependencyIdMap::const_iterator iter = idMap.begin();
		DependencyIdMap::const_iterator eiter = idMap.end();
		for(; iter!=eiter; iter++) {
			str << iter->first << ": " << print(iter->second) << std::endl;
		}
		return str.str();
	}

}

/**
 * Operator to handle program option
 */
std::istream& operator>>(std::istream& in, dependency::auto_dependency& unit)
{
	std::string token;
	in >> token;
	if (token == "on")
		unit = dependency::auto_dependency_on;
	else if (token == "off")
		unit = dependency::auto_dependency_off;
	else {
		//in case of unknown value
		unit = dependency::auto_dependency_on;
	}
//    else throw boost::program_options::validation_error("Invalid unit");
	return in;
}

std::ostream& operator<<(std::ostream& out, const dependency::auto_dependency& unit) {
	switch(unit) {
	case dependency::auto_dependency_on: {
		out << "on";
		break;
	}
	case dependency::auto_dependency_off: {
		out << "off";
		break;
	}
	default: {
//		out << "unknown";
		out << "on";
		break;
	}
	}
	return out;
}
