// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterCharacter.h"
#include "ShooterWeapon.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "ShooterGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h" 
#include "Blueprint/UserWidget.h"

AShooterCharacter::AShooterCharacter()
{
	// create the noise emitter component
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Pawn Noise Emitter"));

	// configure movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// reset HP to max
	CurrentHP = MaxHP;

	// update the HUD
	OnDamaged.Broadcast(1.0f);
}

void AShooterCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the respawn timer
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// base class handles move, aim and jump inputs
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AShooterCharacter::DoStartFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AShooterCharacter::DoStopFiring);

		// Switch weapon (Next)
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &AShooterCharacter::DoSwitchWeapon);

		// Switch weapon (Previous)
		EnhancedInputComponent->BindAction(PreviousWeaponAction, ETriggerEvent::Triggered, this, &AShooterCharacter::DoSwitchWeaponPrevious);

		// Reload gun
		EnhancedInputComponent->BindAction(ReloadWeaponAction, ETriggerEvent::Triggered, this, &AShooterCharacter::DoReloadWeapon);

		// Weapon Wheel (Hold to Show, Release to Hide)
		EnhancedInputComponent->BindAction(WeaponWheelAction, ETriggerEvent::Started, this, &AShooterCharacter::ShowWeaponWheel);
		EnhancedInputComponent->BindAction(WeaponWheelAction, ETriggerEvent::Completed, this, &AShooterCharacter::HideWeaponWheel);

		// The Pause Action
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AShooterCharacter::TogglePauseMenu);
	}

}

float AShooterCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// ignore if already dead
	if (CurrentHP <= 0.0f)
	{
		return 0.0f;
	}

	// Reduce HP
	CurrentHP -= Damage;

	// Have we depleted HP?
	if (CurrentHP <= 0.0f)
	{
		Die();
	}

	// update the HUD
	OnDamaged.Broadcast(FMath::Max(0.0f, CurrentHP / MaxHP));

	return Damage;
}

void AShooterCharacter::DoStartFiring()
{
	// fire the current weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFiring();
	}
}

void AShooterCharacter::DoStopFiring()
{
	// stop firing the current weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFiring();
	}
}

void AShooterCharacter::EquipSpecificWeapon(AShooterWeapon* WeaponToEquip)
{
	if (!WeaponToEquip) return;
	if (CurrentWeapon == WeaponToEquip) return;
	if (!OwnedWeapons.Contains(WeaponToEquip)) return;

	// Put away old weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->DeactivateWeapon();
	}

	// Equip new weapon
	CurrentWeapon = WeaponToEquip;
	CurrentWeapon->ActivateWeapon();
}

void AShooterCharacter::DoSwitchWeapon()
{
	// ensure we have at least two weapons two switch between
	if (OwnedWeapons.Num() > 1)
	{
		CurrentWeapon->DeactivateWeapon();

		int32 WeaponIndex = OwnedWeapons.Find(CurrentWeapon);

		if (WeaponIndex == OwnedWeapons.Num() - 1)
		{
			WeaponIndex = 0;
		}
		else {
			++WeaponIndex;
		}

		CurrentWeapon = OwnedWeapons[WeaponIndex];

		CurrentWeapon->ActivateWeapon();
	}
}

void AShooterCharacter::DoSwitchWeaponPrevious()
{
	if (OwnedWeapons.Num() > 1)
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->DeactivateWeapon();
		}

		int32 WeaponIndex = OwnedWeapons.Find(CurrentWeapon);


		if (WeaponIndex <= 0)
		{
			WeaponIndex = OwnedWeapons.Num() - 1;
		}
		else
		{
			--WeaponIndex;
		}

		CurrentWeapon = OwnedWeapons[WeaponIndex];

		if (CurrentWeapon)
		{
			CurrentWeapon->ActivateWeapon();
		}
	}
}

void AShooterCharacter::DoReloadWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void AShooterCharacter::AttachWeaponMeshes(AShooterWeapon* Weapon)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::KeepRelative, false);

	// attach the weapon actor
	Weapon->AttachToActor(this, AttachmentRule);

	// attach the weapon meshes
	Weapon->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);
	
}

void AShooterCharacter::PlayFiringMontage(UAnimMontage* Montage)
{
	
}

void AShooterCharacter::AddWeaponRecoil(float Recoil)
{
	Recoil = -Recoil;
	// apply the recoil as reverse of pitch input
	AddControllerPitchInput(Recoil);
}

void AShooterCharacter::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	OnBulletCountUpdated.Broadcast(MagazineSize, CurrentAmmo);
}

