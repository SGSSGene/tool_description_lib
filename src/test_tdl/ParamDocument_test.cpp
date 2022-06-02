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
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" />
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
<tool category="test-category" ctdVersion="1.7" docurl="example.com" name="testApp" version="7.6.5">
    <description><![CDATA[a demonstration of how this tool info works]]></description>
    <executableName><![CDATA[test]]></executableName>
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" />
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
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" />
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testParamTreeSingleInt() {
    { // Single Int, no tags, no limits
        auto doc = ParamDocument{
            { ParamTree{/*.name        =*/ "foo",
                        /*.description =*/ "testing a single int with no tags and no limits",
                        /*.tags        =*/ {},
                        /*.value       =*/ IntValue{5}
            }}
        };
        auto output = convertToCTD(tdl::ToolInfo{}, doc);
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single int with no tags and no limits" name="foo" type="int" value="5" />
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
              /*.value       =*/ IntValue{5, 1}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single int with no tags and a min limit" name="foo" restrictions="1:" type="int" value="5" />
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
              /*.value       =*/ IntValue{/*.value =*/ 5, /*.minLimit =*/ std::nullopt, /*.maxLimit =*/ 9}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single int with no tags and a max limit" name="foo" restrictions=":9" type="int" value="5" />
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
              /*.value       =*/ IntValue{/*.value =*/ 5, /*.minLimit =*/ 2, /*.maxLimit =*/ 11}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single int with no tags and with limits" name="foo" restrictions="2:11" type="int" value="5" />
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
              /*.value       =*/ DoubleValue{/*.value =*/ 3.5, /*.minLimit =*/ 1.25, /*.maxLimit =*/ 5.125}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single double with no tags and with limits" name="foo" restrictions="1.25:5.125" type="double" value="3.5" />
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
              /*.value       =*/ StringValue{"hallo", {{"a", "b", "c", "uiae", "dtrn", "hallo"}}}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single string with no tags and with a validator list" name="foo" restrictions="a,b,c,uiae,dtrn,hallo" type="string" value="hallo" />
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
              /*.value       =*/ IntValueList{/*.value =*/ {7, 3, 4}, /*.minLimit =*/ -1, /*.maxLimit =*/ 99}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a list of ints with no tags and with limits" name="foo" restrictions="-1:99">
            <ITEMLIST type="int">
                <LISTITEM value="7" />
                <LISTITEM value="3" />
                <LISTITEM value="4" />
            </ITEMLIST>
        </NODE>
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
              /*.value       =*/ DoubleValueList{/*.value =*/ {2.5, 3.5, 4.5}, /*.minLimit =*/ 1.25, /*.maxLimit =*/ 5.125}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a list of doubles with no tags and with limits" name="foo" restrictions="1.25:5.125">
            <ITEMLIST type="double">
                <LISTITEM value="2.5" />
                <LISTITEM value="3.5" />
                <LISTITEM value="4.5" />
            </ITEMLIST>
        </NODE>
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
              /*.value       =*/ StringValueList{{"hallo", "b", "c"}, {{"a", "b", "c", "uiae", "dtrn", "hallo"}}}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a list of strings with no tags and with a validator list" name="foo" restrictions="a,b,c,uiae,dtrn,hallo">
            <ITEMLIST type="string">
                <LISTITEM value="hallo" />
                <LISTITEM value="b" />
                <LISTITEM value="c" />
            </ITEMLIST>
        </NODE>
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
              /*.description =*/ "testing a single string with only 'false' and 'true' as options",
              /*.tags        =*/ {},
              /*.value       =*/ StringValue{"false", {{"true", "false"}}}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single string with only &apos;false&apos; and &apos;true&apos; as options" name="foo" type="bool" value="false" />
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
              /*.value       =*/ ParamTree::Children {}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="some major command" name="foo" />
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
              /*.value       =*/ ParamTree::Children {{
                /*.name        =*/ "input",
                /*.description =*/ "input file",
                /*.tags        =*/ {},
                /*.value       =*/ IntValue{/*.value =*/ 1, /*.minLimit =*/ 0, /*.maxLimit =*/ 63}
              }}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="some major command" name="foo">
            <NODE description="input file" name="input" restrictions="0:63" type="int" value="1" />
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
              /*.value       =*/ ParamTree::Children {{
                /*.name        =*/ "input",
                /*.description =*/ "input file",
                /*.tags        =*/ {},
                /*.value       =*/ StringValueList{}
              }}
        },  { /*.name        =*/ "search",
              /*.description =*/ "reusing index to search",
              /*.tags        =*/ {},
              /*.value       =*/ ParamTree::Children {{
                /*.name        =*/ "queries",
                /*.description =*/ "files with search queries",
                /*.tags        =*/ {},
                /*.value       =*/ StringValueList{}
              }, {
                /*.name        =*/ "index",
                /*.description =*/ "path to the index file",
                /*.tags        =*/ {},
                /*.value       =*/ StringValue{}
              }}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="builds some index for search" name="build">
            <NODE description="input file" name="input">
                <ITEMLIST type="string" />
            </NODE>
        </NODE>
        <NODE description="reusing index to search" name="search">
            <NODE description="files with search queries" name="queries">
                <ITEMLIST type="string" />
            </NODE>
            <NODE description="path to the index file" name="index" type="string" value="" />
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
              /*.value       =*/ IntValue{5}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single int with special tag &quot;required&quot;" name="foo" required="true" type="int" value="5" />
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
              /*.value       =*/ IntValue{5}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE advanced="true" description="testing a single int with special tag &quot;advanced&quot;" name="foo" type="int" value="5" />
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
              /*.value       =*/ IntValue{5}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single int with normal tags &quot;fast&quot; and &quot;easy&quot;" name="foo" tags="easy,fast" type="int" value="5" />
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
              /*.value       =*/ StringValue{"input"}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single string with special tag &quot;input file&quot;" name="foo" type="input-file" value="input" />
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
              /*.value       =*/ StringValue{"output"}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single string with special tag &quot;output file&quot;" name="foo" type="output-file" value="output" />
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
              /*.value       =*/ StringValue{"output-path"}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a single string with special tag &quot;output prefix&quot;" name="foo" type="output-prefix" value="output-path" />
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
              /*.value       =*/ StringValueList{}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a list of strings with special tag &quot;input file&quot;" name="foo">
            <ITEMLIST type="input-file" />
        </NODE>
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
              /*.value       =*/ StringValueList{}
        }}} );
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd">
        <NODE description="testing a list of strings with special tag &quot;output file&quot;" name="foo">
            <ITEMLIST type="output-file" />
        </NODE>
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
    <PARAMETERS version="1.7.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" />
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
