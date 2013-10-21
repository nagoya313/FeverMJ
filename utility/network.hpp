#ifndef FEVERMJ_UTILITY_NETWORK_HPP_
#define FEVERMJ_UTILITY_NETWORK_HPP_
#include <array>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include <DxLib.h>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include "log.hpp"

BOOST_FUSION_ADAPT_STRUCT(
  IPDATA,
  (unsigned char, d1)
  (unsigned char, d2)
  (unsigned char, d3)
  (unsigned char, d4)
)

namespace FeverMJ { namespace Utility {
class NetWorkError : public std::runtime_error {
 public:
  explicit NetWorkError(const std::string &str) : runtime_error(str) {}
};

class NetWorkHandle {
 public:
  NetWorkHandle() = default;

  explicit NetWorkHandle(int h) : handle(h) {}

  NetWorkHandle(NetWorkHandle &&rhs) : handle(rhs.handle) {
    rhs.handle = -1;
  }

  NetWorkHandle &operator =(NetWorkHandle &&rhs) {
    handle = rhs.handle;
    rhs.handle = -1;
    return *this;
  }

  ~NetWorkHandle() {
    if (handle != -1) {
      CloseNetWork(handle);
    }
  }

  NetWorkHandle(const NetWorkHandle &) = delete;
  NetWorkHandle &operator =(const NetWorkHandle &) = delete;

  void Send(std::string message) const {
    if (NetWorkSend(handle, &message[0], message.length()) == -1) {
      if (NetWorkSend(handle, &message[0], message.length()) == -1) {
        throw Utility::NetWorkError{"送信失敗"};  
      }
    }
  }

  boost::optional<std::string> Receive(int size) const {
    if (handle == -1) {
      throw Utility::NetWorkError{"受信失敗"};
    }
    if (GetNetWorkDataLength(handle)) {
      std::string str(size, ' ');
      if (NetWorkRecv(handle, &str[0], str.length()) != -1) {
        return str;
      }
    }
    return boost::none;
  }

  explicit operator bool() const {
    return handle != -1;
  }

  int Get() const {
    return handle;
  }

 private:
  int handle = -1;
};

using  NetHandleArray = std::array<NetWorkHandle, 2>;

struct InitData {
  int firstParent;
  int seed;
  std::string func;
  std::string ip;
};

struct ResponseData {
  std::string func;
  int value;
};

struct SquealResponseData {
  std::string func;
  int value0;
  int value1;
};

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
  ip0 << std::setw(3) << std::setfill('0') << static_cast<int>(ip.d1);
  std::stringstream ip1;
  ip1 << std::setw(3) << std::setfill('0') << static_cast<int>(ip.d2);
  std::stringstream ip2;
  ip2 << std::setw(3) << std::setfill('0') << static_cast<int>(ip.d3);
  std::stringstream ip3;
  ip3 << std::setw(3) << std::setfill('0') << static_cast<int>(ip.d4);
  FEVERMJ_LOG("IP %s\n", (ip0.str() + "." + ip1.str() + "." + ip2.str() + "." + ip3.str()).c_str());
  return ip0.str() + "." + ip1.str() + "." + ip2.str() + "." + ip3.str();
}

inline
std::string GetSeedString(std::uint32_t seed) {
  std::stringstream s;
  s << std::setw(std::numeric_limits<std::uint32_t>::digits10 + 1) << std::setfill('0') << seed;
  return s.str();
}

inline
boost::optional<InitData> GetInitData(const std::string &data) {
  InitData d;
  FEVERMJ_LOG("受信データ %s\n", data.c_str());
  auto it = data.cbegin();
  const bool ret = boost::spirit::qi::parse(it,
                                            data.cend(),
                                            "fp" >> boost::spirit::qi::int_ >>
                                            "sd" >> boost::spirit::qi::uint_ >>
                                            *(boost::spirit::qi::alpha) >> 
                                            *(boost::spirit::qi::digit | boost::spirit::qi::char_(".")), d) && it == data.end();
  return ret ? boost::make_optional(d) : boost::none;
}

inline
boost::optional<ResponseData> GetResponceData(const std::string &data) {
  ResponseData d;
  FEVERMJ_LOG("受信データ %s\n", data.c_str());
  auto it = data.cbegin();
  const bool ret = boost::spirit::qi::parse(it,
                                            data.cend(),
                                            *(boost::spirit::qi::alpha) >> boost::spirit::qi::int_, d) && it == data.end();
  return ret ? boost::make_optional(d) : boost::none;
}

inline
boost::optional<SquealResponseData> GetSquealResponceData(const std::string &data) {
  SquealResponseData d;
  FEVERMJ_LOG("受信データ %s\n", data.c_str());
  auto it = data.cbegin();
  const bool ret = boost::spirit::qi::parse(it,
                                            data.cend(),
                                            *(boost::spirit::qi::alpha) >>
                                            boost::spirit::qi::int_ >> 
                                            "," >>
                                            boost::spirit::qi::int_, d) && it == data.end();
  FEVERMJ_LOG("鳴きデータ %d %d\n", d.value0, d.value1);
  return ret ? boost::make_optional(d) : boost::none;
}
}}

BOOST_FUSION_ADAPT_STRUCT(
  FeverMJ::Utility::InitData,
  (int, firstParent)
  (int, seed)
  (std::string, func)
  (std::string, ip)
)

BOOST_FUSION_ADAPT_STRUCT(
  FeverMJ::Utility::ResponseData,
  (std::string, func)
  (int, value)
)

BOOST_FUSION_ADAPT_STRUCT(
  FeverMJ::Utility::SquealResponseData,
  (std::string, func)
  (int, value0)
  (int, value1)
)

#endif