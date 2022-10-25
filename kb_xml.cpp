
#include "kb_xml.h"
#include <sstream>


using namespace xercesc;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	save as xml
int kb::xml_xercesc::save(std::string& path)
{
	if (m_doc == NULL)
		return -1;


	XMLCh* str1 = xercesc::XMLString::transcode("LS");
	xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(str1);
	xercesc::XMLString::release(&str1);

	//	output destination object
	xercesc::DOMLSOutput       *output = ((xercesc::DOMImplementationLS*)impl)->createLSOutput();

	xercesc::XMLFormatTarget *myFormTarget = new xercesc::LocalFileFormatTarget(path.c_str());
	output->setByteStream(myFormTarget);

	//
	{
		xercesc::DOMLSSerializer   *serializer = ((xercesc::DOMImplementationLS*)impl)->createLSSerializer();
		serializer->write(m_doc, output);
		serializer->release();
	}


	delete myFormTarget;
	output->release();

	return 0;
}

//	append an element
xercesc::DOMElement* kb::xml_xercesc::append_if_not_exist(xercesc::DOMElement* e0, const char* tag)
{
	if (m_doc == NULL || e0 == NULL)
		return NULL;

	//
	{
		DOMNode* node2 = e0->getFirstChild();
		while (node2) {
			DOMNode* next2 = node2->getNextSibling();

			int type = node2->getNodeType();
			if (type == DOMNode::ELEMENT_NODE) {
				DOMElement* e1 = static_cast<DOMElement*>(node2);

				char* tag_name = XMLString::transcode(e1->getTagName());
				std::string str1 = tag_name;
				std::string str2 = tag;
				if (str1 == str2) {
					return e1;
				}
			}

			node2 = next2;
		}
	}

	//	append
	{
		XMLCh* xstr1 = xercesc::XMLString::transcode(tag);

		xercesc::DOMElement*   e1 = m_doc->createElement(xstr1);
		e0->appendChild(e1);

		xercesc::XMLString::release(&xstr1);

		return e1;
	}
}

xercesc::DOMElement* kb::xml_xercesc::append_only(xercesc::DOMElement* e0, const char* tag)
{
	if (m_doc == NULL || e0 == NULL)
		return NULL;

	XMLCh* xstr1 = xercesc::XMLString::transcode(tag);

	xercesc::DOMElement*   e1 = m_doc->createElement(xstr1);
	e0->appendChild(e1);

	xercesc::XMLString::release(&xstr1);

	return e1;
}


//	append an element
xercesc::DOMElement* kb::xml_xercesc::append_if_not_exist(xercesc::DOMElement* e0, const char* tag, const char* value)
{
	if (m_doc == NULL || e0 == NULL)
		return NULL;

	//
	{
		DOMNode* node2 = e0->getFirstChild();
		while (node2) {
			DOMNode* next2 = node2->getNextSibling();

			int type = node2->getNodeType();
			if (type == DOMNode::ELEMENT_NODE) {
				DOMElement* e1 = static_cast<DOMElement*>(node2);

				char* tag_name = XMLString::transcode(e1->getTagName());
				std::string str1 = tag_name;
				std::string str2 = tag;
				if (str1 == str2) {
					XMLCh* str3 = xercesc::XMLString::transcode(value);
					e1->setTextContent(str3);
					xercesc::XMLString::release(&str3);

					return e1;
				}
			}

			node2 = next2;
		}
	}


	//	append
	{
		XMLCh* xstr1 = xercesc::XMLString::transcode(tag);
		XMLCh* xstr2 = xercesc::XMLString::transcode(value);

		xercesc::DOMElement*   e1 = m_doc->createElement(xstr1);
		e0->appendChild(e1);

		xercesc::DOMText*       textNode = m_doc->createTextNode(xstr2);
		e1->appendChild(textNode);

		xercesc::XMLString::release(&xstr1);
		xercesc::XMLString::release(&xstr2);

		return e1;
	}
}

xercesc::DOMElement* kb::xml_xercesc::append_only(xercesc::DOMElement* e0, const char* tag, const char* value)
{
	if (m_doc == NULL || e0 == NULL)
		return NULL;


	//	append
	{
		XMLCh* xstr1 = xercesc::XMLString::transcode(tag);
		XMLCh* xstr2 = xercesc::XMLString::transcode(value);

		xercesc::DOMElement* e1 = m_doc->createElement(xstr1);
		e0->appendChild(e1);

		xercesc::DOMText* textNode = m_doc->createTextNode(xstr2);
		e1->appendChild(textNode);

		xercesc::XMLString::release(&xstr1);
		xercesc::XMLString::release(&xstr2);

		return e1;
	}
}


