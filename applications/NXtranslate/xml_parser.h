#ifndef __XML_PARSER_H_GUARD
#define __XML_PARSER_H_GUARD

#include <string>
#include <napi.h>
#include <map>

namespace xml_parser{
  extern bool parse_xml_file(const std::map<std::string,std::string>&map,
                             const std::string &filename, NXhandle *handle);
}
#endif
