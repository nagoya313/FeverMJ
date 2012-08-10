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
      next = GotoGame(-1, time(nullptr), {});
    }, [this] {
      next = GotoServer();
    }, [this] {
      next = GotoClient();
    });
  }

  std::unique_ptr<Sequence> Update() {
    input.Update();
    titleView.Draw(input);
    return std::move(next);
  }

 private:
  Input input;
  View::Title titleView;
  std::unique_ptr<Sequence> next = nullptr;
};
}}

#endif
