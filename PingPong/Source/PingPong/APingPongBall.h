#pragma once
#include "CoreMinimal.h"
#include "PingPongGoal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "APingPongBall.generated.h"
UCLASS()
class PINGPONG_API APingPongBall : public AActor
{
	GENERATED_BODY()
	protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* BodyCollision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	float MoveSpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	UParticleSystem* HitEffect;
	UPROPERTY(Replicated)
	bool IsMoving = true;
	float coordinatZ;
	
	////////Lesson 4 /////
	
	FVector StartingPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	APingPongGoal * gate1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	APingPongGoal * gate2;
	////////Lesson 4 /////
	
	
	public:
	// Sets default values for this actor's properties
	APingPongBall();
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Move(float DeltaTime);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartMove();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopMove();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_HitEffect();
	public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void StartMove();
	UFUNCTION(BlueprintCallable)
	void StopMove();
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > &	OutLifetimeProps) const;
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor*
OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
bFromSweep, const FHitResult& SweepResult);
};