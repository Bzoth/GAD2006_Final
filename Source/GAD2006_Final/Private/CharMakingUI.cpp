// Fill out your copyright notice in the Description page of Project Settings.


#include "CharMakingUI.h"

bool UCharMakingUI::ApplyPoints(EPlayerStat Stat, float Change)
{

	int* TargetStat = nullptr;

	if (SkillPoints > Change - 1 )
	{

		switch (Stat)
		{
		case EPlayerStat::Vigor:        TargetStat = &Vigor;        break;
		case EPlayerStat::Endurance:    TargetStat = &Endurance;    break;
		case EPlayerStat::Intelligence: TargetStat = &Intelligence; break;
		case EPlayerStat::Strength:     TargetStat = &Strength;     break;
		}
		
		SkillPoints = SkillPoints - Change;
		*TargetStat = *TargetStat + Change;

		if (*TargetStat == 1 || *TargetStat == 10 || SkillPoints == 0)
		{
			return true;
		}

		return false;
	}
	else
	{
		return false;
	}
}

bool UCharMakingUI::IsReady()
{
	return (SkillPoints == 0);
}
