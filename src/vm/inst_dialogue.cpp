#include "inst_dialogue.h"

#include "inst_macros.inc"

#include "expression.h"
#include "../profile/scriptvars.h"
#include "../mem.h"
#include "../log.h"
#include "../audio/audiostream.h"
#include "../profile/vm.h"
#include "../hud/saveicondisplay.h"
#include "../hud/tipsnotification.h"
#include "../data/savesystem.h"
#include "../data/tipssystem.h"
#include "../ui/ui.h"
#include "interface/input.h"
#include "../text.h"
#include "vm.h"

namespace Impacto {

namespace Vm {

using namespace Impacto::Profile::ScriptVars;

VmInstruction(InstMesViewFlag) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0: {  // Set
      PopExpression(arg1);
      PopExpression(arg2);
      ImpLogSlow(
          LL_Warning, LC_VMStub,
          "STUB instruction MesViewFlag(type: Set, arg1: %i, arg2: %i)\n", arg1,
          arg2);
    } break;
    case 1: {  // Check
      PopExpression(dest);
      PopExpression(arg1);
      PopExpression(arg2);
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction MesViewFlag(type: Check, dest: %i, arg1: "
                 "%i, arg2: %i)\n",
                 dest, arg1, arg2);
    } break;
  }
}
VmInstruction(InstSetMesWinPri) {
  StartInstruction;
  PopExpression(arg1);
  PopExpression(arg2);
  PopExpression(unused);
  ImpLogSlow(LL_Warning, LC_VMStub,
             "STUB instruction MesViewFlag(arg1: %i, arg2: "
             "%i, unused: %i)\n",
             arg1, arg2, unused);
}
VmInstruction(InstMesSync) {}
VmInstruction(InstMesSetID) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0: {  // SetSavePointPage0
      if (Profile::Vm::UseReturnIds) {
        PopUint16(savePointId);
        if (!GetFlag(1288)) {
          SaveSystem::SetCheckpointId(savePointId);
        }
        ImpLogSlow(
            LL_Warning, LC_VMStub,
            "STUB instruction MesSetID(type: SetSavePoint, savePointId: %i)\n",
            savePointId);
      } else {
        ImpLogSlow(LL_Warning, LC_VMStub,
                   "STUB instruction MesSetID(type: SetSavePoint)\n");
      }
    } break;
    case 1: {  // SetSavePointForPage
      PopUint16(savePointId);
      PopExpression(dialoguePageId);
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction MesSetID(type: SetSavePoint1, "
                 "savePointId: %i, arg1: %i)\n",
                 savePointId, dialoguePageId);
      if (!GetFlag(1288 + dialoguePageId)) {
        SaveSystem::SetCheckpointId(savePointId);
      }
      thread->DialoguePageId = dialoguePageId;
    } break;
    case 2: {  // SetPage
      PopExpression(dialoguePageId);
      ImpLogSlow(
          LL_Warning, LC_VMStub,
          "STUB instruction MesSetID(type: SetPage, dialoguePageId: %i)\n",
          dialoguePageId);
      thread->DialoguePageId = dialoguePageId;
    } break;
  }
}
VmInstruction(InstMesCls) {
  StartInstruction;
  PopUint8(type);  // TODO: Implement types 0, 1, 2, 3, 4, 5, 6, 7, 8
  if ((type & 0xFE) != 4 && !(type & 1)) {
    PopExpression(arg1);
    ImpLogSlow(LL_Warning, LC_VMStub,
               "STUB instruction MesCls(type: %i, arg1: %i)\n", type, arg1);
  } else {
    ImpLogSlow(LL_Warning, LC_VMStub, "STUB instruction MesCls(type: %i)\n",
               type);
  }
}
VmInstruction(InstMesVoiceWait) {
  StartInstruction;
  ImpLogSlow(LL_Warning, LC_VMStub, "STUB instruction MesVoiceWait()\n");
}
VmInstruction(InstMes) {
  // Save before advancing Ip
  SaveSystem::SaveMemory();

  StartInstruction;

  DialoguePage dialoguePage = DialoguePages[thread->DialoguePageId];
  uint32_t scriptId = LoadedScriptMetas[dialoguePage.Id].Id;

  // After loading a save we need to make sure the textbox is actually shown
  if (dialoguePage.FadeAnimation.IsOut() &&
      GetFlag(thread->DialoguePageId + SF_MESWINDOW0OPENFL)) {
    dialoguePage.Mode =
        (DialoguePageMode)ScrWork[thread->DialoguePageId * 10 + SW_MESMODE0];
    dialoguePage.FadeAnimation.StartIn(true);
  }

  PopUint8(type);
  switch (type) {
    case 0: {  // LoadDialogue
      PopExpression(characterId);
      PopUint16(lineId);
      uint8_t* line =
          ScriptGetStrAddress(ScriptBuffers[thread->ScriptBufferId], lineId);

      if (!(ScrWork[10 * thread->DialoguePageId + 4362] & 0b1000000)) {
        SetFlag(SF_MESREAD, SaveSystem::IsLineRead(scriptId, lineId));
        ChkMesSkip();
      }

      ScrWork[2 * dialoguePage.Id + SW_LINEID] = lineId;
      ScrWork[2 * dialoguePage.Id + SW_SCRIPTID] = scriptId;

      uint8_t* oldIp = thread->Ip;
      thread->Ip = line;
      DialoguePages[thread->DialoguePageId].AddString(thread);
      thread->Ip = oldIp;
      UI::BacklogMenuPtr->AddMessage(line);
    } break;
    case 1: {  // LoadVoicedUnactedDialogue
      PopExpression(audioId);
      PopExpression(characterId);
      PopUint16(lineId);
      uint8_t* line =
          ScriptGetStrAddress(ScriptBuffers[thread->ScriptBufferId], lineId);

      if (!(ScrWork[10 * thread->DialoguePageId + 4362] & 0b1000000)) {
        SetFlag(SF_MESREAD, SaveSystem::IsLineRead(scriptId, lineId));
        ChkMesSkip();
      }

      ScrWork[2 * dialoguePage.Id + SW_LINEID] = lineId;
      ScrWork[2 * dialoguePage.Id + SW_SCRIPTID] = scriptId;

      uint8_t* oldIp = thread->Ip;
      thread->Ip = line;
      DialoguePages[thread->DialoguePageId].AddString(thread);
      thread->Ip = oldIp;
    } break;
    case 3: {  // LoadVoicedDialogue
      PopExpression(audioId);
      PopExpression(animationId);
      PopExpression(characterId);
      PopUint16(lineId);
      uint8_t* line =
          ScriptGetStrAddress(ScriptBuffers[thread->ScriptBufferId], lineId);

      if (!(ScrWork[10 * thread->DialoguePageId + 4362] & 0b1000000)) {
        SetFlag(SF_MESREAD, SaveSystem::IsLineRead(scriptId, lineId));
        ChkMesSkip();
      }

      ScrWork[2 * dialoguePage.Id + SW_LINEID] = lineId;
      ScrWork[2 * dialoguePage.Id + SW_SCRIPTID] = scriptId;

      Io::Stream* stream;
      IoError err = Io::VfsOpen("voice", audioId, &stream);
      bool playAudio = (err == IoError_OK && !GetFlag(SF_MESALLSKIP));

      uint8_t* oldIp = thread->Ip;
      thread->Ip = line;
      DialoguePages[thread->DialoguePageId].AddString(
          thread, playAudio ? Audio::AudioStream::Create(stream) : 0,
          animationId);
      thread->Ip = oldIp;
      UI::BacklogMenuPtr->AddMessage(line, audioId);
    } break;
    case 0x0B: {  // LoadVoicedDialogue0B
      PopExpression(audioId);
      PopExpression(animationId);
      PopExpression(characterId);
      PopUint16(lineId);
      uint8_t* line =
          ScriptGetStrAddress(ScriptBuffers[thread->ScriptBufferId], lineId);

      if (!(ScrWork[10 * thread->DialoguePageId + 4362] & 0b1000000)) {
        SetFlag(SF_MESREAD, SaveSystem::IsLineRead(scriptId, lineId));
        ChkMesSkip();
      }

      ScrWork[2 * dialoguePage.Id + SW_LINEID] = lineId;
      ScrWork[2 * dialoguePage.Id + SW_SCRIPTID] = scriptId;

      uint8_t* oldIp = thread->Ip;
      thread->Ip = line;
      DialoguePages[thread->DialoguePageId].AddString(thread, 0, animationId);
      thread->Ip = oldIp;
    } break;
    case 0x80: {  // LoadDialogueMSB
      PopExpression(characterId);
      PopUint16(lineId);
      uint8_t* line =
          MsbGetStrAddress(MsbBuffers[thread->ScriptBufferId], lineId);

      if (!(ScrWork[10 * thread->DialoguePageId + 4362] & 0b1000000)) {
        SetFlag(SF_MESREAD, SaveSystem::IsLineRead(scriptId, lineId));
        ChkMesSkip();
      }

      ScrWork[2 * dialoguePage.Id + SW_LINEID] = lineId;
      ScrWork[2 * dialoguePage.Id + SW_SCRIPTID] = scriptId;

      uint8_t* oldIp = thread->Ip;
      thread->Ip = line;
      DialoguePages[thread->DialoguePageId].AddString(thread);
      thread->Ip = oldIp;
      UI::BacklogMenuPtr->AddMessage(line);
    } break;
    case 0x83: {  // LoadVoicedDialogueMSB
      PopExpression(audioId);
      PopExpression(animationId);
      PopExpression(characterId);
      PopUint16(lineId);
      uint8_t* line =
          MsbGetStrAddress(MsbBuffers[thread->ScriptBufferId], lineId);

      if (!(ScrWork[10 * thread->DialoguePageId + 4362] & 0b1000000)) {
        SetFlag(SF_MESREAD, SaveSystem::IsLineRead(scriptId, lineId));
        ChkMesSkip();
      }

      ScrWork[2 * dialoguePage.Id + SW_LINEID] = lineId;
      ScrWork[2 * dialoguePage.Id + SW_SCRIPTID] = scriptId;

      Io::Stream* stream;
      IoError err = Io::VfsOpen("voice", audioId, &stream);
      bool playAudio = (err == IoError_OK && !GetFlag(SF_MESALLSKIP));

      uint8_t* oldIp = thread->Ip;
      thread->Ip = line;
      DialoguePages[thread->DialoguePageId].AddString(
          thread, playAudio ? Audio::AudioStream::Create(stream) : 0,
          animationId);
      thread->Ip = oldIp;
      UI::BacklogMenuPtr->AddMessage(line);
    } break;
  }

  DialoguePages[thread->DialoguePageId].AutoWaitTime =
      DialoguePages[thread->DialoguePageId].Length;
}
VmInstruction(InstMesMain) {
  StartInstruction;
  PopUint8(type);
  DialoguePage* currentPage = &DialoguePages[thread->DialoguePageId];

  bool advanceButtonWentDown =
      Interface::PADinputButtonWentDown & Interface::PAD1A ||
      Interface::PADinputMouseWentDown & Interface::PAD1A;

  if (type == 0) {  // Normal mode
    if (!currentPage->TextIsFullyOpaque()) {
      // Text is still appearing
      if (advanceButtonWentDown || GetFlag(SF_MESALLSKIP)) {
        if (!currentPage->Typewriter.IsCancelled) {
          currentPage->Typewriter.CancelRequested = true;
        }
      }
    } else {
      // Text is fully opaque
      if (!GetFlag(SF_UIHIDDEN)) {
        if (advanceButtonWentDown || GetFlag(SF_MESALLSKIP) ||
            !currentPage->AutoWaitTime) {
          // Advance to next line
          SaveSystem::SetLineRead(ScrWork[2 * currentPage->Id + SW_SCRIPTID],
                                  ScrWork[2 * currentPage->Id + SW_LINEID]);

          BlockThread;
          return;
        }
      }
    }

    ResetInstruction;
    BlockThread;
  }
  // TODO: Type 1 - Skip mode(?)
}
VmInstruction(InstSetMesModeFormat) {
  StartInstruction;
  PopExpression(id);
  PopLocalLabel(modeDataAdr);
  (void)modeDataAdr;
  ImpLogSlow(LL_Warning, LC_VMStub,
             "STUB instruction SetMesModeFormat(id: %i)\n", id);
}
VmInstruction(InstSetNGmoji) {
  StartInstruction;
  PopString(strAdr1);
  PopString(strAdr2);
  (void)strAdr1;
  (void)strAdr2;
  ImpLogSlow(LL_Warning, LC_VMStub, "STUB instruction SetNGmoji()\n");
}
VmInstruction(InstMesRev) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0:  // DispInit
      UI::BacklogMenuPtr->Show();
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetMesModeFormat(type: DispInit)\n");
      break;
    case 1:  // Main
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetMesModeFormat(type: Main)\n");
      break;
    case 2:  // AllCls
      UI::BacklogMenuPtr->Hide();
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetMesModeFormat(type: AllCls)\n");
      break;
    case 3:  // ChkLoad
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetMesModeFormat(type: ChkLoad)\n");
      break;
    case 4:  // SAVELoad
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetMesModeFormat(type: SAVELoad)\n");
      break;
    case 5:  // SoundUnk
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetMesModeFormat(type: SoundUnk)\n");
      break;
    case 0xA:  // DispInit
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetMesModeFormat(type: DispInit)\n");
      break;
  }
}
VmInstruction(InstMessWindow) {
  StartInstruction;
  PopUint8(type);
  DialoguePage* currentPage = &DialoguePages[thread->DialoguePageId];
  switch (type) {
    case 0:  // HideCurrent
      if (!currentPage->FadeAnimation.IsOut()) {
        currentPage->FadeAnimation.StartOut();
        SetFlag(thread->DialoguePageId + SF_MESWINDOW0OPENFL, 0);
      }
      break;
    case 1:  // ShowCurrent
      if (!currentPage->FadeAnimation.IsIn()) {
        currentPage->Mode = (DialoguePageMode)
            ScrWork[thread->DialoguePageId * 10 + SW_MESMODE0];
        currentPage->FadeAnimation.StartIn(true);
        SetFlag(thread->DialoguePageId + SF_MESWINDOW0OPENFL, 1);
      }
      break;
    case 2:  // AwaitShowCurrent
      if (currentPage->FadeAnimation.State == AS_Playing) {
        ResetInstruction;
        BlockThread;
      }
      break;
    case 3:  // AwaitHideCurrent
      if (currentPage->FadeAnimation.State == AS_Playing) {
        ResetInstruction;
        BlockThread;
      } else if (currentPage->FadeAnimation.IsOut()) {
        currentPage->Clear();
      }
      break;
    case 4:  // HideCurrent04
      if (!currentPage->FadeAnimation.IsOut()) {
        currentPage->FadeAnimation.StartOut();
        SetFlag(thread->DialoguePageId + SF_MESWINDOW0OPENFL, 0);
      }
      break;
    case 5: {  // Hide
      PopExpression(messWindowId);
      if (!DialoguePages[messWindowId].FadeAnimation.IsOut()) {
        DialoguePages[messWindowId].FadeAnimation.StartOut();
        SetFlag(messWindowId + SF_MESWINDOW0OPENFL, 0);
      }
    } break;
    case 6: {  // HideSlow
      PopExpression(messWindowId);
      ImpLogSlow(
          LL_Warning, LC_VMStub,
          "STUB instruction MessWindow(type: HideSlow, messWindowId: %i)\n",
          messWindowId);
    } break;
    case 7: {  // HideSlow
      PopExpression(messWindowId);
      ImpLogSlow(
          LL_Warning, LC_VMStub,
          "STUB instruction MessWindow(type: HideSlow, messWindowId: %i)\n",
          messWindowId);
    } break;
  }
}
VmInstruction(InstSel) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0: {  // SelInit
      if (Profile::Vm::GameInstructionSet == +InstructionSet::Dash ||
          Profile::Vm::GameInstructionSet == +InstructionSet::CC ||
          Profile::Vm::GameInstructionSet == +InstructionSet::MO8) {
        PopUint16(savepointid);
        // 1288 + dialog page's field 5 in decompile?
        // if (GetFlag(1288 + thread->DialoguePageId) == 0) {
        if (ScrWork[SW_TITLE] != 0xffff) {
          SaveSystem::SetCheckpointId(savepointid);
          SaveSystem::SaveMemory();
          SetFlag(1206, 1);
          SetFlag(1285, 1);
          BlockThread;
        }
        // }
      }
      PopExpression(arg1);
      UI::SelectionMenuPtr->Init((bool)arg1);
      // SaveIconDisplay::Show();
      break;
    }
    case 1: {
      PopUint16(selStrNum);
      UI::SelectionMenuPtr->AddChoice(ScriptGetStrAddress(
          ScriptBuffers[thread->ScriptBufferId], selStrNum));
      break;
    }
    case 0x81: {
      PopMsbString(line);
      UI::SelectionMenuPtr->AddChoice(line);
    } break;
    case 2: {
      PopUint16(selStrNum);
      UI::SelectionMenuPtr->AddChoice(ScriptGetStrAddress(
          ScriptBuffers[thread->ScriptBufferId], selStrNum));
      PopExpression(arg2);
      break;
    }
    case 0x82: {
      PopMsbString(line);
      PopExpression(arg2);
      UI::SelectionMenuPtr->AddChoice(line);
    } break;
  }
}
VmInstruction(InstSelect) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0: {
      UI::SelectionMenuPtr->Show();
      SaveIconDisplay::ShowFor(2.4f);
    } break;
    case 1: {
      if (!UI::SelectionMenuPtr->ChoiceMade) {
        ResetInstruction;
        BlockThread;
      } else if (UI::SelectionMenuPtr->State == UI::MenuState::Hiding) {
        ResetInstruction;
        BlockThread;
      } else if (UI::SelectionMenuPtr->State == UI::MenuState::Hidden) {
        BlockThread;
      } else {
        UI::SelectionMenuPtr->Hide();
        ResetInstruction;
        BlockThread;
      }
    } break;
    case 2: {
      PopExpression(arg1);
      ScrWork[arg1] = UI::SelectionMenuPtr->SelectedChoiceId;
    } break;
  }
}
VmInstruction(InstSysSel) {
  StartInstruction;
  PopUint8(type);  // TODO: Implement type 0, 1
  if (type >= 2) {
    PopString(arg1);
    (void)arg1;
  }
  ImpLogSlow(LL_Warning, LC_VMStub, "STUB instruction SysSel(type: %i)\n",
             type);
}
VmInstruction(InstSysSelect) {
  StartInstruction;
  PopUint8(type);
  switch (type & 0xF) {
    case 0: {
      PopExpression(arg1);
      PopExpression(arg2);
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SysSelect(type: %i, arg1: %i, arg2: %i)\n",
                 type, arg1, arg2);
    } break;
    case 2:
    case 3: {
      PopExpression(destination);
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SysSelect(type: %i, destination: %i)\n",
                 type, destination);
    } break;
  }
}
VmInstruction(InstSetTextTable) {
  StartInstruction;
  PopExpression(id);
  PopLocalLabel(tableDataAdr);
  TextTable[id].scriptBufferAdr = ScriptBuffers[thread->ScriptBufferId];
  TextTable[id].labelAdr = tableDataAdr;
  ImpLogSlow(LL_Warning, LC_VMStub, "STUB instruction SetTextTable(id: %i)\n",
             id);
}
VmInstruction(InstSetDic) {
  StartInstruction;
  PopUint8(type);
  PopExpression(tipId);
  switch (type) {
    case 0:    // NewTip
    case 1: {  // Check
      if (TipsSystem::GetTipLockedState(tipId)) {
        TipsSystem::SetTipLockedState(tipId, false);
        TipsNotification::AddTip(tipId);
      }
      if (type == 1) {
        PopExpression(flagId);
      }
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetDic(type: NewTip, tipId: %i)\n", tipId);
    } break;
    case 2:  // SetDic02
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetDic(type: %i, tipId: %i)\n", type, tipId);
      break;
  }
}
VmInstruction(InstEncyclopedia) {
  StartInstruction;
  PopExpression(tipId);
  if (TipsSystem::GetTipLockedState(tipId)) {
    TipsSystem::SetTipLockedState(tipId, false);
    TipsNotification::AddTip(tipId);
  }
}
VmInstruction(InstNameID) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0:
      if (Profile::Vm::GameInstructionSet == +InstructionSet::CC ||
          Profile::Vm::GameInstructionSet == +InstructionSet::MO8 ||
          Profile::Vm::GameInstructionSet == +InstructionSet::CHN) {
        PopLocalLabel(namePlateDataBlock);
        if (!Profile::Vm::UseMsbStrings)
          InitNamePlateData((uint16_t*)namePlateDataBlock);
      } else if (Profile::Vm::GameInstructionSet == +InstructionSet::MO6TW) {
        PopExpression(arg1);
        PopExpression(arg2);
        PopExpression(arg3);
      }
      ImpLogSlow(LL_Warning, LC_VMStub, "STUB instruction NameID(type: %i)\n",
                 type);
      break;
    case 1: {
      PopLocalLabel(namePlateDataBlock);
      (void)namePlateDataBlock;
      ImpLogSlow(LL_Warning, LC_VMStub, "STUB instruction NameID(type: %i)\n",
                 type);
    } break;
    case 2:
      ImpLogSlow(LL_Warning, LC_VMStub, "STUB instruction NameID(type: %i)\n",
                 type);
      break;
  }
}
VmInstruction(InstTips) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0: {  // TipsDataInit
      PopLocalLabel(tipsDataAdr);
      if (Profile::Vm::GameInstructionSet == +InstructionSet::MO8 ||
          Profile::Vm::GameInstructionSet == +InstructionSet::CHN) {
        PopLocalLabel(tipsDataAdr1);
        (void)tipsDataAdr1;
      }

      TipsSystem::DataInit(thread->ScriptBufferId, tipsDataAdr);
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction Tips(type: TipsDataInit)\n");
    } break;
    case 1:  // TipsInit
      TipsSystem::UpdateTipRecords();
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction Tips(type: TipsInit)\n");
      break;
    case 2:  // TipsMain
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction Tips(type: TipsMain)\n");
      break;
    case 3:  // TipsEnd
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction Tips(type: TipsEnd)\n");
      break;
    case 4:  // TipsSet
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction Tips(type: TipsSet)\n");
      break;
    case 10:  // Tips_ProfSetXboxEvent
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction Tips(type: Tips_ProfSetXboxEvent)\n");
      break;
  }
}
VmInstruction(InstSetRevMes) {
  StartInstruction;
  PopUint8(type);
  switch (type) {
    case 0: {
      PopString(message);
      (void)message;
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetRevMes(type: %i)\n", type);
    } break;
    case 0x80: {
      PopExpression(messageId);
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetRevMes(type: %i)\n", type);
    } break;
    case 1: {
      PopExpression(arg1);
      PopExpression(arg2);
      PopString(message);
      (void)message;
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetRevMes(type: %i, arg1: %i, arg2: %i)\n",
                 type, arg1, arg2);
    } break;
    case 0x81: {
      PopExpression(arg1);
      PopExpression(arg2);
      PopExpression(message);
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetRevMes(type: %i, arg1: %i, arg2: %i)\n",
                 type, arg1, arg2);
    } break;
    case 2: {
      PopString(arg1);
      (void)arg1;
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetRevMes(type: %i)\n", type);
    } break;
    case 3: {
      PopString(arg1);
      (void)arg1;
      PopExpression(arg2);
      PopExpression(arg3);
      PopExpression(arg4);
      ImpLogSlow(LL_Warning, LC_VMStub,
                 "STUB instruction SetRevMes(type: %i, arg2: %i, arg3: %i, "
                 "arg4: %i)\n",
                 type, arg2, arg3, arg4);
    } break;
  }
}

