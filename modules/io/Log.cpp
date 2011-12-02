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

Log log = Log();

/* tt3d::IO::LogStream */

LogStream::LogStream(const TimeFloat timestamp, const boost::thread::id threadID, const std::string callSymbol, Log *submitTo, const MessageLevel level):
    std::ostringstream(ios_base::app | ios_base::out),
    _submitTo(submitTo),
    _timestamp(timestamp),
    _callSymbol(callSymbol),
    _threadID(threadID),
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

/* tt3d::IO::LogStreamTarget */

LogStreamTarget::LogStreamTarget(const MessageLevels levels):
    _levels(levels)
{
    
}

/* tt3d::IO::LogOStreamTarget */

LogOStreamTarget::LogOStreamTarget(const OStreamHandle stream, 
    const MessageLevels levels):
    LogStreamTarget::LogStreamTarget(levels),
    _stream(stream)
{
    
}

LogTargetBase &LogOStreamTarget::operator<<(const LogStream &logStream) {
    std::string str = (boost::format("[%12.5f] [tid=%016.16x] [%s] [%s] ") % logStream.getTimestamp() % logStream.getThreadID() % logStream.getCallSymbol() % messageLevelNames[logStream.getLevel()]).str();
    std::ostream *stream = _stream.get();
    
    std::operator<<(*stream, str);
    std::operator<<(*stream, logStream.str());
    std::endl(*stream);
    return *this;
}

/* tt3d::IO::Log */

Log::Log():
    _targets(new LogTargets()),
    _startTime(nanotime())
{
    
}

Log::~Log() {
    for (LogTargets::iterator it = _targets->begin();
        it != _targets->end();
        it++) 
    {
        LogTargetBase *target = *it;
        delete target;
    }
    delete _targets;
}

void Log::append(const LogStream &toAppend) {
    const MessageLevel level = toAppend.getLevel();
    for (LogTargets::iterator it = _targets->begin();
        it != _targets->end();
        it++) 
    {
        LogTargetBase *target = *it;
        if ((target->getMessageLevels() & level) != 0) {
            target->operator<<(toAppend);
        }
    }
}

void Log::submitLogStream(LogStream *stream) {
    append(*stream);
    delete stream;
}

void Log::addLogTarget(LogTargetBase *target) {
    _targets->push_back(target);
}

std::ostream &submit(std::ostream &os) {
    LogStream *ls = static_cast<LogStream*>(&os);
    ls->submit();
    return os;
}

LogStream &operator<<(Log &log, MessageLevel level) {
    void *btData[2];
    backtrace(btData, 2);
    char **traced = backtrace_symbols(btData, 2);
    timespec time = nanotime();
    LogStream *stream = new LogStream(timeIntervalToDouble(log.getStartTime(), time), boost::this_thread::get_id(), std::string(traced[1]), &log, level);
    free(traced);
    return *stream;
}

}
}
