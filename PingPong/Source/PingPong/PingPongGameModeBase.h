// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PingPongGoal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "PingPongGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	protected:
	UPROPERTY()
	class APingPongPlayerController* Player1 = NULL;
	UPROPERTY()
	class APingPongPlayerController* Player2 = NULL;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerStart* Player1Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerStart* Player2Start;

	//////////////////// lesson 4 /////////////////////
		
	int32 ScoreGate1;
	int32 ScoreGate2;
	
	//////////////////// lesson 4 /////////////////////
	public:
	APingPongGameModeBase();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
