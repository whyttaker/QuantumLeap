// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable) void MenuSetup(int32 NumberPublicConnections = 4, FString TypeOfMatch = FString(TEXT("Duel")), FString LobbyPath = FString("/Game/FirstPerson/Maps/WaitRoom"));

protected:
	virtual bool Initialize() override;
	
	//Prev func Deprecated OnLevelRemovedFromWorld()
	virtual void NativeDestruct() override;

	UFUNCTION() void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION() void OnDestroySession(bool bWasSuccessful);
	UFUNCTION() void onStartSession(bool bWasSuccessful);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	void MenuTearDown();

	class UMultiplayerSessionSubsystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections{ 4 };
	FString MatchType{ TEXT("Duel") };
	FString PathToLobby{ TEXT("") };
};
