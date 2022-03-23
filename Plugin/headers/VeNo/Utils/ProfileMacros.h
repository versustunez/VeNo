#include "Profile.h"
#include "Timer.h"
// Inspired from Hazel :)
#if VENO_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) ||    \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define VENO_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define VENO_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define VENO_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) ||              \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define VENO_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define VENO_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define VENO_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define VENO_FUNC_SIG __func__
#else
#define VENO_FUNC_SIG "VENO_FUNC_SIG unknown!"
#endif

#define VENO_PROFILE_BEGIN_SESSION(name, filepath)                             \
  VeNo::Perf::Profiler::the().begin(name, filepath)
#define VENO_PROFILE_END_SESSION() VeNo::Perf::Profiler::the().end()
#define VENO_PROFILE_SCOPE_LINE2(name, line)                                   \
  VeNo::Perf::ProfilerTimer timer##line(name)
#define VENO_PROFILE_SCOPE_LINE(name, line) VENO_PROFILE_SCOPE_LINE2(name, line)
#define VENO_PROFILE_SCOPE(name) VENO_PROFILE_SCOPE_LINE(name, __LINE__)
#define VENO_PROFILE_FUNCTION() VENO_PROFILE_SCOPE(VENO_FUNC_SIG)
#else
#define VENO_PROFILE_BEGIN_SESSION(name, filepath)
#define VENO_PROFILE_END_SESSION()
#define VENO_PROFILE_SCOPE(name)
#define VENO_PROFILE_FUNCTION()
#endif
