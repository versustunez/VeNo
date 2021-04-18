#include <VeNo/Sound/Generation/WaveTables/Structs.h>

namespace VeNo::Audio::Waves
{
WaveTableObject::~WaveTableObject()
{
    delete[] waveTable;
}
} // namespace VeNo::Audio::Waves