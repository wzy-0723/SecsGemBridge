#pragma once
#include <iostream>
#include <filesystem>

namespace why
{
	/**
	* @author	why
	* @brief 为当前路径拼接构建路径
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::string AppendBuildPath(const std::string& path);

	/**
	* @author	why
	* @brief 为当前路径拼接构建路径
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::string GetExecutableDirectory();

	/**
	* @author	why
	* @brief	链式编程为源路径添加子路径
	*/
	class PathAppender
	{
	public:		
		std::string GetPath() { return m_strPath; };
		PathAppender& AppendChildPath(const std::string& strChildPath)
		{
			m_strPath += std::filesystem::path::preferred_separator;
			m_strPath += strChildPath;
			return *this;
		}
		PathAppender& SetSourcePath(const std::string& strSourcePath)
		{
			Clear();
			m_strPath = strSourcePath;
			return *this;
		}
	private:
		void Clear() { m_strPath = ""; };
		std::string m_strPath;
	};


	/**
	* @brief 从UTF8编码的文件名获取文件路径对象
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::filesystem::path UTF8ToPath(const std::string& strUTF8);

	/**
	* @brief 从文件对象获取UTF-8编码的文件名称
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::string PathToUTF8(const std::filesystem::path& filePath);
}