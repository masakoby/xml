#include <string>
#include <vector>
#include <iostream>
#include <fstream>

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
	//	xml xercescを使うためのセッションオブジェクト

	class session_xercesc
	{
	public:
		session_xercesc()
		{
			init();
		}
		virtual ~session_xercesc()
		{
			xercesc::XMLPlatformUtils::Terminate();
		}

		inline void init()
		{
			// Initialize the XML4C2 system
			try
			{
				xercesc::XMLPlatformUtils::Initialize();
			}
			catch (const xercesc::XMLException& toCatch)
			{
				std::cerr << "Error during Xerces-c Initialization.\n"
					<< "  Exception message:"
					<< toCatch.getMessage() << std::endl;
			}
		}
	};



	class xml_xercesc
	{
	public:
		xml_xercesc()
		{
			init();
		}

		virtual ~xml_xercesc()
		{
			clear();
		}

		inline void init()
		{
			m_impl = NULL;
			m_doc = NULL;
			m_parser = NULL;
		}

		inline void clear()
		{
			if (m_parser != NULL) {
				delete m_parser;
				m_doc = NULL;
			}
			if (m_doc != NULL) {
				m_doc->release();
			}
			init();
		}

		//	create new XML object
		inline int create(const char* features, const char* name_root)
		{
			if (m_impl != NULL || m_doc != NULL)
				return -1;

			XMLCh* str1 = xercesc::XMLString::transcode(features);
			XMLCh* str2 = xercesc::XMLString::transcode(name_root);

			m_impl = xercesc::DOMImplementationRegistry::getDOMImplementation(str1);

			//	root
			m_doc = m_impl->createDocument(0, str2, 0);
			//DOMElement*   root = doc->getDocumentElement();

			xercesc::XMLString::release(&str1);
			xercesc::XMLString::release(&str2);

			return 0;
		}

		//	load XML file
		inline int create(std::string& path_xml)
		{
			{
				std::ifstream file(path_xml);
				if (file.is_open() == false)
					return -1;
				file.close();
			}
			if (m_impl != NULL || m_doc != NULL || m_parser != NULL)
				return -1;
			m_parser = new xercesc::XercesDOMParser;

			m_parser->parse(path_xml.c_str());

			m_doc = m_parser->getDocument();

			return 0;
		}

		//	save as xml
		int save(std::string& path);

		inline xercesc::DOMElement*   getRoot()
		{
			if (m_doc == NULL)
				return NULL;

			return m_doc->getDocumentElement();
		}

		//	append an element
		xercesc::DOMElement* append_if_not_exist(xercesc::DOMElement* e0, const char* tag);
		xercesc::DOMElement* append_only(xercesc::DOMElement* e0, const char* tag);

		//	append an element
		xercesc::DOMElement* append_if_not_exist(xercesc::DOMElement* e0, const char* tag, const char* value);
		xercesc::DOMElement* append_only(xercesc::DOMElement* e0, const char* tag, const char* value);


		//	set attribute
		int setAttribute(xercesc::DOMElement* e0, const char* name, const char* value);
		int setAttribute(xercesc::DOMElement* e0, const char* name, int value);
		int setAttribute(xercesc::DOMElement* e0, const char* name, double value);

		//	get attribute
		int getAttribute(xercesc::DOMElement* e0, const char* name, std::string& value);
		//	get all attributes
		int getAttributeAll(xercesc::DOMElement* e0, std::vector<std::string>& v_name, std::vector<std::string>& v_value);


		//	get tag
		int getTag(xercesc::DOMElement* e0, std::string& tag);

		//	get element text
		int getText(xercesc::DOMElement* e0, std::string& text);
		int getText(xercesc::DOMNode* node1, std::string& text);
		//	set element text
		int setText(xercesc::DOMElement* e0, const char* text);



	public:
		xercesc::DOMImplementation * m_impl;
		xercesc::DOMDocument*   m_doc;
		xercesc::XercesDOMParser *m_parser;

	};

	//	XML operation
	int getTag(xercesc::DOMNode* node1, std::string& tag);
	int getTag(xercesc::DOMElement* node1, std::string& tag);
	int getText(xercesc::DOMNode* node1, std::string& tag);
	int getAttribute(xercesc::DOMNode* node1, std::vector<std::string>& v_attribute);

	int getTagText(xercesc::DOMNode* node1, std::string& tag, std::string& text);

	int trim(const XMLCh* buf, std::string& a);

	int cout_attributeAll(xercesc::DOMNode* node1);

	xercesc::DOMElement* search_child_tag(xercesc::DOMNode* node1, const char* tag);

};
