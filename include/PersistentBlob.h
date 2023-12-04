#pragma once
#ifndef PERSISTENT_BLOB_H
#define PERSISTENT_BLOB_H

#include "Debug.h"
#include <EEPROM.h>
#include <nvs_flash.h>

template <typename T>
class PersistentBlob
{
private:
	class Header
	{
	public:
		uint32 Hash;
		uint16 Size;
		T Object;
	};

public:
	PersistentBlob(const char *Name)
		: m_EEPROM(Name)
	{
		ASSERT(Name != nullptr, "Name cannot be null");

		nvs_flash_init();
		m_EEPROM.begin(sizeof(Header));

		uint32 hash = 0;
		uint8 nameLength = strlen(Name);
		for (uint8 i = 0; i < nameLength; ++i)
			hash += (Name[i] >> 4);

		Read();

		if (m_Header.Hash == hash)
		{
			ASSERT(m_Header.Size == sizeof(T), "PersistentBlob %s has already initialized with %ib, but you're initializing it again with %ib", Name, m_Header.Size, sizeof(T));

			Log::WriteDebug("PersistentBlob", "PersistentBlob %s has already intialized", Name);

			return;
		}

		Log::WriteDebug("PersistentBlob", "Initializing PersistentBlob %s", Name);

		Memory::Set(&m_Header, 0);
		m_Header.Hash = hash;
		m_Header.Size = sizeof(T);

		Write();
	}

	void Delete(void)
	{
		Memory::Set(&m_Header, 0);

		Write();
	}

	void Update(const T &Object)
	{
		m_Header.Object = Object;
		Write();
	}

	const T &Get(void) const
	{
		return m_Header.Object;
	}

private:
	void Read(void)
	{
		m_EEPROM.readBytes(0, &m_Header, sizeof(Header));
	}

	void Write(void)
	{
		m_EEPROM.writeBytes(0, &m_Header, sizeof(Header));
		m_EEPROM.commit();
	}

private:
	EEPROMClass m_EEPROM;
	Header m_Header;
};

#endif