//	set attribute
int kb::xml_xercesc::setAttribute(xercesc::DOMElement* e0, const char* name, const char* value)
{
	if (e0 == NULL)
		return -1;

	XMLCh* xstr1 = xercesc::XMLString::transcode(name);
	XMLCh* xstr2 = xercesc::XMLString::transcode(value);

	e0->setAttribute(xstr1, xstr2);

	xercesc::XMLString::release(&xstr1);
	xercesc::XMLString::release(&xstr2);

	return 0;
}
int kb::xml_xercesc::setAttribute(xercesc::DOMElement* e0, const char* name, int value)
{
	char buf[128];
	snprintf(buf, 128, "%d", value);


	return setAttribute(e0, name, buf);
}

int kb::xml_xercesc::setAttribute(xercesc::DOMElement* e0, const char* name, double value)
{
	char buf[128];
	snprintf(buf, 128, "%f", value);


	return setAttribute(e0, name, buf);
}


//	get attribute
int kb::xml_xercesc::getAttribute(xercesc::DOMElement* e0, const char* name, std::string& value)
{
	std::string str0 = name;

	int rtn = -1;
	if (e0->hasAttributes() == false)
		return -1;

	DOMNamedNodeMap* pNamedNodeMap = e0->getAttributes();
	for (int i = 0; i < pNamedNodeMap->getLength(); i++) {
		DOMNode* pAttributeNode = pNamedNodeMap->item(i);
		char* str1 = XMLString::transcode(pAttributeNode->getNodeName());

		std::string str1a = str1;
		if (str1a == str0) {
			char* str2 = XMLString::transcode(pAttributeNode->getTextContent());

			value = str2;

			xercesc::XMLString::release(&str2);

			rtn = 0;
		}

		xercesc::XMLString::release(&str1);

		if (rtn == 0)
			break;
	}

	return rtn;
}

int kb::xml_xercesc::getAttributeAll(xercesc::DOMElement* e0, std::vector<std::string>& v_name, std::vector<std::string>& v_value)
{
	if (e0->hasAttributes() == false)
		return -1;

	DOMNamedNodeMap* pNamedNodeMap = e0->getAttributes();
	for (int i = 0; i < pNamedNodeMap->getLength(); i++) {
		DOMNode* pAttributeNode = pNamedNodeMap->item(i);
		char* str1 = XMLString::transcode(pAttributeNode->getNodeName());
		char* str2 = XMLString::transcode(pAttributeNode->getTextContent());

		std::cout << str1 << std::endl;
		std::cout << str2 << std::endl;

		v_name.push_back(str1);
		v_value.push_back(str2);

		xercesc::XMLString::release(&str2);
		xercesc::XMLString::release(&str1);
	}

	return v_name.size();
}

int kb::getTagText(xercesc::DOMNode* node1, std::string& tag, std::string& text)
{
	int type = node1->getNodeType();
	if (type != DOMNode::ELEMENT_NODE)
		return -1;

	DOMElement* e0 = static_cast<DOMElement*>(node1);

	{
		char* tag_name = XMLString::transcode(e0->getTagName());
		tag = tag_name;
		xercesc::XMLString::release(&tag_name);
	}
	{
		char* str1 = XMLString::transcode(e0->getTextContent());
		text = str1;
		xercesc::XMLString::release(&str1);
	}
	return 0;
}

//	get tag
int kb::xml_xercesc::getTag(xercesc::DOMElement* e0, std::string& tag)
{
	return kb::getTag(e0, tag);
}

//	get element text
int kb::xml_xercesc::getText(xercesc::DOMElement* e0, std::string& tag)
{
	char* str1 = XMLString::transcode(e0->getTextContent());
	tag = str1;
	xercesc::XMLString::release(&str1);

	return 0;
}

int kb::xml_xercesc::getText(xercesc::DOMNode* node1, std::string& text)
{
	int type = node1->getNodeType();
	if (type != DOMNode::ELEMENT_NODE)
		return -1;
	DOMElement* e0 = static_cast<DOMElement*>(node1);

	char* str1 = XMLString::transcode(e0->getTextContent());
	text = str1;
	xercesc::XMLString::release(&str1);

	return 0;
}


