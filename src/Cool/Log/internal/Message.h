#pragma once

namespace Cool {

struct Message {
    enum class Severity {
        Info,
        Warn, 
        Error
    };

    Message(Severity severity, std::string_view category, std::string_view body)
        : severity{severity}, category{category}, body{body}, timestamp{std::chrono::system_clock::now()}
    {
    }
    
    Severity severity;
    std::string category;
	std::string body;
    std::chrono::system_clock::time_point timestamp;
};

}