#pragma once


#include "ParamDocument.h"

#include <cassert>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>


#if defined(_WIN32) || defined(WIN32)
#   // Maybe visual studio will get there one day to support the c++ standard...
#   // Until then we have to live with this:
#   define and &&
#   define or ||
#   define not !
#endif


namespace tdl {

//\brief Citation information of the app
struct Citation {
    std::string doi; //!\brief the doi (document object identifier)
    std::string url; //!\brief an url for direct access.
};

//!\brief Meta data of the tool
struct ToolInfo {
    std::string version{};              //!\brief version as a string
    std::string name{};                 //!\brief name of the app
    std::string docurl{};               //!\brief url to the documentation of the app
    std::string category{};             //!\brief category of the app
    std::string description{};          //!\brief a brief description of the app
    std::string executableName{};       //!\brief the actual call of this app
    std::vector<Citation> citations{};  //!\brief list publication integrated into this app
};


namespace detail {

//!\brief overload structure allowing fancy 'std::visit` syntax
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

//!\brief required deduction guide for c++17 (not required for c++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


//!\brief replaces all occurrences of a character with a string
inline auto replace(std::string replace_in, char to_replace, std::string const& replace_with) {
    auto out = std::string{};
    out.reserve(replace_in.size());
    for (auto c : replace_in) {
        if (c == to_replace) {
            out.append(replace_with);
        } else {
            out.append(std::string{c});
        }
    }
    return out;
}

//!\brief required replacements to use strings safely inside XML documents
inline auto escapeXML(std::string str) {
    str = replace(str, '&', "&amp;");
    str = replace(str, '>', "&gt;");
    str = replace(str, '"', "&quot;");
    str = replace(str, '<', "&lt;");
    str = replace(str, '\'', "&apos;");
    str = replace(str, '\t', "&#x9;");
    str = replace(str, '\n', "#br#");

    return str;
}

//!\brief Simple structure representing an XML document (or parts of it)
struct XMLNode {
    std::string tag{};                                         //!\brief tag of the xml Node, e.g. "div" for <div>
    std::map<std::string, std::optional<std::string>> attr{};  //!\brief a list of attributes included in the tag
    std::vector<XMLNode> children{};                           //!\brief a list of child XMLNodes
    std::optional<std::string> data{};                         //!\brief plain data included in this tag

