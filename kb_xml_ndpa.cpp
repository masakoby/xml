#include "kb_xml_ndpa.h"


int kb::load_ndpa(
	std::string& path_xml,
	std::vector<kb::slide_annotation>& v_an,
	kb::slide_info& info
)
{
	{
		std::ifstream file(path_xml);
		if (file.is_open() == false)
			return -1;
	}
	//	xml session start
	kb::session_xercesc session;

	kb::xml_xercesc xml;
	xml.create(path_xml);
	xercesc::DOMElement* root = xml.getRoot();


	//	浜ホト用アノテーションファイル読み込み
	kb::recursive_load_ndpa(root, v_an, 0);


	//	浜フォトに合わせて、アノテーションファイルはナノメーター基準とする。
	//	アペリオアノテーションファイルは、解像度情報は入っているが、
	//	画像サイズ情報が掛かれていない。
	//	浜フォト仕様の物理座標計算を行うためには、画像サイズがわからないと計算できないため、
	//	ピクセル単位で格納した後、
	//	この場所でスライド画像が読み込まれた段階で変換する。

	int n_annotation = v_an.size();
	for (int ia = 0; ia < n_annotation; ia++) {
		int num_pt = v_an[ia].m_pointlist_i.size() / 2;
		for (int ii = 0; ii < num_pt; ii++) {
			double x1 = (double)(v_an[ia].m_pointlist_i[ii * 2 + 0]) * 0.001;
			double y1 = (double)(v_an[ia].m_pointlist_i[ii * 2 + 1]) * 0.001;

			int x2, y2;
			info.micrometer2pixel(x1, y1, x2, y2);

			v_an[ia].m_pointlist_i[ii * 2 + 0] = x2;
			v_an[ia].m_pointlist_i[ii * 2 + 1] = y2;
		}
	}

	return 0;
}

void kb::recursive_load_ndpa(
	xercesc::DOMNode* node,
	std::vector<kb::slide_annotation>& v_ndpa,
	int mode_plist)
{
	{
		std::string tag1, tag2;
		if (kb::getText(node, tag1) == 0) {
			xercesc::DOMNode* node2 = node->getFirstChild();
			if (kb::getText(node2, tag2) == 0) {
				//if (tag1 == "title") {
				//	std::cout << tag1 << ": " << tag2 << std::endl;
				//}
			}
		}

		std::vector<std::string> v_attribute;
		if (kb::getAttribute(node, v_attribute) > 0) {
			//int n = v_attribute.size() / 2;
			//for (int k = 0; k < n; k++) {
			//	std::cout << "    " << v_attribute[k * 2] << ": " << v_attribute[k * 2 + 1] << std::endl;
			//}
		}

		if (tag1 == "ndpviewstate") {
			v_ndpa.push_back(kb::slide_annotation());

			int num_v = v_ndpa.size();
			int n = v_attribute.size() / 2;
			for (int k = 0; k < n; k++) {
				if (v_attribute[k * 2] == "id") {
					v_ndpa[num_v - 1].m_id = std::stoi(v_attribute[k * 2 + 1]);
				}
			}
		}
		if (tag1 == "annotation") {
			mode_plist = 1;
		}


		int num_v = v_ndpa.size();
		if (num_v > 0) {

			if (tag1 == "annotation") {
				int n = v_attribute.size() / 2;
				for (int k = 0; k < n; k++) {
					if (v_attribute[k * 2] == "color") {
						//std::cout << v_attribute[k * 2 + 1] << std::endl;
						//v_ndpa[num_v - 1].m_annotation_color = v_attribute[k * 2 + 1];
						std::string str1=v_attribute[k * 2 + 1];
						int len1 = str1.length();
						if (len1 > 6) {

							std::string str2 = str1.substr(1, len1 - 1);

							int val = std::stoi(str2, NULL, 16);

							int v0 = 255;
							int v1 = val >> 16;
							int v2m = val >> 8;
							int v2 = v2m & v0;
							int v3 = val & v0;

							v_ndpa[num_v - 1].m_color[0] = v3;
							v_ndpa[num_v - 1].m_color[1] = v2;
							v_ndpa[num_v - 1].m_color[2] = v1;

						}

					}
					else if (v_attribute[k * 2] == "displayname") {
						v_ndpa[num_v - 1].m_annotation_displayname = v_attribute[k * 2 + 1];
					}
					else if (v_attribute[k * 2] == "type") {
						v_ndpa[num_v - 1].m_annotation_type = v_attribute[k * 2 + 1];
					}

				}
			}
			else if (tag1 == "title") {
				v_ndpa[num_v - 1].m_title = tag2;
			}
			else if (tag1 == "details") {
				v_ndpa[num_v - 1].m_details = tag2;
			}
			else if (tag1 == "lens") {
				v_ndpa[num_v - 1].m_lens = tag2;
			}
			else if (tag1 == "coordformat") {
				v_ndpa[num_v - 1].m_coordformat = tag2;
			}
			else if (tag1 == "showtitle") {
				v_ndpa[num_v - 1].m_showtitle = tag2;
			}
			else if (tag1 == "showhistogram") {
				v_ndpa[num_v - 1].m_showhistogram = tag2;
			}
			else if (tag1 == "showlineprofile") {
				v_ndpa[num_v - 1].m_showlineprofile = tag2;
			}


			if (mode_plist == 1) {
				if (tag1 == "x" || tag1 == "y") {
					v_ndpa[num_v - 1].m_pointlist_i.push_back(std::stoi(tag2));
					//std::cout << tag2 << std::endl;
				}
				else if (tag1 == "measuretype") {
					v_ndpa[num_v - 1].m_annotation_measuretype = tag2;
				}
				else if (tag1 == "radius") {
					v_ndpa[num_v - 1].m_annotation_radius = tag2;
				}
				else if (tag1 == "specialtype") {
					v_ndpa[num_v - 1].m_annotation_specialtype = tag2;
				}
				else if (tag1 == "closed") {
					v_ndpa[num_v - 1].m_annotation_closed = tag2;
				}
			}
			else {
				if (tag1 == "x") {
					v_ndpa[num_v - 1].m_x = tag2;
				}
				else if (tag1 == "y") {
					v_ndpa[num_v - 1].m_y = tag2;
				}
				else if (tag1 == "z") {
					v_ndpa[num_v - 1].m_z = tag2;
				}

			}
		}
	}

	xercesc::DOMNode* child = node->getFirstChild();
	while (child) {
		xercesc::DOMNode* next = child->getNextSibling();
		kb::recursive_load_ndpa(child, v_ndpa, mode_plist);

		child = next;
	}
}

