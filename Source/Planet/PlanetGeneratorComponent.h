// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UDynamicMesh.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "SimplexNoise.h"

#include "PlanetGeneratorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_API UPlanetGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlanetGeneratorComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void GeneratePlanetMesh(UDynamicMesh* TargetMesh);

private:

	SimplexNoise* Noise;

	UPROPERTY(EditAnywhere)
	int32 Resolution = 128;

	UPROPERTY(EditAnywhere)
	float PlanetRadius = 1.0;

	UPROPERTY(EditAnywhere)
	float NoiseRadius = 1.0f;
	UPROPERTY(EditAnywhere)
	int NoiseOctave = 1;
	UPROPERTY(EditAnywhere)
	float NoiseFrequency = 1.0f;
	UPROPERTY(EditAnywhere)
	float NoiseAmplitude = 1.0f;
	UPROPERTY(EditAnywhere)
	float NoiseLacunarity = 2.0f;
	UPROPERTY(EditAnywhere)
	float NoisePersistence = 0.5f;

	void GenerateFaceMesh(int FaceIndex, FVector3d LocalUp, FDynamicMesh3& Mesh);

};
