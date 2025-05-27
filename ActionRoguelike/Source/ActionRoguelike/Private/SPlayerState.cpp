// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	Credits = 0;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	
	int32 NewCredits = Credits;
	NewCredits += Delta;

	Credits = NewCredits;
	
	OnCreditsChanged.Broadcast(this, NewCredits,Delta);
}

