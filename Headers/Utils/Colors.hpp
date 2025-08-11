#ifndef __COLORS_HPP__
#define __COLORS_HPP__

namespace Vox
{
	namespace Utils
	{
		namespace Colors
		{
			constexpr const char *RESET = "\e[0m";
			constexpr const char *LWHITE = "\e[1;97m";
			constexpr const char *RED = "\e[1;31m";
			constexpr const char *LRED = "\e[1;91m";
			constexpr const char *GREEN = "\e[1;32m";
			constexpr const char *LGREEN = "\e[1;92m";
			constexpr const char *YELLOW = "\e[1;33m";
			constexpr const char *LYELLOW = "\e[1;93m";
			constexpr const char *DBLUE = "\e[1;34m";
			constexpr const char *LBLUE = "\e[1;94m";
			constexpr const char *MAGENTA = "\e[1;35m";
			constexpr const char *CYAN = "\e[1;36m";
		} // namespace Colors
	} // namespace Utils

} // namespace Vox

#endif // __COLORS_HPP__