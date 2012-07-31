#ifndef FEVERMJ_CONTROLLER_SEQUENCE_HPP_
#define FEVERMJ_CONTROLLER_SEQUENCE_HPP_
#include <memory>
#include <boost/noncopyable.hpp>

namespace FeverMJ { namespace Controller {
class Sequence : boost::noncopyable {
 public:
  virtual ~Sequence() = default;
  virtual std::unique_ptr<Sequence> Update() = 0;
};

std::unique_ptr<Sequence> GotoGame();
std::unique_ptr<Sequence> GotoTitle();
std::unique_ptr<Sequence> GotoServer();
std::unique_ptr<Sequence> GotoClient();
}}

#endif
