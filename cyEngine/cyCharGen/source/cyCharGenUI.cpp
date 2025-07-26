#include "cyCharGenUI.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include <SDL3/SDL_opengl.h>
#include <cyVector2i.h>


namespace ADND
{

void
CharGenUI::render()
{
  ImGuiIO& io = ImGui::GetIO();

  // Main menu bar
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Character"))  { m_characterCreator.reset(); }
      if (ImGui::MenuItem("Save Character")) { m_characterCreator.saveCharacter(); }
      if (ImGui::MenuItem("Load Character")) { m_characterCreator.loadCharacter(); }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  // Main content area
  ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
  ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - ImGui::GetFrameHeight()));
  ImGui::Begin("Character Creator", nullptr,
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse);

  // Create character button at top
  if (ImGui::Button("CREATE CHARACTER", ImVec2(-1, 0))) {
    m_characterCreator.reset();
  }

  // Three column layout
  ImGui::Columns(3, "character_columns", false);

  // ================= COLUMN 1: BASIC INFORMATION =================
  ImGui::BeginChild("Column1", ImVec2(0, 0), true);
  {
    ImGui::Text("Basic Information");
    ImGui::Separator();

    // Birthday
    ImGui::Text("Birthday");
    ImGui::SameLine();
    ImGui::Text(m_characterCreator.m_currentCharacter.m_birthDate.toString("YYYY-MM-DD").c_str());

    // Name and sex
    ImGui::InputText("Name", &m_characterCreator.m_currentCharacter.m_name);
    ImGui::InputText("Sex", &m_characterCreator.m_currentCharacter.m_sex);

    // Level and XP
    ImGui::InputInt("Level", &m_characterCreator.m_currentCharacter.m_level);
    ImGui::InputInt("XP", &m_characterCreator.m_currentCharacter.m_xp);

    // Age
    ImGui::InputInt("Age", &m_characterCreator.m_currentCharacter.m_age);

    // Height and Weight
    ImGui::SliderFloat("Height (in)", &m_characterCreator.m_currentCharacter.m_height, 0, 500);
    ImGui::SliderFloat("Weight (lbs)", &m_characterCreator.m_currentCharacter.m_weight, 0, 500);

    // Alignment
    static int alignment = 0;
    const char* alignments[] = { "Lawful Good", "Neutral Good", "Chaotic Good",
                                "Lawful Neutral", "True Neutral", "Chaotic Neutral",
                                "Lawful Evil", "Neutral Evil", "Chaotic Evil" };
    ImGui::Combo("Alignment", &alignment, alignments, IM_ARRAYSIZE(alignments));

    // Extra info
    static char extraInfo[256] = "";
    ImGui::InputTextMultiline("Extra Info", extraInfo, IM_ARRAYSIZE(extraInfo), ImVec2(-1, 60));
  }
  ImGui::EndChild();

  ImGui::NextColumn();

  // ================= COLUMN 2: ABILITIES & CLASS =================
  ImGui::BeginChild("Column2", ImVec2(0, 0), true);
  {
    ImGui::Text("Abilities & Class");
    ImGui::Separator();

    // Method selection
    static int method = 0;
    ImGui::Text("Generation Method:");
    ImGui::RadioButton("Method I", &method, 0); ImGui::SameLine();
    ImGui::RadioButton("Method II", &method, 1);
    ImGui::RadioButton("Method III", &method, 2); ImGui::SameLine();
    ImGui::RadioButton("Method IV", &method, 3);

    if (ImGui::Button("Generate Ability Scores")) {
      m_characterCreator.rollAbilityScores(method);
    }

    // Ability scores display
    // struct AbilityScore { const char* name; int* value; };
    // AbilityScore abilities[] = {
    //     {"Strength", &characterCreator.strength},
    //     {"Dexterity", &characterCreator.dexterity},
    //     {"Constitution", &characterCreator.constitution},
    //     {"Intelligence", &characterCreator.intelligence},
    //     {"Wisdom", &characterCreator.wisdom},
    //     {"Charisma", &characterCreator.charisma}
    // };

    for (auto& ability : ABILITY_SCORES::E::_values()) {
      ImGui::Text("%s", ability._to_string()); ImGui::SameLine();
      ImGui::InputInt(("##" + std::string(ability._to_string())).c_str(), 
                      &m_characterCreator.m_currentCharacter.m_abilityScores[ability]);
    }

    // Race and class selection


    const char* races[] = { "Human", "Elf", "Dwarf", "Halfling", "Gnome", "Half-Elf", "Half-Orc" };
    const char* classes[] = { "Fighter", "Cleric", "Magic-User", "Thief", "Paladin", "Ranger", "Illusionist" };

    ImGui::Combo("Race", &raceIndex, races, IM_ARRAYSIZE(races));
    ImGui::Combo("Subrace", &subraceIndex, races, IM_ARRAYSIZE(races));
    ImGui::Combo("Class", &classIndex, classes, IM_ARRAYSIZE(classes));
    ImGui::Combo("Subclass", &subclassIndex, classes, IM_ARRAYSIZE(classes));

    // Hit points
    static int hitPoints = 1;
    ImGui::InputInt("Hit Points", &hitPoints);

    // Saving throws
    ImGui::Text("Saving Throws");
    ImGui::Separator();

    // struct SavingThrow { const char* name; int* value; };
    // SavingThrow throws[] = {
    //     {"Paralyzation", &characterCreator.saveParalyzation},
    //     {"Poison", &characterCreator.savePoison},
    //     {"Death Magic", &characterCreator.saveDeathMagic},
    //     {"Rod/Staff/Wand", &characterCreator.saveRod},
    //     {"Petrification", &characterCreator.savePetrification},
    //     {"Breath Weapon", &characterCreator.saveBreathWeapon},
    //     {"Spell", &characterCreator.saveSpell}
    // };

    for (auto& st : SAVING_THROW::E::_values()) {
      ImGui::Text("%s", st._to_string()); ImGui::SameLine();
      ImGui::InputInt(("##" + std::string(st._to_string())).c_str(), 
                      &m_characterCreator.m_currentCharacter.m_savingThrows[st]);
    }
  }
  ImGui::EndChild();

  ImGui::NextColumn();

  // ================= COLUMN 3: EQUIPMENT & SPECIAL =================
  ImGui::BeginChild("Column3", ImVec2(0, 0), true);
  {
    ImGui::Text("Equipment & Special");
    ImGui::Separator();

    // Money
    if (ImGui::Button("Roll for Money")) {
      m_characterCreator.rollMoneyAll();
    }

    static int moneyCopper = 0;
    static int moneySilver = 0;
    static int moneyGold = 0;
    static int moneyPlatinum = 0;

    ImGui::Columns(2, "money", false);
    ImGui::InputInt("Copper", &moneyCopper);
    ImGui::NextColumn();
    ImGui::InputInt("Silver", &moneySilver);
    ImGui::NextColumn();
    ImGui::InputInt("Gold", &moneyGold);
    ImGui::NextColumn();
    ImGui::InputInt("Platinum", &moneyPlatinum);
    ImGui::Columns(1);

    // Items
    static char items[512] = "";
    ImGui::InputTextMultiline("Items", items, IM_ARRAYSIZE(items), ImVec2(-1, 60));

    // Combat stats
    static int speed = 12;
    static int encumbrance = 0;
    static int ac = 10;
    static int dexMod = 0;

    ImGui::InputInt("Speed/Encumbrance", &speed);
    ImGui::Columns(2, "ac_dex", false);
    ImGui::InputInt("AC", &ac);
    ImGui::NextColumn();
    ImGui::InputInt("DEX Mod", &dexMod);
    ImGui::Columns(1);

    // Weapon proficiencies
    static char weaponProficiencies[256] = "";
    ImGui::InputTextMultiline("Weapon Proficiencies", weaponProficiencies, IM_ARRAYSIZE(weaponProficiencies), ImVec2(-1, 40));

    // Skills and languages
    if (ImGui::Button("Roll Secondary Skillset")) {
      m_characterCreator.rollSecondarySkillset();
    }

    static char secondarySkillset[128] = "";
    ImGui::InputText("Secondary Skillset", secondarySkillset, IM_ARRAYSIZE(secondarySkillset));

    static char languages[256] = "";
    ImGui::InputTextMultiline("Languages", languages, IM_ARRAYSIZE(languages), ImVec2(-1, 40));

    // Class-specific sections
    ImGui::Separator();
    ImGui::Text("Class Specials");

    // Cleric deity (only shown if class is Cleric)
    if (classIndex == 1) { // Cleric
      static char deity[64] = "";
      ImGui::InputText("Deity to Serve", deity, IM_ARRAYSIZE(deity));
    }

    // Magic-user spells (only shown if class is Magic-User or Illusionist)
    if (classIndex == 2 || classIndex == 6) { // Magic-User or Illusionist
      static char spells[512] = "";
      ImGui::InputTextMultiline("Spells", spells, IM_ARRAYSIZE(spells), ImVec2(-1, 60));
      if (ImGui::Button("Roll Spells")) {
        m_characterCreator.rollSpells();
      }
    }

    // Backstory
    static char backstory[1024] = "";
    ImGui::InputTextMultiline("Backstory", backstory, IM_ARRAYSIZE(backstory), ImVec2(-1, 80));
  }
  ImGui::EndChild();

  ImGui::Columns(1);
  ImGui::End();

}
}