int kb::save_as_ndpa(
	std::string& path,
	std::vector<kb::slide_annotation>& v_ndpa,
	kb::slide_info& info
)
{
	//	xml session start
	kb::session_xercesc session;


	int num_a = v_ndpa.size();
	std::cout << "save_as_ndpa: "<<num_a << std::endl;
	//	xml object
	kb::xml_xercesc xml;

	xml.create("LS", "annotations");
	xercesc::DOMElement* root = xml.getRoot();

	for (int k = 0; k < num_a; k++) {
		//	annotation unit
		xercesc::DOMElement* e1 = xml.append_only(root, "ndpviewstate");
		{
			char buf[64];
			sprintf_s(buf, "%d", v_ndpa[k].m_id);
			xml.setAttribute(e1, "id", buf);
		}

		//	title
		xml.append_only(e1, "title", v_ndpa[k].m_title.c_str());

		//	details
		xml.append_only(e1, "details", v_ndpa[k].m_details.c_str());

		//	coordformat
		xml.append_only(e1, "coordformat", v_ndpa[k].m_coordformat.c_str());

		//	lens
		xml.append_only(e1, "lens", v_ndpa[k].m_lens.c_str());

		//	x,y,z
		xml.append_only(e1, "x", v_ndpa[k].m_x.c_str());
		xml.append_only(e1, "y", v_ndpa[k].m_y.c_str());
		xml.append_only(e1, "z", v_ndpa[k].m_z.c_str());

		//	showtitle
		xml.append_only(e1, "showtitle", v_ndpa[k].m_showtitle.c_str());

		//	showhistogram
		//xml.append_only(e1, "showhistogram", v_ndpa[k].m_showhistogram.c_str());

		//	showlineprofile
		//xml.append_only(e1, "showlineprofile", v_ndpa[k].m_showlineprofile.c_str());

		//	annotation
		{
			xercesc::DOMElement* e2 = xml.append_only(e1, "annotation");
			xml.setAttribute(e2, "type", v_ndpa[k].m_annotation_type.c_str());
			xml.setAttribute(e2, "displayname", v_ndpa[k].m_annotation_displayname.c_str());
			//xml.setAttribute(e2, "color", v_ndpa[k].m_annotation_color.c_str());
			{
				char buf[64];
				sprintf_s(buf, "#%02x%02x%02x", v_ndpa[k].m_color[0], v_ndpa[k].m_color[1], v_ndpa[k].m_color[2]);
				xml.setAttribute(e2, "color", buf);
			}
			if (v_ndpa[k].m_annotation_type == "circle") {
				//	measuretype
				//xml.append_only(e2, "measuretype", v_ndpa[k].m_annotation_measuretype.c_str());

				//nt num_pt = v_ndpa[k].m_pointlist.size() / 2;
				//if (num_pt > 0) {
				//	xml.append(e2, "x", v_ndpa[k].m_annotation_pointlist[0].c_str());
				//	xml.append(e2, "y", v_ndpa[k].m_annotation_pointlist[1].c_str());
				//}
				xml.append_only(e2, "radius", v_ndpa[k].m_annotation_radius.c_str());

			}
			else {
				//	measuretype
				xml.append_only(e2, "measuretype", v_ndpa[k].m_annotation_measuretype.c_str());
				//	closed
				xml.append_only(e2, "closed", v_ndpa[k].m_annotation_closed.c_str());

				//	pointlist
				xercesc::DOMElement* e3 = xml.append_only(e2, "pointlist");

				int num_pt = v_ndpa[k].m_pointlist_i.size() / 2;

				std::cout << "pointlist: " << num_pt << std::endl;

				for (int i = 0; i < num_pt; i++) {
					xercesc::DOMElement* e4 = xml.append_only(e3, "point");

					int x = v_ndpa[k].m_pointlist_i[i * 2];
					int y = v_ndpa[k].m_pointlist_i[i * 2+1];

					double x2, y2;
					info.pixel2micrometer(x, y, x2, y2);

					char buf[128];
					sprintf_s(buf, "%.0f", x2 * 1000.0);
					xml.append_only(e4, "x", buf);

					sprintf_s(buf, "%.0f", y2 * 1000.0);
					xml.append_only(e4, "y", buf);
				}

				//	specialtype
				//xml.append(e1, "specialtype", v_ndpa[k].m_annotation_specialtype.c_str());
			}
		}
	}

	xml.save(path);

	return 0;
}
