#pragma once

#include <string>

namespace WormOptions
{
	static const char* FaceStrings[] = { "Smile :)", "Wide Smile :D", "No Smile :(" };
	enum Face : int
	{
		Smile,
		WideSimle,
		Sad,
		NumFaces,
	};

	enum Eyes
	{
		Default
	};

	struct Options
	{
		SDL_Color outlineColor = { 0 };
		SDL_Color faceColor = { 0 };

		bool hasEyes = false;
		Eyes eyesType = Eyes::Default;

		bool hasFace = false;
		Face faceType = Face::Smile;

		int radius = 30;
		int numParticles = 128;

		bool hasAutoDistance = false;
		float distance = 10.f;

		const char* GetEyesPath(Eyes type)
		{
			switch (type)
			{
			case WormOptions::Default:
				return "res/dotw.png";
			default:
				return "";
				break;
			}
		}

		Face MatchStringToEnum(const char* string)
		{
			for (int i = 0; i < NumFaces; ++i)
			{
				if (FaceStrings[i] == string)
				{
					return Face(i);
				}
			}
		}
	};

	//const char* GetFaceString(Face fEnumType)
	//{
	//	switch (fEnumType)
	//	{
	//	case WormOptions::Smile:
	//		return "Smile :)";
	//		break;
	//	case WormOptions::WideSimle:
	//		return "Wide Smile :D";
	//		break;
	//	case WormOptions::Sad:
	//		return "Sad :(";
	//		break;
	//	default:
	//		break;
	//	}
	//}
}