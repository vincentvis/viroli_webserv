#include "config/Param.hpp"

int Param::_depth = 0;

Param::Param() {
}

Param::Param(std::string name) {
	_name = name;
}

std::string Param::getName() const {
	return (_name);
}

void Param::setName(std::string name) {
	_name = name;
}

void Param::addValue(std::string value) {
	_values.push_back(value);
}

std::vector<std::string>::size_type Param::getNumValues() const {
	return (_values.size());
}

void Param::setChildren(std::map<std::string, std::vector<Param> > children) {
	_children = children;
}

std::string Param::getNthValue(int n) const {
	return _values.at(n);
}

std::vector<std::string> Param::getValues() const {
	return _values;
}

std::map<std::string, std::vector<Param> > Param::getChildren() const {
	return _children;
}

int32_t Param::convertToInt32(std::string source) {
	char *endptr    = NULL;
	errno           = 0;
	long    longval = strtol(source.c_str(), &endptr, 0);
	int32_t intval  = longval;
	if (errno == EINVAL || errno == ERANGE || source.c_str() == endptr ||
		*endptr != '\0' || intval != longval)
	{
		return (-1);
	}
	return (intval);
}

std::pair<std::string, uint16_t> Param::convertToHostAndPort(std::string source) {
	std::string::size_type colon_position = source.find_first_of(":");
	if (colon_position == std::string::npos) {
		return (std::make_pair("failure", -1));
	}
	std::string host = source.substr(0, colon_position);
	errno            = 0;
	int      intval  = convertToInt32(source.substr(colon_position + 1));
	uint16_t port    = (uint16_t)intval;

	if (intval == -1 || port != intval || port > 65535) {
		return (std::make_pair("failure", -1));
	}
	return (std::make_pair(host, port));
}

std::ostream &operator<<(std::ostream &os, const Param &param) {
	os << std::setw(Param::getDepth() * 4) << ""
	   << "{" << std::endl;
	Param::incrementDepth();
	os << std::setw(Param::getDepth() * 4) << ""
	   << "name: \"" << param._name << "\"," << std::endl;
	os << std::setw(Param::getDepth() * 4) << ""
	   << "values: [";
	for (std::size_t i = 0; i < param._values.size(); i++) {
		os << "'" << param._values[i] << "'";
		if (i != param._values.size() - 1)
			os << ", ";
	}
	os << "]," << std::endl;
	if (param._children.size() > 0) {
		os << std::setw(Param::getDepth() * 4) << ""
		   << "children [" << std::endl;
		Param::incrementDepth();
		param.printMap(param._children);
		Param::decrementDepth();
		os << std::setw(Param::getDepth() * 4) << ""
		   << "]," << std::endl;
	}
	Param::decrementDepth();
	os << std::setw(Param::getDepth() * 4) << ""
	   << "}," << std::endl;
	return os;
}

void Param::printVectorOfMaps(
	std::vector<std::map<std::string, std::vector<Param> > > maps
) {
	setDepth(0);
	std::vector<std::map<std::string, std::vector<Param> > >::iterator elem =
		maps.begin();
	std::vector<std::map<std::string, std::vector<Param> > >::iterator end = maps.end();

	while (elem != end) {
		std::cout << "server [" << std::endl;
		incrementDepth();
		printMap(*elem);
		decrementDepth();
		std::cout << "]," << std::endl;
		elem++;
	}
}

void Param::printMap(std::map<std::string, std::vector<Param> > currentMap) const {
	std::map<std::string, std::vector<Param> >::iterator elem = currentMap.begin();
	std::map<std::string, std::vector<Param> >::iterator end  = currentMap.end();

	while (elem != end) {
		std::vector<Param>::iterator directive    = elem->second.begin();
		std::vector<Param>::iterator directiveEnd = elem->second.end();
		while (directive != directiveEnd) {
			std::cout << *directive;
			directive++;
		}
		elem++;
	}
}
