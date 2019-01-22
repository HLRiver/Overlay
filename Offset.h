// UObject:FName
auto ComparisonIndex = 0x0018;

/* ---------------------------------*/

// UEngine:UGameViewportClient*
auto ViewportClient = 0x05A8;

// UGameViewportClient:UWorld*
auto World = 0x0090;

// UWorld:ULevel*
auto PersistentLevel = 0x00F8;

// ULevel:TArray<AActor*>
auto LevelArray = 0x0088;

/* ---------------------------------*/

// UGameViewportClient:UGameInstance*
auto GameInstance = 0x0098;

// UGameInstance:TArray<ULocalPlayer*>
auto LocalPlayers = 0x0038;

// UPlayer:AController*
auto PlayerController = 0x0030;

// AController:APawn*
auto Pawn = 0x0468;

/* ---------------------------------*/

// APawn:USceneComponent*
auto RootComponent = 0x0258;

// USceneComponent:FTransform
auto ComponentToWorld = 0x00D0;

/* ---------------------------------*/

// APlayerController:APlayerCameraManager*
auto PlayerCameraManager = 0x04D0;

// APlayerCameraManager:FCameraCacheEntry
auto CameraCache = 0x04B0;

// FCameraCacheEntry:FMinimalViewInfo
auto POV = 0x0008;

/* ---------------------------------*/

// AShooterCharacter:FString
auto PlayerName = 0x1320;

// APrimalCharacter:UPrimalCharacterStatusComponent*
auto MyCharacterStatusComponent = 0x0C90;

// UPrimalCharacterStatusComponent:int
auto BaseCharacterLevel = 0x0BEC;