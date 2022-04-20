#pragma once
#include "CoreMinimal.h"
#include "PingPongGoal.h"
#include "Components/SphereComponent.h"
#include "Engine/StreamableManager.h"

#include "GameFramework/Actor.h"
#include "APingPongBall.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChageScore, int32, Score, int32, GateId, int32, BallPower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeBallPoewr,int32, BallPower); 

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
	UParticleSystem* HitEffect;

	//////////Lesson 6 ++//////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSoftObjectPtr<UStaticMesh> BodyMeshRef;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInterface> BodyMeshMaterialRef;

	//////////Lesson 6 --//////////

	///////////////////// Lesson 7 ++ //////////////////////////////
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UParticleSystem> HitEffectRef;
	
	TSharedPtr<FStreamableHandle> AssetHandle;
	
	///////////////////// Lesson 7 -- //////////////////////////////


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	float MoveSpeed = 100;
	
	
	
	UPROPERTY(Replicated)
	bool IsMoving = true;
	float coordinatZ;
	
	////////Lesson 4 ++ /////
	
	FVector StartingPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate params")
	APingPongGoal * Gate1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate params")
	APingPongGoal * Gate2;

	////Task 3 * 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BallPower")
	int32 BallPower = 0;

	public:
	UPROPERTY(BlueprintAssignable)
	FOnChageScore OnChageScore;
	UPROPERTY(BlueprintAssignable)
	FOnChangeBallPoewr OnChangeBallPoewr;
	////////Lesson 4 --/////

	
	
	
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
	
	//////////Lesson 6 ++//////////

	UStaticMesh* LoadBodyMesh();

	UMaterialInterface* LoadBodyMaterial();
	
	//////////Lesson 6 --//////////

	///////////////////// Lesson 7 ++ //////////////////////////////

	void LoadHitEffect();
	void OnHitEffectLoaded();

	
	
	//////////////////////// Lesson 7  //////////////////////////////

	
	public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void StartMove();
	
	UFUNCTION(BlueprintCallable)
	void StopMove();
	
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > &	OutLifetimeProps) const;
	
};