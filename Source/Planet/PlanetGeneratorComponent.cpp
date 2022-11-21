// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetGeneratorComponent.h"
#include "UDynamicMesh.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "DynamicMesh/InfoTypes.h"
#include "VectorTypes.h"
#include "SimplexNoise.h"

// Sets default values for this component's properties
UPlanetGeneratorComponent::UPlanetGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPlanetGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UPlanetGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlanetGeneratorComponent::GeneratePlanetMesh(UDynamicMesh *TargetMesh)
{
	UE_LOG(LogTemp, Display, TEXT("UPlanetGeneratorComponent::GeneratePlanetMesh"));

	Noise = new SimplexNoise(
		NoiseFrequency,
		NoiseAmplitude,
		NoiseLacunarity,
		NoisePersistence
	);

	TargetMesh->EditMesh(
		[&, this](FDynamicMesh3 &Mesh)
		{
			Mesh.Clear();
			Mesh.EnableVertexNormals(FVector3f::UnitY());

			FVector3d FaceLocalUps[] = {
				FVector3d::UpVector,
				FVector3d::DownVector,
				FVector3d::LeftVector,
				FVector3d::RightVector,
				FVector3d::ForwardVector,
				FVector3d::BackwardVector,
			};

			int32 VertexIndex = 0;
			int32 TriIndex = 0;

			for (int FaceIndex = 0; FaceIndex < 6; FaceIndex++)
			{
				FVector3d LocalUp = FaceLocalUps[FaceIndex];
				GenerateFaceMesh(FaceIndex, LocalUp, Mesh);
			}

			UE_LOG(
				LogTemp, Display,
				 TEXT("UPlanetGeneratorComponent::GeneratePlanetMesh, HasVertexNormals=%s, HasVertexColors=%s, HasVertexUVs=%s, Valid=%s"),
				 ( Mesh.HasVertexNormals() ? TEXT("true") : TEXT("false") ),
				 ( Mesh.HasVertexColors() ? TEXT("true") : TEXT("false") ),
				 ( Mesh.HasVertexUVs() ? TEXT("true") : TEXT("false") ),
				 ( Mesh.CheckValidity() ? TEXT("true") : TEXT("false") )
			);
		});

	UE_LOG(LogTemp, Display, TEXT("UPlanetGeneratorComponent::GeneratePlanetMesh, triangles=%d"), TargetMesh->GetTriangleCount());
}

void UPlanetGeneratorComponent::GenerateFaceMesh(int FaceIndex, FVector3d LocalUp, FDynamicMesh3& Mesh)
{

	FVector3d AxisA = FVector3d(LocalUp.Y, LocalUp.Z, LocalUp.X);
	FVector3d AxisB = LocalUp.Cross(AxisA);
	int32 FaceVertexIndexStart = FaceIndex * Resolution * Resolution;

	for (int32 y = 0; y < Resolution; y++)
	{
		for (int32 x = 0; x < Resolution; x++)
		{
			FVector2d Percent = FVector2d(x, y) / (Resolution - 1);
			FVector3d PointOnUnitCube =
				LocalUp + ((Percent.X - 0.5) * 2.0 * AxisA) + ((Percent.Y - 0.5) * 2.0 * AxisB);
			FVector3d PointOnUnitSphere = PointOnUnitCube.GetSafeNormal();

			float NoiseComponent = (Noise->fractal(NoiseOctave, PointOnUnitSphere.X, PointOnUnitSphere.Y, PointOnUnitSphere.Z) + 1.0f) * 0.5f;

			FVector3d VertexVector = (PointOnUnitSphere * PlanetRadius) + (PointOnUnitSphere * NoiseComponent * NoiseRadius);
			FVector3f Normal = FVector3f(VertexVector.GetSafeNormal());

			UE::Geometry::FVertexInfo Vertex = UE::Geometry::FVertexInfo(
				VertexVector,
				Normal
			);

			Mesh.AppendVertex(Vertex);
		}
	}

	for (int32 y = 0; y < Resolution - 1; y++)
	{
		for (int32 x = 0; x < Resolution - 1; x++)
		{
			int32 I = FaceVertexIndexStart + (y * Resolution) + x;

			Mesh.AppendTriangle(
				I + Resolution,
				I + Resolution + 1,
				I
			);
			Mesh.AppendTriangle(
				I + Resolution + 1,
				I + 1,
				I
			);
		}
	}
}
