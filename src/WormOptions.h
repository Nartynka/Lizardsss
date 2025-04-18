#pragma once

#include <string>

namespace WormOptions
{
	static const char* FaceStrings[] = { "Smile :)", "Wide Smile :D", "No Smile :(" };
	static const char* EyesStrings[] = { "Default" };
	enum Face : int
	{
		Smile,
		WideSimle,
		Sad,
		NumFaces,
	};

	enum Eyes
	{
		Default,
		NumEyes,
	};

	struct Options
	{
		SDL_Color outlineColor = { 0 };
		SDL_Color faceColor = { 0 };
		SDL_Color legsColor = { 0 };

		bool hasEyes = false;
		Eyes eyesType = Eyes::Default;

		bool hasFace = false;
		Face faceType = Face::Smile;

		bool hasLegs = false;

		int radius = 30;
		int numParticles = 64;

		bool hasAutoDistance = false;
		float distance = 5.f;

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

		Face MatchFaceStringToEnum(const char* string)
		{
			for (int i = 0; i < NumFaces; ++i)
			{
				if (strcmp(FaceStrings[i], string) == 0)
				{
					return Face(i);
				}
			}
		}

		Eyes MatchEyesStringToEnum(const char* string)
		{
			for (int i = 0; i < NumEyes; ++i)
			{
				if (strcmp(EyesStrings[i], string) == 0)
				{
					return Eyes(i);
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