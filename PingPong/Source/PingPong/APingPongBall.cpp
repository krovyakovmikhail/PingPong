#include "APingPongBall.h"
#include "PingPongGoal.h"
#include "DrawDebugHelpers.h"
#include "PingPongGameModeBase.h"
#include "Engine/AssetManager.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"





// Sets default values
APingPongBall::APingPongBall()
{
// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Ball BodyCollider"));
	SetRootComponent(BodyCollision);
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Body Mesh"));
	
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	
	SetReplicates(true);
	SetReplicateMovement(true);

	
}
// Called when the game starts or when spawned
void APingPongBall::BeginPlay()
{	
	Super::BeginPlay();

	////////////////////////////// Lesson 4 ++//////////////////////////////
	
	StartingPosition = GetActorLocation();
	
	////////////////////////////// Lesson 4 --//////////////////////////////

	
	//////////Lesson 6 ++//////////
	
	BodyMesh->SetStaticMesh(LoadBodyMesh());

	
	// установка материала в меше после его загрузки
	BodyMesh->SetMaterial(0, LoadBodyMaterial());


	
	//UParticleSystem* HitEffect;
	//ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'
	
	//HitEffect = LoadObject<UParticleSystem>(NULL,	TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"), NULL,	LOAD_None, NULL);
	
	
	
	/////////Lesson 6 --//////////
	
	//////// Lesson 7 ++ ////////
	LoadHitEffect();
	//////// Lesson 7 -- ////////
}

UStaticMesh* APingPongBall::LoadBodyMesh()
{
	if (BodyMeshRef.IsPending())
    {
    const FSoftObjectPath& AssetRef = BodyMeshRef.ToStringReference();
    FStreamableManager& StreamableManager =    UAssetManager::Get().GetStreamableManager();
    BodyMeshRef = Cast<UStaticMesh>(StreamableManager.LoadSynchronous(AssetRef));
    }
    return BodyMeshRef.Get();

}

UMaterialInterface* APingPongBall::LoadBodyMaterial()
{

	if (BodyMeshMaterialRef.IsPending())
	{
		const FSoftObjectPath& AssetRef = BodyMeshMaterialRef.ToStringReference();
		
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		
		BodyMeshMaterialRef = Cast<UMaterialInterface>(StreamableManager.LoadSynchronous(AssetRef));
	}
	return BodyMeshMaterialRef.Get();
}

///////////////////// Lesson 7 ++ //////////////////////////////
void APingPongBall::LoadHitEffect()
{
	FStreamableDelegate LoadEffectDelegate;
	
	LoadEffectDelegate.BindUObject(this, &APingPongBall::OnHitEffectLoaded);
	
	UAssetManager& assetManager = UAssetManager::Get();
	
	FStreamableManager& streamableManager =	assetManager.GetStreamableManager();
	
	AssetHandle = streamableManager.RequestAsyncLoad(HitEffectRef.ToStringReference(),LoadEffectDelegate);
}

void APingPongBall::OnHitEffectLoaded()
{
	GEngine->AddOnScreenDebugMessage(10, 3,FColor::Red, "Load Effect");
	
	UParticleSystem * loadedEffect=  Cast<UParticleSystem>(AssetHandle.Get()->GetLoadedAsset());
	
	if(loadedEffect)   
	{
		HitEffect = loadedEffect;
	}

}
	

///////////////////// Lesson 7 -- //////////////////////////////

// Called every frame
void APingPongBall::Tick(float DeltaTime)
{
Super::Tick(DeltaTime);

	

	if (GetNetMode() != ENetMode::NM_Client)
	{
		Server_Move(DeltaTime);
	}
}




void APingPongBall::StartMove()
{
Server_StartMove();
}
void APingPongBall::StopMove()
{
Server_StopMove();
}
void APingPongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&
OutLifetimeProps) const
{
Super::GetLifetimeReplicatedProps(OutLifetimeProps);
DOREPLIFETIME(APingPongBall, IsMoving);
}



