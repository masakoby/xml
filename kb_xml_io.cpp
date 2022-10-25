
#include "kb_xml.h"
#include "kb_xml_io.h"
#include <sstream>


using namespace xercesc;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int kb::write_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, int value)
{
	char buf[256];
	sprintf_s(buf, "%d", value);

	return kb::write_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), buf);
}

int kb::write_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, float value)
{
	char buf[256];
	sprintf_s(buf, "%f", value);

	return kb::write_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), buf);
}

int kb::write_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double value)
{
	char buf[256];
	sprintf_s(buf, "%f", value);

	return kb::write_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), buf);
}


int kb::write_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, int* values, int num)
{
	std::string value1;
	for (int i = 0; i < num; i++) {
		char buf[256];
		sprintf_s(buf, "%d ", values[i]);
		value1 += buf;
	}

	return kb::write_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), value1);
}

int kb::write_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, float* values, int num)
{
	std::string value1;
	for (int i = 0; i < num; i++) {
		char buf[256];
		sprintf_s(buf, "%f ", values[i]);
		value1 += buf;
	}

	return kb::write_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), value1);
}

int kb::write_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double* values, int num)
{
	std::string value1;
	for (int i = 0; i < num; i++) {
		char buf[256];
		sprintf_s(buf, "%f ", values[i]);
		value1 += buf;
	}

	return kb::write_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), value1);
}

int kb::write_space_xml(const char* path_xml, const char* group1, const char* tag1, float* values, int num)
{
	std::string value1;
	for (int i = 0; i < num; i++) {
		char buf[256];
		sprintf_s(buf, "%f ", values[i]);
		value1 += buf;
	}

	return kb::write_xml(path_xml, group1, tag1, value1);
}

int kb::write_xml(const char* path_xml, const char* group1, const char* tag1, const char* value1)
{
	std::string path = path_xml;

	//	xml session start
	kb::session_xercesc session;

	//	xml object
	kb::xml_xercesc xml;

	if (xml.create(path) < 0) {
		xml.create("LS", "parameters");

		DOMElement* root = xml.getRoot();
		DOMElement* e1 = xml.append_if_not_exist(root, group1);
		xml.append_if_not_exist(e1, tag1, value1);
	}
	else {
		DOMElement* root = xml.getRoot();
		DOMElement* e1 = xml.append_if_not_exist(root, group1);
		xml.append_if_not_exist(e1, tag1, value1);
	}

	return xml.save(path);
}


int kb::read_xml(const char* path_xml, const char* group1, const char* tag1, std::string& value1)
{
	std::string path = path_xml;

	//	xml session start
	kb::session_xercesc session;

	//	xml object
	kb::xml_xercesc xml;

	if (xml.create(path) < 0)
		return -1;

	DOMElement*   root = xml.getRoot();
	xercesc::DOMElement* e1 = kb::search_child_tag(root, group1);
	if (e1 == NULL)
		return -1;

	xercesc::DOMElement* e2 = kb::search_child_tag(e1, tag1);
	if (e2 == NULL)
		return -1;

	char* str1 = XMLString::transcode(e2->getTextContent());
	value1 = str1;
	xercesc::XMLString::release(&str1);

	return 0;
}

int kb::write_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, const std::string& value1)
{
	return kb::write_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), value1.c_str());
}

int kb::read_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, std::string& value1)
{
	return kb::read_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), value1);
}

int kb::read_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, int& value1)
{
	std::string str1;
	if (kb::read_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), str1) < 0)
		return -1;
	int num=str1.length();
	if (num > 0) {
		value1 = atoi(str1.c_str());
	}
	else {
		return -1;
	}
	return 0;
}

int kb::read_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, float& value1)
{
	std::string str1;
	if (kb::read_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), str1) < 0)
		return -1;
	int num = str1.length();
	if (num > 0) {
		value1 = std::stod(str1);
	}
	else {
		return -1;
	}
	return 0;
}

int kb::read_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double& value1)
{
	std::string str1;
	if (kb::read_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), str1) < 0)
		return -1;
	int num = str1.length();
	if (num > 0) {
		value1 = std::stod(str1);
	}
	else {
		return -1;
	}
	return 0;
}

int kb::read_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, int* oValue, int num)
{
	std::string str1;
	if (kb::read_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), str1) < 0)
		return -1;
	int num_str1 = str1.length();

	if (num_str1 > 0) {
		std::istringstream stream(str1);

		//1行のうち、文字列とコンマを分割する
		for (int k = 0; k < num; k++) {
			std::string token;
			std::getline(stream, token, ' ');
			size_t n1 = token.length();
			if (n1 == 0) {
			}
			else {
				oValue[k] = std::stoi(token);
			}
			if (stream.eof() != false)
				break;
		}
	}
	else {
		return -1;
	}
	return 0;
}

int kb::read_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, float* oValue, int num)
{
	std::string str1;
	if (kb::read_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), str1) < 0)
		return -1;
	int num_str1 = str1.length();

	if (num_str1 > 0) {
		std::istringstream stream(str1);

		//1行のうち、文字列とコンマを分割する
		for (int k = 0; k < num; k++) {
			std::string token;
			std::getline(stream, token, ' ');
			size_t n1 = token.length();
			if (n1 == 0) {
			}
			else {
				oValue[k] = std::stof(token);
			}
			if (stream.eof() != false)
				break;
		}
	}
	else {
		return -1;
	}
	return 0;
}


int kb::read_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double* oValue, int num)
{
	std::string str1;
	if (kb::read_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), str1) < 0)
		return -1;
	int num_str1 = str1.length();

	if (num_str1 > 0) {
		std::istringstream stream(str1);

		//1行のうち、文字列とコンマを分割する
		for (int k = 0; k < num; k++) {
			std::string token;
 			std::getline(stream, token, ' ');
			size_t n1 = token.length();
			if (n1 == 0) {
			}
			else {
				oValue[k] = std::stod(token);
			}
			if (stream.eof() != false)
				break;
		}
	}
	else {
		return -1;
	}
	return 0;
}

int kb::read_csv_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double* oValue, int num)
{
	std::string str1;
	if (kb::read_xml(path_xml.c_str(), group1.c_str(), tag1.c_str(), str1) < 0)
		return -1;
	int num_str1 = str1.length();

	if (num_str1 > 0) {
		std::istringstream stream(str1);

		//1行のうち、文字列とコンマを分割する
		for (int k = 0; k < num; k++) {
			std::string token;
			std::getline(stream, token, ',');
			size_t n1 = token.length();
			if (n1 == 0) {
			}
			else {
				oValue[k] = std::stod(token);
			}
			if (stream.eof() != false)
				break;
		}
	}
	else {
		return -1;
	}
	return 0;
}

