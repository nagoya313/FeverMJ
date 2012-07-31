#include "sequence.hpp"
#include "client.hpp"
#include "game.hpp"
#include "server.hpp"
#include "title.hpp"

namespace FeverMJ { namespace Controller {
std::unique_ptr<Sequence> GotoGame() {
  return std::unique_ptr<Sequence>{new Game{}};
}

std::unique_ptr<Sequence> GotoTitle() {
  return std::unique_ptr<Sequence>{new Title{}};
}

std::unique_ptr<Sequence> GotoServer() {
  return std::unique_ptr<Sequence>{new Server{}};
}

std::unique_ptr<Sequence> GotoClient() {
  return std::unique_ptr<Sequence>{new Client{}};
}
}}