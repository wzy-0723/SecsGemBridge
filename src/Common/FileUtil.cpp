#include "FileUtil.h"
#include "TextUtil.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <libgen.h>
#include <cstdlib>
#endif
namespace why
{
	std::string AppendBuildPath(const std::string& path)
	{
		PathAppender pathAppender;
		const std::string& res = pathAppender.SetSourcePath(GetExecutableDirectory()).
			AppendChildPath(path).
			GetPath();

		return res;
	}

	std::string GetExecutableDirectory()
	{
#if defined(_WIN32) || defined(_WIN64)
		WCHAR buf[4096] = { 0 };
		DWORD ret = GetModuleFileNameW(NULL, buf, ARRAYSIZE(buf));
		if (ret == 0)
			return "";
		std::filesystem::path exePath(buf);

		const std::string& res = exePath.parent_path().string();
		return res;

#elif defined(__linux__)
		// Linux /proc/self/exe
		char buf[PATH_MAX] = { 0 };
		ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
		if (len <= 0)
			return "";
		fs::path exePath(buf);
		return exePath.parent_path().string();
#endif
	}



	std::filesystem::path UTF8ToPath(const std::string& strUTF8)
	{
#ifdef WIN32
		std::wstring	strFileName = UTF8Decode(strUTF8);

		return std::filesystem::path(strFileName);
#else
		return std::filesystem::path(strUTF8);
#endif
	}

	std::string PathToUTF8(const std::filesystem::path& filePath)
	{
#ifdef WIN32
		std::wstring	strFileName = filePath.wstring();

		return UTF8Encode(strFileName);
#else
		return filePath.u8string();
#endif
	}
}