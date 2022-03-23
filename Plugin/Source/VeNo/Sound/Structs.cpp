#include <VeNo/Sound/Structs.h>

namespace VeNo::Audio {
WaveHolder::WaveHolder(size_t id) {
  for (auto &generator : generators) {
    generator = CreateScope<WaveTableLib>();
    generator->setInstanceId(id);
  }
}
} // namespace VeNo::Audio