// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file.
// -----------------------------------------------------------------------------------------------------

#include <tdl/ParamDocument.h>
#include <tdl/ParamDocumentToCTD.h>

#include <cassert>
#include <iostream>

void testToolInfo() {
    { // Testing empty ToolInfo (making sure only minimum number of fields are being printed)
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" />
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // Testing filling all fields (with exception of citations for better overview)
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {"7.6.5"},
            /*.name           =*/ {"testApp"},
            /*.docurl         =*/ {"example.com"},
            /*.category       =*/ {"test-category"},
            /*.description    =*/ {"a demonstration of how this tool info works"},
            /*.executableName =*/ {"test"},
            /*.citations      =*/ {},
        }, {} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7" version="7.6.5" name="testApp" docurl="example.com" category="test-category">
    <description><![CDATA[a demonstration of how this tool info works]]></description>
    <manual><![CDATA[a demonstration of how this tool info works]]></manual>
    <executableName><![CDATA[test]]></executableName>
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" />
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Testing citations list
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {{"doi:123", "https://en.wikipedia.org/wiki/Meaning_of_life"},
                               {"doi:456", "https://en.wikipedia.org/wiki/Turing_completeness"}},
        }, {} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations>
        <citation doi="doi:123" url="https://en.wikipedia.org/wiki/Meaning_of_life" />
        <citation doi="doi:456" url="https://en.wikipedia.org/wiki/Turing_completeness" />
    </citations>
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" />
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testParamTreeSingleInt() {
    { // Single Int, no tags, no limits
        auto doc = tdl::ParamDocument{
            { tdl::ParamTree{/*.name        =*/ "foo",
                        /*.description =*/ "testing a single int with no tags and no limits",
                        /*.tags        =*/ {},
                        /*.value       =*/ tdl::IntValue{5}
            }}
        };
        auto output = convertToCTD(tdl::ToolInfo{}, doc);
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with no tags and no limits" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);

    }
    { // Single Int, no tags, min limits
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single int with no tags and a min limit",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::IntValue{5, 1}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with no tags and a min limit" required="false" advanced="false" restrictions="1:" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single Int, no tags, a max limits
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single int with no tags and a max limit",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::IntValue{/*.value =*/ 5, /*.minLimit =*/ std::nullopt, /*.maxLimit =*/ 9}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with no tags and a max limit" required="false" advanced="false" restrictions=":9" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);

    }
    { // Single Int, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single int with no tags and with limits",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::IntValue{/*.value =*/ 5, /*.minLimit =*/ 2, /*.maxLimit =*/ 11}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with no tags and with limits" required="false" advanced="false" restrictions="2:11" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testParamTreeSingleTypes() {
    { // Single double, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single double with no tags and with limits",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::DoubleValue{/*.value =*/ 3.5, /*.minLimit =*/ 1.25, /*.maxLimit =*/ 5.125}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="3.5" type="double" description="testing a single double with no tags and with limits" required="false" advanced="false" restrictions="1.25:5.125" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single string, no tags, with validator list
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single string with no tags and with a validator list",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::StringValue{"hallo", {{"a", "b", "c", "uiae", "dtrn", "hallo"}}}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="hallo" type="string" description="testing a single string with no tags and with a validator list" required="false" advanced="false" restrictions="a,b,c,uiae,dtrn,hallo" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testParamTreeListTypes() {
    { // List of ints, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a list of ints with no tags and with limits",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::IntValueList{/*.value =*/ {7, 3, 4}, /*.minLimit =*/ -1, /*.maxLimit =*/ 99}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="int" description="testing a list of ints with no tags and with limits" required="false" advanced="false" restrictions="-1:99">
            <LISTITEM value="7" />
            <LISTITEM value="3" />
            <LISTITEM value="4" />
        </ITEMLIST>
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // List of doubles, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a list of doubles with no tags and with limits",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::DoubleValueList{/*.value =*/ {2.5, 3.5, 4.5}, /*.minLimit =*/ 1.25, /*.maxLimit =*/ 5.125}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="double" description="testing a list of doubles with no tags and with limits" required="false" advanced="false" restrictions="1.25:5.125">
            <LISTITEM value="2.5" />
            <LISTITEM value="3.5" />
            <LISTITEM value="4.5" />
        </ITEMLIST>
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // List of strings, no tags, with validator list
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a list of strings with no tags and with a validator list",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::StringValueList{{"hallo", "b", "c"}, {{"a", "b", "c", "uiae", "dtrn", "hallo"}}}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="string" description="testing a list of strings with no tags and with a validator list" required="false" advanced="false" restrictions="a,b,c,uiae,dtrn,hallo">
            <LISTITEM value="hallo" />
            <LISTITEM value="b" />
            <LISTITEM value="c" />
        </ITEMLIST>
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // a single string with only 'false' and 'true' as options
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single bool",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::BoolValue{false}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="false" type="bool" description="testing a single bool" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }

}

void testParamTreeNestedTypes() {
    { // zero nested parameters
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "some major command",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::ParamTree::Children{}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <NODE name="foo" description="some major command" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // a single nested parameter
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "some major command",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::ParamTree::Children {{
                /*.name        =*/ "input",
                /*.description =*/ "input file",
                /*.tags        =*/ {},
                /*.value       =*/ tdl::IntValue{/*.value =*/ 1, /*.minLimit =*/ 0, /*.maxLimit =*/ 63}
              }}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <NODE name="foo" description="some major command">
            <ITEM name="input" value="1" type="int" description="input file" required="false" advanced="false" restrictions="0:63" />
        </NODE>
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // a multi nested parameter
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "build",
              /*.description =*/ "builds some index for search",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::ParamTree::Children {{
                /*.name        =*/ "input",
                /*.description =*/ "input file",
                /*.tags        =*/ {},
                /*.value       =*/ tdl::StringValueList{}
              }}
        },  { /*.name        =*/ "search",
              /*.description =*/ "reusing index to search",
              /*.tags        =*/ {},
              /*.value       =*/ tdl::ParamTree::Children {{
                /*.name        =*/ "queries",
                /*.description =*/ "files with search queries",
                /*.tags        =*/ {},
                /*.value       =*/ tdl::StringValueList{}
              }, {
                /*.name        =*/ "index",
                /*.description =*/ "path to the index file",
                /*.tags        =*/ {},
                /*.value       =*/ tdl::StringValue{}
              }}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <NODE name="build" description="builds some index for search">
            <ITEMLIST name="input" type="string" description="input file" required="false" advanced="false" />
        </NODE>
        <NODE name="search" description="reusing index to search">
            <ITEMLIST name="queries" type="string" description="files with search queries" required="false" advanced="false" />
            <ITEM name="index" value="" type="string" description="path to the index file" required="false" advanced="false" />
        </NODE>
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

}

void testParamTreeTags() {
    { // Single Int with special tag "required"
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single int with special tag \"required\"",
              /*.tags        =*/ {"required"},
              /*.value       =*/ tdl::IntValue{5}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with special tag &quot;required&quot;" required="true" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single Int with special tag "advanced"
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single int with special tag \"advanced\"",
              /*.tags        =*/ {"advanced"},
              /*.value       =*/ tdl::IntValue{5}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with special tag &quot;advanced&quot;" required="false" advanced="true" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single Int with some random tags: "fast" and "easy"
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single int with normal tags \"fast\" and \"easy\"",
              /*.tags        =*/ {"fast", "easy"},
              /*.value       =*/ tdl::IntValue{5}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with normal tags &quot;fast&quot; and &quot;easy&quot;" required="false" advanced="false" tags="easy,fast" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // Single String with special tag "input file"
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single string with special tag \"input file\"",
              /*.tags        =*/ {"input file"},
              /*.value       =*/ tdl::StringValue{"input"}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="input" type="input-file" description="testing a single string with special tag &quot;input file&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single String with special tag "output file"
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single string with special tag \"output file\"",
              /*.tags        =*/ {"output file"},
              /*.value       =*/ tdl::StringValue{"output"}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="output" type="output-file" description="testing a single string with special tag &quot;output file&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // Single String with special tag "output prefix"
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a single string with special tag \"output prefix\"",
              /*.tags        =*/ {"output prefix"},
              /*.value       =*/ tdl::StringValue{"output-path"}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="output-path" type="output-prefix" description="testing a single string with special tag &quot;output prefix&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // List of strings with special tag "input file"
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a list of strings with special tag \"input file\"",
              /*.tags        =*/ {"input file"},
              /*.value       =*/ tdl::StringValueList{}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="input-file" description="testing a list of strings with special tag &quot;input file&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single String with special tag "output file"
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {{{/*.name        =*/ "foo",
              /*.description =*/ "testing a list of strings with special tag \"output file\"",
              /*.tags        =*/ {"output file"},
              /*.value       =*/ tdl::StringValueList{}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="output-file" description="testing a list of strings with special tag &quot;output file&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

}

void testCliMapping() {
    { // Single double, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo {
            /*.version        =*/ {},
            /*.name           =*/ {},
            /*.docurl         =*/ {},
            /*.category       =*/ {},
            /*.description    =*/ {},
            /*.executableName =*/ {},
            /*.citations      =*/ {},
        }, {/*.root =*/ {},
            /*.cliMapping =*/ {
              {/*.optionIdentifier =*/ "--blub",
               /*.referenceName    =*/ "blubRef"
            }
        }} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" />
    <cli>
        <clielement optionIdentifier="--blub">
            <mapping referenceName="blubRef" />
        </clielement>
    </cli>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}





int main() {
    testToolInfo();
    testParamTreeSingleInt();
    testParamTreeSingleTypes();
    testParamTreeListTypes();
    testParamTreeNestedTypes();
    testParamTreeTags();
    testCliMapping();
    std::cout << "Success! Nothing failed!\n";
    return EXIT_SUCCESS;
}