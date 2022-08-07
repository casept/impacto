root.LayerCount = 100;
root.GameFeatures = GameFeature.Sc3VirtualMachine | GameFeature.Renderer2D | GameFeature.Input | GameFeature.Audio | GameFeature.Nuklear | GameFeature.Video;
root.DesignWidth = 1280;
root.DesignHeight = 720;

root.WindowName = "STEINS;GATE";

root.CharaIsMvl = false;
root.LayFileBigEndian = true;
root.LayFileTexXMultiplier = 2048;
root.LayFileTexYMultiplier = 1024;
root.UseScreenCapEffects = false;

root.Vm = {
	StartScript: 2,
	StartScriptBuffer: 0,
	GameInstructionSet: InstructionSet.SG,
	UseReturnIds: false,
	ScrWorkChaStructSize: 20,
	ScrWorkBgStructSize: 20
};

include('common/scriptinput.js');
include('common/scriptvars.js');
include('sg/scriptvars.js');
include('sg/savedata.js');
include('sg/tipssystem.js');
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
//include('sg/hud/systemmenu.js');
include('sg/hud/backlogmenu.js');
include('sg/hud/sysmesboxdisplay.js');
include('sg/hud/selectiondisplay.js');
include('sg/hud/tipsmenu.js');
include('sg/hud/tipsnotification.js');
