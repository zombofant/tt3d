#include "Log.hpp"
#include <cassert>
#include <string>
#include <boost/format.hpp>
#include <execinfo.h>

namespace tt3d {
namespace IO {
    
static const char *messageLevelNames[6] = {
    "debug",
    "info",
    "hint",
    "warning",
    "error",
    "fatal"
};

/* tt3d::IO::LogStream */

LogStream::LogStream(const std::string initial, Log *submitTo, const MessageLevel level):
    std::ostringstream(initial, ios_base::app | ios_base::out),
    _submitTo(submitTo),
    _level(level)
{
    assert(submitTo);
}

/**
 * Submit the contents of this stream to the log. Note that this will 
 * free this instance! */
void LogStream::submit() {
    Log *submitTo = _submitTo;
    _submitTo = 0;
    /* This will free this instance! */
    submitTo->submitLogStream(this);
}

/* tt3d::IO::Log */

Log::Log():
    _targets(new LogTargets())
{
    
}

Log::~Log() {
    delete _targets;
}

void Log::append(const std::string toAppend, const MessageLevel level) {
    for (LogTargets::iterator it = _targets->begin();
        it != _targets->end();
        it++) 
    {
        LogTarget *target = *it;
        if ((target->levels & level) != 0) {
            std::ostream *targetStream = target->targetStream.get();
            std::operator<<(*targetStream, toAppend);
            std::endl(*targetStream);
        }
    }
}

void Log::submitLogStream(LogStream *stream) {
    append(stream->str(), stream->getLevel());
    delete stream;
}

void Log::addLogTarget(const OStreamHandle stream, const MessageLevels levels) {
    _targets->push_back(new LogTarget(stream, levels));
}

std::ostream &submit(std::ostream &os) {
    LogStream *ls = static_cast<LogStream*>(&os);
    ls->submit();
    return os;
}

LogStream &operator<<(Log *log, MessageLevel level) {
    void *btData[2];
    backtrace(btData, 2);
    char **traced = backtrace_symbols(btData, 2);
    const std::string initial = (boost::format("[0x%08.8x] [tid=0x%08.8x] [%s] [%s] ") % 0 % 0 % traced[1] % messageLevelNames[level]).str();
    free(traced);
    LogStream *stream = new LogStream(initial, log, level);
    return *stream;
}

}
}
