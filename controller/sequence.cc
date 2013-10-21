#include "sequence.hpp"
#include "client.hpp"
#include "game.hpp"
#include "server.hpp"
#include "title.hpp"

namespace FeverMJ { namespace Controller {
SequencePtr GotoGame(int firstParent, int seed, Utility::NetHandleArray &&handles, bool endress) {
  return SequencePtr{new Game{firstParent, seed, std::move(handles), endress}};
}

SequencePtr GotoTitle() {
  return SequencePtr{new Title{}};
}

SequencePtr GotoServer() {
  return SequencePtr{new Server{}};
}

SequencePtr GotoClient() {
  return SequencePtr{new Client{}};
}
}}