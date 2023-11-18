#pragma once
#ifndef SAVE_H
#define SAVE_H

#include "Common.h"
#include <EEPROM.h>
#include <nvs_flash.h>

const uint16 FLASH_SIZE = 520;
const uint32 INITIALIZED_SIGNATURE = 0xF0F0;

class Save
{
private:
	class Header
	{
	public:
		uint32 Initialized;
	};

	template <typename T>
	class ObjectHeader
	{
	public:
		uint8 Size;
		T Object;
	};

public:
	static void Initialize(void)
	{
		nvs_flash_init();

		EEPROM.begin(FLASH_SIZE);

		Header header;
		ReadHeader(&header);

		if (header.Initialized == INITIALIZED_SIGNATURE)
		{
			Log::WriteInfo("Save", "EEPROM has already intialized");

			return;
		}

		Log::WriteInfo("Save", "Initializing the EEPROM");

		Delete();

		Header heaedr;
		header.Initialized = INITIALIZED_SIGNATURE;
		WriteHeader(&header);
	}

	static void Delete(void)
	{
		for (uint16 i = 0; i < FLASH_SIZE; ++i)
			EEPROM.write(i, 0);
	}

	template <typename T>
	static void Read(uint16 Index, T *Object)
	{
		uint16 address = IndexToAddress<T>(Index);

		ObjectHeader<T> object;
		ReadFromAddress(address, &object);

		*Object = object.Object;
	}

	template <typename T>
	static void Write(uint16 Index, const T *Object)
	{
		uint16 address = IndexToAddress<T>(Index);

		ObjectHeader<T> object;
		object.Size = sizeof(T);
		object.Object = *Object;
		WriteToAddress(address, &object);
	}

private:
	template <typename T>
	static uint16 IndexToAddress(uint16 Index)
	{
		uint16 address = sizeof(Header);

		for (; address < FLASH_SIZE; ++address)
		{
			ObjectHeader<bool> objectHeader;
			ReadFromAddress(address, &objectHeader);

			if (Index-- == 0)
			{
				ASSERT(objectHeader.Size == 0 || objectHeader.Size == sizeof(T), "Save", "Object stored in the slot %i is having %ib, but the incoming object size is %ib", Index, objectHeader.Size, sizeof(T));

				break;
			}

			address += sizeof(ObjectHeader<bool>::Size) + objectHeader.Size;
		}

		ASSERT(address < FLASH_SIZE, "Save", "%i is out range of the slots", Index);

		return address;
	}

	static void ReadHeader(Header *Header)
	{
		ReadFromAddress(0, Header);
	}

	static void WriteHeader(const Header *Header)
	{
		WriteToAddress(0, Header);
	}

	template <typename T>
	static void ReadFromAddress(uint16 Address, T *Object)
	{
		EEPROM.readBytes(Address, Object, sizeof(T));
	}

	template <typename T>
	static void WriteToAddress(uint16 Address, const T *Object)
	{
		EEPROM.writeBytes(Address, Object, sizeof(T));

		EEPROM.commit();
	}
};

#endif