FVector AShooterCharacter::GetWeaponTargetLocation()
{
	// trace ahead from the camera viewpoint
	FHitResult OutHit;

	const FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector End = Start + (GetFirstPersonCameraComponent()->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	// return either the impact point or the trace end
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void AShooterCharacter::AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass)
{
	// do we already own this weapon?
	AShooterWeapon* OwnedWeapon = FindWeaponOfType(WeaponClass);

	if (!OwnedWeapon)
	{
		// spawn the new weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

		AShooterWeapon* AddedWeapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponClass, GetActorTransform(), SpawnParams);

		if (AddedWeapon)
		{
			// Add the weapon to the list
			OwnedWeapons.Add(AddedWeapon);

			// Sort the array based on the WeaponSlotPriority variable
			OwnedWeapons.Sort([](const AShooterWeapon& A, const AShooterWeapon& B)
				{
					return A.GetWeaponSlotPriority() < B.GetWeaponSlotPriority();
				});

			// if we have an existing weapon, deactivate it
			if (CurrentWeapon)
			{
				CurrentWeapon->DeactivateWeapon();
			}

			// switch to the new weapon
			// current logic forces auto-switch to the new pickup. could modify it later.
			CurrentWeapon = AddedWeapon;
			CurrentWeapon->ActivateWeapon();
		}
	}
}
void AShooterCharacter::OnWeaponActivated(AShooterWeapon* Weapon)
{
	// update the bullet counter
	OnBulletCountUpdated.Broadcast(Weapon->GetMagazineSize(), Weapon->GetBulletCount());

	// set the character mesh AnimInstances
	GetFirstPersonMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
}

void AShooterCharacter::OnWeaponDeactivated(AShooterWeapon* Weapon)
{
	// unused
}

void AShooterCharacter::OnSemiWeaponRefire()
{
	// unused
}

AShooterWeapon* AShooterCharacter::FindWeaponOfType(TSubclassOf<AShooterWeapon> WeaponClass) const
{
	// check each owned weapon
	for (AShooterWeapon* Weapon : OwnedWeapons)
	{
		if (Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}

	// weapon not found
	return nullptr;

}

void AShooterCharacter::Die()
{
	// deactivate the weapon
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->DeactivateWeapon();
	}

	// increment the team score
	if (AShooterGameMode* GM = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->IncrementTeamScore(TeamByte);
	}
		
	// stop character movement
	GetCharacterMovement()->StopMovementImmediately();

	// disable controls
	DisableInput(nullptr);

	// reset the bullet counter UI
	OnBulletCountUpdated.Broadcast(0, 0);

	// call the BP handler
	BP_OnDeath();

	// schedule character respawn
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AShooterCharacter::OnRespawn, RespawnTime, false);
}

void AShooterCharacter::OnRespawn()
{
	// destroy the character to force the PC to respawn
	Destroy();
}

void AShooterCharacter::ShowWeaponWheel()
{
	if (!IsValid(this) || !IsLocallyControlled()) return;

	if (!WeaponWheelClass) return;

	if (!IsValid(WeaponWheelWidget))
	{
		if (UWorld* World = GetWorld())
		{
			WeaponWheelWidget = CreateWidget<UUserWidget>(World, WeaponWheelClass);
		}
	}

	// Viewport Logic
	if (IsValid(WeaponWheelWidget))
	{
		// IMPORTANT: Only run this logic if it is NOT already on screen.
		if (!WeaponWheelWidget->IsInViewport())
		{
			WeaponWheelWidget->AddToViewport();

			AController* BaseController = GetController();

			if (APlayerController* PC = Cast<APlayerController>(BaseController))
			{
				if (IsValid(PC))
				{
					PC->bShowMouseCursor = true;

					int32 ScreenX, ScreenY;
					PC->GetViewportSize(ScreenX, ScreenY);
					PC->SetMouseLocation(ScreenX / 2, ScreenY / 2);

					FInputModeGameAndUI InputMode;
					if (IsValid(WeaponWheelWidget))
					{
						InputMode.SetWidgetToFocus(WeaponWheelWidget->TakeWidget());
					}
					InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

					PC->SetInputMode(InputMode);
				}
			}
		}
	}
}

void AShooterCharacter::HideWeaponWheel()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (IsValid(PC))
		{
			PC->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
		}
	}

	if (IsValid(WeaponWheelWidget))
	{
		WeaponWheelWidget->RemoveFromParent();
	}
}

void AShooterCharacter::TogglePauseMenu()
{
	// Safety Checks
	if (!IsValid(this) || !IsLocallyControlled()) return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!IsValid(PC)) return;

	if (!PauseMenuClass) return;

	if (!IsValid(PauseMenuWidget))
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuClass);
	}

	if (IsValid(PauseMenuWidget))
	{
		if (PauseMenuWidget->IsInViewport())
		{
			PauseMenuWidget->RemoveFromParent();

			UGameplayStatics::SetGamePaused(GetWorld(), false);

			// Input Mode: Game Only (Hide Cursor)
			PC->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
		}
		else
		{
			PauseMenuWidget->AddToViewport();

			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// Input Mode: UI (Show Cursor)
			PC->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
		}
	}
}