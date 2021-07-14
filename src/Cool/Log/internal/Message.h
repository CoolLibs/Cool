#pragma once

struct Message {
    enum class Type {
        Info,
        Warn, 
        Error
    };
    
    Type type;
    std::string category;
	std::string body;
};