// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharMakingUI.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerStat : uint8
{
	Vigor        UMETA(DisplayName = "Vigor"),
	Endurance      UMETA(DisplayName = "Endurance"),
	Intelligence   UMETA(DisplayName = "Intelligence"),
	Strength        UMETA(DisplayName = "Strength")
};

UCLASS()
class GAD2006_FINAL_API UCharMakingUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int SkillPoints = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Vigor = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Endurance = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Intelligence = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Strength = 1;

	UFUNCTION(BlueprintCallable, Category = "Character Creation")
	bool ApplyPoints(EPlayerStat Stat, float Change);

	UFUNCTION(BlueprintCallable)
	bool IsReady();
	
};
