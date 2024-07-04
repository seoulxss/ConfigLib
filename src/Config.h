#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <mutex>

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

namespace ConfigLib
{
	namespace _Config
	{
		class Config
		{
		public:
			Config(const wchar_t* ConfigName, const wchar_t* FilePath) : m_FileName(ConfigName), m_FilePath(FilePath)
			{
				m_FileStream.open(m_FilePath, std::fstream::in | std::fstream::out | std::fstream::app);

				if (m_FileStream.is_open() && m_FileStream.peek() == std::fstream::traits_type::eof())
				{
					m_FileStream.close();
				}

				if (m_FileStream.is_open() && m_FileStream.peek() != std::fstream::traits_type::eof())
				{
					m_Json = nlohmann::json::parse(m_FileStream);
					m_FileStream.close();
				}
			}

			//Move constructor
			Config(Config&& other) noexcept : m_FileName(std::move(other.m_FileName)), m_FilePath(std::move(other.m_FilePath)), m_FileStream(std::move(m_FileStream)), m_Json(std::move(other.m_Json))
			{
			}

			//Move Assignment Operator
			Config& operator=(Config&& other) noexcept
			{
				if (this != &other) {
					std::lock_guard<std::mutex> lock(m_Mutex);
					m_FileName = std::move(other.m_FileName);
					m_FilePath = std::move(other.m_FilePath);
					m_Json = std::move(other.m_Json);
				}
				return *this;
			}

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (int)
			//! @return If it was successfully added (bool)
			bool AddInteger(const char* key, int value);

			//! This function only changes the value
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should change (int)
			//! @return If it was successfully changed (bool)
			bool SetInteger(const char* key, int value);

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (float)
			//! @return If it was successfully added (bool)
			bool AddFloat(const char* key, float value);

			//! This function only changes the value
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should change (float)
			//! @return If it was successfully changed (bool)
			bool SetFloat(const char* key, float value);

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (std::vector)
			//! @return If it was successfully added (bool)
			bool AddArray(const char* key, const std::vector<nlohmann::json>& values);

			//! This function only changes the value
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param values The value which it should add (std::vector)
			//! @return If it was successfully changed (bool)
			bool SetArray(const char* key, const std::vector<nlohmann::json>& values);

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (bool)
			//! @return If it was successfully added (bool)
			bool AddBool(const char* key, bool value);

			//! This function only changes the value
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (bool)
			//! @return If it was successfully changed (bool)
			bool SetBool(const char* key, bool value);

			//! 
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (const char*)
			//! @return If it was successfully added (bool)
			bool AddString(const char* key, const char* value);

			//! This function only changes the value
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @param value The value which it should add (bool)
			//! @return If it was successfully changed (bool)
			bool SetString(const char* key, const char* value);

			//! 
			//! @tparam _Type The returned type (int, bool, float, etc..)
			//! @param key The key which u need, to identify the config (ConfigName)
			//! @return The value or std::nullopt
			template<typename _Type>
			std::optional<_Type> GetValue(const char* key);

			//! Simply saves the config
			//! @return If the saving was succeessfully
			bool SaveConfig();

			//! Loads the json file into memory
			//! @return If the parsing was successfully
			bool LoadConfig();

			//! 
			//! @param Key The key which identifies the value
			//! @return If the deletion was successfully
			bool DeleteEntry(const char* Key);

			//! Get the current Json Object
			//! @return Returns the current Json object
			nlohmann::json* GetJson();

		public:
			std::mutex m_Mutex{};
			std::wstring m_FileName = {};
			std::wstring m_FilePath = {};
			std::fstream m_FileStream = {};
			nlohmann::json m_Json = nullptr;

		};

		template <typename _Type>
		std::optional<_Type> Config::GetValue(const char* key)
		{
			if (!m_Json.contains(key))
				return std::nullopt;

			auto it = m_Json.find(key);
			if (it != m_Json.end())
			{
				_Type val = it.value().get<_Type>();
				return val;
			}
			return std::nullopt;
		}

	}

	class ConfigManager
	{
	public:
		ConfigManager(const wchar_t* ConfigFolderPath);

		//! Simply adds all configs in the folder path to the Hashmap
		void GetAllConfigs();

		//! 
		//! @param FileName The config name 
		//! @return A reference to the config
		ConfigLib::_Config::Config& GetConfig(const wchar_t* FileName);

		//! 
		//! @param ConfigName The config name (needs to be unique)
		//! @return If the config was successfully added
		bool AddConfig(const wchar_t* ConfigName);

		//! 
		//! @param ConfigName The config name (needs to be unique)
		//! @return If the config was successfully removed
		bool DeleteConfig(const wchar_t* ConfigName);

		//! 
		//! @return True if all configs got deleted, false if something fails (mostly wrong path))
		bool DeleteAllConfigs();

		//! 
		//! @return Returns all config names in a vector as a std::wstring
		std::vector<std::wstring> GetAllConfigEntries() const;

		//! 
		//! @return Returns the current folder path
		std::wstring GetConfigFolderPath() const;

	private:
		std::unordered_map<std::wstring, ConfigLib::_Config::Config> m_Configs = {};
		const wchar_t* m_FolderPath = nullptr;

	};
}
