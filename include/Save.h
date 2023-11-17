#pragma once
#ifndef SAVE_H
#define SAVE_H

#include "Common.h"
#include <EEPROM.h>

const uint16 FLASH_SIZE = 65535; // 520;

class Save
{
private:
	class Header
	{
	public:
		uint8 Initialized;
	};

	template <typename T>
	class ObjectHeader
	{
	public:
		uint16 Size;
		T Object;
	};

public:
	static void Initialize(void)
	{
		EEPROM.begin(FLASH_SIZE);

		Header header;
		ReadHeader(&header);

		if (header.Initialized == 255)
			return;

		Delete();

		Header heaedr;
		header.Initialized = 255;
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
				ASSERT(objectHeader.Size == 0 || objectHeader.Size == sizeof(T), "Save", "Object stored int the slot %i is having %ib, but the incoming object size is %ib", Index, objectHeader.Size, sizeof(T));

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