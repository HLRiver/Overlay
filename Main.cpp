#include <Windows.h>
#include "Class.h"
#include "Offset.h"

auto X = 800, Y = 600, B = 32;
bool Debug, Misc;

int Draw(DWORD_PTR ULevel) {
	auto AController = Read<DWORD_PTR>(UPlayer + PlayerController);
	auto ACameraManager = Read<DWORD_PTR>(AController + PlayerCameraManager);
	auto APawn = Read<DWORD_PTR>(AController + Pawn);
	auto ActorArray = Read<TArray>(ULevel + LevelArray);

	for (int i = 0; i < ActorArray.Count; ++i) {
		auto Actor = Read<DWORD_PTR>(ActorArray.Data + i * sizeof(DWORD_PTR));

		if (!Actor)
			continue;

		if (Actor == APawn)
			continue;

		auto ObjectIndex = Read<DWORD>(Actor + ComparisonIndex);
		auto ElementsPerChunk = 16 * 1024;
		auto NamesIndex = Read<DWORD_PTR>(GNames + ObjectIndex / ElementsPerChunk * sizeof(DWORD_PTR));
		auto NamesPointer = Read<DWORD_PTR>(NamesIndex + ObjectIndex % ElementsPerChunk * sizeof(DWORD_PTR));
		string ActorName = Read<FNameEntry>(NamesPointer).AnsiName;

		if (ActorName.find("BP_") == -1)
			continue;

		auto Name = ActorName;
		auto Color = Common;

		/* --------- Sample list --------- */

		// Debug
		if (Debug) {
		// Player
		} else if (ActorName.find("PlayerPirate") != -1) {
			auto PlayerInfo = Read<DWORD_PTR>(Actor + PlayerState);
			auto PlayerIndex = Read<DWORD_PTR>(PlayerInfo + PlayerName);
			wstring PlayerString = Read<FString>(PlayerIndex).WideName;
			Name = string(PlayerString.begin(), PlayerString.end());
			Color = Player;
		// Ships
		} else if (ActorName.find("ShipTemplate") != -1 || ActorName.find("ShipNetProxy") != -1) {
			Name = "Ship";
			Color = Ship;
		// Shipwrecks
		} else if (ActorName.find("Shipwreck_01_a_NetProxy") != -1) {
			Name = "Shipwreck";
			Color = Crate;
		// Chests
		} else if (ActorName.find("TreasureChest") != -1 && ActorName.find("ItemInfo") == -1) {
			if (ActorName.find("Common") != -1) {
				Name = "Castaways Chest";
				Color = Common;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Castaways Chest";
			} else if (ActorName.find("Rare") != -1) {
				Name = "Seafarers Chest";
				Color = Rare;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Seafarers Chest";
			} else if (ActorName.find("Legendary") != -1) {
				Name = "Marauders Chest";
				Color = Legendary;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Marauders Chest";
			} else if (ActorName.find("Mythical") != -1) {
				Name = "Captains Chest";
				Color = Mythical;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Captains Chest";
			} else if (ActorName.find("PirateLegend") != -1) {
				Name = "Chest of Legends";
				Color = Mythical;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Chest of Legends";
			} else if (ActorName.find("Drunken") != -1) {
				Name = "Chest of Thousand Grogs";
				Color = Mythical;
			} else if (ActorName.find("Weeping") != -1) {
				Name = "Chest of Sorrow";
				Color = Mythical;
			} else if (ActorName.find("Fort") != -1) {
				Name = "Stronghold Chest";
				Color = Mythical;
			}
		// Skeleton
		} else if (ActorName.find("Skeleton") != -1 && ActorName.find("Pawn") != -1) {
			Name = "Skeleton";
			Color = Enemy;
		// Skeleton Fort
		} else if (ActorName.find("SkellyFort") != -1) {
			Color = Cloud;
			Name = "Skeleton Fort";
		// Chickens
		} else if (Misc && ActorName.find("BP_Chicken") != -1) {
			if (ActorName.find("Common") != -1) {
				Name = "White Chicken";
				Color = Common;
			} else if (ActorName.find("Rare") != -1) {
				Name = "Red Speckled Chicken";
				Color = Rare;
			} else if (ActorName.find("Legendary") != -1) {
				Name = "Black Plumed Chicken";
				Color = Legendary;
			} else if (ActorName.find("Mythical") != -1) {
				Name = "Golden Chicken";
				Color = Mythical;
			}
		// Pigs
		} else if (Misc && ActorName.find("BP_Pig") != -1) {
			if (ActorName.find("Common") != -1) {
				Name = "Pink Pig";
				Color = Common;
			} else if (ActorName.find("Rare") != -1) {
				Name = "Black Spotted Pig";
				Color = Rare;
			} else if (ActorName.find("Legendary") != -1) {
				Name = "Black Coated Pig";
				Color = Legendary;
			} else if (ActorName.find("Mythical") != -1) {
				Name = "Gold Striped Pig";
				Color = Mythical;
			}
		// Snakes
		} else if (Misc && ActorName.find("BP_Snake") != -1) {
			if (ActorName.find("Common") != -1) {
				Name = "Red Striped Snake";
				Color = Common;
			} else if (ActorName.find("Rare") != -1) {
				Name = "Blue Dappled Snake";
				Color = Rare;
			} else if (ActorName.find("Legendary") != -1) {
				Name = "Black Scaled Snake";
				Color = Legendary;
			} else if (ActorName.find("Mythical") != -1) {
				Name = "Golden Snake";
				Color = Mythical;
			}
		} else {
			continue;
		}

		/* ------------------------------- */

		auto ActorRootComponent = Read<DWORD_PTR>(Actor + RootComponent);
		auto ActorComponent = Read<FTransform>(ActorRootComponent + ComponentToWorld);
		auto PlayerCamera = Read<FMinimalViewInfo>(ACameraManager + CameraCache + POV);

		auto Location = ActorComponent.Translation - PlayerCamera.Location;
		Name += " [" + to_string((int)sqrtf(Location.Dot(Location)) / 100) + "m]";
		wstring NameWide(Name.begin(), Name.end());

		auto Project = WorldToScreen(Location, PlayerCamera, X / 2.f, Y / 2.f);
		RenderTarget->SetTransform(Matrix3x2F::Translation(Project.X, Project.Y));
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, 0, 0), Color);
	}

	return 0;
}

