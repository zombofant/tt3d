#include <boost/smart_ptr/shared_ptr.hpp>
#include <istream>
#include <ostream>
#include <sstream>
#include "vfs/VFS.hpp"

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

struct LogTarget {
    const OStreamHandle targetStream;
    const MessageLevels levels;
    
    LogTarget(const OStreamHandle aTargetStream, const MessageLevels aLevels):
        targetStream(aTargetStream),
        levels(aLevels) {};
};

typedef std::vector<LogTarget*> LogTargets;

class Log;

struct LogStream: public std::ostringstream {
    public:
        LogStream(const std::string initial, Log *submitTo, const MessageLevel level);
    private:
        Log *_submitTo;
        const MessageLevel _level;
    public:
        const MessageLevel getLevel() const { return _level; };
        void submit();
};

class Log {
    public:
        Log();
        virtual ~Log();
    private:
        LogTargets *_targets;
    protected:
        void append(const std::string toAppend, const MessageLevel level);
        void submitLogStream(LogStream *stream);
    public:
        void addLogTarget(const OStreamHandle stream, const MessageLevels levels = ML_DEFAULT);
        void detachLogTarget(const OStreamHandle stream);
        
        
        
    friend class LogStream;
};

std::ostream &submit(std::ostream &os);
LogStream &operator<<(Log *log, MessageLevel level);

}
}
