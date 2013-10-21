#ifndef FEVERMJ_CONTROLLER_SEQUENCE_HPP_
#define FEVERMJ_CONTROLLER_SEQUENCE_HPP_
#include <memory>
#include <boost/noncopyable.hpp>
#include "../utility/network.hpp"

namespace FeverMJ { namespace Controller {
class Sequence;

using SequencePtr = std::unique_ptr<Sequence>;

class Sequence : boost::noncopyable {
 public:
  virtual ~Sequence() = default;
  virtual SequencePtr Update() = 0;
};

SequencePtr GotoGame(int firstParent, int seed, Utility::NetHandleArray &&handles, bool endress = true);
SequencePtr GotoTitle();
SequencePtr GotoServer();
SequencePtr GotoClient();
}}

#endif