int Render(HWND Window) {
	auto Target = FindWindow(0, GameName);
	if (!Target)
		exit(1);

	DwmGetWindowAttribute(Target, DWMWA_EXTENDED_FRAME_BOUNDS, &Frame, sizeof(Frame));
	X = Frame.right - Frame.left;
	Y = Frame.bottom - Frame.top - B;
	MoveWindow(Window, Frame.left, Frame.top + B, X, Y, 0);

	RenderTarget->Resize(SizeU(X, Y));
	RenderTarget->BeginDraw();
	RenderTarget->Clear();

	if (!UEngine) {
		if (Setup()) {
			UViewport = Read<DWORD_PTR>(UEngine + ViewportClient);
			auto UGameInstance = Read<DWORD_PTR>(UViewport + GameInstance);
			auto ULocalPlayer = Read<DWORD_PTR>(UGameInstance + LocalPlayers);
			UPlayer = Read<DWORD_PTR>(ULocalPlayer);
		}
	}

	if (UViewport) {
		auto UWorld = Read<DWORD_PTR>(UViewport + World);
		if (UWorld) {
			Draw(Read<DWORD_PTR>(UWorld + PersistentLevel));
		}
	}

	// Crosshair
	RenderTarget->SetTransform(Matrix3x2F::Identity());
	RenderTarget->DrawLine(Point2F(X / 2.f + 10.f, Y / 2.f), Point2F(X / 2.f - 10.f, Y / 2.f), Common, 1.5f);
	RenderTarget->DrawLine(Point2F(X / 2.f, Y / 2.f + 10.f), Point2F(X / 2.f, Y / 2.f - 10.f), Common, 1.5f);
	RenderTarget->EndDraw();

	// F5 Misc
	if (GetAsyncKeyState(VK_F5) & 1) {
		if (Misc)
			Misc = false;
		else
			Misc = true;
	// F8 Debug
	} else if (GetAsyncKeyState(VK_F8) & 1) {
		if (Debug)
			Debug = false;
		else
			Debug = true;
	// F12 Exit
	} else if (GetAsyncKeyState(VK_F12) & 1) {
		exit(1);
	}

	Sleep(5);

	return 0;
}

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_CREATE:
			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);
			auto RenderProperties = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
			Factory->CreateHwndRenderTarget(RenderProperties, HwndRenderTargetProperties(Window, SizeU(X, Y)), &RenderTarget);
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&DWriteFactory);
			DWriteFactory->CreateTextFormat(L"Tahoma", 0, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12, L"en-us", &TextFormat);

			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::Firebrick), &Player);
			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::LimeGreen), &Ship);
			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::White), &Common);
			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::Blue), &Rare);
			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::Purple), &Legendary);
			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::Orange), &Mythical);
			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::SlateBlue), &Cloud);
			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::Silver), &Crate);
			RenderTarget->CreateSolidColorBrush(ColorF(ColorF::Red), &Enemy);
			return 0;

		case WM_PAINT:
			Render(Window);
			return 0;

		case WM_DESTROY:
			exit(1);
	}

	return DefWindowProc(Window, Message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE, LPSTR, INT) {
	MSG Message;
	MARGINS Margins;

	WNDCLASS wclass = {};
	wclass.lpfnWndProc = WindowProc;
	wclass.hInstance = Instance;
	wclass.lpszClassName = ItemName;
	RegisterClass(&wclass);

	auto Overlay = CreateWindowEx(WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST, ItemName, ItemName, WS_POPUP | WS_VISIBLE, 0, 0, X, Y, 0, 0, Instance, 0);
	SetLayeredWindowAttributes(Overlay, 0, 255, LWA_ALPHA);
	DwmExtendFrameIntoClientArea(Overlay, &Margins);

	while (GetMessage(&Message, Overlay, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return 0;
}