#pragma once

namespace Cool {

struct Message {
    enum class Severity {
        Info,
        Warn, 
        Error
    };
    
    Severity severity;
    std::string category;
	std::string body;
};

}