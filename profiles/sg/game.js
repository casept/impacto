root.LayerCount = 100;
root.GameFeatures = GameFeature.Sc3VirtualMachine | GameFeature.Renderer2D | GameFeature.Input | GameFeature.Audio | GameFeature.Nuklear | GameFeature.Video;
// NOTE: 1080p messes up rendering of "loading system data..." message,
// but is probably correct because otherwise logo slideshow has wrong size.
// Probably a scaling factor somewhere needs to be adjusted for the message.
root.DesignWidth = 1920;
root.DesignHeight = 1080;

root.WindowName = "STEINS;GATE";

root.CharaIsMvl = false;
root.LayFileBigEndian = true;
root.LayFileTexXMultiplier = 2048;
root.LayFileTexYMultiplier = 1024;
root.UseScreenCapEffects = false;

root.Vm = {
	StartScript: 2, // _STARTUP_WIN.SCX
	StartScriptBuffer: 0,
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
include('sg/hud/titlemenu.js');
include('sg/tipssystem.js');
include('sg/dialogue.js');
include('sg/hud/tipsnotification.js');
include('sg/hud/datedisplay.js');
include('sg/hud/selectiondisplay.js');
include('sg/hud/saveicon.js');
include('sg/hud/loadingdisplay.js');
include('sg/hud/sysmesboxdisplay.js');