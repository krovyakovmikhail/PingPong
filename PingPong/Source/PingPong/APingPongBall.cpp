#include "APingPongBall.h"
#include "DrawDebugHelpers.h"
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
	BodyMesh->OnComponentBeginOverlap.AddDynamic(this, &APingPongBall::OnMeshOverlapBegin);
	
	SetReplicates(true);
	SetReplicateMovement(true);
}
// Called when the game starts or when spawned
void APingPongBall::BeginPlay()
{
Super::BeginPlay();
}
// Called every frame
void APingPongBall::Tick(float DeltaTime)
{
Super::Tick(DeltaTime);

	

	if (GetNetMode() != ENetMode::NM_Client)
	{
		Server_Move(DeltaTime);
	}
}

////////////////////////////// Lesson 4 //////////////////////////////////////////////
void APingPongBall::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor*
OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
bFromSweep, const FHitResult& SweepResult)
{
	
	UE_LOG(LogTemp, Error, TEXT(" %s collided with %s. "), *GetName(),	*OtherActor->GetName());
}
////////////////////////////// Lesson 4 //////////////////////////////////////////////
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

//	UE_LOG(LogTemp, Display, TEXT("APingPongBall::Server_Move_Implementation"));
	
if(!SetActorLocation(newLoc, true, &hitResult))
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