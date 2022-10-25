#include <string>
#include <iostream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

//
//	link file: xerces-c_3.lib
//
//	XMLライブラリを使用するときに、最初に下記のような関数コールを行う
//	kb::session_xercesc session;
//

#pragma once

namespace kb
{
	//
	int write_xml(const char* path_xml, const char* group1, const char* tag1, const char* value1);
	int write_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, const std::string& value1);
	int write_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, float value);
	int write_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double value);
	int write_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, int value);

	int write_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double* values, int num);
	int write_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, float* values, int num);
	int write_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, int* values, int num);
	int write_space_xml(const char* path_xml, const char* group1, const char* tag1, float* value1, int num);


	int read_xml(const char* path_xml, const char* group1, const char* tag1, std::string& value1);
	int read_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, std::string& value1);
	int read_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, int& value1);
	int read_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, float& value1);
	int read_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double& value1);

	int read_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, int* value1, int num);
	int read_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, float* value1, int num);
	int read_space_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double* value1, int num);
	int read_csv_xml(const std::string& path_xml, const std::string& group1, const std::string& tag1, double* value1, int num);
};
