#ifndef FEVERMJ_CONTROLLER_SEQUENCE_HPP_
#define FEVERMJ_CONTROLLER_SEQUENCE_HPP_
#include <memory>
#include <boost/noncopyable.hpp>
#include "../utility/network.hpp"

namespace FeverMJ { namespace Controller {
class Sequence : boost::noncopyable {
 public:
  virtual ~Sequence() = default;
  virtual std::unique_ptr<Sequence> Update() = 0;
};

using SequencePtr = std::unique_ptr<Sequence>;

SequencePtr GotoGame(int firstParent, int seed, Utility::NetHandleArray &&handles);
SequencePtr GotoTitle();
SequencePtr GotoServer();
SequencePtr GotoClient();
}}

#endif