//	set element text
int kb::xml_xercesc::setText(xercesc::DOMElement* e0, const char* text)
{
	XMLCh* str1 = xercesc::XMLString::transcode(text);

	e0->setTextContent(str1);
	xercesc::XMLString::release(&str1);

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xercesc::DOMElement* kb::search_child_tag(xercesc::DOMNode* node1, const char* tag)
{
	DOMNode* node2 = node1->getFirstChild();
	while (node2) {
		DOMNode* next2 = node2->getNextSibling();

		int type = node2->getNodeType();
		if (type == DOMNode::ELEMENT_NODE) {
			DOMElement* e1 = static_cast<DOMElement*>(node2);

			char* tag_name = XMLString::transcode(e1->getTagName());
			std::string str1 = tag_name;
			std::string str2 = tag;
			if (str1 == str2) {
				return e1;
			}
		}

		node2 = next2;
	}
	return NULL;
}


int kb::cout_attributeAll(xercesc::DOMNode* node1)
{
	int type = node1->getNodeType();
	if (type != DOMNode::ELEMENT_NODE)
		return -1;

	DOMElement* element1 = static_cast<DOMElement*>(node1);
	if (element1->hasAttributes() == false)
		return -1;

	DOMNamedNodeMap* pNamedNodeMap = element1->getAttributes();
	for (int i = 0; i < pNamedNodeMap->getLength(); i++) {
		DOMNode* pAttributeNode = pNamedNodeMap->item(i);

		char* str1 = XMLString::transcode(pAttributeNode->getNodeName());
		char* str2 = XMLString::transcode(pAttributeNode->getTextContent());

		//std::string str2;
		//kb::trim(pAttributeNode->getTextContent(), str2);

		std::cout <<"\t"<< str1 << ": " << str2 << std::endl;

		xercesc::XMLString::release(&str1);
		xercesc::XMLString::release(&str2);
	}

	return 0;
}

int kb::getTag(xercesc::DOMElement* element1, std::string& tag)
{
	char* tag_name = XMLString::transcode(element1->getTagName());

	tag = tag_name;

	xercesc::XMLString::release(&tag_name);


	return 0;
}

int kb::getTag(xercesc::DOMNode* node1, std::string& tag)
{
	int type = node1->getNodeType();
	if (type != DOMNode::ELEMENT_NODE)
		return -1;

	DOMElement* element1 = static_cast<DOMElement*>(node1);
	{
		char* tag_name = XMLString::transcode(element1->getTagName());
		tag = tag_name;
		xercesc::XMLString::release(&tag_name);

	}
	return 0;
}

int kb::getAttribute(DOMNode* node1, std::vector<std::string>& v_attribute)
{
	int type = node1->getNodeType();
	if (type == DOMNode::ELEMENT_NODE) {

		DOMElement* element1 = static_cast<DOMElement*>(node1);

		if (element1->hasAttributes()) {
			DOMNamedNodeMap* pNamedNodeMap = element1->getAttributes();
			for (int i = 0; i < pNamedNodeMap->getLength(); i++) {
				DOMNode* pAttributeNode = pNamedNodeMap->item(i);
				char* str1 = XMLString::transcode(pAttributeNode->getNodeName());
				char* str2 = XMLString::transcode(pAttributeNode->getTextContent());

				std::string sAttNode = str1;
				std::string sAttValue = str2;

				v_attribute.push_back(sAttNode);
				v_attribute.push_back(sAttValue);

				xercesc::XMLString::release(&str1);
				xercesc::XMLString::release(&str2);
			}
		}
	}
	return v_attribute.size() / 2;
}


int kb::trim(const XMLCh* buf, std::string& a)
{
	int len1 = XMLString::stringLen(buf);

	XMLCh* buf2 = new XMLCh[len1 + 1];
	XMLString::copyString(buf2, buf);
	XMLString::trim(buf2);

	char* tag_name = XMLString::transcode(buf2);
	a = tag_name;

	delete[] buf2;
	XMLString::release(&tag_name);
	return 0;
}

int kb::getText(DOMNode* node1, std::string& tag)
{
	if (node1 == NULL)
		return -1;

	int type = node1->getNodeType();
	if (type == DOMNode::ELEMENT_NODE) {

		DOMElement* element1 = static_cast<DOMElement*>(node1);
		char* tag_name = XMLString::transcode(element1->getTagName());

		tag = tag_name;

		XMLString::release(&tag_name);
	}
	else if (type == DOMNode::TEXT_NODE) {
		DOMText* text = static_cast<DOMText*>(node1);

		
		int len1 = XMLString::stringLen(text->getData());
		//std::cout <<"    "<< len1 << std::endl;

		XMLCh* buffer = new XMLCh[len1 + 1];
		{
			XMLString::copyString(buffer, text->getData());
			XMLString::trim(buffer);
		}

		char* tag_name = XMLString::transcode(buffer);
		std::string str1 = tag_name;

		delete[] buffer;
		XMLString::release(&tag_name);
		

		if (str1.length() > 0) {
			tag = str1;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
	return 0;
}