bool APingPongBall::Server_Move_Validate(float DeltaTime)
{
return true;
}
void APingPongBall::Server_Move_Implementation(float DeltaTime)
{

	
FVector forward = GetActorForwardVector();	
FVector currLoc = GetActorLocation();
FVector newLoc = currLoc + forward * MoveSpeed * DeltaTime;
FHitResult hitResult;
	coordinatZ = currLoc.Z;


	
if(!SetActorLocation(newLoc, true, &hitResult))

	/////// Lesson 4 //////////////////
	if (APingPongGoal * gate = Cast<APingPongGoal>(hitResult.Actor))
	{
		if (gate->id == 1)
		{
			SetActorLocation(StartingPosition);			
		// если попали в ворота 1, то очко получает ворота 2			
		
		  if (Gate2)
			{
		  		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, " GOAL!!!");

		  		// Подсчет
		  	
		  		BallPower > 0 ? Gate2->Score = Gate2->Score + BallPower : Gate2->Score = Gate2->Score +1;

		  		//Как только происходит изменение счета вызывается делегат, на который подписан эвент в виджете.
		  		// передается Счет ворот и ИД
		  		
		  		OnChageScore.Broadcast(Gate2->Score, Gate2->id, BallPower);
				BallPower = 0;
				OnChangeBallPoewr.Broadcast(BallPower);
			}

		}
		
		else if(gate->id == 2)
		{
			SetActorLocation(StartingPosition);
			// если попали в ворота 2, то очко получает ворота 1
			
			if (Gate1)
			{
				GEngine->AddOnScreenDebugMessage(10, 1,FColor::Red, " GOAL!!!");
				Gate1->Score =Gate1->Score + 1;

				//
				//Подсчет. 
				
				BallPower > 0 ? Gate2->Score = Gate2->Score + BallPower : Gate2->Score = Gate2->Score +1;
				
				//Как только происходит изменение счета вызывается делегат, на который подписан эвент в виджете.
				// передается Счет ворот и ИД 
				OnChageScore.Broadcast(Gate1->Score, Gate1->id, BallPower);
				BallPower = 0;
				OnChangeBallPoewr.Broadcast(BallPower);
			}
			
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(10, 1,FColor::Yellow, " Cast Filed");
			
		}
	}
	/////// Lesson 4 //////////////////
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball %s Collided with %s"), *GetName(),        *hitResult.GetActor()->GetName());
	
        FVector moveVector = forward - currLoc;
	
        moveVector.Normalize();
	
        FVector resetPosition = currLoc + moveVector * DeltaTime * 5 * MoveSpeed;
	
        DrawDebugDirectionalArrow(GetWorld(), newLoc + moveVector * 300, newLoc, 30, FColor::Green, true, 3.f, 0, 3);
	
        FVector impactCorrection = hitResult.ImpactPoint + hitResult.ImpactNormal * 300;
	
        DrawDebugDirectionalArrow(GetWorld(), hitResult.ImpactPoint, hitResult.ImpactPoint + hitResult.ImpactNormal
        	* 300, 30, FColor::Blue, true, 3, 0, 3);
	
        float AimAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(moveVector, hitResult.ImpactNormal)));
	
        moveVector = moveVector.RotateAngleAxis(AimAtAngle*2, FVector(0,0,1));
	
        FVector newTargetMove = newLoc + moveVector * 300;
	
        newTargetMove.Z = currLoc.Z;
		resetPosition.Z = coordinatZ;
	
        DrawDebugDirectionalArrow(GetWorld(), newLoc, newTargetMove, 30, FColor::Yellow, true, 3.f, 0, 3);
	
        SetActorLocation(resetPosition);
	
        FRotator currRotation = GetActorRotation();
	
        FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currLoc, newTargetMove);
	
        newRotation.Pitch = currRotation.Pitch;
	
        SetActorRotation(newRotation);
	
        Multicast_HitEffect();

		BallPower = BallPower + 1;
		OnChangeBallPoewr.Broadcast(BallPower);
        }
	}
	

bool APingPongBall::Server_StartMove_Validate()
{
return true;
}
void APingPongBall::Server_StartMove_Implementation()
{
IsMoving = true;
}
bool APingPongBall::Server_StopMove_Validate()
{
return true;
}
void APingPongBall::Server_StopMove_Implementation()
{
IsMoving = false;
}
void APingPongBall::Multicast_HitEffect_Implementation()
{
UWorld * world = GetWorld();
if(world && HitEffect)
{
UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,
GetActorLocation());
}

}