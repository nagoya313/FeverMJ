#ifndef FEVERMJ_CONTROLLER_TITLE_HPP_
#define FEVERMJ_CONTROLLER_TITLE_HPP_
#include <ctime>
#include <boost/noncopyable.hpp>
#include "input.hpp"
#include "sequence.hpp"
#include "../view/title.hpp"

namespace FeverMJ { namespace Controller {
class Title : public Sequence {
 public:
  Title() {
    titleView.Init([this] {
      next = GotoGame(0, time(nullptr), {}, true);
    }, [this] {
      next = GotoServer();
    }, [this] {
      next = GotoClient();
    });
  }

  SequencePtr Update() override {
    input.Update();
    titleView.Draw(input);
    return std::move(next);
  }

 private:
  Input input;
  View::Title titleView;
  SequencePtr next = nullptr;
};
}}

#endif
