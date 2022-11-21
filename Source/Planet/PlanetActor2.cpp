// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetActor2.h"

void APlanetActor2::OnRebuildGeneratedMesh(UDynamicMesh* TargetMesh)
{
    UE_LOG(LogTemp, Display, TEXT("APlanetActor2::OnRebuildGeneratedMesh"));
}

void APlanetActor2::BeginPlay()
{
    UE_LOG(LogTemp, Display, TEXT("APlanetActor2::BeginPlay"));
}