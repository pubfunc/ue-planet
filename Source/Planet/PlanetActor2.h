// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "UDynamicMesh.h"
#include "PlanetActor2.generated.h"

/**
 *
 */
UCLASS()
class PLANET_API APlanetActor2 : public AGeneratedDynamicMeshActor
{
	GENERATED_BODY()

public:
	/**
	 * This event will be fired to notify the BP that the generated Mesh should
	 * be rebuilt. GeneratedDynamicMeshActor BP subclasses should rebuild their
	 * meshes on this event, instead of doing so directly from the Construction Script.
	 */
	void OnRebuildGeneratedMesh(UDynamicMesh* TargetMesh);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
