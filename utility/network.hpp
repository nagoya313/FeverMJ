#ifndef FEVERMJ_UTILITY_NETWORK_HPP_
#define FEVERMJ_UTILITY_NETWORK_HPP_
#include <iomanip>
#include <string>
#include <sstream>
#include <DxLib.h>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

BOOST_FUSION_ADAPT_STRUCT(
  IPDATA,
  (unsigned char, d1)
  (unsigned char, d2)
  (unsigned char, d3)
  (unsigned char, d4)
)

namespace FeverMJ { namespace Utility {
inline
boost::optional<IPDATA> GetIP(const std::string &ipstr) {
  IPDATA ip;
  auto it = ipstr.cbegin();
  const bool ret = boost::spirit::qi::parse(it,
                                            ipstr.cend(),
                                            boost::spirit::qi::int_ >> "." >> 
                                            boost::spirit::qi::int_ >> "." >>
                                            boost::spirit::qi::int_ >> "." >>
                                            boost::spirit::qi::int_, ip) && it == ipstr.end();
  return ret ? boost::make_optional(ip) : boost::none;
}

inline
std::string GetIPString(IPDATA &ip) {
  std::stringstream ip0;
  ip0 << std::setw(3) << std::setfill('0') << ip.d1;
  std::stringstream ip1;
  ip1 << std::setw(3) << std::setfill('0') << ip.d1;
  std::stringstream ip2;
  ip2 << std::setw(3) << std::setfill('0') << ip.d1;
  std::stringstream ip3;
  ip3 << std::setw(3) << std::setfill('0') << ip.d1;
  return ip0.str() + "." + ip1.str() + "." + ip2.str() + "." + ip3.str();
}
}}

#endif