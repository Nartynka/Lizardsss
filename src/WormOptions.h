#pragma once

#include <string>

namespace WormOptions
{
	enum Face
	{
		Smile,
		WideSimle,
		Sad,
		NumElements,
	};

	const char* GetFaceString(Face fEnumType)
	{
		switch (fEnumType)
		{
		case WormOptions::Smile:
			return "Smile :)";
			break;
		case WormOptions::WideSimle:
			return "Wide Smile :D";
			break;
		case WormOptions::Sad:
			return "Sad :(";
			break;
		default:
			break;
		}
	}

	enum Eyes
	{
		Default
	};
}