    //!\brief Converts this structures to a string
    auto generateString(std::string ind = "") const -> std::string {
        auto ss = std::stringstream{};
        ss << ind << "<" << escapeXML(tag);
        for (auto const& [key, v] : attr) {
            ss << " " << key;
            if (v) {
                ss << "=\"" << escapeXML(*v) << "\"";
            }
        }
        assert(children.empty() or not data); // can only have children or data, not both
        if (children.empty() and not data) {
            ss << " />\n";
        } else if (children.empty()) {
            ss << ">" << *data << "</" << escapeXML(tag) << ">\n";
        } else {
            ss << ">\n";
            for (auto const& child : children) {
                ss << child.generateString(ind + "    ");
            }
            ss << ind << "</" << escapeXML(tag) << ">\n";
        }
        return ss.str();
    }
};


//!\brief convenience function, to have highest precision for float/double values
template <typename T>
auto convertToString(T v) -> std::string {
    auto ss = std::stringstream{};
    ss << std::setprecision(std::numeric_limits<std::decay_t<T>>::max_digits10)
       << v;
    return ss.str();
}

// helper function, to generate the restriction attribute
template <typename T>
auto generateRestrictions(T const& value) -> std::optional<std::string> {
    if (!value.minLimit and !value.maxLimit) {
        return std::nullopt;
    }
    auto restrictions = std::string{};
    if (value.minLimit) {
        restrictions = convertToString(*value.minLimit);;
    }
    restrictions += ':';
    if (value.maxLimit) {
        restrictions += convertToString(*value.maxLimit);;
    }
    return restrictions;
}

// helper function to generate the restrictions of string values
template <typename T>
auto generateValidList(T const& value) -> std::optional<std::string> {
    if (!value.validValues or value.validValues->empty()) {
        return std::nullopt;
    }
    auto restrictions = std::string{};
    restrictions = std::accumulate(++begin(*value.validValues), end(*value.validValues), *begin(*value.validValues), [](auto acc, auto const& next) {
        acc.append(",");
        acc.append(next);
        return acc;
    });
    return restrictions;
}


//!\brief converts a ParamTree into an XMLNode
inline auto convertToCTD(ParamTree const& param) -> XMLNode {
    // generate a single Node
    auto xmlNode = XMLNode{/*.tag = */"NODE",
                           /*.attr = */{{"name", param.name}, {"description", param.description}}};


    auto tags = param.tags; // copy tags to make them mutable

    // fill xmlNode depending on the actuall type
    //!TODO why is bool so weirdly decoded?
    std::visit(overloaded{
        [&](IntValue value) {
            xmlNode.attr["type"]  = "int";
            xmlNode.attr["value"] = std::to_string(value.value);
            if (auto v = generateRestrictions(value)) xmlNode.attr["restrictions"] = v;
        },
        [&](DoubleValue value) {
            xmlNode.attr["type"] = "double";
            xmlNode.attr["value"] = convertToString(value.value);
            if (auto v = generateRestrictions(value)) xmlNode.attr["restrictions"] = v;
        },
        [&](StringValue value) {
            if (tags.count("input file") > 0) {
                tags.erase("input file");
                xmlNode.attr["type"] = "input-file";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else if (tags.count("output file") > 0) {
                tags.erase("output file");
                xmlNode.attr["type"] = "output-file";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else if (value.validValues
                       and value.validValues->size() == 2
                       and value.validValues->at(0) == "true"
                       and value.validValues->at(1) == "false") {
                xmlNode.attr["type"] = "bool";
            } else if (tags.count("output prefix") > 0) {
                tags.erase("output prefix");
                xmlNode.attr["type"] = "output-prefix";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else {
                xmlNode.attr["type"] = "string";
                if (auto v = generateValidList(value)) xmlNode.attr["restrictions"] = v;
            }
            xmlNode.attr["value"] = value.value;
        },
        [&](IntValueList value) {
            xmlNode.children.push_back({"ITEMLIST", {{"type", "int"}}});
            for (auto const& v : value.value) {
                xmlNode.children.back().children.push_back({"LISTITEM", {{"value", convertToString(v)}}});
            }
            if (auto v = generateRestrictions(value)) xmlNode.attr["restrictions"] = v;

        },
        [&](DoubleValueList value) {
            xmlNode.children.push_back({"ITEMLIST", {{"type", "double"}}});
            for (auto const& v : value.value) {
                xmlNode.children.back().children.push_back({"LISTITEM", {{"value", convertToString(v)}}});
            }
            if (auto v = generateRestrictions(value)) xmlNode.attr["restrictions"] = v;
        },
        [&](StringValueList value) {
            xmlNode.children.push_back({"ITEMLIST", {{"type", "string"}}});
            if (tags.count("input file") > 0) {
                tags.erase("input file");
                xmlNode.children.back().attr["type"] = "input-file";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else if (tags.count("output file") > 0) {
                tags.erase("output file");
                xmlNode.children.back().attr["type"] = "output-file";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else {
                xmlNode.children.back().attr["type"] = "string";
                if (auto v = generateValidList(value)) xmlNode.attr["restrictions"] = v;
            }
            for (auto const& v : value.value) {
                xmlNode.children.back().children.push_back({"LISTITEM", {{"value", v}}});
            }
        },
        [&](ParamTree::Children const& children) {
            for (auto const& child : children) {
                xmlNode.children.emplace_back(convertToCTD(child));
            }
        },
    }, param.value);

    // add description if available
    if (not param.description.empty()) {
        xmlNode.attr["description"] = param.description;
    }

    // convert special tags to special xml attributes
    auto convertTagToAttr = [&](std::string const& tag) {
        if (tags.count(tag)) xmlNode.attr[tag] = "true";
        tags.erase(tag);
    };
    convertTagToAttr("required");
    convertTagToAttr("advanced");

    // add all other tags to the "tags" xml attribute
    if (not tags.empty()) {
        auto list = std::accumulate(++begin(tags), end(tags), *begin(tags), [](auto acc, auto const& next) {
            acc.append(",");
            acc.append(next);
            return acc;
        });
        xmlNode.attr["tags"] = list;
    }
    return xmlNode;
}

}


/*!\brief converts a ToolInfo and a ParamDocument into a string that
 * holds the CTD representation of the given tool
 */
inline auto convertToCTD(ToolInfo const& tool_info, ParamDocument const& doc) {
    auto& node = doc.root;
    auto const schema_location = std::string{"/SCHEMAS/Param_1_7_0.xsd"};
    auto const schema_version  = std::string{"1.7.0"};

    // Write ctd specific stuff
    auto toolNode = detail::XMLNode{/*.tag = */"tool",
                                    /*.attr = */{{"ctdVersion", "1.7"}}};

    auto emptyOrAttr = [&](std::string const& in, std::string const& tag) {
        if (not in.empty()) {
            toolNode.attr[tag] = in;
        }
    };
    emptyOrAttr(tool_info.version,  "version");
    emptyOrAttr(tool_info.name,     "name");
    emptyOrAttr(tool_info.docurl,   "docurl");
    emptyOrAttr(tool_info.category, "category");

    auto emptyOrTag = [&](std::string tag, std::string const& in) {
        if (not in.empty()) {
            toolNode.children.push_back(detail::XMLNode {/*.tag      =*/ tag,
                                                 /*.attr     =*/ {},
                                                 /*.children =*/ {},
                                                 /*.data     =*/ "<![CDATA[" + in + "]]>"});
        }
    };
    emptyOrTag("description",    tool_info.description);
    emptyOrTag("executableName", tool_info.executableName);


    if (not tool_info.citations.empty()) {
        auto citationNode = detail::XMLNode{/*.tag = */"citations"};
        for (auto& [doi, url] : tool_info.citations) {
            citationNode.children.push_back({/*.tag = */"citation",
                                             /*.attr = */{{"doi", doi}, {"url", url}}});
        }
        toolNode.children.push_back(citationNode);
    }
    auto paramsNode = detail::XMLNode{/*.tag = */"PARAMETERS",
                                      /*.attr = */{{"version", schema_version},
                                               {"xsi:noNamespaceSchemaLocation", "https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS" + schema_location},
                                               {"xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance"}}};

    for (auto child : node) {
        paramsNode.children.emplace_back(detail::convertToCTD(child));
    }
    toolNode.children.push_back(paramsNode);

    // add cli mapping
    if (not doc.cliMapping.empty()) {
        auto cliNode = detail::XMLNode{/*.tag = */"cli"};
        for (auto const& [optionIdentifier, referenceName] : doc.cliMapping) {
            cliNode.children.push_back({/*.tag = */"clielement", /*.attr = */{{"optionIdentifier", optionIdentifier}}});
            cliNode.children.back().children.push_back({/*.tag = */"mapping", /*.attr = */{{"referenceName", referenceName}}});
        }
        toolNode.children.emplace_back(std::move(cliNode));
    }


    auto ss = std::stringstream{};
    ss.precision(std::numeric_limits<double>::digits10);


    // Write ctd specific stuff
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    ss << toolNode.generateString();
    return ss.str();
}

}

#if defined(_WIN32) || defined(WIN32)
#undef and
#undef or
#undef not
#endif