void ChkMesSkip() {
  bool mesSkip = false;

  if (((ScrWork[SW_GAMESTATE] & 0b101) == 0b001) &&
      (ScrWork[SW_SYSMESALPHA] == 255) && !GetFlag(SF_UIHIDDEN)) {
    // Force skip
    mesSkip |=
        (bool)(Interface::PADinputButtonIsDown & Interface::PADcustom[7]);

    // Skip
    if (Interface::PADinputButtonWentDown & Interface::PADcustom[8])
      if (MesSkipMode & (SkipModeFlags::SkipRead | SkipModeFlags::SkipAll))
        // Turn off all skip modes (leaving auto)
        MesSkipMode &= SkipModeFlags::Auto;
      else
        MesSkipMode |=
            (SkipMode ? SkipModeFlags::SkipAll : SkipModeFlags::SkipRead);

    // Auto
    if (Interface::PADinputButtonWentDown & Interface::PADcustom[9])
      MesSkipMode ^= SkipModeFlags::Auto;
  }

  mesSkip |= (bool)(MesSkipMode & SkipModeFlags::SkipAll);
  mesSkip |= (MesSkipMode & SkipModeFlags::SkipRead) && GetFlag(SF_MESREAD);
  SetFlag(SF_MESSKIP, mesSkip);
  SetFlag(SF_MESALLSKIP, mesSkip);  // These two are seemingly identical?
}

}  // namespace Vm

}  // namespace Impacto