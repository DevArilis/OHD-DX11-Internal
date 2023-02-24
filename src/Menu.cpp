#include "../pch.h"
#include "../include/Menu.hpp"
#include <shellapi.h>
#include <string>;


namespace DX11_Base {
	#define KP( k ) ( GetAsyncKeyState(k) & 0x8000 )
	#define Nullcheck(x) if (x == NULL || x == nullptr) {return;}
    bool InfiniteAmmoHack = false;
    bool NoRecoilHack = false;
    bool FullAutoHack = false;
    bool FireRateHack = false;
    bool ShotsPerBurstHack = false;
    int ShotsPerBurstValue = 3;

	bool Flyhack = false;
	bool Noclip = false;
	bool Desync = false;
	bool Speedhack = false;
	float SpeedhackValue = 1.0f;
	bool BaseEyeHeightHack = false;
	float BaseEyeHeightValue = 0.0f;
	bool FOVChanger = false;
	float FOVValue = 0.0f;
	bool NukeTeam = false;
	bool NukerRandomOffsetY = false;
	bool NukerVToggle = false;
	bool NukerAuto = false;

	bool esp = false;
	bool espSnapLines = false;
	bool espShowTeam = false;
	bool espBoxes = false;
	bool espNames = false;
	bool espTeamColor = false;
	ImVec2 snaplinesy = ImVec2(960, 0);
	

	ImVec4 rgb2rgbf(float r, float g, float b) {
		r = r / 255;
		g = g / 255;
		b = b / 255;
		return ImVec4(r, g, b, 1.0f);
	}

