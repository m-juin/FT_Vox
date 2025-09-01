#ifndef __STRINGS_HPP__
#define __STRINGS_HPP__

#include <iostream>

namespace Vox::Utils::Strings
{
    inline bool IsDigit(const std::string &str)
    {
        for (char c : str)
            if (std::isdigit(c) == false)
                return false;
        return true;
    }
} // namespace Vox::Utils::Strings


#endif // __STRINGS_HPP__