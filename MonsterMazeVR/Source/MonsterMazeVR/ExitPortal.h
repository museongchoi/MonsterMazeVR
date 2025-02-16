// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitPortal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelComplete);

UCLASS()
class MONSTERMAZEVR_API AExitPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitPortal();

	UPROPERTY(BlueprintAssignable, Category = Callback)
	FLevelComplete LevelComplete;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void OnPortalBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