	float RandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}

	namespace Styles {
        void InitStyle()
        {
            ImGuiStyle& style = ImGui::GetStyle();
            ImVec4* colors = ImGui::GetStyle().Colors;

            //	STYLE PROPERTIES
            style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

            //  Base ImGui Styling , Aplying a custyom style is left up to you.
            ImGui::StyleColorsClassic();

            /// EXAMPLE COLOR 
            //colors[ImGuiCol_FrameBg] = ImVec4(0, 0, 0, 0);

            //	COLORS
            if (g_Menu->dbg_RAINBOW_THEME) {
                //  RGB MODE STLYE PROPERTIES
                colors[ImGuiCol_Separator] = ImVec4(g_Menu->dbg_RAINBOW);
                colors[ImGuiCol_TitleBg] = ImVec4(152.0f, 66.0f, 245, 1.0f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 1.0f);
            }
            else {
				colors[ImGuiCol_Tab] = rgb2rgbf(136, 84, 208); 
				colors[ImGuiCol_TabActive] = rgb2rgbf(165, 94, 234);
				colors[ImGuiCol_TabHovered] = rgb2rgbf(165, 94, 234);
				colors[ImGuiCol_ScrollbarGrabActive] = rgb2rgbf(136, 84, 208);
				colors[ImGuiCol_ScrollbarGrab] = rgb2rgbf(165, 94, 234);
				colors[ImGuiCol_ScrollbarGrabHovered] = rgb2rgbf(165, 94, 234);
				colors[ImGuiCol_TitleBgActive] = rgb2rgbf(165, 94, 234);
				colors[ImGuiCol_TitleBg] = rgb2rgbf(165, 94, 234);
				colors[ImGuiCol_ButtonActive] = rgb2rgbf(136, 84, 208);
				colors[ImGuiCol_Button] = rgb2rgbf(165, 94, 234);
				colors[ImGuiCol_ButtonHovered] = rgb2rgbf(165, 94, 234);
				style.FrameRounding = 3.0f;
				style.TabRounding = 1.0f;
				style.TabBorderSize = 1.0f;
				style.FrameBorderSize = 1.0f;
            }
        }
	}
	void wait(int seconds)
	{
		clock_t endwait;
		endwait = clock() + seconds * CLOCKS_PER_SEC;
		while (clock() < endwait) { NULL; };
	}


	int Ticks = 0;
	void Nuker() {
		CG::UWorld** p_uworld = reinterpret_cast<CG::UWorld**>(CG::UWorld::GWorld);
		Nullcheck(p_uworld);
		Nullcheck(*p_uworld);
		CG::UGameInstance* OwningGameInstance = (*p_uworld)->OwningGameInstance;
		Nullcheck(OwningGameInstance);
		auto GameState = (*p_uworld)->GameState;
		Nullcheck(GameState);
		CG::UWorld* gworld = CG::UWorld::GWorld[0];
		Nullcheck(gworld);
		CG::ULocalPlayer* localplayer = gworld->OwningGameInstance->LocalPlayers[0];
		Nullcheck(localplayer);

		auto pArray = GameState->PlayerArray;
		if (Ticks > 10000) {
			Ticks = 0;
		}
		else {
			Ticks++;
			if (pArray.Count() > 1 && Ticks % 2 == 0) {
				for (USHORT i = 0; i < pArray.Count(); i++) {
					auto& ent = pArray[i];
					if (ent != localplayer->PlayerController->PlayerState) {
						auto Character = static_cast<CG::AHDPlayerCharacter*>(ent->PawnPrivate);
						Nullcheck(Character);
						auto Location = Character->ReplicatedMovement.Location;
						CG::AHDPlayerCharacter* SelfPlayer = static_cast<CG::AHDPlayerCharacter*>(localplayer->PlayerController->AcknowledgedPawn);
						Nullcheck(SelfPlayer);
						auto BaseWeapon = reinterpret_cast<CG::AHDBaseWeapon*>(SelfPlayer->EquippedItem);
						Nullcheck(BaseWeapon);
						
						if (NukeTeam) {
							Nullcheck(BaseWeapon);
							if (NukerRandomOffsetY) {
								Location.Z = -100000.0f;
								Nullcheck(BaseWeapon);
								BaseWeapon->ServerFireProjectile(Location, {});
							}
							else {
								Nullcheck(BaseWeapon);
								BaseWeapon->ServerFireProjectile(Location, {});
							}
						}
						else {
							if (Character->TeamNum != SelfPlayer->TeamNum) {
								Nullcheck(BaseWeapon);
								if (NukerRandomOffsetY) {
									Location.Z = -100000.0f;
									Nullcheck(BaseWeapon);
									BaseWeapon->ServerFireProjectile(Location, {});
								}
								else {
									Nullcheck(BaseWeapon);
									BaseWeapon->ServerFireProjectile(Location, {});
								}
							}
						}
					}
				}
			}
		}
	}
	namespace Tabs {
        void TABWeaponCheeze()
        {
            ImGui::Checkbox("Full Auto", &FullAutoHack);
            ImGui::Checkbox("Infinite Ammo", &InfiniteAmmoHack);
            ImGui::Checkbox("No Recoil", &NoRecoilHack);
            ImGui::Checkbox("Fire Rate", &FireRateHack);
            ImGui::Checkbox("Enable Shots Per Burst", &ShotsPerBurstHack);
			ImGui::SameLine();
            ImGui::SliderInt("Shots Per Burst", &ShotsPerBurstValue, 3, 250, "%1.0f");
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Checkbox("Nuker Targets Team", &NukeTeam);
			ImGui::SameLine();
			ImGui::Checkbox("Smoke Rain", &NukerRandomOffsetY);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Enable Offsetter with this, hold a smoke, and press nuke!");
			ImGui::SameLine();
			ImGui::Checkbox("Auto-Nuke", &NukerAuto);
			ImGui::SameLine();
			ImGui::Checkbox("Nuke on V", &NukerVToggle);
			if (ImGui::Button("Projectile Nuker", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
				CG::UWorld** p_uworld = reinterpret_cast<CG::UWorld**>(CG::UWorld::GWorld);
				Nullcheck(p_uworld);
				Nullcheck(*p_uworld);
				CG::UGameInstance* OwningGameInstance = (*p_uworld)->OwningGameInstance;
				Nullcheck(OwningGameInstance);
				auto GameState = (*p_uworld)->GameState;
				Nullcheck(GameState);
				CG::UWorld* gworld = CG::UWorld::GWorld[0]; 
				Nullcheck(gworld);
				CG::ULocalPlayer* localplayer = gworld->OwningGameInstance->LocalPlayers[0];
				Nullcheck(localplayer);
				CG::AHDPlayerCharacter* SelfPlayer = static_cast<CG::AHDPlayerCharacter*>(localplayer->PlayerController->AcknowledgedPawn);
				Nullcheck(SelfPlayer);
				auto BaseWeapon = reinterpret_cast<CG::AHDBaseWeapon*>(SelfPlayer->EquippedItem);
				Nullcheck(BaseWeapon);
				auto pArray = GameState->PlayerArray;
				if (pArray.Count() > 1) {
					for (USHORT i = 0; i < pArray.Count(); i++) {
						auto& ent = pArray[i];
						if (ent != localplayer->PlayerController->PlayerState) {
							auto Character = static_cast<CG::AHDPlayerCharacter*>(ent->PawnPrivate);
							Nullcheck(Character);
							auto Location = Character->ReplicatedMovement.Location;
							Nullcheck(SelfPlayer);
							if (NukeTeam) {
								Nullcheck(BaseWeapon);
								if (NukerRandomOffsetY) {
									Location.Z = -100000.0f;
									Nullcheck(BaseWeapon);
									BaseWeapon->ServerFireProjectile(Location, {});
								}
								else {
									Nullcheck(BaseWeapon);
									BaseWeapon->ServerFireProjectile(Location, {});
								}

							}
							else {
								if (Character->TeamNum != SelfPlayer->TeamNum) {
									Nullcheck(BaseWeapon);
									if (NukerRandomOffsetY) {
										Location.Z = -100000.0f;
										Nullcheck(BaseWeapon);
										BaseWeapon->ServerFireProjectile(Location, {});
									}
									else {
										Nullcheck(BaseWeapon);
										BaseWeapon->ServerFireProjectile(Location, {});
									}	
								}
							}
						}
					}
				}
			}
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
			ImGui::Text("Press INSERT to open/close the menu.");
        };
		void TABESP() {
			ImGui::TextColored(ImVec4(255, 0, 0, 1), "This is an experimental feature, expect bugs.");
			ImGui::Checkbox("ESP Enabled", &esp);
			ImGui::SameLine();
			ImGui::Checkbox("Show Team", &espShowTeam);
			ImGui::Checkbox("Boxes", &espBoxes);
			ImGui::SameLine();
			ImGui::Checkbox("Names", &espNames);
			ImGui::SameLine();
			ImGui::Checkbox("Snaplines", &espSnapLines);
			ImGuiIO& io = ImGui::GetIO();
			ImGui::SliderFloat("Snaplines Y", &snaplinesy.y, 0, io.DisplaySize.y, "%1.0f");
			ImGui::Checkbox("Team Colors", &espTeamColor);
		};
		void TABMisc() {
			
			ImGui::Checkbox("Fly", &Flyhack);
			ImGui::Checkbox("Noclip", &Noclip);
			ImGui::Checkbox("Desync", &Desync);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Other players will not see your movement, you will be standing where you enabled.");
			ImGui::Checkbox("FOV Changer", &FOVChanger);
			ImGui::SameLine();
			ImGui::SliderFloat("FOV", &FOVValue, 0.0f, 160.0f, "%1.0f");
			ImGui::Checkbox("Enable Speed", &Speedhack);
			ImGui::SameLine();
			ImGui::SliderFloat("Speed Factor", &SpeedhackValue, 1.0f, 5.0f, "%1.0f");
			ImGui::Checkbox("Birds Eye View", &BaseEyeHeightHack);
			ImGui::SameLine();
			ImGui::SliderFloat("Birds Eye Value", &BaseEyeHeightValue, 0.0f, 150000.0f, "%1.0f");
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::Button("UNHOOK DLL", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
#if DEBUG
				g_Console->printdbg("\n\n[+] UNHOOK INITIALIZED [+]\n\n", g_Console->color.red);
#endif
				g_KillSwitch = TRUE;
			}
		};		
		void TABCredits() {
			ImGui::Text("ArilisDev - For the cheat in and of itself.");
			ImGui::Text("NightFyre - DX11 Base.");
			if (ImGui::Button("Join The Discord", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) {
				ShellExecute(0, 0, L"https://discord.gg/8T28TdwR5d", 0, 0, SW_SHOW);
			}
		}
	}
	
	void Menu::Draw()
	{

		const ImGuiWindowFlags windowFlags{ ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground };
		//  I like to use tabs to display my content in an organized manner, Here is an example on how you could do the same
		//  As a courtesy I have left the TABS namespace with an Example Tab
		ImGui::SetNextWindowPos(ImVec2(1, 1));
		ImGui::Begin("watermark", nullptr, windowFlags);
		{
			ImGui::TextColored(rgb2rgbf(252, 232, 3), "[OHD Internal V3]\nMenu Key: INSERT\nBy ArilisDev");
			
		}
		ImGui::End();
		if (g_GameVariables->m_ShowMenu)
			MainMenu();

		if (g_GameVariables->m_ShowHud)
			HUD(&g_GameVariables->m_ShowHud);

		if (g_GameVariables->m_ShowDemo)
			ImGui::ShowDemoWindow();

		if (esp) {
			CG::UWorld** p_uworld = reinterpret_cast<CG::UWorld**>(CG::UWorld::GWorld);
			Nullcheck(p_uworld);
			Nullcheck(*p_uworld);
			CG::UGameInstance* OwningGameInstance = (*p_uworld)->OwningGameInstance;
			Nullcheck(OwningGameInstance);
			auto GameState = (*p_uworld)->GameState;
			Nullcheck(GameState);
			CG::UWorld* gworld = CG::UWorld::GWorld[0];
			Nullcheck(gworld);
			CG::ULocalPlayer* localplayer = gworld->OwningGameInstance->LocalPlayers[0];
			Nullcheck(localplayer);
			Nullcheck(localplayer->PlayerController);
			Nullcheck(localplayer->PlayerController->AcknowledgedPawn);
			CG::AHDPlayerCharacter* SelfPlayer = static_cast<CG::AHDPlayerCharacter*>(localplayer->PlayerController->AcknowledgedPawn);
			Nullcheck(SelfPlayer);
			auto pArray = GameState->PlayerArray;
			if (pArray.Count() > 1) {
				for (USHORT i = 0; i < pArray.Count(); i++) {
					auto& ent = pArray[i];
					if (ent != localplayer->PlayerController->PlayerState) {
						localplayer->PlayerController->PlayerState->GetPlayerName();
						auto Character2 = static_cast<CG::AHDPlayerCharacter*>(ent->PawnPrivate);
						Nullcheck(Character2);
						Nullcheck(SelfPlayer);
						if (ent->PawnPrivate == nullptr) return;
						if (ent->PawnPrivate == nullptr) return;
						if (Character2->Health <= 0) return;
						if (espShowTeam) {
							auto Location = Character2->ReplicatedMovement.Location;
							CG::FVector2D screen;
							localplayer->PlayerController->ProjectWorldLocationToScreen(Location, &screen, NULL);
							auto draw = ImGui::GetBackgroundDrawList();
							if (espTeamColor) {
								if (Character2->TeamNum != SelfPlayer->TeamNum) {
									if (espSnapLines) {
										draw->AddLine(ImVec2(960, snaplinesy.y), ImVec2(screen.X, screen.Y - 10), ImColor(255, 0, 0), 1);
									}

									auto name = Character2->PlayerState->GetPlayerName().ToString();
									if (espNames) {
										int label_size = ImGui::CalcTextSize(name.c_str(), NULL, true).x;

										draw->AddText(ImVec2((float)screen.X - label_size / 2, screen.Y - 16), ImColor(255, 0, 0), name.c_str());
									}

									if (espBoxes) {
										draw->AddRect({ (float)screen.X - 10 / 2 - 1, screen.Y - 5 }, { (float)screen.X + 10 / 2 + 3 , screen.Y + 10}, ImColor(255, 0, 0));
									}
								}
								else {
									if (espSnapLines) {
										draw->AddLine(ImVec2(960, snaplinesy.y), ImVec2(screen.X, screen.Y - 10), ImColor(0, 255, 0), 1);
									}

									auto name = Character2->PlayerState->GetPlayerName().ToString();
									if (espNames) {
										int label_size = ImGui::CalcTextSize(name.c_str(), NULL, true).x;
										draw->AddText(ImVec2((float)screen.X - label_size / 2, screen.Y - 16), ImColor(0, 255, 0), name.c_str());
									}

									if (espBoxes) {
										draw->AddRect({ (float)screen.X - 10 / 2 - 1, screen.Y - 5 }, { (float)screen.X + 10 / 2 + 3 , screen.Y + 10}, ImColor(0, 255, 0));
									}
								}

							}
							else {
								if (espSnapLines) {
									draw->AddLine(ImVec2(960, snaplinesy.y), ImVec2(screen.X, screen.Y - 10), ImColor(165, 94, 234), 1);
								}

								auto name = Character2->PlayerState->GetPlayerName().ToString();
								if (espNames) {
									int label_size = ImGui::CalcTextSize(name.c_str(), NULL, true).x;
									draw->AddText(ImVec2((float)screen.X - label_size / 2, screen.Y - 16), ImColor(165, 94, 234), name.c_str());
								}

								if (espBoxes) {
									draw->AddRect({ (float)screen.X - 10 / 2 - 1, screen.Y - 5 }, { (float)screen.X + 10 / 2 + 3 , screen.Y + 10}, ImColor(165, 94, 234));
								}
							}
							
						}
						else {

							if (Character2->TeamNum != SelfPlayer->TeamNum) {
								auto Location = Character2->ReplicatedMovement.Location;
								CG::FVector2D screen;
								localplayer->PlayerController->ProjectWorldLocationToScreen(Location, &screen, NULL);
								auto draw = ImGui::GetBackgroundDrawList();
								if (espTeamColor) {
									if (espSnapLines) {
										draw->AddLine(ImVec2(960, snaplinesy.y), ImVec2(screen.X, screen.Y - 10), ImColor(255, 0, 0), 1);
									}

									auto name = Character2->PlayerState->GetPlayerName().ToString();
									if (espNames) {
										int label_size = ImGui::CalcTextSize(name.c_str(), NULL, true).x;

										draw->AddText(ImVec2((float)screen.X - label_size / 2, screen.Y - 16), ImColor(255, 0, 0), name.c_str());
									}

									if (espBoxes) {
										draw->AddRect({ (float)screen.X - 10 / 2 - 1, screen.Y - 5 }, { (float)screen.X + 10 / 2 + 3 , screen.Y + 10}, ImColor(255, 0, 0));
									}

								}
								else {
									if (espSnapLines) {
										draw->AddLine(ImVec2(960, snaplinesy.y), ImVec2(screen.X, screen.Y - 10), ImColor(165, 94, 234), 1);
									}
									Nullcheck(Character2);
									Nullcheck(Character2->PlayerState);
									auto name = Character2->PlayerState->GetPlayerName().ToString();
									if (espNames) {
										int label_size = ImGui::CalcTextSize(name.c_str(), NULL, true).x;
										draw->AddText(ImVec2((float)screen.X - label_size / 2, screen.Y - 16), ImColor(165, 94, 234), name.c_str());
									}

									if (espBoxes) {
										draw->AddRect({ (float)screen.X - 10 / 2 - 1, screen.Y - 5 }, { (float)screen.X + 10 / 2 + 3 , screen.Y + 10}, ImColor(165, 94, 234));
									}
								}
							}
						}

					}
				}
			}
		}
		
		
	}


	void Menu::MainMenu()
	{
        if (!g_GameVariables->m_ShowDemo)
            Styles::InitStyle();

        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(g_Menu->dbg_RAINBOW));
        }
		
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        if (!ImGui::Begin("Opertation: Harsh Doorstop Internal", &g_GameVariables->m_ShowMenu, 96))
        {
            ImGui::End();
            return;
        }
        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }
        
        //  Display Menu Content
        //Tabs::TABMain();
		
        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Weapon"))
            {
                Tabs::TABWeaponCheeze();
                ImGui::EndTabItem();
            }
			if (ImGui::BeginTabItem("ESP"))
			{
				Tabs::TABESP();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Miscellaneous"))
			{
				Tabs::TABMisc();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Credits"))
			{
				Tabs::TABCredits();
				ImGui::EndTabItem();
			}

            ImGui::EndTabBar();
        }
        ImGui::End();
	}

	void Menu::HUD(bool* p_open)
	{

	}
	CG::FVector2D CalcAngle(const CG::FVector& src, const CG::FVector& dst, const CG::FVector currentAngles, float smoothness);

	#define UCONST_Pi 3.1415926
	#define URotation180 32768
	#define URotationToRadians  UCONST_Pi / URotation180 
	
	CG::FVector RotationToVector(CG::FRotator R)
	{
		CG::FVector Vec;
		float fYaw = R.Yaw * URotationToRadians;
		float fPitch = R.Pitch * URotationToRadians;
		float CosPitch = cos(fPitch);
		Vec.X = cos(fYaw) * CosPitch;
		Vec.Y = sin(fYaw) * CosPitch;
		Vec.Z = sin(fPitch);

		return Vec;
	}
	CG::FRotator VecToRot(CG::FVector2D InVec)
	{
		CG::FRotator Result;
		float direction = atan2(InVec.Y, InVec.X);
		float magnitude = sqrt(InVec.X * InVec.X + InVec.Y * InVec.Y);

		int yaw = magnitude * cos(direction);
		int pitch = magnitude * sin(direction);

		Result.Pitch = pitch;
		Result.Yaw = yaw;
		Result.Roll = 0;
		return Result;
	}
	
	void Menu::Loops()
	{
		CG::UWorld** p_uworld = reinterpret_cast<CG::UWorld**>(CG::UWorld::GWorld);
		Nullcheck(p_uworld);
		Nullcheck(*p_uworld);
		CG::UGameInstance* OwningGameInstance = (*p_uworld)->OwningGameInstance;
		Nullcheck(OwningGameInstance);
		auto GameState = (*p_uworld)->GameState;
		Nullcheck(GameState);
		CG::UWorld* gworld = CG::UWorld::GWorld[0];
		Nullcheck(gworld);
		auto persistent_level = gworld->PersistentLevel;
		Nullcheck(persistent_level);
		CG::ULocalPlayer* localplayer = gworld->OwningGameInstance->LocalPlayers[0];
		Nullcheck(localplayer);
		CG::APlayerController* local_controller = OwningGameInstance->LocalPlayers[0]->PlayerController;
		Nullcheck(local_controller);
		CG::AHDPlayerCharacter* SelfPlayer = static_cast<CG::AHDPlayerCharacter*>(local_controller->AcknowledgedPawn);
		Nullcheck(SelfPlayer);
		auto Character = reinterpret_cast<CG::AHDPlayerCharacter*>(SelfPlayer);
		Nullcheck(Character);
		auto BaseWeapon = reinterpret_cast<CG::AHDBaseWeapon*>(SelfPlayer->EquippedItem);
		Nullcheck(BaseWeapon);
		CG::AHDBaseGameMode* Gamemode = reinterpret_cast<CG::AHDBaseGameMode*>(local_controller->AcknowledgedPawn);
		Nullcheck(Gamemode);
		Vector3 GetVectorForward(const Vector3 & angles);
		static bool bResetFly = false;
		
		if (Flyhack && Desync) {
			Desync = false;
		}


		if (Flyhack)
		{
			Character->SetReplicateMovement(false);
			Character->CharacterMovement->MovementMode = CG::EMovementMode::MOVE_Flying; // set to fly mode so we don't fall down
			bResetFly = true;
		}
		else if (bResetFly)
		{
			Character->CharacterMovement->MovementMode = CG::EMovementMode::MOVE_Falling;
			Character->SetReplicateMovement(true);
			bResetFly = false;
		}

		if (Noclip) {
			Character->bActorEnableCollision = false;
		}
		else {
			Character->bActorEnableCollision = true;
		}
		bool check = false;
		if (Desync && !Flyhack) {
			Character->SetReplicateMovement(false);
			check = true;
		}
		else if (check) {
			Character->SetReplicateMovement(true);
		}

		if (NukerVToggle && KP(0x56)) {
			Nuker();
			Sleep(2000);
		}

		if (NukerAuto) {
			Nuker();
			Sleep(2000);
		}

		if (Speedhack) {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr) {
				if (local_controller != nullptr && Character != nullptr) {
					Character->CustomTimeDilation = SpeedhackValue;
				}
			}
		}
		// Weapon Shit
		if (InfiniteAmmoHack) {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					Nullcheck(BaseWeapon);
					BaseWeapon->bUsesAmmo = false;
				}
			}
		}
		else {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					Nullcheck(BaseWeapon);
					BaseWeapon->bUsesAmmo = true;
				}
			}
		}

		if (NoRecoilHack) {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					Nullcheck(BaseWeapon);
					BaseWeapon->bNoRecoil = true;
				}
			}
		}
		else {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					Nullcheck(BaseWeapon);
					BaseWeapon->bNoRecoil = false;
				}
			}
		}

		if (FireRateHack) {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					Nullcheck(BaseWeapon);
					BaseWeapon->FireRate = 0.0001f;
				}
			}
		}
		else {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					Nullcheck(BaseWeapon);
					BaseWeapon->FireRate = 0.1f;

				}
			}
		}
		if (ShotsPerBurstHack) {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					Nullcheck(BaseWeapon);
					BaseWeapon->ShotsPerBurst = ShotsPerBurstValue;
				}
			}
		}
		else {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					Nullcheck(BaseWeapon);
					BaseWeapon->ShotsPerBurst = 3;

				}
			}
		}

		if (FullAutoHack) {
			if (localplayer != nullptr && OwningGameInstance != nullptr && local_controller != nullptr && SelfPlayer != nullptr && SelfPlayer->EquippedItem != nullptr) {
				if (local_controller != nullptr && BaseWeapon != nullptr) {
					auto Mode = CG::EFireMode::Auto;
					if (BaseWeapon->SelectedFireMode != Mode) {
						//BaseWeapon->SetFireMode(Mode, false, true);
						BaseWeapon->SelectedFireMode = Mode;
					}
				}
			}
		}

		if (BaseEyeHeightHack) {
			Nullcheck(Character);
			Character->BaseEyeHeight = 164.0f + BaseEyeHeightValue;
		}
		else {
			Nullcheck(Character);
			Character->BaseEyeHeight = 164.0f;
		}

		if (FOVChanger) {
			local_controller->PlayerCameraManager->DefaultFOV = FOVValue;
		}
	}
}