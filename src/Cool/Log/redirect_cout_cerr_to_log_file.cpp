#include "redirect_cout_cerr_to_log_file.hpp"
#include "Log.hpp"

namespace Cool {

namespace {
class Stringbuf_LogInternal : public std::stringbuf {
public:
    explicit Stringbuf_LogInternal(bool is_cout)
        : _is_cout{is_cout}
    {}

    ~Stringbuf_LogInternal() override
    {
        sync();
    }

    int sync() override
    {
        std::string msg = str();
        if (!msg.empty())
        {
            if (_is_cout)
                Log::internal_info("std::cout", msg);
            else
                Log::internal_warning("std::cerr", msg);
            str("");
        }
        return 0;
    }

private:
    bool _is_cout{};
};
} // namespace

void redirect_cout_cerr_to_log_file()
{
    {
        static auto buff   = Stringbuf_LogInternal{true /*is_cout*/};
        static auto stream = std::ostream{&buff};
        std::cout.rdbuf(stream.rdbuf());
    }
    {
        static auto buff   = Stringbuf_LogInternal{false /*is_cout*/};
        static auto stream = std::ostream{&buff};
        std::cerr.rdbuf(stream.rdbuf());
    }
}

} // namespace Cool