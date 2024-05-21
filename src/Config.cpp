/*MIT License

Copyright (c) 2024 seoulxss

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Config.h"

bool ConfigLib::_Config::Config::AddInteger(const char* key, int value)
{
	if (m_Json.contains(key))
		return false;

	m_Json[key] = value;
	return true;
}

bool ConfigLib::_Config::Config::SetInteger(const char* key, int value)
{
	if (!m_Json.contains(key))
	{
		AddInteger(key, value);
		return true;
	}

	m_Json[key] = value;
	return true;
}

bool ConfigLib::_Config::Config::AddFloat(const char* key, float value)
{
	if (m_Json.contains(key))
		return false;

	m_Json[key] = value;
	return true;
}

bool ConfigLib::_Config::Config::SetFloat(const char* key, float value)
{
	if (!m_Json.contains(key))
	{
		AddFloat(key, value);
		return true;
	}

	m_Json[key] = value;
	return true;
}

bool ConfigLib::_Config::Config::AddArray(const char* key, const std::vector<nlohmann::json>& values)
{
	if (m_Json.contains(key))
		return false;

	nlohmann::json arrayJson = nlohmann::json::array();
	for (const auto& value : values) {
		arrayJson.push_back(value);
	}

	m_Json[key] = arrayJson;
	return true;
}

bool ConfigLib::_Config::Config::SetArray(const char* key, const std::vector<nlohmann::json>& values)
{
	if (!m_Json.contains(key))
	{
		AddArray(key, values);
		return true;
	}

	nlohmann::json arrayJson = nlohmann::json::array();
	for (const auto& value : values) {
		arrayJson.push_back(value);
	}

	m_Json[key] = arrayJson;
	return true;
}

bool ConfigLib::_Config::Config::AddBool(const char* key, bool value)
{
	if (m_Json.contains(key))
		return false;

	m_Json[key] = value;
	return true;
}

bool ConfigLib::_Config::Config::SetBool(const char* key, bool value)
{
	if (!m_Json.contains(key))
	{
		AddBool(key, value);
		return true;
	}

	m_Json[key] = value;
	return true;
}

bool ConfigLib::_Config::Config::AddString(const char* key, const char* value)
{
	if (m_Json.contains(key))
		return false;

	m_Json[key] = value;
	return true;
}

bool ConfigLib::_Config::Config::SetString(const char* key, const char* value)
{
	if (!m_Json.contains(key))
	{
		AddString(key, value);
		return true;
	}

	m_Json[key] = value;
	return true;
}

bool ConfigLib::_Config::Config::SaveConfig()
{
	if (m_FileStream.is_open())
	{

		std::string buff = m_Json.dump();
		m_FileStream.clear();
		m_FileStream << buff;
		m_FileStream.close();
		return true;
	}

	else
	{
		m_FileStream.open(m_FilePath, std::fstream::in | std::fstream::out | std::fstream::trunc);
		if (m_FileStream.is_open())
		{
			std::string buff = m_Json.dump();
			m_FileStream << std::setw(20) << buff;
			m_FileStream.close();
			return true;
		}
		return false;
	}

	return false;
}

bool ConfigLib::_Config::Config::LoadConfig()
{
	if (std::filesystem::exists(m_FilePath))
	{
		if (!m_FileStream.is_open())
			m_FileStream.open(m_FilePath, std::fstream::in | std::fstream::out | std::fstream::app);

		if (m_FileStream.is_open() && m_FileStream.peek() == std::fstream::traits_type::eof())
		{
				m_FileStream.close();
				return false;
		}

		if (m_FileStream.is_open() && m_FileStream.peek() != std::fstream::traits_type::eof())
		{
			m_Json = nlohmann::json::parse(m_FileStream);
			m_FileStream.close();
			return true;
		}

	}
	return false;
}

ConfigLib::ConfigManager::ConfigManager(const wchar_t* ConfigFolderPath) : m_FolderPath(ConfigFolderPath)
{
	if (!std::filesystem::exists(ConfigFolderPath))
		std::filesystem::create_directories(ConfigFolderPath);

	m_Configs.reserve(30);
}

void ConfigLib::ConfigManager::GetAllConfigs()
{
	m_Configs.clear();
	
	std::filesystem::directory_iterator it(m_FolderPath);
	for (const auto& entry : it)
	{
		//SUS
		if (entry.path().extension() == (L".json"))
		{
			m_Configs.emplace(entry.path().filename().c_str(), _Config::Config(entry.path().filename().c_str(), entry.path().wstring().c_str()));
		}
	}
}

ConfigLib::_Config::Config& ConfigLib::ConfigManager::GetConfig(const wchar_t* FileName)
{
	auto it = m_Configs.find(FileName);

	if (it != m_Configs.end())
	{
		return it->second;
	}
}

bool ConfigLib::ConfigManager::AddConfig(const wchar_t* ConfigName)
{
	if (m_Configs.contains(ConfigName))
		return false;

	std::wstring path;
	path.append(m_FolderPath);
	path.append(ConfigName);

	m_Configs.emplace(ConfigName, ConfigLib::_Config::Config(ConfigName, path.c_str()));
	return true;
}

bool ConfigLib::ConfigManager::DeleteConfig(const wchar_t* ConfigName)
{
	if (std::filesystem::exists(m_FolderPath))
	{
		std::wstring str = {m_FolderPath};
		str.append(ConfigName);

		if (std::filesystem::exists(str))
		{
			std::filesystem::remove(str);
			return true;
		}

		m_Configs.erase(ConfigName);
	}
	return false;
}

bool ConfigLib::ConfigManager::DeleteAllConfigs()
{
	if (std::filesystem::exists(m_FolderPath))
	{
		std::filesystem::directory_iterator it(m_FolderPath);

		for (auto& entry : it)
		{
			//sus
			if (entry.path().extension() == (".json"))
			{
				std::filesystem::remove(entry.path());
			}
		}

		m_Configs.clear();
		return true;
	}

	return false;
}

std::vector<std::wstring> ConfigLib::ConfigManager::GetAllConfigEntries()
{
	std::vector<std::wstring> vec;
	vec.reserve(m_Configs.size());

	for (const auto& entries : m_Configs)
	{
		vec.push_back(entries.second.m_FileName);
	}

	return vec;
}

std::wstring ConfigLib::ConfigManager::GetConfigFolderPath() const
{
	return std::wstring(m_FolderPath);
}
