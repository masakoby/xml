
#include "kb_slide_annotation.h"
#include "kb_xml.h"
#include "kb_slide_info.h"

#pragma once


namespace kb
{
	//	slide file name: ***.ndpi 
	//	annotation file name: ***.ndpi.ndpa

	int load_ndpa(
		std::string& path,
		std::vector<kb::slide_annotation>& v_ndpa,
		kb::slide_info& info
	);

	//	subroutine called in load_ndpa()
	void recursive_load_ndpa(
		xercesc::DOMNode* node,
		std::vector<kb::slide_annotation>& v_ndpa,
		int mode_plist);

	int save_as_ndpa(
		std::string& path,
		std::vector<kb::slide_annotation>& v_ndpa,
		kb::slide_info& info
	);

};
