#include "debug.h"

DEBUG * DEBUG::_debug_instance = nullptr;

DEBUG::DEBUG()
{
    _debug_log_file = fopen(DEBUG_FILE, "w+");
}
DEBUG::~DEBUG()
{
    if(_debug_log_file && _debug_log_file != stderr) {
        fclose(_debug_log_file);
    }
}
DEBUG * DEBUG::GetInstance()
{
    if(!_debug_instance) {
        _debug_instance = new DEBUG();
        if(!_debug_instance) {
            return nullptr;
        }
    }
    return _debug_instance;
}
FILE * DEBUG::GetLogFile()
{
    return _debug_log_file;
}
