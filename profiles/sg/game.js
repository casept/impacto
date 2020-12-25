root.LayerCount = 100;
root.GameFeatures = GameFeature.Sc3VirtualMachine | GameFeature.Renderer2D |
    GameFeature.Input | GameFeature.Audio | GameFeature.Nuklear;
root.DesignWidth = 1280;
root.DesignHeight = 720;

root.WindowName = 'STEINS;GATE';

root.CharaIsMvl = false;
root.LayFileBigEndian = true;
root.LayFileTexXMultiplier = 2048;
root.LayFileTexYMultiplier = 1024;

root.Vm = {
  StartScript: 2,  // _STARTUP_WIN.SCX
                   // Depending on the buffer we either get an infinite loop w/
                   // black screen or segfault
  StartScriptBuffer: 0,  // 0, 4, 6, 7, 8, 9, 10
  GameInstructionSet: InstructionSet.SG,
  UseReturnIds: true,
  ScrWorkChaStructSize: 40,
  ScrWorkBgStructSize: 40
};

include('common/scriptinput.js');
include('common/scriptvars.js');
include('sg/scriptvars.js');
include('sg/savedata.js');
include('sg/vfs.js');
include('sg/sprites.js');
include('common/animation.js');
include('sg/charset.js');
include('sg/font.js');
include('sg/dialogue.js');
include('sg/hud/saveicon.js');
include('sg/hud/loadingdisplay.js');
include('sg/hud/datedisplay.js');
include('sg/hud/titlemenu.js');
include('sg/hud/mainmenu.js');
include('sg/hud/sysmesboxdisplay.js');
include('sg/hud/selectiondisplay.js');
// Idea: Watch address of _STARTUP_WIN (the string?) or the lookup_resource function to find what reads .scx
