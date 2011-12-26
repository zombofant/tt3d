/**********************************************************************
File name: Log.cpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#include "Log.hpp"
#include <cassert>
#include <string>
#include <boost/format.hpp>
#include <execinfo.h>
#include <typeinfo>

namespace tt3d {
namespace IO {
    
using namespace tt3d::Utils;
    
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

LogOStreamTarget::LogOStreamTarget(const StreamHandle stream, 
    const MessageLevels levels):
    LogStreamTarget::LogStreamTarget(levels),
    _stream(stream)
{
    
}

LogTargetBase &LogOStreamTarget::operator<<(const LogStream &logStream) {
    std::string str = (boost::format("[%12.5f] [tid=%016.16x] [%s] [%s] ") % logStream.getTimestamp() % logStream.getThreadID() % logStream.getCallSymbol() % messageLevelNames[logStream.getLevel()]).str();
    Stream *stream = _stream.get();
    
    stream->writeString(str);
    stream->writeString(logStream.str());
    stream->writeEndl();
    stream->flush();
    return *this;
}

/* tt3d::IO::LogXMLFormatter */

LogXMLFormatter::LogXMLFormatter(const StreamHandle stream, 
    const MessageLevels levels):
    LogOStreamTarget::LogOStreamTarget(stream, levels),
    _xslPath()
{
    initOutput();
}

LogXMLFormatter::LogXMLFormatter(const StreamHandle stream,
    const std::string xslPath, 
    const MessageLevels levels):
    LogOStreamTarget::LogOStreamTarget(stream, levels),
    _xslPath(xslPath)
{
    initOutput();
}

LogXMLFormatter::~LogXMLFormatter() {
    Stream &stream = *_stream.get();
    stream.writeString("</log>");
    stream.writeEndl();
}

void LogXMLFormatter::initOutput() {
    Stream &stream = *_stream.get();
    stream.writeString("<?xml version=\"1.0\"?>");
    stream.writeEndl();
    if (_xslPath.length() > 0) {
        stream.writeString("<?xml-stylesheet type=\"text/xsl\" href=\"");
        stream.writeString(_xslPath);
        stream.writeString("\"?>");
        stream.writeEndl();
    }
    stream.writeString("<log application=\"tt3d\">");
    stream.writeEndl();
}

LogTargetBase &LogXMLFormatter::operator<<(const LogStream &logStream) {
    // Not sure whether and how ugly this is; But its probably the most
    // clean solution one can get, considering that some of the parts
    // which are to be written are numbers which should be represented
    // as human-readable.
    std::ostringstream tmp;
    Stream &stream = *_stream.get();
    tmp << "  <event level=\"" << messageLevelNames[logStream.getLevel()] << "\">" << std::endl;
    tmp << "    <timestamp>" << logStream.getTimestamp() << "</timestamp>" << std::endl;
    tmp << "    <thread>" << logStream.getThreadID() << "</thread>" << std::endl;
    tmp << "    <call-symbol><![CDATA[" << logStream.getCallSymbol() << "]]></call-symbol>" << std::endl;
    tmp << "    <message><![CDATA[" << logStream.str() << "]]></message>" << std::endl;
    tmp << "  </event>" << std::endl;
    stream.writeString(tmp.str());
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
    const MessageLevels mask = 1<<toAppend.getLevel();
    for (LogTargets::iterator it = _targets->begin();
        it != _targets->end();
        it++) 
    {
        LogTargetBase *target = *it;
        if ((target->getMessageLevels() & mask) != 0) {
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

std::ostream &operator<<(std::ostream &stream, Exception &exception)  {
    unsigned int count;
    void * const* traceback = exception.traceback(&count);
    stream << "Exception occured: " << typeid(exception).name() << std::endl;
    stream << "Message: " << exception.what() << std::endl;
    stream << "Traceback: (most recent call first)" << std::endl;
    char **tracebackSymbols = backtrace_symbols(traceback, count);
    for (unsigned int i = 0; i < count; i++) {
        stream << tracebackSymbols[i] << std::endl;
    }
    free(tracebackSymbols);
    return stream;
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
