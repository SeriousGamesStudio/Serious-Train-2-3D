#ifndef _SCENE_MAKER_STRING_OPERATIONS_
#define _SCENE_MAKER_STRING_OPERATIONS_

#include <string>
#include <vector>

using string = std::string;

namespace SceneMaker {
	struct KeyValue
	{
		KeyValue(string k, string v) : key(k), value(v) {};
		string key;
		string value;
		string operator()()
		{
			return string("\"" + value + "\"" + " = " + "\"" + key + "\"");
		}
	};
	namespace StringOperations{
		/***********************************************************
		 * Makes a group of Items
		 * params:
		 * - tag: group's tag.
		 * - group: a vector with the items to put into the group
		************************************************************/
		static string makeGroup(string tag, std::vector<string> group)
		{
			string result = "<" + tag + ">\n\t";
			for (string s : group)
				result += s;
			result += "<" + tag + "/>\n";
			return result;
		}
		/***********************************************************
		* Makes a group of Items
		* params:
		* - tag: group's tag.
		* - group: a compacted string with the items to put into the group(must have been already formated)
		************************************************************/
		static string makeGroup(string tag, string group)
		{
			string result = "<" + tag + ">\n\t";
			result += group;
			result += "<" + tag + "/>\n";
			return result;
		}
		/***********************************************************
		* Makes a group of Items
		* params:
		* - tag: group's tag.
		* - group: a vector with the items to put into the group
		************************************************************/
		template<class T> static string makeGroup(string tag, std::vector<T> group)
		{
			std::vector<string> vs;
			for (T g : group)
				vs.push_back(std::to_string(g));
			return makeGroup(tag, vs);
		}

		/***********************************************
		* Makes an Item
		* params:
		* - tag: Item's tag
		* - attrs: attributes with the Item's metadata
		* - data: the data of the item
		************************************************/
		static string makeClosingItem(string tag, std::vector<KeyValue> attrs, string data) 
		{
			string result = "<" + tag + " ";
			for (KeyValue a : attrs)
				result += " " + a();
			result += ">\n";
			result += data;
			result += "<" + tag + "/>\n";
			return result;
		}
		static string makeClosingItem(string tag, string data)
		{
			string result = "<" + tag + " >\n";
			result += data;
			result += "<" + tag + "/>\n";
			return result;
		}

		/****************************************
		* Makes a self-closing Item inline
		* params:
		* - tag: Item's tag
		* - attrs: Item's attributes
		*****************************************/
		static string makeSelfClosingItem(string tag, std::vector<KeyValue> attrs)
		{
			string result = "<" + tag;
			for (KeyValue a : attrs)
				result += " " + a();
			result += " />\n";
			return result;
		}
	}
}
#endif // !_SCENE_MAKER_STRING_OPERATIONS_