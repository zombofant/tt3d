/**********************************************************************
File name: Log.hpp
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
#include <boost/smart_ptr/shared_ptr.hpp>
#include <istream>
#include <ostream>
#include <sstream>
#include "vfs/VFS.hpp"
#include "Time.hpp"
#include <boost/thread.hpp>

namespace tt3d {
namespace IO {
    
enum MessageLevel {
    ML_DEBUG = 0,
    ML_INFO = 1,
    ML_HINT = 2,
    ML_WARNING = 3,
    ML_ERROR = 4,
    ML_FATAL = 5
};
typedef unsigned int MessageLevels;

static MessageLevels ML_DEFAULT = (1<<ML_INFO) | (1<<ML_HINT) | (1<<ML_WARNING) | (1<<ML_ERROR) | (1<<ML_FATAL);

class Log;

struct LogStream: public std::ostringstream {
    public:
        LogStream(const TimeFloat timestamp, const boost::thread::id threadID, const std::string callSymbol, Log *submitTo, const MessageLevel level);
    private:
        Log *_submitTo;
        const TimeFloat _timestamp;
        const std::string _callSymbol;
        const boost::thread::id _threadID;
        const MessageLevel _level;
    public:
        const std::string &getCallSymbol() const { return _callSymbol; }
        const MessageLevel getLevel() const { return _level; }
        const boost::thread::id getThreadID() const { return _threadID; }
        const TimeFloat getTimestamp() const { return _timestamp; }
        void submit();
};

class LogTargetBase {
    public:
        virtual const MessageLevels getMessageLevels() = 0;
        virtual LogTargetBase &operator<< (const LogStream &logStream) = 0;
};

class LogStreamTarget: public LogTargetBase {
    public:
        LogStreamTarget(const MessageLevels levels);
    protected:
        const MessageLevels _levels;
    public:
        virtual const MessageLevels getMessageLevels() { return _levels; };
};

class LogOStreamTarget: public LogStreamTarget {
    public:
        LogOStreamTarget(const OStreamHandle stream, 
            const MessageLevels levels = ML_DEFAULT);
    protected:
        const OStreamHandle _stream;
    public:
        virtual LogTargetBase &operator<< (const LogStream &logStream);
};

class LogXMLFormatter: public LogOStreamTarget {
    public:
        LogXMLFormatter(const OStreamHandle stream, 
            const MessageLevels levels = ML_DEFAULT);
        LogXMLFormatter(const OStreamHandle stream,
            const std::string xslPath, 
            const MessageLevels levels = ML_DEFAULT);
        virtual ~LogXMLFormatter();
    protected:
        const std::string _xslPath;
    protected:
        void initOutput();
    public:
        virtual LogTargetBase &operator<< (const LogStream &logStream);
};

typedef std::vector<LogTargetBase*> LogTargets;

class Log {
    public:
        Log();
        virtual ~Log();
    private:
        LogTargets *_targets;
        timespec _startTime;
    protected:
        void append(const LogStream &toAppend);
        void submitLogStream(LogStream *stream);
    public:
        void addLogTarget(LogTargetBase *target);
        const timespec &getStartTime() const { return _startTime; };
    friend class LogStream;
};

typedef boost::shared_ptr<Log> LogHandle;

std::ostream &submit(std::ostream &os);
LogStream &operator<<(Log &log, MessageLevel level);
extern Log log;

}
}
