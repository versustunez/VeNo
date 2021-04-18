#include <JuceHeader.h>
#include <VUtils/Math.h>
#include <VeNo/Core/Config.h>
#include <VeNo/Sound/Generation/WaveTables/Creator/TableCreatorHelper.h>

namespace VeNo::Audio::Waves
{
void TableCreatorHelper::createTable (RawTable* table)
{
    table->setSize (findTableLen());
    table->generate();
    if (table->m_isRaw)
        return createTableRaw (table);
    fillTables (table);
    table->finish();
}

void TableCreatorHelper::createTableRaw (RawTable* table)
{
    auto group = table->m_group;
    auto topFreq = VeNo::Core::Config::get().sampleRate / 2.0;
    auto index = group->m_numWaveTables;
    createWaveTableOnIndex (table, index, topFreq);
    auto& waveTable = group->m_WaveTables[index].waveTable;
    for (size_t x = 0; x < table->m_len; x++)
        waveTable[x] = table->m_freqWaveRe[x];
    waveTable[table->m_len] = waveTable[0];
    table->finish();
}

double TableCreatorHelper::makeWaveTable (RawTable* table, double scale, double topFreq)
{
    fft (table->m_len, table);
    findCutoff (topFreq, table);
    auto& ai = table->m_tempIm;
    auto len = table->m_len;
    auto group = table->m_group;
    scale = getScale (table, scale);

    if (group->m_numWaveTables > WaveTableGroup::numWaveTableSlots)
        return 0;

    auto index = group->m_numWaveTables;
    createWaveTableOnIndex (table, index, topFreq);
    auto waveTable = group->m_WaveTables[index];
    for (size_t idx = 0; idx < len; idx++)
        waveTable.waveTable[idx] = ai[idx] * scale;

    waveTable.waveTable[len] = waveTable.waveTable[0];

    return 0.0;
}

size_t TableCreatorHelper::findTableLen()
{
    int maxHarms = (int) std::lround (VeNo::Core::Config::get().sampleRate / (3.0 * 20));
    return (size_t) VUtils::Math::nextPowerOfTwo (maxHarms) * 2;
}

double TableCreatorHelper::getNextRand()
{
    return ((std::rand() / double (RAND_MAX)) * 2) - 1;
}

int TableCreatorHelper::fillTables (RawTable* table)
{
    auto& freqWaveRe = table->m_freqWaveRe;
    auto& freqWaveIm = table->m_freqWaveIm;
    auto& ar = table->m_tempRe;
    auto& ai = table->m_tempIm;

    freqWaveRe[0] = freqWaveIm[0] = 0.0;
    freqWaveRe[table->m_len >> 1] = freqWaveIm[table->m_len >> 1] = 0.0;

    unsigned int maxHarmonic = table->m_len >> 1u;
    const double minValue = 0.000001;
    while ((std::fabs (freqWaveRe[maxHarmonic]) + std::fabs (freqWaveIm[maxHarmonic]) < minValue) && maxHarmonic)
        --maxHarmonic;
    double topFreq = 2.0 / 3.0 / maxHarmonic;
    double scale = 0.0;
    int numTables = 0;
    while (maxHarmonic)
    {
        table->cleanUpTemp();
        size_t i;
        size_t numSamples = table->m_len;
        for (i = 1; i <= maxHarmonic; i++)
        {
            ar[i] = freqWaveRe[i];
            ai[i] = freqWaveIm[i];
            ar[numSamples - i] = freqWaveRe[numSamples - i];
            ai[numSamples - i] = freqWaveIm[numSamples - i];
        }
        scale = makeWaveTable (table, scale, topFreq);
        numTables++;
        topFreq *= 2;
        maxHarmonic >>= 1u;
    }
    return numTables;
}

void TableCreatorHelper::fft (unsigned int N, RawTable* table)
{
    auto& ar = table->m_tempRe;
    auto& ai = table->m_tempIm;

    size_t NV2 = N >> 1;
    size_t NM1 = N - 1;
    size_t TEMP = N;
    size_t M = 0;
    while (TEMP >>= 1)
        ++M;

    size_t j = 1;
    size_t i;
    double t;
    for (i = 1; i <= NM1; i++)
    {
        if (i < j)
        {
            t = ar[j - 1];
            ar[j - 1] = ar[i - 1];
            ar[i - 1] = t;
            t = ai[j - 1];
            ai[j - 1] = ai[i - 1];
            ai[i - 1] = t;
        }

        size_t k = NV2;
        while (k < j)
        {
            j -= k;
            k /= 2;
        }

        j += k;
    }

    size_t LE = 1;
    for (size_t L = 1; L <= M; L++)
    {
        size_t LE1 = LE;
        LE *= 2;
        double Ur = 1.0;
        double Ui = 0.;
        double Wr = std::cos (VUtils::Math::PI / (float) LE1);
        double Wi = -std::sin (VUtils::Math::PI / (float) LE1);
        for (j = 1; j <= LE1; j++)
        {
            for (i = j; i <= N; i += LE)
            {
                size_t ip = i + LE1;
                double Tr = ar[ip - 1] * Ur - ai[ip - 1] * Ui;
                double Ti = ar[ip - 1] * Ui + ai[ip - 1] * Ur;
                ar[ip - 1] = ar[i - 1] - Tr;
                ai[ip - 1] = ai[i - 1] - Ti;
                ar[i - 1] = ar[i - 1] + Tr;
                ai[i - 1] = ai[i - 1] + Ti;
            }
            double Ur_old = Ur;
            Ur = Ur_old * Wr - Ui * Wi;
            Ui = Ur_old * Wi + Ui * Wr;
        }
    }
}

double TableCreatorHelper::getScale (RawTable* table, double scale)
{
    if (scale == 0.0)
    {
        auto& ai = table->m_tempIm;
        auto len = table->m_len;
        double max = 0;
        for (size_t idx = 0; idx < len; idx++)
        {
            double temp = std::fabs (ai[idx]);
            if (max < temp)
                max = temp;
        }
        return 1.0 / max * .999;
    }
    return 0.0;
}

void TableCreatorHelper::createWaveTableOnIndex (RawTable* table, int index, double topFreq)
{
    auto group = table->m_group;
    auto len = table->m_len;
    auto tableObj = group->m_WaveTables[index] = WaveTableObject();
    group->m_WaveTables[index].waveTable = new double[len + 1];
    tableObj.tableLen = len;
    tableObj.topFreq = topFreq;
    ++group->m_numWaveTables;
}

// makes init time longer but hey... better waveforms ;)
void TableCreatorHelper::findCutoff (double topFreq, RawTable* table)
{
    // filter the ai!
    auto& ai = table->m_tempIm;
    double sRate = VeNo::Core::Config::get().sampleRate;
    double cutOff = topFreq * (sRate / 2) * 2;
    double RC = 0.9 / (cutOff * 2 * VUtils::Math::PI);
    double dt = 1.0 / sRate;
    double alpha = dt / (RC + dt);

    double prev_sample = 0.0;
    for (double& i : ai)
    {
        double tmp = i;
        i = prev_sample + (alpha * (i - prev_sample));
        prev_sample = tmp;
    }
}
} // namespace VeNo::Audio::Waves